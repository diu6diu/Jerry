/* CONFIDENTIAL */
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_*/
/*_/_/_/        $DWG-No.: MT95P1                                                         _/_/_/_*/
/*_/_/_/        $Content: SENT Port Layer Implementation                                 _/_/_/_*/
/*_/_/_/        $Category: Source                                                       _/_/_/_*/
/*_/_/_/        $Date: 2024                                                            _/_/_/_*/
/*_/_/_/        $Design: Wingman                                                       _/_/_/_*/
/*_/_/_/        $Check: Wingman                                                        _/_/_/_*/
/*_/_/_/        $Header$                                                               _/_/_/_*/
/*_/_/_/        $Copyright(C) Bito Corporation                                         _/_/_/_*/
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_*/

/*###############################################################################################*/
/*###                                     $INCLUDE FILES$                                    ####*/
/*###############################################################################################*/
#include "dd_sent.h"
#include "sent_device_driver.h"
#include "io_scale.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     $RAM_EXTERN$                                        +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     $RAM_PUBLIC$                                        +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Port_Initialize_Device                                            ****/
/****       $Content: Initialize SENT device                                                  ****/
/****                                                                                         ****/
/****       $argument: void                                                                   ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void SENT_Port_Initialize_Device( void )
{
   SENT_Initialize_Device();
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Port_Initialize_Channel                                           ****/
/****       $Content: Initialize specific SENT channel                                        ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                   ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void SENT_Port_Initialize_Channel( HIODEVICE io_device )
{
   SENT_Port_C_T* sent_port = (SENT_Port_C_T*) io_device;
   SENT_Initialize_Channel(sent_port->Configuration, sent_port->pmd_channel);
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Port_Get_Data                                                    ****/
/****       $Content: Get SENT data from specified channel                                   ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                   ****/
/****       $return value: uint16_t* - Pointer to data buffer                               ****/
/*************************************************************************************************/
/*************************************************************************************************/
uint16_t* SENT_Port_Get_Data( HIODEVICE io_device )
{
   SENT_Port_C_T* sent_port = (SENT_Port_C_T*) io_device;
   return(SENT_Get_Data(sent_port->Configuration));
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Port_Get_SerialData                                              ****/
/****       $Content: Get SENT serial data from specified channel                            ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                   ****/
/****       $return value: uint16_t* - Pointer to serial data buffer                        ****/
/*************************************************************************************************/
/*************************************************************************************************/
uint16_t* SENT_Port_Get_SerialData( HIODEVICE io_device )
{
   SENT_Port_C_T* sent_port = (SENT_Port_C_T*) io_device;
   return(SENT_Get_SerialData(sent_port->Configuration));
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Port_Get_CRC                                                     ****/
/****       $Content: Get SENT CRC value from specified channel                              ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                   ****/
/****       $return value: uint8_t - CRC value                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
uint8_t SENT_Port_Get_CRC( HIODEVICE io_device )
{
   SENT_Port_C_T* sent_port = (SENT_Port_C_T*) io_device;
   return(SENT_Get_CRC(sent_port->Configuration));
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Port_Get_Sensor_Status                                           ****/
/****       $Content: Get SENT sensor status from specified channel                          ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                   ****/
/****       $return value: uint8_t - Sensor status                                          ****/
/*************************************************************************************************/
/*************************************************************************************************/
uint8_t SENT_Port_Get_Sensor_Status( HIODEVICE io_device )
{
   SENT_Port_C_T* sent_port = (SENT_Port_C_T*) io_device;
   return(SENT_Get_Sensor_Status(sent_port->Configuration));
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Port_Get_Channel_Status                                          ****/
/****       $Content: Get SENT channel status                                                ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                   ****/
/****       $return value: SENT_Port_DG_Enum_T - Channel status                             ****/
/*************************************************************************************************/
/*************************************************************************************************/
SENT_Port_DG_Enum_T SENT_Port_Get_Channel_Status( HIODEVICE io_device )
{
   SENT_Port_C_T* sent_port = (SENT_Port_C_T*) io_device;
   return(SENT_Get_Channel_Status(sent_port->Configuration));
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Port_Get_Interrupt_Status                                        ****/
/****       $Content: Get SENT interrupt status                                              ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                   ****/
/****       $return value: uint32_t - Interrupt status                                      ****/
/*************************************************************************************************/
/*************************************************************************************************/
uint32_t SENT_Port_Get_Interrupt_Status( HIODEVICE io_device )
{
   SENT_Port_C_T* sent_port = (SENT_Port_C_T*) io_device;
   return(SENT_Get_Interrupt_Status(sent_port->Configuration));
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Port_Get_MsgAgeSTime                                             ****/
/****       $Content: Get SENT message age system time                                       ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                   ****/
/****       $return value: uint32_t - Message age system time                               ****/
/*************************************************************************************************/
/*************************************************************************************************/
uint32_t SENT_Port_Get_MsgAgeSTime( HIODEVICE io_device )
{
   SENT_Port_C_T* sent_port = (SENT_Port_C_T*) io_device;
   return(SENT_Get_MsgAgeSTime(sent_port->Configuration));
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Port_SetTick                                                     ****/
/****       $Content: Set SENT tick value                                                    ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                   ****/
/****                 uint32_t tick - Tick value to set                                      ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void SENT_Port_SetTick( HIODEVICE io_device, uint32_t tick )
{
   SENT_Port_C_T* sent_port = (SENT_Port_C_T*) io_device;
   SENT_SetTick(sent_port->Configuration, tick);
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Port_SetPausePresent                                            ****/
/****       $Content: Set SENT pause present flag                                            ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                   ****/
/****                 bool pause_present - Pause present flag                                ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void SENT_Port_SetPausePresent( HIODEVICE io_device, bool pause_present )
{
   SENT_Port_C_T* sent_port = (SENT_Port_C_T*) io_device;
   SENT_SetPausePresent(sent_port->Configuration, pause_present);
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Port_SetSerialMsgEnable                                         ****/
/****       $Content: Enable/Disable SENT serial message                                     ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                   ****/
/****                 bool serialmsg_enable - Serial message enable flag                     ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void SENT_Port_SetSerialMsgEnable( HIODEVICE io_device, bool serialmsg_enable )
{
   SENT_Port_C_T* sent_port = (SENT_Port_C_T*) io_device;
   SENT_SetSerialMsgEnable(sent_port->Configuration, serialmsg_enable);
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Port_SetSerialMsgType                                           ****/
/****       $Content: Set SENT serial message type                                           ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                   ****/
/****                 bool enhancedmsg_enable - Enhanced message enable flag                ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void SENT_Port_SetSerialMsgType( HIODEVICE io_device, bool enhancedmsg_enable )
{
   SENT_Port_C_T* sent_port = (SENT_Port_C_T*) io_device;
   SENT_SetSerialMsgType(sent_port->Configuration, enhancedmsg_enable);
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Port_SetNibbleOrder                                             ****/
/****       $Content: Set SENT nibble order                                                 ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                   ****/
/****                 uint32_t nibble_order - Nibble order value                            ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void SENT_Port_SetNibbleOrder( HIODEVICE io_device, uint32_t nibble_order )
{
   SENT_Port_C_T* sent_port = (SENT_Port_C_T*) io_device;
   SENT_SetNibbleOrder(sent_port->Configuration, nibble_order);
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Port_Process_Receive_Interrupt                                   ****/
/****       $Content: Process SENT receive interrupt                                         ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                   ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void SENT_Port_Process_Receive_Interrupt( HIODEVICE io_device )
{
   SENT_Port_C_T* sent_port = (SENT_Port_C_T*) io_device;
   SENT_Process_Receive_Interrupt(sent_port->pmd_channel, sent_port->Configuration);
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Port_SetWatchDogTimer                                           ****/
/****       $Content: Set SENT watchdog timer                                                ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                   ****/
/****                 uint32_t Timer - Timer value                                           ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void SENT_Port_SetWatchDogTimer( HIODEVICE io_device, uint32_t Timer )
{
   SENT_Port_C_T* sent_port = (SENT_Port_C_T*) io_device;
   SENT_SetRxMsgFailTime(sent_port->Configuration, Timer);
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Port_Clear_Interrupt_Status                                      ****/
/****       $Content: Clear SENT interrupt status                                            ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                   ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void SENT_Port_Clear_Interrupt_Status( HIODEVICE io_device )
{
   SENT_Port_C_T* sent_port = (SENT_Port_C_T*) io_device;
   SENT_Clear_Interrupt_Status(sent_port->Configuration);
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: SENT_Port_SetCrcImplement                                             ****/
/****       $Content: Set SENT CRC implementation type                                       ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                   ****/
/****                 uint8_t CrcType - CRC type                                             ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void SENT_Port_SetCrcImplement( HIODEVICE io_device, uint8_t CrcType )
{
   SENT_Port_C_T* sent_port = (SENT_Port_C_T*) io_device;
   SENT_SetCrcImplement(sent_port->Configuration, ((HWI_SENT_CRC_AUGMENTATION_ENUM_TYPE)CrcType));
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