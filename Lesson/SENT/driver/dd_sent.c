#include "dd_sent.h"
#include "dd_sent_init.h"

#define SECTION_SENT_REGISTER
#include "section.h"
SENT_T SENT;
#define SECTION_END
#include "section.h"

/*******************************************************************************
 *  Static Variables
 *******************************************************************************/
static HWI_SENT_DG_ENUM_TYPE HWI_Sent_diag_status[USED_SENT_CHANNELS];
static uint32_t HWI_Sent_InterruptStatus[USED_SENT_CHANNELS];
static uint16_t HWI_Sent_msg_lost_cnt[USED_SENT_CHANNELS];
static uint32_t HWI_Sent_RxInt_TimeStamp[USED_SENT_CHANNELS];
static uint8_t  SENT_HW2USED_Map[HWI_SENT_CHANNEL_MAX] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

/*******************************************************************************
 *  Global Variables
 *******************************************************************************/
uint16_t FtsSentData[USED_SENT_CHANNELS][SENT_DATA_SIZE];
uint16_t FtsSerialData[USED_SENT_CHANNELS][SENT_DATA_SIZE];

/*******************************************************************************
 *
 *  Function Name   :    HWI_Sent_Init
 *  Model Reference :
 *  Description     :
 *
 *******************************************************************************/
void SENT_Initialize_Device( void )
{
   SENT_T *sent;
   uint32_t interrupt_context;

   sent = &SENT;
   interrupt_context = SCU_Unprotect_System_ENDINIT();

   /* Enable the module */
   sent->CLC.U32 = SENT_INITIAL_CLC.U32;

   while( sent->CLC.F.DISS == 1 )
   {
      //Wait till module is enabled.
   }

   /* configure global SENT clock module */
   sent->FDR.F.DM = HWI_SENT_FDR_DIV_MODE;
   sent->FDR.F.STEP = HWI_SENT_FDR_STEP;

   /* Re-lock Endinit protection */
   SCU_Protect_System_ENDINIT(interrupt_context);
}

void SENT_Initialize_Channel( const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr, uint8_t sent_channel )
{
   SENT_T *sent;
   HWI_SENT_CHANNEL_TYPE hwi_sent_channel;
   HWI_SENT_CFG_CHANNEL_STRUCT_TYPE* l_sent_channel_cfg_ptr;
   uint32_t hwi_sent_clock_tick_duration;
   HWI_SENT_VERSION_ENUM_TYPE hwi_sent_version;
   HWI_SENT_SERIAL_FRAME_ENUM_TYPE hwi_sent_serial_method;
   HWI_SENT_CRC_AUGMENTATION_ENUM_TYPE hwi_sent_crc_type;
   bool hwi_sent_serial_frame_req;
   bool hwi_sent_pause_enable;
   SENT_INTEN_T ie;
   bool erri;
   uint32_t hwi_sent_wdt_timer;

   sent = &SENT;
   hwi_sent_channel             = hwi_sent_channel_ptr->HWI_SENT_CHANNEL_NUMBER;
   l_sent_channel_cfg_ptr       = hwi_sent_channel_ptr->HWI_SENT_CHANNEL_CONFIG;
   hwi_sent_clock_tick_duration = l_sent_channel_cfg_ptr->HWI_SENT_CLOCK_TICK_DURATION / 1000; /* convert to microsecond */
   hwi_sent_version             = hwi_sent_channel_ptr->HWI_SENT_VERSION;
   hwi_sent_serial_method       = l_sent_channel_cfg_ptr->HWI_SENT_SERIAL_FRAME_METHOD;
   hwi_sent_serial_frame_req    = l_sent_channel_cfg_ptr->HWI_SENT_SERIAL_FRAME_REQ;
   hwi_sent_pause_enable        = l_sent_channel_cfg_ptr->HWI_SENT_ENDPULSE_IGNORE;
   hwi_sent_wdt_timer           = l_sent_channel_cfg_ptr->HWI_SENT_WDT_MAX_COUNTER;
   hwi_sent_crc_type            = l_sent_channel_cfg_ptr->HWI_SENT_CRC_IMPLEMENT_TYPE;
   SENT_HW2USED_Map[hwi_sent_channel] = sent_channel;

   sent->Channel[hwi_sent_channel].IOCR.F.ALTI = l_sent_channel_cfg_ptr->HWI_SENT_ALT_IN;
   /* active high */
   sent->Channel[hwi_sent_channel].IOCR.F.IIE = l_sent_channel_cfg_ptr->HWI_SENT_POLARITY;

   HWI_Sent_diag_status[sent_channel] = HWI_SENT_DG_NO_ERROR;

   /* disable SENT channel and clear buffer before set up channel clock */
   sent->Channel[hwi_sent_channel].RCR.U32 = HWI_RCR_CLEAR;

   /* configure clock channel pre-divider*/
   sent->Channel[hwi_sent_channel].CPDR.U32 = HWI_SENT_CPDR_GEN_CONF(HWI_SENT_CPDR_PDIV(hwi_sent_clock_tick_duration));

   /* configure clock channel fractionnal-divider*/
   sent->Channel[hwi_sent_channel].CFDR.U32 |= HWI_SENT_CFDR_DEF_CONF;

   switch (hwi_sent_version)
   {
      case HWI_SENT_V2008:
      /* NOT SUPPORTED */
      /* break; */

      case HWI_SENT_V2010:
      default:
         sent->Channel[hwi_sent_channel].RCR.U32 = SENT_INITIAL_RCR[sent_channel].U32;
      break;
   }

   // These are configurable during runtime initializations
   sent->Channel[hwi_sent_channel].RCR.F.IEP = hwi_sent_pause_enable;
   sent->Channel[hwi_sent_channel].RCR.F.SDP = hwi_sent_serial_frame_req;
   sent->Channel[hwi_sent_channel].RCR.F.ESF = hwi_sent_serial_method;
   sent->Channel[hwi_sent_channel].RCR.F.CRZ = hwi_sent_crc_type;

   /* Enable watch dog timer : frame timeout -> no refresh | lost message detection */
   /* WDT in ticks */
   sent->Channel[hwi_sent_channel].WDT.U32 = (HWI_SENT_MAX_TICK + (hwi_sent_wdt_timer/hwi_sent_clock_tick_duration)) & HWI_BITS_MASK(BIT_0, 16);

   /* enable SENT channel */
   sent->Channel[hwi_sent_channel].RCR.F.CEN = true;

   /* clear ISR status */
   sent->Channel[hwi_sent_channel].INTCLR.U32 = HWI_SENT_CLEAR_ISR_STATUS;
   sent->Channel[hwi_sent_channel].VIEW.U32 = HWI_SENT_NIBBLE_ORDER;

   /* Enable Isr */
   sent->Channel[hwi_sent_channel].INTEN.U32 = SENT_INITIAL_INTEN[sent_channel].U32;
   sent->Channel[hwi_sent_channel].INTEN.F.RDI = 1;
   if (true == hwi_sent_serial_frame_req)
   {
      sent->Channel[hwi_sent_channel].INTEN.F.SDI = 1;
      sent->Channel[hwi_sent_channel].INTEN.F.SCRI = 1;
      if ( HWI_SENT_SHORT_ID == hwi_sent_serial_method )
      {
         sent->Channel[hwi_sent_channel].INTEN.F.WSI = 1;
      }
   }

   /* Set up Interrupt Node Pointer */
   ie = sent->Channel[hwi_sent_channel].INTEN;
   erri =(bool) ( ie.F.FRI || ie.F.FDI || ie.F.NNI || ie.F.NVI || ie.F.CRCI || ie.F.WSI || ie.F.SCRI );
   sent->Channel[hwi_sent_channel].INP.U32 = (   ( ie.F.RSI ? sent_channel : 0xF )         |
                                               ( ( ie.F.RDI ? sent_channel : 0xF ) << 4 )  |
                                               ( ( ie.F.RBI ? sent_channel : 0xF ) << 8 )  |
                                               ( ( ie.F.TDI ? sent_channel : 0xF ) << 12 ) |
                                               ( ( ie.F.TBI ? sent_channel : 0xF ) << 16 ) |
                                               ( ( erri     ? sent_channel : 0xF ) << 20 ) |
                                               ( ( ie.F.SDI ? sent_channel : 0xF ) << 24 ) |
                                               ( ( ie.F.WDI ? sent_channel : 0xF ) << 28 ) );
}

/*******************************************************************************
 *
 *  Function Name   :    HWI_Sent_Fast_Read
 *  Model Reference :
 *  Description     : Used to read basic frame
 *
 *******************************************************************************/
HWI_SENT_DG_ENUM_TYPE HWI_Sent_Fast_Read( const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr,
                                                                    uint16_t* hwi_sent_data1_ptr,
                                                                    uint16_t* hwi_sent_data2_ptr )
{
   SENT_T *sent;
   HWI_SENT_DG_ENUM_TYPE l_hwi_sent_error;
   HWI_SENT_CHANNEL_TYPE hwi_sent_channel;
   SENT_CHAN_TYPE sent_channel;

   sent = &SENT;
   hwi_sent_channel = hwi_sent_channel_ptr->HWI_SENT_CHANNEL_NUMBER;
   *hwi_sent_data1_ptr = HWI_SENT_READ_DATA_LSB(sent->RDR[hwi_sent_channel].U32);
   sent_channel =(SENT_CHAN_TYPE) SENT_HW2USED_Map[hwi_sent_channel];

   if (hwi_sent_data2_ptr != NULL)
   {
      /* Otherwise not requested by the application */
      *hwi_sent_data2_ptr = HWI_SENT_READ_DATA_MSB(sent->RDR[hwi_sent_channel].U32);
   }
   else { /*NTD*/ }

   l_hwi_sent_error = HWI_Sent_diag_status[sent_channel];

   return l_hwi_sent_error;
}

/*******************************************************************************
 *
 *  Function Name   :    HWI_Sent_Slow_Read
 *  Model Reference :
 *  Description     : Used to read serial data : standard - Enhanced frame
 *
 *******************************************************************************/
HWI_SENT_DG_ENUM_TYPE HWI_Sent_Slow_Read( const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr,
                                                                    uint16_t* hwi_sent_id_ptr,
                                                                    uint16_t* hwi_sent_data_ptr )
{
   SENT_T *sent;
   HWI_SENT_CHANNEL_TYPE hwi_sent_channel;
   HWI_SENT_DG_ENUM_TYPE l_hwi_sent_error;
   SENT_CHAN_TYPE sent_channel;

   sent = &SENT;
   hwi_sent_channel = hwi_sent_channel_ptr->HWI_SENT_CHANNEL_NUMBER;
   sent_channel =(SENT_CHAN_TYPE) SENT_HW2USED_Map[hwi_sent_channel];

   /* Read serial ID */
   * hwi_sent_id_ptr = HWI_SENT_READ_S_IDCRC(sent->Channel[hwi_sent_channel].SDS.U32);

   /* Read serial Data */
   *hwi_sent_data_ptr = HWI_SENT_READ_S_DATA(sent->Channel[hwi_sent_channel].SDS.U32);
   l_hwi_sent_error = HWI_Sent_diag_status[sent_channel];

   return l_hwi_sent_error;
}

/*******************************************************************************
 *  Local Task Function Definition
 *******************************************************************************/
/********************************************************************
 *
 *   MAIN FUNCTION:  SENT_Process_Receive_Interrupt()
 *
 *   RETURN VALUE:  void
 *
 *   PARAMETERS:  void
 *   ABSTRACT:
 *   Description: Sent Error handler
 ********************************************************************/
void SENT_Process_Receive_Interrupt( SENT_CHAN_TYPE in_sent_channel,  HIODEVICE io_sent_device)
{
   SENT_T *sent;
   SENT_CHAN_TYPE                sent_channel         = in_sent_channel;
   HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr = (HWI_SENT_CHANNEL_STRUCT_TYPE*)io_sent_device;
   HWI_SENT_CHANNEL_TYPE         hwi_sent_channel     = hwi_sent_channel_ptr->HWI_SENT_CHANNEL_NUMBER;

   HWI_SENT_DG_ENUM_TYPE l_hwi_sent_error;

   sent = &SENT;
   l_hwi_sent_error = HWI_SENT_DG_NO_ERROR;

   /* Check if channel x rise an enabled Isr */
   if ( ( ( sent->Channel[hwi_sent_channel].INTSTAT.U32 ) & ( sent->Channel[hwi_sent_channel].INTEN.U32 ) ) != 0x00000000 )
   {
      HWI_Sent_InterruptStatus[sent_channel] |= sent->Channel[hwi_sent_channel].INTSTAT.U32;
      if ( ( ( sent->Channel[hwi_sent_channel].INTSTAT.F.WDI == 1 ) && ( sent->Channel[hwi_sent_channel].INTEN.F.WDI == 1 ) ))
      {
         if( HWI_Sent_msg_lost_cnt[sent_channel] < HWI_SENT_MSG_LOST_CPV )
         {
            l_hwi_sent_error = HWI_SENT_DG_NO_REFRESHED_FRAME;
            HWI_Sent_msg_lost_cnt[sent_channel]++;
         }
         else
         {
            sent->Channel[hwi_sent_channel].INTEN.F.WDI = 0;
            l_hwi_sent_error = HWI_SENT_DG_LOST_SIGNAL;
         }
      }
      else if ( sent->Channel[hwi_sent_channel].INTSTAT.F.RDI == 1 )
      {
         HWI_Sent_RxInt_TimeStamp[sent_channel] = 0;
         sent->Channel[hwi_sent_channel].INTEN.F.WDI = 1;
         HWI_Sent_msg_lost_cnt[sent_channel] = 0;
         HWI_Sent_Fast_Read( hwi_sent_channel_ptr, &FtsSentData[sent_channel][0], &FtsSentData[sent_channel][1] );
         HWI_Sent_Slow_Read( hwi_sent_channel_ptr, &FtsSerialData[sent_channel][0], &FtsSerialData[sent_channel][1] );
      }
      else
      {
         if ( sent->Channel[hwi_sent_channel].INTSTAT.F.FRI == 1 )
         {
            l_hwi_sent_error = HWI_SENT_DG_CALPULSE_DRIFT;
         }
         else if ( sent->Channel[hwi_sent_channel].INTSTAT.F.FDI == 1 )
         {
            l_hwi_sent_error = HWI_SENT_DG_CLOCK_DRIFT;
         }
         else if ( sent->Channel[hwi_sent_channel].INTSTAT.F.NNI == 1 )
         {
            l_hwi_sent_error = HWI_SENT_DG_N_NIBBLE_ERROR;
         }
         else if ( sent->Channel[hwi_sent_channel].INTSTAT.F.NVI == 1 )
         {
            l_hwi_sent_error = HWI_SENT_DG_NIBBLE_DATA_ERROR;
         }
         else if ( sent->Channel[hwi_sent_channel].INTSTAT.F.CRCI == 1 )
         {
            l_hwi_sent_error = HWI_SENT_DG_CRC_ERROR;
         }
         else if ( ( sent->Channel[hwi_sent_channel].INTSTAT.F.SCRI == 1 ) || ( sent->Channel[hwi_sent_channel].INTSTAT.F.WSI == 1 ) )
         {
            l_hwi_sent_error = HWI_SENT_DG_ERROR;
         }
         else {/*NTD*/}
      }
   }

   /* clear ISR status */
   sent->Channel[hwi_sent_channel].INTCLR.U32 = 0x00003FFF;

   HWI_Sent_diag_status[sent_channel] = l_hwi_sent_error;
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Get_Data                                                         ****/
/****       $Content: Return a pointer to the received data buffer                           ****/
/****                                                                                         ****/
/****       $argument: const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr             ****/
/****       $return value: uint16_t* - Pointer to data buffer                               ****/
/*************************************************************************************************/
/*************************************************************************************************/
uint16_t* SENT_Get_Data(const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr)
{
   HWI_SENT_CHANNEL_TYPE hwi_sent_channel;
   uint16_t *data_ptr;
   SENT_CHAN_TYPE sent_channel;

   hwi_sent_channel = hwi_sent_channel_ptr->HWI_SENT_CHANNEL_NUMBER;
   sent_channel =(SENT_CHAN_TYPE) SENT_HW2USED_Map[hwi_sent_channel];
   data_ptr = &FtsSentData[sent_channel][0];

   return data_ptr;
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Get_SerialData                                                   ****/
/****       $Content: Return a pointer to the received serial data buffer                    ****/
/****                                                                                         ****/
/****       $argument: const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr             ****/
/****       $return value: uint16_t* - Pointer to serial data buffer                        ****/
/*************************************************************************************************/
/*************************************************************************************************/
uint16_t* SENT_Get_SerialData(const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr)
{
   HWI_SENT_CHANNEL_TYPE hwi_sent_channel;
   uint16_t *data_ptr;
   SENT_CHAN_TYPE sent_channel;

   hwi_sent_channel = hwi_sent_channel_ptr->HWI_SENT_CHANNEL_NUMBER;
   sent_channel =(SENT_CHAN_TYPE) SENT_HW2USED_Map[hwi_sent_channel];
   data_ptr = &FtsSerialData[sent_channel][0];

   return data_ptr;
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Get_CRC                                                          ****/
/****       $Content: Return CRC of last frame. CRC0 is on bit position 0                    ****/
/****                                                                                         ****/
/****       $argument: const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr             ****/
/****       $return value: uint8_t - CRC value                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
uint8_t SENT_Get_CRC(const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr)
{
   SENT_T *sent;
   HWI_SENT_CHANNEL_TYPE hwi_sent_channel;
   uint8_t crc;

   sent = &SENT;
   hwi_sent_channel = hwi_sent_channel_ptr->HWI_SENT_CHANNEL_NUMBER;

   crc = HWI_SENT_READ_RSR_CRC(sent->Channel[hwi_sent_channel].RSR.U32);

   return crc;
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Get_Sensor_Status                                                ****/
/****       $Content: Return Status and Communication Nibble of last frame                   ****/
/****                                                                                         ****/
/****       $argument: const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr             ****/
/****       $return value: uint8_t - Sensor status                                          ****/
/*************************************************************************************************/
/*************************************************************************************************/
uint8_t SENT_Get_Sensor_Status(const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr)
{
   SENT_T *sent;
   HWI_SENT_CHANNEL_TYPE hwi_sent_channel;
   uint8_t status;

   sent = &SENT;
   hwi_sent_channel = hwi_sent_channel_ptr->HWI_SENT_CHANNEL_NUMBER;

   status = HWI_SENT_READ_RSR_SCN(sent->Channel[hwi_sent_channel].RSR.U32);

   return status;
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Get_Channel_Status                                               ****/
/****       $Content: Return SENT channel diagnostic status                                  ****/
/****                                                                                         ****/
/****       $argument: const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr             ****/
/****       $return value: HWI_SENT_DG_ENUM_TYPE - Channel status                           ****/
/*************************************************************************************************/
/*************************************************************************************************/
HWI_SENT_DG_ENUM_TYPE SENT_Get_Channel_Status(const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr)
{
   HWI_SENT_CHANNEL_TYPE hwi_sent_channel;
   SENT_CHAN_TYPE sent_channel;
   hwi_sent_channel = hwi_sent_channel_ptr->HWI_SENT_CHANNEL_NUMBER;
   sent_channel =(SENT_CHAN_TYPE) SENT_HW2USED_Map[hwi_sent_channel];

   return HWI_Sent_diag_status[sent_channel];
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Get_Interrupt_Status                                             ****/
/****       $Content: Return SENT channel interrupt status                                   ****/
/****                                                                                         ****/
/****       $argument: const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr             ****/
/****       $return value: uint32_t - Interrupt status                                      ****/
/*************************************************************************************************/
/*************************************************************************************************/
uint32_t SENT_Get_Interrupt_Status(const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr)
{
   HWI_SENT_CHANNEL_TYPE hwi_sent_channel;
   SENT_CHAN_TYPE sent_channel;
   hwi_sent_channel = hwi_sent_channel_ptr->HWI_SENT_CHANNEL_NUMBER;
   sent_channel =(SENT_CHAN_TYPE) SENT_HW2USED_Map[hwi_sent_channel];

   return HWI_Sent_InterruptStatus[sent_channel];
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Get_MsgAgeSTime                                                  ****/
/****       $Content: Return SENT channel message received time stamp                        ****/
/****                                                                                         ****/
/****       $argument: const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr             ****/
/****       $return value: uint32_t - Message age in microseconds                           ****/
/*************************************************************************************************/
/*************************************************************************************************/
uint32_t SENT_Get_MsgAgeSTime( const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr )
{  // Message age in microseconds
   HWI_SENT_CHANNEL_TYPE hwi_sent_channel;
   SENT_CHAN_TYPE sent_channel;
   hwi_sent_channel = hwi_sent_channel_ptr->HWI_SENT_CHANNEL_NUMBER;
   sent_channel =(SENT_CHAN_TYPE) SENT_HW2USED_Map[hwi_sent_channel];
   return ( HWI_Sent_RxInt_TimeStamp[sent_channel] );
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_SetTick                                                          ****/
/****       $Content: Configure SENT channel tick time                                       ****/
/****                                                                                         ****/
/****       $argument: const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr             ****/
/****                 uint32_t tick                                                          ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void SENT_SetTick( const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr, uint32_t tick )
{
   HWI_SENT_CFG_CHANNEL_STRUCT_TYPE* l_sent_channel_cfg_ptr;
   l_sent_channel_cfg_ptr       = hwi_sent_channel_ptr->HWI_SENT_CHANNEL_CONFIG;
   l_sent_channel_cfg_ptr->HWI_SENT_CLOCK_TICK_DURATION = tick;
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_SetPausePresent                                                  ****/
/****       $Content: Configure SENT channel pause pulse handling                            ****/
/****                                                                                         ****/
/****       $argument: const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr             ****/
/****                 bool pause_present                                                      ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void SENT_SetPausePresent( const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr, bool pause_present )
{
   HWI_SENT_CFG_CHANNEL_STRUCT_TYPE* l_sent_channel_cfg_ptr;
   l_sent_channel_cfg_ptr       = hwi_sent_channel_ptr->HWI_SENT_CHANNEL_CONFIG;
   l_sent_channel_cfg_ptr->HWI_SENT_ENDPULSE_IGNORE = pause_present;
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_SetSerialMsgEnable                                               ****/
/****       $Content: Enable/Disable SENT channel serial message reading                     ****/
/****                                                                                         ****/
/****       $argument: const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr             ****/
/****                 bool serialmsg_enable                                                   ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void SENT_SetSerialMsgEnable( const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr, bool serialmsg_enable )
{
   HWI_SENT_CFG_CHANNEL_STRUCT_TYPE* l_sent_channel_cfg_ptr;
   l_sent_channel_cfg_ptr       = hwi_sent_channel_ptr->HWI_SENT_CHANNEL_CONFIG;
   l_sent_channel_cfg_ptr->HWI_SENT_SERIAL_FRAME_REQ = serialmsg_enable;
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_SetSerialMsgType                                                 ****/
/****       $Content: Configure SENT channel serial message type - short or enhanced         ****/
/****                                                                                         ****/
/****       $argument: const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr             ****/
/****                 bool enhancedmsg_enable                                                ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void SENT_SetSerialMsgType( const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr, bool enhancedmsg_enable )
{
   HWI_SENT_CFG_CHANNEL_STRUCT_TYPE* l_sent_channel_cfg_ptr;
   l_sent_channel_cfg_ptr       = hwi_sent_channel_ptr->HWI_SENT_CHANNEL_CONFIG;
   l_sent_channel_cfg_ptr->HWI_SENT_SERIAL_FRAME_METHOD =(HWI_SENT_SERIAL_FRAME_ENUM_TYPE) enhancedmsg_enable;
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_SetRxMsgFailTime                                                 ****/
/****       $Content: Configure SENT channel message failure time                            ****/
/****                                                                                         ****/
/****       $argument: const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr             ****/
/****                 uint32_t RxMsgFailCounter                                              ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void SENT_SetRxMsgFailTime( const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr, uint32_t RxMsgFailCounter )
{
   HWI_SENT_CFG_CHANNEL_STRUCT_TYPE* l_sent_channel_cfg_ptr;
   l_sent_channel_cfg_ptr       = hwi_sent_channel_ptr->HWI_SENT_CHANNEL_CONFIG;
   l_sent_channel_cfg_ptr->HWI_SENT_WDT_MAX_COUNTER=RxMsgFailCounter;
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_SetNibbleOrder                                                   ****/
/****       $Content: Configure SENT channel received nibble order                           ****/
/****                                                                                         ****/
/****       $argument: const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr             ****/
/****                 uint32_t nibble_order                                                  ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void SENT_SetNibbleOrder( const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr, uint32_t nibble_order )
{
   SENT_T *sent;
   HWI_SENT_CHANNEL_TYPE hwi_sent_channel;
   sent = &SENT;
   hwi_sent_channel             = hwi_sent_channel_ptr->HWI_SENT_CHANNEL_NUMBER;
   sent->Channel[hwi_sent_channel].VIEW.U32 = nibble_order;
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Clear_Interrupt_Status                                           ****/
/****       $Content: Clear SENT channel interrupt status                                    ****/
/****                                                                                         ****/
/****       $argument: const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr             ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void SENT_Clear_Interrupt_Status(const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr)
{
   HWI_SENT_CHANNEL_TYPE hwi_sent_channel;
   SENT_CHAN_TYPE sent_channel;
   hwi_sent_channel = hwi_sent_channel_ptr->HWI_SENT_CHANNEL_NUMBER;
   sent_channel =(SENT_CHAN_TYPE) SENT_HW2USED_Map[hwi_sent_channel];

   HWI_Sent_InterruptStatus[sent_channel] = 0;
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_SetCrcImplement                                                  ****/
/****       $Content: Configure SENT channel CRC implementation type                         ****/
/****                                                                                         ****/
/****       $argument: const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr             ****/
/****                 HWI_SENT_CRC_AUGMENTATION_ENUM_TYPE CrcType                           ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void SENT_SetCrcImplement(const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr, HWI_SENT_CRC_AUGMENTATION_ENUM_TYPE CrcType)
{
   HWI_SENT_CFG_CHANNEL_STRUCT_TYPE* l_sent_channel_cfg_ptr;
   l_sent_channel_cfg_ptr       = hwi_sent_channel_ptr->HWI_SENT_CHANNEL_CONFIG;
   l_sent_channel_cfg_ptr->HWI_SENT_CRC_IMPLEMENT_TYPE=CrcType;
}

/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
/*---                                   REVISION MANAGEMENT                                   ---*/
/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/*---                                                                                         ---*/
/*---    $Rev:No:    date:       name:       company:    minute:    content:                 ---*/
/*---    $Rev:0      2024-01-14  Wingman    Bito        Initial    Initial Version          ---*/
/*---                                                                                         ---*/
/*-----------------------------------------------------------------------------------------------*/
/*---
        $Log$
                                                                                              ---*/
/*-----------------------------------------------------------------------------------------------*/