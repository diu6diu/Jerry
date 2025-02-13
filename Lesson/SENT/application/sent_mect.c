/* CONFIDENTIAL */
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_*/
/*_/_/_/        $DWG-No.: NONE                                                                _/_/_/_*/
/*_/_/_/        $Content: SENT ECT Communication Protocol                                     _/_/_/_*/
/*_/_/_/        $Category: Application                                                        _/_/_/_*/
/*_/_/_/        $Date: 2024                                                                  _/_/_/_*/
/*_/_/_/        $Design: ECT Message Processing                                              _/_/_/_*/
/*_/_/_/        $Check: NONE                                                                _/_/_/_*/
/*_/_/_/        $Header: sent_mect.c                                                        _/_/_/_*/
/*_/_/_/        $Copyright(C) 2024                                                          _/_/_/_*/
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_*/

/*###############################################################################################*/
/*###                                     $INCLUDE FILES$                                     ####*/
/*###############################################################################################*/
/* Global Types */
#include "t_gentypes.h"      /* For Global Data Types                */
#include "sent_fexi.h"       /* For external resource declarations   */

/* Subsystem Headers */
#include "sent_io.h"
#include "sent_cald.h"

/*###############################################################################################*/
/*###                                     $GLOBAL VARIABLES$                                  ####*/
/*###############################################################################################*/
TbBOOLEAN            VOL0ADDR VbSENT_ECT_ActPstn_Ch1ErrFlag;
T_PERCENT_MULTb      VOL0ADDR VfSENT_Pct_ECT_ActPstn;
SENT_MsgFault_T      VOL0ADDR VsSENT_ECT_ActPstn_MsgFaults;
TbBOOLEAN            VOL0ADDR VbSENT_ECT_ActPstn_CommuFault;
SENT_MsgParams_T     VOL0ADDR VsSENT_ECT_MsgStructure;

/*###############################################################################################*/
/*###                                     $STATIC VARIABLES$                                  ####*/
/*###############################################################################################*/
static T_COUNT_BYTE ScSENT_GPF_CRCFailCnt;
static T_COUNT_BYTE ScSENT_ECT_ActPstn_CommuFault_Counter;

/*###############################################################################################*/
/*###                                     $FUNCTION DEFINITIONS$                              ####*/
/*###############################################################################################*/

/*************************************************************************************************/
/****       $Function: InitSENT_ECT_ResetToKeyOn                                              ****/
/****       $Content: Initialize SENT ECT parameters at key on                                 ****/
/****                                                                                         ****/
/****       $argument: None                                                                   ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
void InitSENT_ECT_ResetToKeyOn(void)
{
    if (CeSENT_Triaxis == KeSENT_ECT_Type)
    {
        SetHWIO_SENTParameters(SENT_SENSOR_ECT, KsSENT_ECT_MsgInit);
    }
}

/*************************************************************************************************/
/****       $Function: ProcessSENT_ECT_MessageData                                            ****/
/****       $Content: Process received ECT SENT message data                                  ****/
/****                - Check message faults                                                    ****/
/****                - Process actual position value                                          ****/
/****                - Handle communication faults                                            ****/
/****                                                                                         ****/
/****       $argument: None                                                                   ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
void ProcessSENT_ECT_MessageData(void)
{
    WORD LwSENT_ECT_ActPstn_Value;

    /* Only process if ECT type is Triaxis */
    if (CeSENT_Triaxis == KeSENT_ECT_Type)
    {
        /* Get message data and fault status */
        VsSENT_ECT_ActPstn_MsgFaults.Byte = (BYTE)GetHWIO_SENT_Msg(SENT_SENSOR_ECT, 
                                                                   &VsSENT_ECT_MsgStructure);
        
        /* Extract error flag from fast nibbles */
        VbSENT_ECT_ActPstn_Ch1ErrFlag = (TbBOOLEAN)(VsSENT_ECT_MsgStructure.FastNibbles[0] 
                                                    & SENT_ERROR_MASK1);
        
        /* Combine fast nibbles to form position value */
        LwSENT_ECT_ActPstn_Value = ((((WORD)VsSENT_ECT_MsgStructure.FastNibbles[1]) << 8) +
                                   (VsSENT_ECT_MsgStructure.FastNibbles[2] << 4) +
                                    VsSENT_ECT_MsgStructure.FastNibbles[3]);

        /* Handle communication fault detection */
        if (CbTRUE == VsSENT_ECT_ActPstn_MsgFaults.bits.TotalMsgFault)
        {
            if (ScSENT_ECT_ActPstn_CommuFault_Counter >= KfSENT_ECT_ActPstn_CommuFaultThrsh)
            {
                VbSENT_ECT_ActPstn_CommuFault = CbTRUE;
            }
            else
            {
                ScSENT_ECT_ActPstn_CommuFault_Counter++;
            }
        }
        else
        {
            VbSENT_ECT_ActPstn_CommuFault = CbFALSE;
            ScSENT_ECT_ActPstn_CommuFault_Counter = 0;
        }

        /* Process position value if no faults */
        if ((CbFALSE == VbSENT_ECT_ActPstn_Ch1ErrFlag) &&
            (CbFALSE == VbSENT_ECT_ActPstn_CommuFault))
        {
            /* Apply scaling to raw position value */
            VfSENT_Pct_ECT_ActPstn = usADD_us_usp(
                usMUL_us_usp(LwSENT_ECT_ActPstn_Value,
                            KsSENT_ECT_ActPstnSignalScalling.KfSENT_Slope,
                            S_COUNT_WORD,
                            S_RATIO_0_1,
                            S_PERCENT_MULTb),
                KsSENT_ECT_ActPstnSignalScalling.KfSENT_Offset,
                S_PERCENT_MULTb,
                S_PERCENT_MULTb,
                S_PERCENT_MULTb);
        }
    }
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