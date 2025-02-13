/* CONFIDENTIAL */
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_*/
/*_/_/_/        $DWG-No.: NONE                                                                _/_/_/_*/
/*_/_/_/        $Content: SENT Device Driver Interface Header File                            _/_/_/_*/
/*_/_/_/        $Category: HAL                                                               _/_/_/_*/
/*_/_/_/        $Date: 2024                                                                  _/_/_/_*/
/*_/_/_/        $Design: Hardware Abstraction Layer Interface                                _/_/_/_*/
/*_/_/_/        $Check: NONE                                                                _/_/_/_*/
/*_/_/_/        $Header: sent_device_driver.h                                               _/_/_/_*/
/*_/_/_/        $Copyright(C) 2024                                                          _/_/_/_*/
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_*/

#ifndef SENT_DEVICE_DRIVER_H
#define SENT_DEVICE_DRIVER_H

/*###############################################################################################*/
/*###                                     $INCLUDE FILES$                                     ####*/
/*###############################################################################################*/
#include "io_type.h"
#include "dd_sent.h"
#include "dd_sent_init.h"

/*###############################################################################################*/
/*###                                     $TYPE DEFINITIONS$                                  ####*/
/*###############################################################################################*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Diagnostic Enums                                      +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
typedef enum
{
    SENT_PORT_DG_NO_ERROR = 0,        /* Lowest priority : no error       */
    SENT_PORT_DG_NO_REFRESHED_FRAME,  /* WDI > HWI_SENT_MSG_LOST_CPV      */
    SENT_PORT_DG_ERROR,
    SENT_PORT_DG_CRC_ERROR,           /* CRC Nibble error                 */
    SENT_PORT_DG_NIBBLE_DATA_ERROR,   /* Nibble data value out of range   */
    SENT_PORT_DG_N_NIBBLE_ERROR,      /* Number of nibbles out of range   */
    SENT_PORT_DG_CLOCK_DRIFT,         /* Consecutive CAL pulse > 1.56%    */
    SENT_PORT_DG_CALPULSE_DRIFT,      /* CAL pulse > +/- 25%              */
    SENT_PORT_DG_LOST_SIGNAL          /* highest priority : no signal     */
} SENT_Port_DG_Enum_T;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Function Pointers                                     +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Initialization Functions */
typedef void (*SENT_Port_Initialize_Device_T)(void);
typedef void (*SENT_Port_Initialize_Channel_T)(HIODEVICE io_device);

/* Data Access Functions */
typedef uint16_t* (*SENT_Port_Get_Data_T)(HIODEVICE io_device);
typedef uint16_t* (*SENT_Port_Get_SerialData_T)(HIODEVICE io_device);
typedef uint8_t (*SENT_Port_Get_CRC_T)(HIODEVICE io_device);
typedef uint8_t (*SENT_Port_Get_Sensor_Status_T)(HIODEVICE io_device);
typedef SENT_Port_DG_Enum_T (*SENT_Port_Get_Channel_Status_T)(HIODEVICE io_device);
typedef uint32_t (*SENT_Port_Get_Interrupt_Status_T)(HIODEVICE io_device);
typedef uint32_t (*SENT_Port_Get_MsgAgeSTime_T)(HIODEVICE io_device);

/* Configuration Functions */
typedef void (*SENT_Port_SetTick_T)(HIODEVICE io_device, uint32_t tick);
typedef void (*SENT_Port_SetPausePresent_T)(HIODEVICE io_device, bool pause_present);
typedef void (*SENT_Port_SetSerialMsgEnable_T)(HIODEVICE io_device, bool serialmsg_enable);
typedef void (*SENT_Port_SetSerialMsgType_T)(HIODEVICE io_device, bool enhancedmsg_enable);
typedef void (*SENT_Port_SetNibbleOrder_T)(HIODEVICE io_device, uint32_t nibble_order);
typedef void (*SENT_Port_SetWdtTimer)(HIODEVICE io_device, uint32_t nibble_order);
typedef void (*SENT_Port_SetCrcType)(HIODEVICE io_device, uint8_t CrcType);

/* Interrupt Functions */
typedef void (*SENT_Port_Process_Receive_Interrupt_T)(HIODEVICE io_device);
typedef void (*SENT_Port_Clear_Interrupt_Status_T)(HIODEVICE io_device);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Interface Structures                                  +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
typedef struct
{
    /* Initialization Functions */
    SENT_Port_Initialize_Device_T         device_initialize;
    SENT_Port_Initialize_Channel_T        channel_initialize;
    
    /* Data Access Functions */
    SENT_Port_Get_Data_T                  get_data;
    SENT_Port_Get_SerialData_T            get_serial_data;
    SENT_Port_Get_CRC_T                   get_crc;
    SENT_Port_Get_Sensor_Status_T         get_sensor_status;
    SENT_Port_Get_Channel_Status_T        get_channel_status;
    SENT_Port_Get_Interrupt_Status_T      get_interrupt_status;
    SENT_Port_Clear_Interrupt_Status_T    clear_interrupt_status;
    SENT_Port_Get_MsgAgeSTime_T           get_msg_age;
    
    /* Configuration Functions */
    SENT_Port_SetTick_T                   set_tick;
    SENT_Port_SetPausePresent_T           set_pause_preset;
    SENT_Port_SetSerialMsgEnable_T        set_serial_msg_enable;
    SENT_Port_SetSerialMsgType_T          set_serial_msg_type;
    SENT_Port_SetNibbleOrder_T            set_nibble_order;
    SENT_Port_SetWdtTimer                 set_wdt_timer;
    SENT_Port_SetCrcType                  set_crc_type;
} Sent_Port_Interface_T;

typedef struct
{
    SENT_Port_Process_Receive_Interrupt_T receive_interrupt;
} SENT_Port_Interface_Callback_T;

typedef struct SENT_Port_Tag
{
    SENT_CHAN_TYPE                     pmd_channel;
    const HWI_SENT_CHANNEL_STRUCT_TYPE *Configuration;
    const Sent_Port_Interface_T        *I;
    SENT_Port_Interface_Callback_T     *Callback;
} SENT_Port_T;

typedef const SENT_Port_T const SENT_Port_C_T;

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

#endif /* end SENT_DEVICE_DRIVER_H */