/* CONFIDENTIAL */
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_*/
/*_/_/_/        $DWG-No.: NONE                                                                _/_/_/_*/
/*_/_/_/        $Content: SENT HAL Interface Header File                                      _/_/_/_*/
/*_/_/_/        $Category: HAL                                                               _/_/_/_*/
/*_/_/_/        $Date: 2024                                                                  _/_/_/_*/
/*_/_/_/        $Design: Hardware Abstraction Layer Interface                                _/_/_/_*/
/*_/_/_/        $Check: NONE                                                                _/_/_/_*/
/*_/_/_/        $Header: io_sent.h                                                          _/_/_/_*/
/*_/_/_/        $Copyright(C) 2024                                                          _/_/_/_*/
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_*/

#ifndef IO_SENT_H
#define IO_SENT_H

/*###############################################################################################*/
/*###                                     $INCLUDE FILES$                                     ####*/
/*###############################################################################################*/
#include "sent.h"

/*###############################################################################################*/
/*###                                     $API FUNCTIONS$                                     ####*/
/*###############################################################################################*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Initialization Functions                              +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*************************************************************************************************/
/****       $Function: SENT_Initialize_Channel                                                 ****/
/****       $Content: Initialize the SENT channel with given configuration                     ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE in_device - Device handle                                    ****/
/****                 const void *in_init - Initialization structure                          ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
INLINE void SENT_Initialize_Channel(
    HIODEVICE         in_device,
    const void       *in_init )
{
    IO_Sent_T *sent = (IO_Sent_T*)in_device;
    sent->P->Sent.Initialize_Channel( sent, in_init );
}

/*************************************************************************************************/
/****       $Function: SENT_Shutdown_Channel                                                   ****/
/****       $Content: Shutdown the SENT channel                                               ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE in_device - Device handle                                    ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
INLINE void SENT_Shutdown_Channel(
    HIODEVICE         in_device )
{
    IO_Sent_T *sent = (IO_Sent_T*)in_device;
    sent->P->Sent.Shutdown_Channel( sent );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Parameter Functions                                   +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*************************************************************************************************/
/****       $Function: SENT_Set_Parameter                                                      ****/
/****       $Content: Sets the given parameter for the selected channel                       ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE in_device - Device handle                                    ****/
/****                 Sent_Parameter_T in_parameter - Parameter to set                        ****/
/****                 uint32_t in_value - Parameter value                                     ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
INLINE void SENT_Set_Parameter(
    HIODEVICE         in_device,
    Sent_Parameter_T  in_parameter,
    uint32_t          in_value )
{
    IO_Sent_T *sent = (IO_Sent_T*)in_device;
    sent->P->Sent.Set_Parameter( sent, in_parameter, in_value );
}

/*************************************************************************************************/
/****       $Function: SENT_Get_Parameter                                                      ****/
/****       $Content: Returns the parameter value for the selected channel                    ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE in_device - Device handle                                    ****/
/****                 Sent_Parameter_T in_parameter - Parameter to get                        ****/
/****       $return value: uint32_t - Parameter value                                         ****/
/*************************************************************************************************/
INLINE uint32_t SENT_Get_Parameter(
    HIODEVICE         in_device,
    Sent_Parameter_T  in_parameter )
{
    IO_Sent_T *sent = (IO_Sent_T*)in_device;
    return sent->P->Sent.Get_Parameter( sent, in_parameter );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Status Functions                                      +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*************************************************************************************************/
/****       $Function: SENT_Get_Flag                                                          ****/
/****       $Content: Returns the flag status for the selected channel                        ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE in_device - Device handle                                    ****/
/****                 Sent_Flag_T in_flag - Flag to check                                     ****/
/****       $return value: bool - Flag status                                                 ****/
/*************************************************************************************************/
INLINE bool SENT_Get_Flag(
    HIODEVICE         in_device,
    Sent_Flag_T       in_flag )
{
    IO_Sent_T *sent = (IO_Sent_T*)in_device;
    return sent->P->Sent.Get_Flag( sent, in_flag );
}

/*************************************************************************************************/
/****       $Function: SENT_Get_Error_Count                                                    ****/
/****       $Content: Returns the error count for specified type                              ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE in_device - Device handle                                    ****/
/****                 Sent_Count_Type_T in_count - Count type                                 ****/
/****       $return value: uint8_t - Error count                                              ****/
/*************************************************************************************************/
INLINE uint8_t SENT_Get_Error_Count(
    HIODEVICE           in_device,
    Sent_Count_Type_T   in_count )
{
    IO_Sent_T *sent = (IO_Sent_T*)in_device;
    return sent->P->Sent.Get_Error_Count( sent, in_count );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Message Functions                                     +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*************************************************************************************************/
/****       $Function: SENT_Get_Serial_Msg_And_Count                                          ****/
/****       $Content: Store the serial message and message count                              ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE in_device - Device handle                                    ****/
/****                 Serial_Msg_And_Count_T *const in_ser_msg_and_count - Message buffer     ****/
/****                 bool in_valid - Valid or Buffered message selection                     ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
INLINE void SENT_Get_Serial_Msg_And_Count(
    HIODEVICE                 in_device,
    Serial_Msg_And_Count_T   *const in_ser_msg_and_count,
    bool                      in_valid )
{
    IO_Sent_T *sent = (IO_Sent_T*)in_device;
    sent->P->Sent.Get_Serial_Msg_And_Count( sent, in_ser_msg_and_count, in_valid );
}

/*************************************************************************************************/
/****       $Function: SENT_Get_Cal_Edge_Count                                                ****/
/****       $Content: Returns the time stamp for Cal/Sync pulse                               ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE in_device - Device handle                                    ****/
/****                 bool in_valid - Valid or Buffered timestamp selection                   ****/
/****       $return value: uint32_t - Timestamp value                                         ****/
/*************************************************************************************************/
INLINE uint32_t SENT_Get_Cal_Edge_Count(
    HIODEVICE         in_device,
    bool              in_valid)
{
    IO_Sent_T *sent = (IO_Sent_T*)in_device;
    return sent->P->Sent.Get_Cal_Edge_Count( sent, in_valid );
}

/*************************************************************************************************/
/****       $Function: SENT_Get_Nibbles1_14_And_Count                                         ****/
/****       $Content: Store the nibbles and nibble count                                      ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE in_device - Device handle                                    ****/
/****                 uint8_t *in_buff_nibble - Nibble buffer                                 ****/
/****                 uint8_t *in_nibble_count - Nibble count                                ****/
/****                 bool in_valid - Valid or Buffered selection                             ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
INLINE void SENT_Get_Nibbles1_14_And_Count(
    HIODEVICE                in_device,
    uint8_t                 *in_buff_nibble,
    uint8_t                 *in_nibble_count,
    bool                     in_valid )
{
    IO_Sent_T *sent = (IO_Sent_T*)in_device;
    sent->P->Sent.Get_Nibbles1_14_And_Count( sent, in_buff_nibble, in_nibble_count, in_valid );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Interrupt Functions                                   +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*************************************************************************************************/
/****       $Function: SENT_Set_Host_Interrupt_Enable                                         ****/
/****       $Content: Set the interrupt enable state                                          ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE in_device - Device handle                                    ****/
/****                 bool in_state - Enable state (true=enabled, false=disabled)             ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
INLINE void SENT_Set_Host_Interrupt_Enable(
    HIODEVICE   in_device,
    bool        in_state )
{
    IO_Sent_T *sent = (IO_Sent_T*)in_device;
    sent->P->Sent.Set_Host_Interrupt_Enable( sent, in_state );
}

/*************************************************************************************************/
/****       $Function: SENT_Get_Host_Interrupt_Enable                                         ****/
/****       $Content: Get the interrupt enable state                                          ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE in_device - Device handle                                    ****/
/****       $return value: bool - Enable state (true=enabled, false=disabled)                 ****/
/*************************************************************************************************/
INLINE bool SENT_Get_Host_Interrupt_Enable(
    HIODEVICE   in_device )
{
    IO_Sent_T *sent = (IO_Sent_T*)in_device;
    return sent->P->Sent.Get_Host_Interrupt_Enable( sent );
}

/*************************************************************************************************/
/****       $Function: SENT_Set_Host_Interrupt_Status                                         ****/
/****       $Content: Set the interrupt status                                                ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE in_device - Device handle                                    ****/
/****                 bool in_status - Status (true=request, false=clear)                     ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
INLINE void SENT_Set_Host_Interrupt_Status(
    HIODEVICE   in_device,
    bool        in_status )
{
    IO_Sent_T *sent = (IO_Sent_T*)in_device;
    sent->P->Sent.Set_Host_Interrupt_Status( sent, in_status );
}

/*************************************************************************************************/
/****       $Function: SENT_Get_Host_Interrupt_Status                                         ****/
/****       $Content: Get the interrupt status                                                ****/
/****                                                                                         ****/
/****       $argument: HIODEVICE in_device - Device handle                                    ****/
/****       $return value: bool - Status (true=pending, false=not pending)                    ****/
/*************************************************************************************************/
INLINE bool SENT_Get_Host_Interrupt_Status(
    HIODEVICE   in_device )
{
    IO_Sent_T *sent = (IO_Sent_T*)in_device;
    return sent->P->Sent.Get_Host_Interrupt_Status( sent );
}

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

#endif /* end IO_SENT_H */