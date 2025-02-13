/* CONFIDENTIAL */
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_*/
/*_/_/_/        $DWG-No.: NONE                                                                _/_/_/_*/
/*_/_/_/        $Content: SENT Calibration Definitions                                        _/_/_/_*/
/*_/_/_/        $Category: Application                                                        _/_/_/_*/
/*_/_/_/        $Date: 2024                                                                  _/_/_/_*/
/*_/_/_/        $Design: Calibration Parameters                                              _/_/_/_*/
/*_/_/_/        $Check: NONE                                                                _/_/_/_*/
/*_/_/_/        $Header: sent_cald.c                                                        _/_/_/_*/
/*_/_/_/        $Copyright(C) 2024                                                          _/_/_/_*/
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_*/

/*###############################################################################################*/
/*###                                     $INCLUDE FILES$                                     ####*/
/*###############################################################################################*/
#include "sent_cald.h"      /* To force dec-def checking */

/*###############################################################################################*/
/*###                                     $CALIBRATION DEFINITIONS$                           ####*/
/*###############################################################################################*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     GPF Delta Pressure                                    +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const TsSENT_GPF_SignalScalling SENT_CALXADDR KsSENT_GPF_DeltaPres_SignalScalling = {
    V_RATIO_0_1(100.0/3703), 
    V_KPA_N128_128(-93360.0/3703)
};

const TsSENT_GPF_AbsPres_SignalScalling SENT_CALXADDR KsSENT_GPF_InletPres_SignalScalling = {
    V_RATIO_0_1(0.045908722), 
    V_KPA_N256_256(-58.86038347)
};

const TeSENT_GPF_PresSensorType SENT_CALXADDR KeSENT_GPF_PressSensor = CeSENT_Sensata_None;

const SENT_MsgInit_T SENT_CALXADDR KsSENT_GPF_MsgInit = {
    V_UW_09(3), 0, 0, 1, 1, 0, V_COUNT_BYTE(10)
};

const T_COUNT_BYTE SENT_CALXADDR KfSENT_GPF_DeltPres_CommuFaultThrsh = V_COUNT_BYTE(2);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     GPF Pressure                                          +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const SENT_MsgInit_T SENT_CALXADDR KsSENT_GPF_OutLetPres_MsgInit = {
    V_UW_09(2.9), 0, 0, 1, 1, 0, V_COUNT_BYTE(10)
};

const SENT_MsgInit_T SENT_CALXADDR KsSENT_GPF_InletPres_MsgInit = {
    V_UW_09(2.9), 0, 0, 1, 1, 0, V_COUNT_BYTE(10)
};

const T_COUNT_BYTE SENT_CALXADDR KcSENT_GPF_OutletPres_CommuFaultThrsh = V_COUNT_BYTE(2);
const T_COUNT_BYTE SENT_CALXADDR KcSENT_GPF_InletPres_CommuFaultThrsh = V_COUNT_BYTE(2);

const TsSENT_GPF_AbsPres_SignalScalling SENT_CALXADDR KsSENT_GPF_OutletPres_SignalScalling = {
    V_RATIO_0_1(0.017364299), 
    V_KPA_N256_256(-17.6)
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     GPF Temperature                                       +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const TsSENT_GPF_Temp_SignalScalling SENT_CALXADDR KsSENT_GPF_Temp_SignalScalling = {
    V_RATIO_0_1(0.3333333333), 
    V_DEG_Cb(-73.1499999967)
};

const TeSENT_GPF_Type SENT_CALXADDR KeSENT_GPF_Temp_Type = CeSENT_None;

const T_COUNT_WORD SENT_CALXADDR KcSENT_GPF_Temp_InvalidMax = V_COUNT_WORD(3550);
const T_COUNT_WORD SENT_CALXADDR KcSENT_GPF_Temp_InvalidMin = V_COUNT_WORD(99);

const SENT_MsgInit_T SENT_CALXADDR KsSENT_GPF_Temp_MsgInit = {
    V_UW_09(18), 0, 0, 1, 1, 0, V_COUNT_BYTE(10)
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     ECT Actuator Position                                 +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const TsSENT_ECT_SignalScalling SENT_CALXADDR KsSENT_ECT_ActPstnSignalScalling = {
    V_RATIO_0_1(0.0002442), 
    V_PERCENT_MULTb(0)
};

const TeSENT_ECT_Type SENT_CALXADDR KeSENT_ECT_Type = CeSENT_ECT_None;

const SENT_MsgInit_T SENT_CALXADDR KsSENT_ECT_MsgInit = {
    V_UW_09(3), 0, 0, 1, 0, 0, V_COUNT_BYTE(10)
};

const T_COUNT_BYTE SENT_CALXADDR KfSENT_ECT_ActPstn_CommuFaultThrsh = V_COUNT_BYTE(2);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     EGR Delta Pressure                                    +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const TsSENT_EGR_SignalScalling SENT_CALXADDR KsSENT_EGR_DeltPres_SignalScalling = {
    V_RATIO_0_1(100.0/3703), 
    V_KPA_N128_128(-93360.0/3703)
};

const TeSENT_EGR_Type SENT_CALXADDR KeSENT_EGR_Type = CeSENT_EGR_None;

const SENT_MsgInit_T SENT_CALXADDR KsSENT_EGR_MsgInit = {
    V_UW_09(3), 0, 0, 1, 1, 0, V_COUNT_BYTE(10)
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Common Parameters                                     +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const T_COUNT_BYTE SENT_CALXADDR KcSENT_FastSamplingAvgCountThrsh = V_COUNT_BYTE(10);

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