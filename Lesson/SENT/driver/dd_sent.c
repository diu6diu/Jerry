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