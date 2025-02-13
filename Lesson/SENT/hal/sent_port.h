/* CONFIDENTIAL */
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_*/
/*_/_/_/        $DWG-No.: NONE                                                                _/_/_/_*/
/*_/_/_/        $Content: SENT Port Interface Header File                                     _/_/_/_*/
/*_/_/_/        $Category: HAL                                                               _/_/_/_*/
/*_/_/_/        $Date: 2024                                                                  _/_/_/_*/
/*_/_/_/        $Design: Hardware Abstraction Layer Interface                                _/_/_/_*/
/*_/_/_/        $Check: NONE                                                                _/_/_/_*/
/*_/_/_/        $Header: sent_port.h                                                        _/_/_/_*/
/*_/_/_/        $Copyright(C) 2024                                                          _/_/_/_*/
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_*/

#ifndef SENT_PORT_H
#define SENT_PORT_H

/*###############################################################################################*/
/*###                                     $INCLUDE FILES$                                     ####*/
/*###############################################################################################*/
#include "dd_sent.h"
#include "sent_device_driver.h"

/*###############################################################################################*/
/*###                                     $API FUNCTIONS$                                     ####*/
/*###############################################################################################*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Initialization Functions                              +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*************************************************************************************************/
/****       $Function: SENT_Port_Initialize_Device                                             ****/
/****       $Content: Initialize the SENT device                                               ****/
/****                                                                                         ****/
/****       $argument: None                                                                   ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
extern void SENT_Port_Initialize_Device(void);

/*************************************************************************************************/
/****       $Function: SENT_Port_Initialize_Channel                                            ****/
/****       $Content: Initialize a specific SENT channel                                       ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                    ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
extern void SENT_Port_Initialize_Channel(HIODEVICE io_device);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Data Access Functions                                 +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*************************************************************************************************/
/****       $Function: SENT_Port_Get_Data                                                     ****/
/****       $Content: Get SENT data from specified channel                                    ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                    ****/
/****       $return value: uint16_t* - Pointer to data                                        ****/
/*************************************************************************************************/
extern uint16_t* SENT_Port_Get_Data(HIODEVICE io_device);

/*************************************************************************************************/
/****       $Function: SENT_Port_Get_SerialData                                               ****/
/****       $Content: Get serial data from specified channel                                  ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                    ****/
/****       $return value: uint16_t* - Pointer to serial data                                 ****/
/*************************************************************************************************/
extern uint16_t* SENT_Port_Get_SerialData(HIODEVICE io_device);

/*************************************************************************************************/
/****       $Function: SENT_Port_Get_CRC                                                      ****/
/****       $Content: Get CRC value from specified channel                                    ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                    ****/
/****       $return value: uint8_t - CRC value                                                ****/
/*************************************************************************************************/
extern uint8_t SENT_Port_Get_CRC(HIODEVICE io_device);

/*************************************************************************************************/
/****       $Function: SENT_Port_Get_Sensor_Status                                            ****/
/****       $Content: Get sensor status from specified channel                                ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                    ****/
/****       $return value: uint8_t - Sensor status                                            ****/
/*************************************************************************************************/
extern uint8_t SENT_Port_Get_Sensor_Status(HIODEVICE io_device);

/*************************************************************************************************/
/****       $Function: SENT_Port_Get_Channel_Status                                           ****/
/****       $Content: Get channel diagnostic status                                           ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                    ****/
/****       $return value: SENT_Port_DG_Enum_T - Channel status                               ****/
/*************************************************************************************************/
extern SENT_Port_DG_Enum_T SENT_Port_Get_Channel_Status(HIODEVICE io_device);

/*************************************************************************************************/
/****       $Function: SENT_Port_Get_Interrupt_Status                                         ****/
/****       $Content: Get interrupt status from specified channel                             ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                    ****/
/****       $return value: uint32_t - Interrupt status                                        ****/
/*************************************************************************************************/
extern uint32_t SENT_Port_Get_Interrupt_Status(HIODEVICE io_device);

/*************************************************************************************************/
/****       $Function: SENT_Port_Get_MsgAgeSTime                                              ****/
/****       $Content: Get message age time from specified channel                             ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                    ****/
/****       $return value: uint32_t - Message age time                                        ****/
/*************************************************************************************************/
extern uint32_t SENT_Port_Get_MsgAgeSTime(HIODEVICE io_device);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Configuration Functions                               +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*************************************************************************************************/
/****       $Function: SENT_Port_SetTick                                                      ****/
/****       $Content: Set tick value for specified channel                                    ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                    ****/
/****                 uint32_t tick - Tick value                                              ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
extern void SENT_Port_SetTick(HIODEVICE io_device, uint32_t tick);

/*************************************************************************************************/
/****       $Function: SENT_Port_SetPausePresent                                              ****/
/****       $Content: Set pause present flag for specified channel                            ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                    ****/
/****                 bool pause_present - Pause present flag                                  ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
extern void SENT_Port_SetPausePresent(HIODEVICE io_device, bool pause_present);

/*************************************************************************************************/
/****       $Function: SENT_Port_SetSerialMsgEnable                                           ****/
/****       $Content: Enable/disable serial messages for specified channel                    ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                    ****/
/****                 bool serialmsg_enable - Enable flag                                      ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
extern void SENT_Port_SetSerialMsgEnable(HIODEVICE io_device, bool serialmsg_enable);

/*************************************************************************************************/
/****       $Function: SENT_Port_SetSerialMsgType                                             ****/
/****       $Content: Set serial message type for specified channel                           ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                    ****/
/****                 bool enhancedmsg_enable - Enhanced message enable flag                   ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
extern void SENT_Port_SetSerialMsgType(HIODEVICE io_device, bool enhancedmsg_enable);

/*************************************************************************************************/
/****       $Function: SENT_Port_SetNibbleOrder                                               ****/
/****       $Content: Set nibble order for specified channel                                  ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                    ****/
/****                 uint32_t nibble_order - Nibble order value                              ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
extern void SENT_Port_SetNibbleOrder(HIODEVICE io_device, uint32_t nibble_order);

/*************************************************************************************************/
/****       $Function: SENT_Port_SetWatchDogTimer                                             ****/
/****       $Content: Set watchdog timer for specified channel                                ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                    ****/
/****                 uint32_t Timer - Timer value                                            ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
extern void SENT_Port_SetWatchDogTimer(HIODEVICE io_device, uint32_t Timer);

/*************************************************************************************************/
/****       $Function: SENT_Port_SetCrcImplement                                              ****/
/****       $Content: Set CRC implementation type for specified channel                       ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                    ****/
/****                 uint8_t CrcType - CRC type                                              ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
extern void SENT_Port_SetCrcImplement(HIODEVICE io_device, uint8_t CrcType);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Interrupt Functions                                   +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*************************************************************************************************/
/****       $Function: SENT_Port_Process_Receive_Interrupt                                    ****/
/****       $Content: Process receive interrupt for specified channel                         ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                    ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
extern void SENT_Port_Process_Receive_Interrupt(HIODEVICE io_device);

/*************************************************************************************************/
/****       $Function: SENT_Port_Clear_Interrupt_Status                                       ****/
/****       $Content: Clear interrupt status for specified channel                            ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE io_device - Device handle                                    ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
extern void SENT_Port_Clear_Interrupt_Status(HIODEVICE io_device);

/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
/*---                                   REVISION MANAGEMENT                                    ---*/
/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/*---                                                                                         ---*/
/*---    $Rev:No:    date:       name:       company:    minute:    content:                 ---*/
/*---    $Rev:0      2024-02-14                                    Initial version           ---*/
/*---                                                                                         ---*/
/*-----------------------------------------------------------------------------------------------*/
/*---
        $Log$
                                                                                              ---*/
/*-----------------------------------------------------------------------------------------------*/

#endif /* end SENT_PORT_H */