#include "sent_port.h"

const Sent_Port_Interface_T SENT_PORT_INTERFACE = 
{
   SENT_Port_Initialize_Device,
   SENT_Port_Initialize_Channel,
   SENT_Port_Get_Data,
   SENT_Port_Get_SerialData,
   SENT_Port_Get_CRC,
   SENT_Port_Get_Sensor_Status,
   SENT_Port_Get_Channel_Status,
   SENT_Port_Get_Interrupt_Status,
   SENT_Port_Clear_Interrupt_Status,
   SENT_Port_Get_MsgAgeSTime,
   SENT_Port_SetTick,
   SENT_Port_SetPausePresent,
   SENT_Port_SetSerialMsgEnable,
   SENT_Port_SetSerialMsgType,
   SENT_Port_SetNibbleOrder,
   SENT_Port_SetWatchDogTimer,
   SENT_Port_SetCrcImplement,
};

const SENT_Port_Interface_Callback_T SENT_PORT_CALLBACK = 
{
   SENT_Port_Process_Receive_Interrupt,
};

/*===========================================================================*/
/* FILE REVISION HISTORY (top to bottom: first revision to last revision)    */
/*===========================================================================*/
/* Rev. SCR        Date     By       Description                             */
/*                 dd/mm/yy                                                  */
/* ==== ========== ======== ======== ======================================= */
/* 1.0  RSM_CTC_8731 24/08/17 hhl   Created Initial version for MT92.1       */
/* 2.0  RSM_CTC_8731 25/08/17 hhl   Add SENT_Port_Clear_Interrupt_Status     */
/* 3.0  RSM_CTC_8737 31/08/17 hhl   Add SENT_Port_SetCrcImplement            */
/*                                                                           */
/*****************************************************************************/