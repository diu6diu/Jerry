
#include "hal.h"
#include "io.h"
#include "io_sent_port.h"
#include "sent_io.h"

extern HIODEVICE HAL_SENT_In_Array[];
#define Hal_Set_SENT_In_Config(x) (HAL_SENT_In_Array[x])

#define SENT_INVALID_CH          ( 0xFF )
#define CORE_TICK_RES            ( 1000.0 / 512 )
#define SENT_TOTAL_FLT_MASK      ( 0xF7E0 )
#define SENT_MSG_TOTAL_FLT_MASK  ( 0x7E )

typedef enum
{
	SENT_CRCType_LegacyImplementation=0,
	SENT_CRCType_RecommendedImplementation
}HAL_SENT_CRC_Type_T;

/******************************************************************************
* Global Functions prototype
******************************************************************************/
T_COUNT_BYTE GetHWIO_SENT_Msg( SENT_SENSOR_TYPE_T sensor, SENT_MsgParams_T* pMsgParam  );
void SetHWIO_SENTParameters( SENT_SENSOR_TYPE_T sensor, SENT_MsgInit_T msginit );

/******************************************************************************
* Static Functions prototype
******************************************************************************/
static T_COUNT_BYTE   Hal_Get_SENT_Msg(SENT_Port_C_T *sent_port, SENT_MsgParams_T* pMsgParam);
static void           Hal_Set_SENT_Msg(SENT_Port_C_T *sent_port, SENT_MsgInit_T msginit);

/******************************************************************************
* Imported Functions
******************************************************************************/
extern uint32_t GetHWIO_FreeRunningDeltaTime (uint32_t SavedCounts, uint8_t Units);

/******************************************************************************
* Static function
******************************************************************************/
static T_COUNT_BYTE   Hal_Get_SENT_Msg(SENT_Port_C_T *sent_port, SENT_MsgParams_T* pMsgParam)
{
   SENT_MsgFault_T msgflt;
   HIODEVICE sent_dev;
   uint16_t *sentdata;
   SENT_INTSTAT_T hwio_sent_int;
   uint32_t critical;
   SENT_Port_DG_Enum_T channel_status;
   // initialize return variable
   msgflt.Byte = 0;
   if(NULL != sent_port)
   {
      // Interrupt protection
      critical = Enter_Critical_Section();

      hwio_sent_int.U32 = IO_SENT_Get_Interrupt_Status( sent_port );
      IO_SENT_Clear_Interrupt_Status( sent_port );
      // Get the Fast Channel Status Communication Nibble, data and CRC
      pMsgParam->FastNibbles[0] = IO_SENT_Get_Sensor_Status( sent_port );
      sentdata = IO_SENT_Get_Data( sent_port );
      pMsgParam->FastNibbles[1] = ( *sentdata & 0x000F );
      pMsgParam->FastNibbles[2] = ( *sentdata & 0x00F0 ) >> 4;
      pMsgParam->FastNibbles[3] = ( *sentdata & 0x0F00 ) >> 8;
      pMsgParam->FastNibbles[4] = ( *sentdata & 0xF000 ) >> 12;
      sentdata++;
      pMsgParam->FastNibbles[5] = (BYTE)( *sentdata & 0x000F );
      pMsgParam->FastNibbles[6] = (BYTE)( *sentdata & 0x00F0 ) >> 4;
      pMsgParam->FastNibbles[7] = IO_SENT_Get_CRC( sent_port );

      // Get the Slow Channel Msg ID, data and CRC
      sentdata = IO_SENT_Get_SerialData( sent_port );
      pMsgParam->SerialMsg.SerialMsgID = (BYTE)(*sentdata & 0x00FF);
      pMsgParam->SerialMsg.SerialMsgCRC = (BYTE)( (*sentdata & 0x3F00) >> 8 );
      sentdata++;
      pMsgParam->SerialMsg.SerialMsgData = *sentdata;
      Leave_Critical_Section( critical );

      // Get the message fault status
      // CalTimeOut = FRI,  ClockShift = FDI
      msgflt.bits.CalTimeOut = hwio_sent_int.F.FRI;
      msgflt.bits.ClockShift = hwio_sent_int.F.FDI;
      msgflt.bits.TooFewOrTooMany=hwio_sent_int.F.NNI;
      msgflt.bits.DataTooSmallOrTooLarge=hwio_sent_int.F.NVI;
      msgflt.bits.CRC_Error = hwio_sent_int.F.CRCI;
      channel_status = SENT_Port_Get_Channel_Status( sent_port );
      msgflt.bits.RxMsgTimeout = ( hwio_sent_int.F.WDI || ( channel_status != SENT_PORT_DG_NO_ERROR) );
      //if( ((uint8_t)0) != ( msgflt.Byte & SENT_MSG_TOTAL_FLT_MASK ) )   /* Error Flag of Sent*/
      if( ( ((uint16_t)0) != ( hwio_sent_int.U16[0] & SENT_TOTAL_FLT_MASK ) ) || ( msgflt.bits.RxMsgTimeout == true ) )  /* Error Flag of Sent*/
      {
         msgflt.bits.TotalMsgFault = TRUE;
      }

   }
   return msgflt.Byte;
}

static void Hal_Set_SENT_Msg(SENT_Port_C_T *sent_port, SENT_MsgInit_T msginit)
{  // This should be called from EMS SENT initialization from StartUp_Hook()
   uint32_t tick;
   HIODEVICE sent_dev;
   uint8_t crc_type;

   // the Tick_Time is of type FIXED_UW_9 in us.
   tick = msginit.Tick_Time * CORE_TICK_RES;
   if(NULL != sent_port)
   {
      if(SENT_CRCType_LegacyImplementation == msginit.CRC_ImplementType)
      {
         crc_type = 1;
      }
      else
      {
         crc_type = 0;
      }

      IO_SENT_Set_Tick( sent_port, tick );
      IO_SENT_Set_PausePresent( sent_port, msginit.Pause_Pulse_Enbl);
      IO_SENT_Set_SerialMsgEnable( sent_port, msginit.Serial_Msg_Enbl);
      IO_SENT_Set_SerialMsgType( sent_port, msginit.Serial_Enhanced_Enbl);
      IO_SENT_Set_WdtTimeout(sent_port,  ((uint32_t)msginit.RxMsgFailTime*1000));
      IO_SENT_Set_CRCType(sent_port,crc_type);
   }
}

/******************************************************************************
* HAL Interface for EMS
******************************************************************************/
T_COUNT_BYTE GetHWIO_SENT_Msg( SENT_SENSOR_TYPE_T sensor, SENT_MsgParams_T* pMsgParam  )
{
   T_COUNT_BYTE sent_fault = 0;
   switch(sensor)
   {
#ifdef USE_CALIBRATION_IO
      case SENT_SENSOR_GPF:
         sent_fault = Hal_Get_SENT_Msg(Hal_Set_SENT_In_Config(SENT_CHANNEL_B), pMsgParam);
         break;

      case SENT_SENSOR_ECT:
         sent_fault = Hal_Get_SENT_Msg(Hal_Set_SENT_In_Config(SENT_CHANNEL_D), pMsgParam);
         break;

      case SENT_SENSOR_GPF_TEMP:
         sent_fault = Hal_Get_SENT_Msg(Hal_Set_SENT_In_Config(SENT_CHANNEL_E), pMsgParam);
         break;

      case SENT_SENSOR_EGR:
         sent_fault = Hal_Get_SENT_Msg(Hal_Set_SENT_In_Config(SENT_CHANNEL_A), pMsgParam);
         break;

      case SENT_SENSOR_GPF_Inlet:
         sent_fault = Hal_Get_SENT_Msg(Hal_Set_SENT_In_Config(SENT_CHANNEL_F), pMsgParam);
         break;

      case SENT_SENSOR_GPF_Outlet:
         sent_fault = Hal_Get_SENT_Msg(Hal_Set_SENT_In_Config(SENT_CHANNEL_B), pMsgParam);
         break;
      #if 0 //ywh
      case SENT_SENSOR_PAT:
         sent_fault = Hal_Get_SENT_Msg(Hal_Set_SENT_In_Config(SENT_SENSOR_PAT), pMsgParam);
         break;
      #endif
#endif
      default:
         break;
   }
   return sent_fault;
}

void SetHWIO_SENTParameters( SENT_SENSOR_TYPE_T sensor, SENT_MsgInit_T msginit )
{  // This should be called from EMS SENT initialization from StartUp_Hook()
   switch(sensor)
   {
#ifdef USE_CALIBRATION_IO
      case SENT_SENSOR_GPF:
         Hal_Set_SENT_Msg(Hal_Set_SENT_In_Config(SENT_CHANNEL_B), msginit);
         break;

      case SENT_SENSOR_ECT:
         Hal_Set_SENT_Msg(Hal_Set_SENT_In_Config(SENT_CHANNEL_D), msginit);
         break;

      case SENT_SENSOR_GPF_TEMP:
         Hal_Set_SENT_Msg(Hal_Set_SENT_In_Config(SENT_CHANNEL_E), msginit);
         break;

      case SENT_SENSOR_EGR:
         Hal_Set_SENT_Msg(Hal_Set_SENT_In_Config(SENT_CHANNEL_A), msginit);
         break;

      case SENT_SENSOR_GPF_Inlet:
         Hal_Set_SENT_Msg(Hal_Set_SENT_In_Config(SENT_CHANNEL_F), msginit);
         break;

      case SENT_SENSOR_GPF_Outlet:
         Hal_Set_SENT_Msg(Hal_Set_SENT_In_Config(SENT_CHANNEL_B), msginit);
         break;
#if 0 //ywh
      case SENT_SENSOR_PAT:
         Hal_Set_SENT_Msg(Hal_Set_SENT_In_Config(SENT_SENSOR_PAT), msginit);
         break;
#endif
#endif
      default:
         break;
   }
}


