/* CONFIDENTIAL */
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_*/
/*_/_/_/        $DWG-No.: MT95P1                                                        _/_/_/_*/
/*_/_/_/        $Content: SENT subsystem calibration declarations                       _/_/_/_*/
/*_/_/_/        $Category: Header                                                      _/_/_/_*/
/*_/_/_/        $Date: 2024/02/14                                                     _/_/_/_*/
/*_/_/_/        $Design: Wenjie                                                       _/_/_/_*/
/*_/_/_/        $Check:                                                              _/_/_/_*/
/*_/_/_/        $Header: sent_cald.h                                                 _/_/_/_*/
/*_/_/_/        $Copyright(C) 2024 Delphi                                           _/_/_/_*/
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_*/

#ifndef SENT_CALD_H
#define SENT_CALD_H

/*###############################################################################################*/
/*###                                     $INCLUDE FILES$                                    ####*/
/*###############################################################################################*/
#include "t_gentypes.h"       /* For global data types                */
#include "sent_styp.h"
#include "sent_io.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     $MACRO DEFINITION$                                   +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define SENT_CALXADDR

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                              $GPF CALIBRATION DECLARATIONS$                             +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* GPF Pressure Related Calibrations */
extern const TsSENT_GPF_SignalScalling         SENT_CALXADDR KsSENT_GPF_DeltaPres_SignalScalling;
extern const TsSENT_GPF_AbsPres_SignalScalling SENT_CALXADDR KsSENT_GPF_InletPres_SignalScalling;
extern const TsSENT_GPF_AbsPres_SignalScalling SENT_CALXADDR KsSENT_GPF_OutletPres_SignalScalling;
extern const TeSENT_GPF_PresSensorType         SENT_CALXADDR KeSENT_GPF_PressSensor;

/* GPF Message Initialization */
extern const SENT_MsgInit_T                    SENT_CALXADDR KsSENT_GPF_MsgInit;
extern const SENT_MsgInit_T                    SENT_CALXADDR KsSENT_GPF_InletPres_MsgInit;
extern const SENT_MsgInit_T                    SENT_CALXADDR KsSENT_GPF_OutLetPres_MsgInit;
extern const SENT_MsgInit_T                    SENT_CALXADDR KsSENT_GPF_Temp_MsgInit;

/* GPF Temperature Related Calibrations */
extern const TsSENT_GPF_Temp_SignalScalling    SENT_CALXADDR KsSENT_GPF_Temp_SignalScalling;
extern const TeSENT_GPF_Type                   SENT_CALXADDR KeSENT_GPF_Temp_Type;
extern const T_COUNT_WORD                      SENT_CALXADDR KcSENT_GPF_Temp_InvalidMax;
extern const T_COUNT_WORD                      SENT_CALXADDR KcSENT_GPF_Temp_InvalidMin;

/* GPF Communication Fault Thresholds */
extern const T_COUNT_BYTE                      SENT_CALXADDR KfSENT_GPF_DeltPres_CommuFaultThrsh;
extern const T_COUNT_BYTE                      SENT_CALXADDR KcSENT_GPF_OutletPres_CommuFaultThrsh;
extern const T_COUNT_BYTE                      SENT_CALXADDR KcSENT_GPF_InletPres_CommuFaultThrsh;
extern const T_COUNT_BYTE                      SENT_CALXADDR KcSENT_FastSamplingAvgCountThrsh;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                              $ECT CALIBRATION DECLARATIONS$                             +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
extern const TsSENT_ECT_SignalScalling         SENT_CALXADDR KsSENT_ECT_ActPstnSignalScalling;
extern const TeSENT_ECT_Type                   SENT_CALXADDR KeSENT_ECT_Type;
extern const SENT_MsgInit_T                    SENT_CALXADDR KsSENT_ECT_MsgInit;
extern const T_COUNT_BYTE                      SENT_CALXADDR KfSENT_ECT_ActPstn_CommuFaultThrsh;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                              $EGR CALIBRATION DECLARATIONS$                             +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
extern const TsSENT_EGR_SignalScalling         SENT_CALXADDR KsSENT_EGR_DeltPres_SignalScalling;
extern const TeSENT_EGR_Type                   SENT_CALXADDR KeSENT_EGR_Type;
extern const SENT_MsgInit_T                    SENT_CALXADDR KsSENT_EGR_MsgInit;

#endif /* SENT_CALD_H */

/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
/*---                                   REVISION MANAGEMENT                                   ---*/
/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/*---                                                                                         ---*/
/*---    $Rev:No:    date:       name:       company:    minute:    content:                 ---*/
/*---    $Rev:0      2024/02/14  Wenjie      Delphi     Initial version                     ---*/
/*---                                                                                         ---*/
/*-----------------------------------------------------------------------------------------------*/
/*---
        $Log$
                                                                                              ---*/
/*-----------------------------------------------------------------------------------------------*/