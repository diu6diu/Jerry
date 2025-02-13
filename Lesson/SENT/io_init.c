

#include "io.h"
#include "io_scale.h"
#include "io_initialize.h"
#include "dd_port.h"
#include "dd_ir.h"
#include "dd_dma.h"
#include "dd_vadc.h"
#include "dd_msc.h"

#ifdef USE_EPIO
#include "dd_epio.h"
#endif
#include "dd_diflex.h"
#include "dd_diflex_cfg.h"
#include "io_vector.h"
#include "io_spi_scheduler.h"
#include "io_spi_config.h"
#include "dd_dsadc.h"
#include "dd_sent.h"
#include "dd_sent_init.h"
#include "dd_cpu.h"
#include "dd_scu.h"
#include "dd_smu.h"
#include "dd_csfr.h"
#include "dd_mtu.h"
#include "dd_mcu.h"
#ifdef USE_FS_TEST
#include "dd_fs_shutoff.h"
#endif
#include "hwiocald.h"
#ifdef COMPLEX_IO
#include "io_crank.h"
#endif
#ifndef BUILD_TSW
#include "ccp2main.h"
#include "inst_overlay.h"
#include "hal_iocald.h"
#include "os_loop_scheduler.h"
#include "hal_diag_nvm.h"
#include "hal_diag.h"
#include "hal_adc_diag.h"
#include "hwi_mpm_methods.h"
#else
#define Base_Loop_Time_mS (2.5)
#include "io_can_port.h"
#include "commport.h"
#endif
//Experimental externs to be removed when verification not required
//Fixme !!!! just to import FOR_DEV_ONLY
#include "io_config.h"
#include "dd_tim_support.h"

#ifdef USE_L9301
#include "dd_l9301.h"
#endif

#ifdef USE_DRV8873
#include "dd_drv8873_init.h"
#include "dd_drv8873.h"
#endif

#ifdef USE_CALIBRATION_IO
#include "hal_calio_interface.h"
#endif
#include "dd_gtm.h"

#ifdef USE_LIN_DLL
#include "io_lin_port.h"
#endif

#include "dd_stm.h"
#include "dd_flash.h"

#ifdef ETC_SOH_HAL
#include "soh.h"
#endif

#ifdef USE_MINI_APP
#include "sapp_cald.h"
#endif

#if ( USE_WRAF_HWIO == 1 )
#include "C3WRAF_Driver.h"
#include "dd_c3wraf_send_spi.h"
#include "hwio_wraf.h"
#endif

#include "hwio_can.h"


#define SCU_SPB_TO_WDT_DIVIDER  (16384)
#define SCU_WDT_FREQUENCY                 (SPB_FREQUENCY_HZ/SCU_SPB_TO_WDT_DIVIDER)
#define SCU_Convert_MS_To_WDT_Count(time) ((time*SCU_WDT_FREQUENCY)/1000)
#define WDT_TIMEOUT_MS_INIT  (400)
#ifdef BUILD_TSW
#define WDT_TIMEOUT_MS_RUN   (125)    /* special requirement for testability */
#else
#define WDT_TIMEOUT_MS_RUN   (50)
#endif

#ifdef USE_DRV8873
//extern static uint8_t       HWCfg_Motor2To5_Installed;
//extern static uint8_t       HWCfg_NumMotor2To5_Installed;

extern uint8_t NUMBER_OF_DRV8873_DAISY_CHAIN_IN_RUNTIME;
#endif

#if 1
bool DIFLEX_FIRST_SPI_AFT_RST_POR = false;
#endif

#define HWTAG_MAX_VALUE             ( 0xFFC0 )   /* 10 Bit Resolution */
#define HWTAG_DEF_A_UPPER_LIMIT     ( (uint16_t)HWTAG_MAX_VALUE )
#define HWTAG_DEF_A_LOWER_LIMIT     ( (uint16_t)( ( HWTAG_MAX_VALUE * (4.75/5.0) ) * 1.0 ) )
#define HWTAG_DEF_B_UPPER_LIMIT     ( HWTAG_DEF_A_LOWER_LIMIT )
#define HWTAG_DEF_B_LOWER_LIMIT     ( (uint16_t)( ( HWTAG_MAX_VALUE * (4.45/5.0) ) * 1.0 ) )
#define HWTAG_DEF_C_UPPER_LIMIT     ( HWTAG_DEF_B_LOWER_LIMIT )
#define HWTAG_DEF_C_LOWER_LIMIT     ( (uint16_t)( ( HWTAG_MAX_VALUE * (4.15/5.0) ) * 1.0 ) )

#define HWTAG_DEF_P_UPPER_LIMIT     ( (uint16_t)( ( HWTAG_MAX_VALUE * (2.35/5.0) ) * 1.0 ) )
#define HWTAG_DEF_P_LOWER_LIMIT     ( (uint16_t)( ( HWTAG_MAX_VALUE * (2.05/5.0) ) * 1.0 ) )
#define HWTAG_DEF_Q_UPPER_LIMIT     ( HWTAG_DEF_P_LOWER_LIMIT )
#define HWTAG_DEF_Q_LOWER_LIMIT     ( (uint16_t)( ( HWTAG_MAX_VALUE * (1.75/5.0) ) * 1.0 ) )
#define HWTAG_DEF_R_UPPER_LIMIT     ( HWTAG_DEF_Q_LOWER_LIMIT )
#define HWTAG_DEF_R_LOWER_LIMIT     ( (uint16_t)( ( HWTAG_MAX_VALUE * (1.45/5.0) ) * 1.0 ) )

#define EWG_CURSENVI_RAW_AD_MAX_VALUE     ( 0xFFC0 )   /* 10 Bit Resolution */
#define EWG_CURSENVI_UPPER_RAW_AD_VALUE   ( (uint16_t)((EWG_CURSENVI_RAW_AD_MAX_VALUE*(4.5/5))*1.0) )
#define EWG_CURSENVI_LOWER_RAW_AD_VALUE   ( (uint16_t)((EWG_CURSENVI_RAW_AD_MAX_VALUE*(0.5/5))*1.0) )

#define VADC_OFFSET_GAIN_DIAG_LOOP_COUNT 15

typedef enum
{
   INVALID_HBRIDGE,
   H_BRIDGE_1,
   H_BRIDGE_2,
   H_BRIDGE_3,
   H_BRIDGE_4
} H_Bridge_Option_T;

typedef enum
{
   INVALID_SPARK,
   SPRK_IGBT,
   SPRK_EST3MA,
   SPRK_EST10MA,
} SPRK_Option_T;


#ifndef BUILD_TSW
void HAL_ResetDiag_Log_Parameters_C0( Cause_Of_Trap_T reset_cause, uint32_t reset_address );
void HAL_ResetDiag_Log_Parameters_C1( Cause_Of_Trap_T reset_cause, uint32_t reset_address );

const uint8_t  MTSA_PERIODC_CALLBACK_PRIORITY = OS_PERIODIC_PRIORITY;
const Cause_Of_Trap_Callback_T SCU_Cause_Of_Reset_Callback_C1   = HAL_ResetDiag_Log_Parameters_C1;
const Cause_Of_Trap_Callback_T SCU_Cause_Of_Reset_Callback_C0 = HAL_ResetDiag_Log_Parameters_C0;
#else

void HAL_ResetDiag_Log_Parameters( Cause_Of_Trap_T reset_cause, uint32_t reset_address );

void HAL_ResetDiag_Log_Parameters( Cause_Of_Trap_T reset_cause, uint32_t reset_address )
{
   Parameter_Not_Used(reset_cause);
   Parameter_Not_Used(reset_address);
   SCU_Force_Software_System_Reset();

}

const Cause_Of_Trap_Callback_T SCU_Cause_Of_Reset_Callback_C1   = HAL_ResetDiag_Log_Parameters;
const Cause_Of_Trap_Callback_T SCU_Cause_Of_Reset_Callback_C0 = HAL_ResetDiag_Log_Parameters;
#endif


bool                 IO_Battery_Disconnect = 0;
Hardware_Version_T   HW_Version = HWVER_ILLEGAL;
uint16_t             IO_DFX_VBSTVI_Update_10ms;

static uint8_t       STM0_Interrupt_Counter = 0;

extern SCU_Reset_Status_T      Reset_Status;

extern IO_Discrete_T       HAL_DISCRETE_FuelPump;
extern IO_Configuration_T  DIFLEX_Diagnostic_Device_Configuration[ NUMBER_OF_DIFLEX ];


FLASH_HF_PROCONDF_T         Procon_Deflash;
uint32_t SW_Init_Time[TIME_MEASURE_MAX];

#ifdef USE_DRV8873
extern void DRV8873_Initialize_Device( IO_Configuration_T in_configuration );
extern void DRV8873_Read_Device_Message( IO_Configuration_T in_configuration );

#endif


//=============================================================================
// InitializeResetSources
//
// @func Initialize reset sources.The function initialize the following reset
// sources
// SW, ESR0, ESR1, STM0,STM1,SMU
// SW and ESR0 are initialzed to System reset.
// ESR1,STM0,STM1,SMU are initialzied to no reset.
//
// @rdesc first lo resolution event angle.
//
// @end
//=============================================================================
void InitializeResetSources( void )
{
   SCU_RESET_Set_Reset_Type(SCU_RESET_SOURCE_ESR0,SCU_RESET_TYPE_SYSTEM);
   SCU_RESET_Set_Reset_Type(SCU_RESET_SOURCE_SW,SCU_RESET_TYPE_SYSTEM);
   SCU_RESET_Set_Reset_Type(SCU_RESET_SOURCE_STM0,SCU_RESET_TYPE_NO_RESET);
   SCU_RESET_Set_Reset_Type(SCU_RESET_SOURCE_STM1,SCU_RESET_TYPE_NO_RESET);
   SCU_RESET_Set_Reset_Type(SCU_RESET_SOURCE_SMU,SCU_RESET_TYPE_NO_RESET);
   SCU_RESET_Set_Reset_Type(SCU_RESET_SOURCE_ESR1,SCU_RESET_TYPE_NO_RESET);
}


//=============================================================================
// Initialize pulse inputs
//=============================================================================
void InitializePulseInputs( void )
{
   IO_PULSE_FI_Initialize_Channel( &MTSA_FI_VSS );
   IO_PULSE_FI_Initialize_Channel( &MTSA_FI_WSS );
   IO_PULSE_FI_Initialize_Channel( &MTSA_FI_MAFS );
   IO_PULSE_IC_Initialize_Channel(&MTSA_IC_CLUTCHPOSS);
   IO_PULSE_IC_Initialize_Channel(&MTSA_IC_ALTSIGNALO);
   //IO_PULSE_IC_Initialize_Channel(&MTSA_IC_DIFLEX_DIAG1);
   //IO_PULSE_IC_Initialize_Channel(&MTSA_IC_DIFLEX_DIAG2);
   //IO_PULSE_IC_Initialize_Channel(&MTSA_IC_DIFLEX_DIAG3);
   IO_PULSE_IC_Initialize_Channel(&MTSA_IC_DIFLEX_FLEX_OUT);
   IO_PULSE_IC_Initialize_Channel(&MTSA_IC_PNSWS);
   IO_PULSE_IC_Initialize_Channel( &MTSA_IC_TMM_Position);
   IO_PULSE_IC_Initialize_Channel( &MTSA_IC_PWM_DIAG_1_FB);
   IO_PULSE_IC_Initialize_Channel( &MTSA_IC_PWM_DIAG_2_FB);
#ifdef BUILD_TSW
   IO_PULSE_FI_Initialize_Channel( &MTSA_FI_CAM1 );
   IO_PULSE_FI_Initialize_Channel( &MTSA_FI_CAM2 );
   IO_PULSE_IC_Initialize_Channel( &MTSA_IC_EPPWMT);
   IO_PULSE_IC_Initialize_Channel( &MTSA_IC_POSCOLS);
#else
   IO_PULSE_IC_Initialize_Channel( &MTSA_IC_POSCOLS);
#endif
}
//=============================================================================
// Initialize pulse outputs as defined in io_config.h
//=============================================================================
void InitializePulseOutputs( void )
{
   uint8_t idx;
#ifdef USE_CALIBRATION_IO
   Initialize_CalIO_PulseOutputPolarity();
#endif

   for ( idx = 0; idx < IO_GTM_PWM_CHANNELS_SIZE; idx ++ )
   {
      IO_PULSE_PWM_Initialize_Channel((IO_GTM_PWM_CHANNELS[idx]));
	  /*this function will enable all used GTM channel interrupt, this will cause unexpected interrupt occur if these two channel use same gtm interrupt source,
	  so bypass this function here, and enable channel only used*/
      //IO_PULSE_PWM_Enable_Channel((IO_GTM_PWM_CHANNELS[idx]));
   }
/*Set the BINT frequency to 50hz(20ms) to clear and to re-start the internal BINT timer.*/
   IO_PULSE_PWM_Set_Frequency_And_Duty_Cycle(
      &MTSA_PWM_BINT,
      50,     // Frequency in Hertz
      0,       // Frequency Percision
      50,      // Duty cycle
      100 );   // Max Duty Cycle

#ifdef BUILD_TSW
	IO_PULSE_PWM_Initialize_Channel(&MTSA_PWM_GDI_INJ_SYNC);
	IO_PULSE_PWM_Initialize_Channel(&MTSA_PWM_PFI_INJ_SYNC);
	IO_PULSE_PWM_Initialize_Channel(&MTSA_PWM_SPARK_SYNC);
	IO_PULSE_PWM_Initialize_Channel(&MTSA_PWM_SPILL_VALVE);

	/* GDI INJ OC Timer Initialization */
	IO_PULSE_OC_Initialize_Channel(&MTSA_OC_INJ_SEL_A);
	IO_PULSE_OC_Initialize_Channel(&MTSA_OC_INJ_SEL_B);
	IO_PULSE_OC_Initialize_Channel(&MTSA_OC_INJ_SEL_C);
	IO_PULSE_OC_Initialize_Channel(&MTSA_OC_INJ_SEL_D);

	/* PFI INJ OC Timer Initialization */
	IO_PULSE_OC_Initialize_Channel(&MTSA_OC_PFI_INJA_C);
	IO_PULSE_OC_Initialize_Channel(&MTSA_OC_PFI_INJB_C);
	IO_PULSE_OC_Initialize_Channel(&MTSA_OC_PFI_INJC_C);
	IO_PULSE_OC_Initialize_Channel(&MTSA_OC_PFI_INJD_C);

	/* SPARK OC Timer Initialization */
	IO_PULSE_OC_Initialize_Channel(&MTSA_OC_EST_SPARK);
#endif

   IO_PULSE_OC_Initialize_Channel(&MTSA_OC_EST_SYNC);
   IO_PULSE_PWM_Initialize_Channel(&MTSA_PWM_DIFLEX_CLKIN );
   IO_PULSE_PWM_Set_Frequency_And_Duty_Cycle(
      &MTSA_PWM_DIFLEX_CLKIN,
      100,     // Frequency in Hertz
      0,       // Frequency Percision
      50,      // Duty cycle
      100 );   // Max Duty Cycle

#ifdef ETC_SOH_HAL
   IO_PULSE_OC_Initialize_Channel(&MTSA_OC_ETC_SOH);
#endif   //ETC_SOH_HAL

#ifndef BUILD_TSW
   IO_PULSE_PWM_Initialize_Channel(&MTSA_OC_1MS_PERIOD);//1ms task
   HAL_OS_1ms_Setup_Interrupt(1000);//1000us
#endif


}

//=============================================================================
// InitializePulseTimers
//=============================================================================
void InitializePulseTimers( void )
{
   // For ASCLIN, SRPN are assigned here, interrupts are enabled in UART_InitializePort()
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_ASCLIN0TX, ASC_PORT_0_TBUF_PRIORITY, NULL, ASC_PORT_0_TBUF_SOURCE );
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_ASCLIN0RX, ASC_PORT_0_RIR_PRIORITY, NULL, ASC_PORT_0_RIR_SOURCE );
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_ASCLIN0ERR, ASC_PORT_0_TIR_PRIORITY, NULL, ASC_PORT_0_TIR_SOURCE );

   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_ASCLIN0TX, true );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_ASCLIN0RX, true );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_ASCLIN0ERR, true );

   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_ASCLIN1TX, ASC_PORT_1_TBUF_PRIORITY, NULL, ASC_PORT_1_TBUF_SOURCE );
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_ASCLIN1RX, ASC_PORT_1_RIR_PRIORITY, NULL, ASC_PORT_1_RIR_SOURCE );
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_ASCLIN1ERR, ASC_PORT_1_TIR_PRIORITY, NULL, ASC_PORT_1_TIR_SOURCE );

   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_ASCLIN1TX, true );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_ASCLIN1RX, true );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_ASCLIN1ERR, true );

    MCAN_IR_Initialize();

   // Set up STM interrupt for SPI scheduler
   IO_INTERRUPT_Initialize( &MTSA_SPI_INTERRUPT, MTSA_SPI_INTERRUPT_PRIORITY, NULL, STM_0_COMPARE_0_SOURCE );
   IO_INTERRUPT_Set_Enable( &MTSA_SPI_INTERRUPT, true );

   // Set up callback interrupt for SPI scheduler
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_SPI_CALLBACK, MTSA_SPI_CALLBACK_PRIORITY, NULL, SPI_PERIODIC_SOURCE );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_SPI_CALLBACK, true );

   // Set up STM interrupt for OS scheduler
   IO_INTERRUPT_Initialize( &MTSA_PERIODIC_INTERRUPT, MTSA_PERIODIC_INTERRUPT_PRIORITY, NULL, STM_0_COMPARE_1_SOURCE );
   IO_INTERRUPT_Set_Enable( &MTSA_PERIODIC_INTERRUPT, true );

#ifndef BUILD_TSW
   // Set up callback interrupt for OS scheduler
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_PERIODIC_CALLBACK, MTSA_PERIODC_CALLBACK_PRIORITY, NULL, OS_PERIODIC_SOURCE );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_PERIODIC_CALLBACK, true );

   #if 0//tz68d9

//   Initialize_VADC_Interrupts();

   if(CeHAL_SENT_SENSOR_NULL_INDEX < KeHWIO_C38_IF_Configuration)
   {
       IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_SENT1RX, SENT_1_RX_PRIORITY, NULL, SENT_1_RX_SOURCE );
       IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_SENT1RX, true );
   }

   if(CeHAL_SENT_SENSOR_NULL_INDEX < KeHWIO_C09_IF_Configuration)
   {
       IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_SENT2RX, SENT_2_RX_PRIORITY, NULL, SENT_2_RX_SOURCE );
       IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_SENT2RX, true );
   }

   if(CeHAL_SENT_SENSOR_NULL_INDEX < KeHWIO_C15_IF_Configuration)
   {
       IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_SENT3RX, SENT_3_RX_PRIORITY, NULL, SENT_3_RX_SOURCE );
       IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_SENT3RX, true );
   }
   #endif

   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_SENT0RX, SENT_0_RX_PRIORITY, NULL, SENT_0_RX_SOURCE );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_SENT0RX, true );
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_SENT1RX, SENT_1_RX_PRIORITY, NULL, SENT_1_RX_SOURCE );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_SENT1RX, true );
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_SENT2RX, SENT_2_RX_PRIORITY, NULL, SENT_2_RX_SOURCE );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_SENT2RX, true );
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_SENT3RX, SENT_3_RX_PRIORITY, NULL, SENT_3_RX_SOURCE );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_SENT3RX, true );
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_SENT4RX, SENT_4_RX_PRIORITY, NULL, SENT_4_RX_SOURCE );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_SENT4RX, true );
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_SENT5RX, SENT_5_RX_PRIORITY, NULL, SENT_5_RX_SOURCE );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_SENT5RX, true );

   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_MAIN_CORE_CALLBACK_HIGH,   CORE_MAIN_CB_HIGH_PRIORITY,   NULL, CORE_MAIN_CB_HIGH_SOURCE );
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_MAIN_CORE_CALLBACK_MEDIUM, CORE_MAIN_CB_MEDIUM_PRIORITY, NULL, CORE_MAIN_CB_MEDIUM_SOURCE );
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_MAIN_CORE_CALLBACK_LOW,    CORE_MAIN_CB_LOW_PRIORITY,    NULL, CORE_MAIN_CB_LOW_SOURCE );
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_MAIN_CORE_CALLBACK_COM,    CORE_MAIN_CB_COM_PRIORITY,    NULL, CORE_MAIN_CB_COM_SOURCE );

   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_MAIN_CORE_CALLBACK_HIGH, true );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_MAIN_CORE_CALLBACK_MEDIUM, true );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_MAIN_CORE_CALLBACK_LOW, true );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_MAIN_CORE_CALLBACK_COM, true );

#else
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_SENT0RX, SENT_0_RX_PRIORITY, NULL, SENT_0_RX_SOURCE );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_SENT0RX, true );
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_SENT1RX, SENT_1_RX_PRIORITY, NULL, SENT_1_RX_SOURCE );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_SENT1RX, true );
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_SENT2RX, SENT_2_RX_PRIORITY, NULL, SENT_2_RX_SOURCE );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_SENT2RX, true );
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_SENT3RX, SENT_3_RX_PRIORITY, NULL, SENT_3_RX_SOURCE );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_SENT3RX, true );
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_SENT4RX, SENT_4_RX_PRIORITY, NULL, SENT_4_RX_SOURCE );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_SENT4RX, true );
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_SENT5RX, SENT_5_RX_PRIORITY, NULL, SENT_5_RX_SOURCE );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_SENT5RX, true );
#endif
#ifdef BUILD_TSW
	  /* TOM1_0 ISR */
	  IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_SPARK_SYNC, CPU_INTERRUPT_SPARK_SYNC,   NULL, SPARK_SYNC_SOURCE );
	  IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_SPARK_SYNC, true );
	  IO_PULSE_PWM_Enable_Channel(&MTSA_PWM_SPARK_SYNC);

	  	  /* ATOM0_4 ISR */
	  IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_PFI_INJ_SYNC, PFI_INJ_SYNC_PRIORITY,  NULL, PFI_INJ_SYNC_SOURCE );
	  IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_PFI_INJ_SYNC, true );
	  IO_PULSE_PWM_Enable_Channel(&MTSA_PWM_PFI_INJ_SYNC);

	  /******************************************************************************************
	   *  Multi-pulse for GDI INJ int process
	   *******************************************************************************************/
	  /* ATOM3_1 ISR */
	  IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_GDI_INJSEL_A, GDI_INJ_SEL_A_PRIORITY, NULL, GDI_INJ_SEL_A_SOURCE );
	  IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_GDI_INJSEL_A, true );
	  /* ATOM3_2 ISR */
	  IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_GDI_INJSEL_B, GDI_INJ_SEL_B_PRIORITY, NULL, GDI_INJ_SEL_B_SOURCE );
	  IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_GDI_INJSEL_B, true );
	  /* ATOM2_3 ISR */
	  IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_GDI_INJSEL_C, GDI_INJ_SEL_C_PRIORITY, NULL, GDI_INJ_SEL_C_SOURCE );
	  IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_GDI_INJSEL_C, true );
	  /* ATOM3_4 ISR */
	  IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_GDI_INJSEL_D, GDI_INJ_SEL_D_PRIORITY,  NULL, GDI_INJ_SEL_D_SOURCE );
	  IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_GDI_INJSEL_D, true );

	  /******************************************************************************************
	   *  GDI INJ/PFI INJ/SPARK SYNC int process
	   *******************************************************************************************/
	  /* ATOM0_7 ISR */
	  IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_GDI_INJ_SYNC, GDI_INJ_SYNC_PRIORITY,  NULL, GDI_INJ_SYNC_SOURCE);
	  IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_GDI_INJ_SYNC, true );
	  IO_PULSE_PWM_Enable_Channel(&MTSA_PWM_GDI_INJ_SYNC);

#endif

#ifdef ETC_SOH_HAL
   // Set up interrupt for ETC SOH
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_ETC_SOH, ETC_SOH_ISR_PRIORITY, NULL, ETC_SOH_ISR_SOURCE );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_ETC_SOH, true );
#endif

#ifndef BUILD_TSW
   // Set up interrupt for 1ms period task
   IO_INTERRUPT_Initialize( &MTSA_INTERRUPT_OC_1MS_PERIOD, OS_1MS_PRIORITY, NULL, OS_1MS_SOURCE );
   IO_INTERRUPT_Set_Enable( &MTSA_INTERRUPT_OC_1MS_PERIOD, true );
   IO_PULSE_PWM_Enable_Channel(&MTSA_OC_1MS_PERIOD);
#endif
}

//=============================================================================
// InitializeHardwareRegisters
//=============================================================================
void InitializeHardwareRegisters( void )
{

#ifdef INSTRUCAN_PORT_HW_DETECTION_ENABLED
   Hardware_Installed_Version.U16 = 0;
#endif

   //Must be done for each core
   CSFR_Set_PCACHE_Enable(true);

#ifndef BUILD_TSW
   if (CbTRUE == KbHWIO_USE_MPM)
   {
      HWI_Mpm_Switch_Context_Main_Core_Init();
   }
#endif

   Initialize_Main_Watchdogs();
   InitializeResetSources();

   TC3xx_FLASH_Initialize_Device(0);

   Initialize_OVC_Device(Overlay_Set_Core_Index(0,OVC_CORE_0));
   Initialize_OVC_Device(Overlay_Set_Core_Index(0,OVC_CORE_1));

   Procon_Deflash.U32 = FLASH.HF_PROCONDF.U32;

//   LMU_Initialize_Device(0);

#ifdef INSTRUCAN_PORT_HW_DETECTION_ENABLED
   // Install Status must be Checked before PORT_DISCRETE_Initialize_Device
   IO_Check_CAN_Installed_Status();
#endif

   PORT_DISCRETE_Initialize_Device(0);


#ifdef INSTRUCAN_PORT_HW_DETECTION_ENABLED

   if(IO_Get_CAN_C_Installed_Status() == false)
   {
      IO_Port_Discrete_Set_Input_No_Pull(MTSA_D_OUT_TXDCANC.Configuration);
   }
   if(IO_Get_CAN_D_Installed_Status() == false)
   {
      IO_Port_Discrete_Set_Input_No_Pull(MTSA_D_OUT_TXDCAND.Configuration);
   }
#endif


   STM_Initialize_Device(STM_Set_Device_Index(0,MAIN_CORE));
   DMA_Initialize_Device(0);

   GTM_Initialize_Device(0);
   GTM_Initialize_Connections(0);

   SPIPort_Initialize(&MTSA_SPI_CHANNEL_A);
   SPIPort_Initialize(&MTSA_SPI_CHANNEL_B);

   SPI_SCHEDULER_Initialize(SPI_SCHEDULER_Message, SPI_SCHEDULER_Message_Numberof);
   SPI_SCHEDULER_Set_Enable(true);

   Check_Hardware_Config();

   VADC_Init();
   DSADC_Initialize_Device(0);

#ifdef BUILD_TSW
   //Initialize DSADC for Testability SW. For app, the initialization will be
   //handled by knock module
   DSADC_Initialize_Channel( MTSA_DSADC_KNOCK.Configuration );
#endif

   SENT_Initialize_Device();

   MSC_Initialize_Device(MSC_Set_Index(0, MSC_INDEX_00));

   InitializePulseInputs();

#ifdef USE_L9301
   /* L9301 EN must set "0" before configuration,RES must be "1" in case of restart */
   L9301_Initialize_Device(L9301_DEVICE_0);
#endif

   Initialize_Diflex();
#ifdef BUILD_TSW
   // Get the Diflex FIRST_SPI_AFT_RST, available only from rxd of the very 1st message
   DIFLEX_FIRST_SPI_AFT_RST_POR = DIFLEX_Msg_Get_Status_First_SPI_Message_After_Reset( DIFLEX_7_WORD_RXD[DIFLEX_DEVICE_0][0] );
#endif

#ifdef BUILD_TSW
   Reset_Status=SCU_RESET_Get_Status();
#endif

#ifdef USE_FS_TEST
#ifndef ESM_ASIL
      // [SSR-1-2-2-10] The Shutoff path test shall not be performed in the event of a running reset
      if ( ( true == KbHWIO_ShutOffPathTest_Enable ) && ( true == Reset_Status.PORST ) )
      {
         FS_ShutOffPathTest();
      }
      else
#endif
#endif

      {
      #ifdef USE_EPIO
         Initialize_EPIO_1P0();
      #endif
      }

      HWIO_External_COP_Service();

#ifdef USE_CALIBRATION_IO
	 /* close Calibratable_IO */
	 Initialize_Calibratable_IO();
#endif
   InitializePulseOutputs();
   InitializePulseTimers();
#ifndef BUILD_TSW
   /* It is in the COD diag process; Set the TOD for true, make it work in key off */
   if (true == Get_WDT_DIAG_RUNNING_STATUS())
   {
      #ifdef USE_EPIO
      IO_DISCRETE_Set_State(&MTSA_D_OUT_TOD_SPI, true);
      #endif
   }
   else
   {
      /* Get EPIO battery low status (include battery disconnect) from first read of REG_OOR Register */
      IO_Battery_Disconnect = IO_DISCRETE_Get_Immediate_State(&MTSA_EPIO_BATT_LOW_STATUS);
   }
#endif

#if ( USE_WRAF_HWIO == 1 )
#ifdef BUILD_TSW
   Initialize_C3WRAF();
#else
   C3WRAF_Init();
#endif
#endif

#ifdef USE_SMP580
   SMP580_Initialize_Device(SMP580_DEVICE_0);
#endif
}

//=============================================================================
// This procedure is the very first to be called after processor
// reset.  It is assumed that all the hardware registers have been
// initialized.
//=============================================================================
void InitializeHardwareFirst(void)
{
#ifdef COMPLEX_IO
   InitializeComplexIO();
#endif

   DIFLEX_Post_Shut_Off_Path_Reinitialize( MTSA_DIFLEX_DEVICE[ 0 ] );

#ifndef BUILD_TSW
   // Must be called after initializing the PMU
   INST_Initialize_Calibration_Pages();
#if 0
   if( KbHWIO_Knock_Cal_Outputs_Enabled )
   {
      GTM_Initialize_Channel_To_Output( WINGATE_ON_MIL_ENABLE, 34, 2 );
//      GTM_Initialize_Channel_To_Output(KNOCK_GATE_ON_WG_ENABLE,73,3 );
   }

   if( KbHWIO_QMIN_Cal_Outputs_Enabled )
   {
      GTM_Initialize_Channel_To_Output( QMINGATE_ON_VCPEX, 24, 2 );
   }
#endif
   //Scan all AD channels one time
   //VADC_Trigger_Autoscan();

   //Enable AD interrupts
   //Initialize_VADC_Interrupts();
#endif
}

extern void Mtu_Initialize_test(void);

//=============================================================================
// InitializeHardwareLast
//=============================================================================
void InitializeHardwareLast( void )
{
    uint32_t spi_time;
    uint32_t periodic_time;
    uint32_t lin_baud_rate_default = 9600UL;

#ifndef BUILD_TSW

   // Initialize CCP
   CCP_Initialize(); // Initialize prior to enabling CAN bus

   //Required for CAN Port
   CAN_Device_Initialize();

   // Initialize CCP Message Objects
   CCP_Message_Init();

   //Initialize EMS messages
   CAN_Reset_Init();

#if 0
   //Initialize SENT channels
   SENT_Initialize_Channel( SENT_PORT_A.Configuration, SENT_PORT_A.pmd_channel);

#ifdef USE_CALIBRATION_IO
   if(CeHAL_SENT_SENSOR_NULL_INDEX < KeHWIO_C38_IF_Configuration)
   {
      SENT_Initialize_Channel( SENT_PORT_B.Configuration, SENT_PORT_B.pmd_channel);
   }

   if(CeHAL_SENT_SENSOR_NULL_INDEX < KeHWIO_C09_IF_Configuration)
   {
      SENT_Initialize_Channel( SENT_PORT_C.Configuration, SENT_PORT_C.pmd_channel);
   }

   if(CeHAL_SENT_SENSOR_NULL_INDEX < KeHWIO_C15_IF_Configuration)
   {
      SENT_Initialize_Channel( SENT_PORT_D.Configuration, SENT_PORT_D.pmd_channel);
   }
#endif
#endif
   SENT_Initialize_Channel( SENT_PORT_A.Configuration, SENT_PORT_A.pmd_channel);
   SENT_Initialize_Channel( SENT_PORT_B.Configuration, SENT_PORT_B.pmd_channel);
   SENT_Initialize_Channel( SENT_PORT_C.Configuration, SENT_PORT_C.pmd_channel);
   SENT_Initialize_Channel( SENT_PORT_D.Configuration, SENT_PORT_D.pmd_channel);
   SENT_Initialize_Channel( SENT_PORT_E.Configuration, SENT_PORT_E.pmd_channel);
   SENT_Initialize_Channel( SENT_PORT_F.Configuration, SENT_PORT_F.pmd_channel);

#else
   SENT_Initialize_Channel( SENT_PORT_A.Configuration, SENT_PORT_A.pmd_channel);
   SENT_Initialize_Channel( SENT_PORT_B.Configuration, SENT_PORT_B.pmd_channel);
   SENT_Initialize_Channel( SENT_PORT_C.Configuration, SENT_PORT_C.pmd_channel);
   SENT_Initialize_Channel( SENT_PORT_D.Configuration, SENT_PORT_D.pmd_channel);
   SENT_Initialize_Channel( SENT_PORT_E.Configuration, SENT_PORT_E.pmd_channel);
   SENT_Initialize_Channel( SENT_PORT_F.Configuration, SENT_PORT_F.pmd_channel);

    // For ASCLIN, SRPN are assigned here, interrupts are enabled in UART_InitializePort()
   CommPort_Initialize ( &ASC_PORT_0 );
   CommPort_Initialize ( &ASC_PORT_1 );

   CommPort_SetSpeed(&ASC_PORT_0,&lin_baud_rate_default);
   CommPort_SetSpeed(&ASC_PORT_1,&lin_baud_rate_default);

#endif

	// CAN interrupt enable
	CAN_Interrupt_Enable();

    IO_PULSE_OC_Set_Time_And_Edge_Immediate( &MTSA_SPI_TIMER,
                                              IO_EDGE_RISING,
                                              IO_TIME_RELATIVE,
                                              MTSA_SPI_DELTA_TIME,
                                              0,
                                              0,
                                              0 );

#ifndef BUILD_TSW

    spi_time = IO_PULSE_OC_Get_Time( &MTSA_SPI_TIMER, IO_EDGE_RISING, 0, 0 );

    periodic_time = spi_time + MTSA_SPI_TO_PERIODIC_DELTA_TIME;

    IO_PULSE_OC_Set_Time_And_Edge_Immediate( &MTSA_PERIODIC_TIMER,
                                          IO_EDGE_RISING,
                                          IO_TIME_ABSOLUTE,
                                          periodic_time,
                                          0,
                                          0,
                                          0 );
#else

    IO_PULSE_OC_Set_Time_And_Edge_Immediate( &MTSA_PERIODIC_TIMER,
                                              IO_EDGE_RISING,
                                              IO_TIME_RELATIVE,
                                              MTSA_PERIODIC_DELTA_TIME,
                                              0,
                                              0,
                                              0 );
#endif

#ifdef USE_LIN_DLL
       // Initialize HWIO LIN
       IO_LIN_Initailize(IO_LIN_PORT_1);
       IO_LIN_Initailize(IO_LIN_PORT_2);
#endif

    Update_SW_Initialization_Time(TIME_RTI_STRT);

   //Enabling watchdogs moved to after secondary core completes init
#ifdef BUILD_TSW
   Enable_Main_Watchdogs();
#endif
#ifndef BUILD_TSW
#ifdef USE_FS_TEST
   /****complete Lockstep test first and then initial SMU in order to clear emulation fault****/
   FS_Check_LockStep_Error();
#endif
#endif

   /* Disable ESR0 & ESR1 trap */
   SCU_Enable_NMI_Trap( SCU_TRAP_ESR0, false );
   SCU_Enable_NMI_Trap( SCU_TRAP_ESR1, false );
   SCU_Set_Reset_Type (SCU_SYS_RESET_WITHOUT_EXT_RESET);
   SCU_Discrete_Initialize(SCU_PC1);


   Mtu_Initialize_Device();
   Mtu_Mic_Enable_Clear_ECC();

   SMU_Initialize_Device(0);

   SCU_Enable_NMI_Trap(SCU_TRAP_SMU,true);

#ifndef BUILD_TSW
   IO_INTERRUPT_Initialize( &MTSA_SMU_INTERRUPT_0, SMU_IR0_PRIORITY, NULL, SMU_IR0_SOURCE);
   IO_INTERRUPT_Set_Enable( &MTSA_SMU_INTERRUPT_0, true );

   SMU_Initialize_test();//clear SMU alarm status

   SMU_RelaseFSP(0);

   if( KfHWIO_HWAlarm_EnableEmulation != HW_SMU_ALARM_EMULATION_ENABLE_PATTERN)
   {
       SMU_Lock_Configurations(0);
       SMU_Start(0);
   }

   //MCU_Initialize_Access_Enable();
#ifdef USE_FS_TEST
   if ( ( true == KbHWIO_ShutOffPathTest_Enable ) && ( true == Reset_Status.PORST ) )
   {
      Update_SOPT_NVM();
   }
#endif
#endif

#ifdef USE_DRV8873
   #ifndef BUILD_TSW
   IO_CPU_Delay(&MTSA_SYSTEM_TIMER, 4, MILLISECOND_RESOLUTION); // waiting for MRIGN
   #endif
   Initialize_DRV8873();
#endif

}

//=============================================================================
// InitializeHardwareLast_SecondCore
//=============================================================================
#ifdef BUILD_TSW
void InitializeHardwareLast_SecondCore( void )
{
#if ( 0 == MAIN_CORE )
	 SCU_WDT_Set_CPU1_Watch_Dog_Timeout(SCU_Convert_MS_To_WDT_Count(WDT_TIMEOUT_MS_RUN));
     SCU_WDT_Enable_CPU1_Watch_Dog_Timer( true);
#endif
#if ( 1 == MAIN_CORE )
	 SCU_WDT_Set_CPU0_Watch_Dog_Timeout(SCU_Convert_MS_To_WDT_Count(WDT_TIMEOUT_MS_RUN));
	 SCU_WDT_Enable_CPU0_Watch_Dog_Timer( true);
#endif
   STM_Initialize_Device(STM_Set_Device_Index(0,SECONDARY_CORE));
   Disable_Interrupts();
}
#endif

//=============================================================================
// STM_Compare_0
//=============================================================================
void STM_Compare_0( void* in_context )
{
   uint32_t spi_time,current_time,delta_time;
   IO_Time_T time_type = IO_TIME_RELATIVE_LAST_EDGE;
   uint8_t  timer_update_interval = 0;

   Parameter_Not_Used(in_context);

   current_time = IO_PULSE_Timer_Get_Value( &MTSA_SPI_TIMER, 0, 0 );

   spi_time = IO_PULSE_OC_Get_Time( &MTSA_SPI_TIMER, IO_EDGE_RISING, 0, 0 );

   //delta time should be a few us
   delta_time = (current_time - spi_time)&0xFFFFFFFF;

   if ( delta_time > MTSA_SPI_DELTA_TIME )
   {
      time_type = IO_TIME_RELATIVE;
   }


   IO_PULSE_OC_Set_Time_And_Edge_Immediate( &MTSA_SPI_TIMER,
                                             IO_EDGE_RISING,
                                             time_type,
                                             MTSA_SPI_DELTA_TIME,
                                             0,
                                             0,
                                             0 );

   IR_INTERRUPT_SW_Request(MTSA_INTERRUPT_SPI_CALLBACK.Configuration);

};

uint32_t OS_Delta_Count =0;
uint32_t OS_Interrupt_Count_Prev =0;
uint16_t OS_Delta_Time =0;
uint32_t OS_Timer_Frequency = STM_FREQUENCY_HZ;
#ifndef BUILD_TSW
//=============================================================================
// STM_Compare_1
//=============================================================================
void STM_Compare_1( void* in_context )
{
    uint32_t spi_time;
    uint32_t periodic_time;
    uint32_t stm1_match_time;

    Parameter_Not_Used(in_context);

    //read current time
#ifdef USE_HW_TEST
    stm1_match_time = IO_PULSE_OC_Get_Time( &MTSA_PERIODIC_TIMER, IO_EDGE_RISING, 0, 0 );
    OS_Delta_Count = stm1_match_time-OS_Interrupt_Count_Prev;
    OS_Interrupt_Count_Prev = stm1_match_time;
#endif
    spi_time = IO_PULSE_OC_Get_Time( &MTSA_SPI_TIMER, IO_EDGE_RISING, 0, 0 );

    periodic_time = spi_time + MTSA_SPI_TO_PERIODIC_DELTA_TIME;

    IO_PULSE_OC_Set_Time_And_Edge_Immediate( &MTSA_PERIODIC_TIMER,
                                              IO_EDGE_RISING,
                                              IO_TIME_ABSOLUTE,
                                              periodic_time,
                                              0,
                                              0,
                                              0 );

   IR_INTERRUPT_SW_Request(MTSA_INTERRUPT_PERIODIC_CALLBACK.Configuration);
};
#else
void STM_Compare_1( void* in_context )
{
    uint32_t current_time;
    uint32_t stm1_match_time;
	uint32_t delta_time;

	IO_Time_T time_type = IO_TIME_RELATIVE_LAST_EDGE;

	Parameter_Not_Used(in_context);


	current_time = IO_PULSE_Timer_Get_Value( &MTSA_PERIODIC_TIMER, 0, 0 );

	stm1_match_time = IO_PULSE_OC_Get_Time( &MTSA_PERIODIC_TIMER, IO_EDGE_RISING, 0, 0 );

	//delta time should be a few us
	delta_time = (current_time - stm1_match_time)&0xFFFFFFFF;

	if ( delta_time > MTSA_PERIODIC_DELTA_TIME )
	{
	   time_type = IO_TIME_RELATIVE;
	}


	IO_PULSE_OC_Set_Time_And_Edge_Immediate( &MTSA_PERIODIC_TIMER,
											  IO_EDGE_RISING,
											  time_type,
											  MTSA_PERIODIC_DELTA_TIME,
											  0,
											  0,
											  0 );

   //IR_INTERRUPT_SW_Request(MTSA_INTERRUPT_PERIODIC_CALLBACK.Configuration);

   TSW_HAL_E_Interrupt_Task();
}
#endif

//=============================================================================
// HWIO_Executive
//=============================================================================
#ifndef BUILD_TSW
void HWIO_Executive(uint8_t loop)
{
   Update_SW_Initialization_Time(TIME_BASELOOP_FIRST);

#ifdef USE_EPIO
   HAL_EPIO_SPI_Fault_Handler();
#endif

#ifdef USE_DIFLEX_ADV_DIAG
   HAL_Diflex_Diag_Manager();
   HAL_Diflex_GetExitInfo();
#endif
#if 1
   #if 0 //ywh: no channel designed for this feature in 
   // VCCL is also used for PDD.The value is not reliable while PDD is in progress.
   if(HAL_ADC_Get_VCCL_Diag_Started()!=true)
   {
      HAL_VADC_REF_Fault_Monitor();
   }
   #endif

  if((KbHWIO_VADC_OffsetGain_Diag_Enable == CbTRUE)&&(loop == VADC_OFFSET_GAIN_DIAG_LOOP_COUNT))
   {
      HAL_VADC_Gain_Fault_Monitor_ADCx(VADC_KERNEL_0);
      HAL_VADC_Gain_Fault_Monitor_ADCx(VADC_KERNEL_1);
      HAL_VADC_Gain_Fault_Monitor_ADCx(VADC_KERNEL_2);
      HAL_VADC_Gain_Fault_Monitor_ADCx(VADC_KERNEL_3);
     // HAL_VADC_Gain_Fault_Monitor_ADCx(VADC_KERNEL_8);
     // HAL_VADC_Offset_Diag();
   }


  // HAL_VADC_Conversion_Time_Fault_Monitor();
#endif

#ifdef USE_HW_TEST//OS_Delta_Time/256 is ms
   OS_Delta_Time = IO_Convert_Count_To_Time(
                      OS_Delta_Count,
                      STM_FREQUENCY_HZ,
                      S_MILLISECONDSe, MILLISECOND_RESOLUTION);
#endif

   CCP_Port_BusOffRecovery();

#if 0
   if ( loop == 3 )
   {
      PCFS_HWIO_Task();
   }
#endif
   IO_DFX_VBSTVI_Update_10ms = IO_ANALOG_Get_Buffered_Value(&MTSA_ANALOG_BUFFERED_VHV_BST_AI);
   HAL_Diflex_Errata_Handler();
   #ifdef USE_SMP580
   SMP580_FAULT_Diagnose_Fault( MTSA_SMP580_Fault.Configuration );
   HAL_SMP580_SPI_Fault_Handler();
   #endif

   #ifdef USE_DRV8873_SPI_DIAG
   HAL_DRV8873_SPI_Fault_Handler();
   #endif

   #ifdef USE_L9301
   L9301_Fault_Diagnose_Channels();
   HAL_L9301_SPI_Fault_Handler();
   #endif

   HAL_QSPI0_Fault_Handler();
   HAL_QSPI1_Fault_Handler();

#if ( USE_WRAF_HWIO == 1 )
   HAL_C3WRAF_SPI_Fault_Handler();
   WRAF_Scheduler();
#endif

}
#endif

/* ****************************************************************************************************
* Function Name:         Check_Hardware_Config
***************************************************************************************************** */
#ifndef INSTRUCAN_PORT_HW_DETECTION_ENABLED
Hardware_Installed_T Hardware_Installed_Version;
#endif
void Check_Hardware_Config(void)
{
   uint8_t  sparkcfg, bridgecfg, cylcfg;
   uint16_t AD_HWTAGVI_Raw_Value;
   bool     mot2_state = false;
   bool     mot3_state = false;
   bool     mot4_state = false;
   bool     mot5_state = false;

#if 1 // detect installed 8873 by using hardware
#ifdef USE_DRV8873
   IO_DISCRETE_Set_Immediate_State( &MTSA_D_OUT_MOTOR2_DISABLE, true );
   IO_DISCRETE_Set_Immediate_State( &MTSA_D_OUT_MOTOR3_DISABLE, true );
   IO_DISCRETE_Set_Immediate_State( &MTSA_D_OUT_MOTOR4_DISABLE, true );
   IO_DISCRETE_Set_Immediate_State( &MTSA_D_OUT_MOTOR5_DISABLE, true );

   IO_DISCRETE_Set_Channel_Direction( &MTSA_D_OUT_MOTOR2_DISABLE, IO_DATA_IN );
   IO_DISCRETE_Set_Channel_Direction( &MTSA_D_OUT_MOTOR3_DISABLE, IO_DATA_IN );
   IO_DISCRETE_Set_Channel_Direction( &MTSA_D_OUT_MOTOR4_DISABLE, IO_DATA_IN );
   IO_DISCRETE_Set_Channel_Direction( &MTSA_D_OUT_MOTOR5_DISABLE, IO_DATA_IN );

   mot2_state = IO_DISCRETE_Get_Immediate_State(&MTSA_D_OUT_MOTOR2_DISABLE);
   mot3_state = IO_DISCRETE_Get_Immediate_State(&MTSA_D_OUT_MOTOR3_DISABLE);
   mot4_state = IO_DISCRETE_Get_Immediate_State(&MTSA_D_OUT_MOTOR4_DISABLE);
   mot5_state = IO_DISCRETE_Get_Immediate_State(&MTSA_D_OUT_MOTOR5_DISABLE);

   Hardware_Installed_Version.F.TRV8873_0_Installed = 1;
   Hardware_Installed_Version.F.TRV8873_1_Installed = mot2_state;
   Hardware_Installed_Version.F.TRV8873_2_Installed = mot3_state;
   Hardware_Installed_Version.F.TRV8873_3_Installed = mot4_state;
   Hardware_Installed_Version.F.TRV8873_4_Installed = mot5_state;


   IO_DISCRETE_Set_Channel_Direction( &MTSA_D_OUT_MOTOR2_DISABLE, IO_DATA_OUT );
   IO_DISCRETE_Set_Channel_Direction( &MTSA_D_OUT_MOTOR3_DISABLE, IO_DATA_OUT );
   IO_DISCRETE_Set_Channel_Direction( &MTSA_D_OUT_MOTOR4_DISABLE, IO_DATA_OUT );
   IO_DISCRETE_Set_Channel_Direction( &MTSA_D_OUT_MOTOR5_DISABLE, IO_DATA_OUT );

   HWCfg_Motor2To5_Installed = ( ( mot5_state == true ) << 3) |\
                               ( ( mot4_state == true ) << 2) |\
                               ( ( mot3_state == true ) << 1) |\
                                 ( mot2_state == true );

   HWCfg_NumMotor2To5_Installed = ( (mot2_state == true ) ? 1 : 0 ) +\
                                  ( (mot3_state == true ) ? 1 : 0 ) +\
                                  ( (mot4_state == true ) ? 1 : 0 ) +\
                                  ( (mot5_state == true ) ? 1 : 0 );

   NUMBER_OF_DRV8873_DAISY_CHAIN_IN_RUNTIME_AFTER_CHECKED = 1 + HWCfg_NumMotor2To5_Installed;
#endif
#else // detect installed 8873 by using spi. Note: this method should be peformed after MPR ready (make sure V_8873 ready)
	DRV8873_Read_Device_Message(DRV8873_DEVICE_CONFIG_INIT_0);
    Check_DRV8873_Config_Using_SPI();
#endif
}

/* ****************************************************************************************************
* Function Name:         Get_Hardware_Version
***************************************************************************************************** */
Hardware_Version_T Get_Hardware_Version (void)
{
   return HW_Version;
}

/* ****************************************************************************************************
* Function Name:         Initialize_Main_Watchdogs
***************************************************************************************************** */
void Initialize_Main_Watchdogs( void )
{
   // Disable CPU0 and Safety watchdog for CDU
   // Set the watchdog timeout for 400ms during initialization
#if ( 0 == MAIN_CORE )
	   SCU_WDT_Set_CPU0_Watch_Dog_Timeout(SCU_Convert_MS_To_WDT_Count(WDT_TIMEOUT_MS_INIT));
	   SCU_WDT_Enable_CPU0_Watch_Dog_Timer( false);
#endif
#if ( 1 == MAIN_CORE )
	   SCU_WDT_Set_CPU1_Watch_Dog_Timeout(SCU_Convert_MS_To_WDT_Count(WDT_TIMEOUT_MS_INIT));
	   SCU_WDT_Enable_CPU1_Watch_Dog_Timer( false);
#endif

   SCU_WDT_Set_Safety_Watch_Dog_Timeout( SCU_Convert_MS_To_WDT_Count(WDT_TIMEOUT_MS_INIT));
   SCU_WDT_Enable_Safety_Watch_Dog_Timer( false);

}

/* ****************************************************************************************************
* Function Name:         Enable_Main_Watchdogs
***************************************************************************************************** */
void Enable_Main_Watchdogs(void)
{
   //Going to start the OS. Set the WDT to desired time out value
#if ( 0 == MAIN_CORE )
		  SCU_WDT_Set_CPU0_Watch_Dog_Timeout(SCU_Convert_MS_To_WDT_Count(WDT_TIMEOUT_MS_RUN));
		  SCU_WDT_Enable_CPU0_Watch_Dog_Timer( true);
#endif
#if ( 1 == MAIN_CORE )
		  SCU_WDT_Set_CPU1_Watch_Dog_Timeout(SCU_Convert_MS_To_WDT_Count(WDT_TIMEOUT_MS_RUN));
		  SCU_WDT_Enable_CPU1_Watch_Dog_Timer( true);
#endif

   SCU_WDT_Set_Safety_Watch_Dog_Timeout( SCU_Convert_MS_To_WDT_Count(WDT_TIMEOUT_MS_RUN));
   SCU_WDT_Enable_Safety_Watch_Dog_Timer( true);

#ifdef ETC_SOH_HAL
   SOH_ETC_Initialize();
#endif
}

/* ****************************************************************************************************
* Function Name:         Update_SW_Initialization_Time
***************************************************************************************************** */
void Update_SW_Initialization_Time( Measure_Time_T in_type )
{
   if(in_type == TIME_HWIO_INIT_STRT)
   {
       SW_Init_Time[in_type] = GetIO_CPU_System_TimerCount();
   }
   else if(in_type == TIME_APP_INIT_START)
   {
       SW_Init_Time[TIME_APP_INIT_START] = GetIO_CPU_System_TimerCount()-SW_Init_Time[TIME_HWIO_INIT_STRT];
   }
   else if(in_type == TIME_RTI_STRT)
   {
       SW_Init_Time[TIME_RTI_STRT] = GetIO_CPU_System_TimerCount()-SW_Init_Time[TIME_HWIO_INIT_STRT];
   }
   else if(in_type == TIME_INIT_END)
   {
       SW_Init_Time[TIME_INIT_END] = GetIO_CPU_System_TimerCount()-SW_Init_Time[TIME_HWIO_INIT_STRT];
   }
   else if((in_type == TIME_RTI_FIRST)&&(SW_Init_Time[TIME_RTI_FIRST]==0))
   {
       SW_Init_Time[TIME_RTI_FIRST] = GetIO_CPU_System_TimerCount()-SW_Init_Time[TIME_HWIO_INIT_STRT];
   }
   else if((in_type == TIME_BASELOOP_FIRST)&&(SW_Init_Time[TIME_BASELOOP_FIRST]==0))
   {
       SW_Init_Time[TIME_BASELOOP_FIRST] = GetIO_CPU_System_TimerCount()-SW_Init_Time[TIME_HWIO_INIT_STRT];
   }
   else if((in_type == TIME_SPI_FIRST)&&(SW_Init_Time[TIME_SPI_FIRST]==0))
   {
       SW_Init_Time[TIME_SPI_FIRST] = GetIO_CPU_System_TimerCount()-SW_Init_Time[TIME_HWIO_INIT_STRT];
   }
   else
   {

   }
}

/* ****************************************************************************************************
* Function Name:         Initialize_Diflex
***************************************************************************************************** */
void Initialize_Diflex( void )
{
   /*
    *   For EDU, this is under discrete control.
    *   Setting it to false as there is no FSM to govern it in miniapp.
    */
    IO_DISCRETE_Set_Immediate_State( &MTSA_D_IN_DIFELX_FSAFE_IO_B, false );

   DIFLEX_SPI_Initialize();
   DIFLEX_Initialize_Device( MTSA_DIFLEX_DEVICE[ 0 ] );

#ifdef USE_DIFLEX_ADV_DIAG
   //Need to review and get it at the right place
   DIFLEX_diagFunctions_initialize();
#endif
}

#ifdef USE_EPIO
/* ****************************************************************************************************
* Function Name:         Initialize_EPIO_1P0
***************************************************************************************************** */
uint8_t EPIO_VERSION;
void Initialize_EPIO_1P0(void)
{
#ifdef BUILD_TSW
   EPIO_EEPROM_CHECK();
#endif

#ifndef BUILD_TSW
   EPIO_VERSION=EPIO_Get_Immediate_Versn_Value(0);
   if(EPIO_VERSION==0)//EPIO 1.0
   {
	   //disable this operation in tsw, because it may influece LIN test in manufacture side
	   //but it is needed to save pncan related data, so it should be peformed if the pncan wkup needs to be test
	   EPIO_Fix_Core_Reset_Errta_1p0();
   }
#endif

   Initial_Common_Set_EPIO();

   //Set FSE_ENABLE high for EPIO - Check when to set
   IO_DISCRETE_Set_State( &MTSA_D_OUT_FSE_ENABLE, true );

   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CONFIG_MAIN);        /* SDI 2 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CONFIG_VRCC);        /* SDI 3 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CONFIG_CR);          /* SDI 4 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CONFIG_HMPW);        /* SDI 5 */

   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CAN2_D_LIN2);		/* SDI 15 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CAN2_C); 			/* SDI 14 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CAN2_B); 			/* SDI 13 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CAN2_A); 			/* SDI 12 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CAN1_D_LIN1);		/* SDI 11 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CAN1_C); 			/* SDI 10 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CAN1_B); 			/* SDI 9 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CAN1_A); 			/* SDI 8 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_IO_MODE);            /* SDI 141 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_OUT_CONFIG);         /* SDI 142 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_IGBT);               /* SDI 140 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_HSPI);               /* SDI 148 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_SKIP_HORN);          /* SDI 149 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_FAULT_LEVEL);        /* SDI 137 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_FILTER);             /* SDI 138 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_FILTER_PAIRED);      /* SDI 150 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_HSPI_ORDER_SELECT1); /* SDI 151 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_HSPI_ORDER_SELECT2); /* SDI 152 */

   EPIO_COP_Service_Immediate();

   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_IO_MODE);            /* SDI 141 */

   EPIO_SOH_EXECUTE_ONE_CnR_CYCLE();
   EPIO_SOH_EXECUTE_ONE_CnR_CYCLE();

   EPIO_COP_Service_Immediate();

   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_SLEW);               /* SDI 136 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_DEPS);               /* SDI 144 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_PWM_1);              /* SDI 134 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_PWM_2);              /* SDI 135 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_PCH);                /* SDI 132 */
   //EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_LPC_DC);           /* SDI 7 *///no need initial LPC every time
   //EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_LPC_WK);           /* SDI 6 *///no need initial LPC every time
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_DIAG_MODE);          /* SDI 146 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_ONSTATE_DIAG);       /* SDI 147 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CAL_REQUREST);       /* SDI 143 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_IGNITION_TIME);      /* SDI 139 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_EST);                /* SDI 145 */

   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CONFIG_MAIN);	/* SDO 2 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CONFIG_VRCC);	/* SDO 3 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CONFIG_CR); 	/* SDO 4 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CONFIG_HMPW);	/* SDO 5 */

   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CAN1_A);       /* SDO 8 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CAN1_B);       /* SDO 9 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CAN1_C);       /* SDO 10 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CAN1_D_LIN1);  /* SDO 11 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CAN2_A);       /* SDO 12 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CAN2_B);       /* SDO 13 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CAN2_C);       /* SDO 14 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CAN2_D_LIN2);  /* SDO 15 */

   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_IO_MODE);		/* SDO 141 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_FSE_CR_SOH); 		/* SDO 191 */

   EPIO_SPI_Immediate_Transfer( 0, EPIO_MESSAGE_READ_USER_1 ); 		/* SDO 20 */
   EPIO_SPI_Immediate_Transfer( 0, EPIO_MESSAGE_READ_USER_2 ); 		/* SDO 21 */
   EPIO_SPI_Immediate_Transfer( 0, EPIO_MESSAGE_READ_USER_3 ); 		/* SDO 22 */
   EPIO_SPI_Immediate_Transfer( 0, EPIO_MESSAGE_READ_USER_4 ); 		/* SDO 23 */
   EPIO_SPI_Immediate_Transfer( 0, EPIO_MESSAGE_READ_USER_5 ); 		/* SDO 24 */

#ifndef BUILD_TSW
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_FAULT_READ_IO_MODE);
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_FAULT_READ_SOH_STATUS);

   EPIO_FAULT_Initialize_Device(0);
#endif
}

/* ****************************************************************************************************
* Function Name:         Initialize_EPIO_PreShutOffTest
***************************************************************************************************** */
void Initialize_EPIO_PreShutOffTest( void )
{
#ifdef BUILD_TSW
   EPIO_EEPROM_CHECK();
#endif

#ifndef BUILD_TSW
   EPIO_VERSION=EPIO_Get_Immediate_Versn_Value(0);
   if(EPIO_VERSION==0)//EPIO 1.0
   {
       //disable this operation in tsw, because it may influece LIN test in manufacture side
       //but it is needed to save pncan related data, so it should be peformed if the pncan wkup needs to be test
       EPIO_Fix_Core_Reset_Errta_1p0();
   }
#endif

   Initial_Common_Set_EPIO();

   //Set FSE_ENABLE high for EPIO - Check when to set
   //IO_DISCRETE_Set_State( &MTSA_D_OUT_FSE_ENABLE, true );

   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CONFIG_MAIN);        /* SDI 2 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CONFIG_VRCC);        /* SDI 3 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CONFIG_CR);          /* SDI 4 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CONFIG_HMPW);        /* SDI 5 */

   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CAN2_D_LIN2);		/* SDI 15 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CAN2_C); 			/* SDI 14 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CAN2_B); 			/* SDI 13 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CAN2_A); 			/* SDI 12 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CAN1_D_LIN1);		/* SDI 11 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CAN1_C); 			/* SDI 10 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CAN1_B); 			/* SDI 9 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CAN1_A); 			/* SDI 8 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_IO_MODE);            /* SDI 141 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_OUT_CONFIG);         /* SDI 142 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_IGBT);               /* SDI 140 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_HSPI);               /* SDI 148 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_SKIP_HORN);          /* SDI 149 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_FAULT_LEVEL);        /* SDI 137 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_FILTER);             /* SDI 138 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_FILTER_PAIRED);      /* SDI 150 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_HSPI_ORDER_SELECT1); /* SDI 151 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_HSPI_ORDER_SELECT2); /* SDI 152 */

   EPIO_COP_Service_Immediate();

   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_IO_MODE);            /* SDI 141 */
}

/* ****************************************************************************************************
* Function Name:         Initialize_EPIO_PostShutOffTest
***************************************************************************************************** */
void Initialize_EPIO_PostShutOffTest( void )
{
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_SLEW);               /* SDI 136 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_DEPS);               /* SDI 144 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_PWM_1);              /* SDI 134 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_PWM_2);              /* SDI 135 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_PCH);                /* SDI 132 */
   //EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_LPC_DC);           /* SDI 7 *///no need initial LPC every time
   //EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_LPC_WK);           /* SDI 6 *///no need initial LPC every time
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_DIAG_MODE);          /* SDI 146 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_ONSTATE_DIAG);       /* SDI 147 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_CAL_REQUREST);       /* SDI 143 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_IGNITION_TIME);      /* SDI 139 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_EST);                /* SDI 145 */

   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CONFIG_MAIN);	/* SDO 2 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CONFIG_VRCC);	/* SDO 3 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CONFIG_CR); 	/* SDO 4 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CONFIG_HMPW);	/* SDO 5 */

   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CAN1_A);       /* SDO 8 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CAN1_B);       /* SDO 9 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CAN1_C);       /* SDO 10 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CAN1_D_LIN1);  /* SDO 11 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CAN2_A);       /* SDO 12 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CAN2_B);       /* SDO 13 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CAN2_C);       /* SDO 14 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_CAN2_D_LIN2);  /* SDO 15 */

   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_READ_IO_MODE);		/* SDO 141 */
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_FSE_CR_SOH); 		/* SDO 191 */

   EPIO_SPI_Immediate_Transfer( 0, EPIO_MESSAGE_READ_USER_1 ); 		/* SDO 20 */
   EPIO_SPI_Immediate_Transfer( 0, EPIO_MESSAGE_READ_USER_2 ); 		/* SDO 21 */
   EPIO_SPI_Immediate_Transfer( 0, EPIO_MESSAGE_READ_USER_3 ); 		/* SDO 22 */
   EPIO_SPI_Immediate_Transfer( 0, EPIO_MESSAGE_READ_USER_4 ); 		/* SDO 23 */
   EPIO_SPI_Immediate_Transfer( 0, EPIO_MESSAGE_READ_USER_5 ); 		/* SDO 24 */

#ifndef BUILD_TSW
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_FAULT_READ_IO_MODE);
   EPIO_SPI_Immediate_Transfer(0, EPIO_MESSAGE_FAULT_READ_SOH_STATUS);

   EPIO_FAULT_Initialize_Device(0);
#endif
}


#endif

