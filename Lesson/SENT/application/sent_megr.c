/* CONFIDENTIAL */
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_*/
/*_/_/_/        $DWG-No.: NONE                                                                _/_/_/_*/
/*_/_/_/        $Content: SENT EGR Communication Protocol                                     _/_/_/_*/
/*_/_/_/        $Category: Application                                                        _/_/_/_*/
/*_/_/_/        $Date: 2024                                                                  _/_/_/_*/
/*_/_/_/        $Design: EGR Message Processing                                              _/_/_/_*/
/*_/_/_/        $Check: NONE                                                                _/_/_/_*/
/*_/_/_/        $Header: sent_megr.c                                                        _/_/_/_*/
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
TbBOOLEAN            VOL0ADDR VbSENT_EGR_DeltPres_Ch1ErrFlag;
T_KPA_N128_128       VOL0ADDR VfSENT_EGR_DeltPres;
SENT_MsgFault_T      VOL0ADDR VsSENT_EGR_MsgFaults;
SENT_MsgParams_T     VOL0ADDR VsSENT_EGR_MsgStructure;

/*###############################################################################################*/
/*###                                     $FUNCTION DEFINITIONS$                              ####*/
/*###############################################################################################*/

/*************************************************************************************************/
/****       $Function: InitSENT_EGR_ResetToKeyOn                                              ****/
/****       $Content: Initialize SENT EGR parameters at key on                                 ****/
/****                                                                                         ****/
/****       $argument: None                                                                   ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
void InitSENT_EGR_ResetToKeyOn(void)
{
    if (CeSENT_EGR_Sensata == KeSENT_EGR_Type)
    {
        SetHWIO_SENTParameters(SENT_SENSOR_EGR, KsSENT_EGR_MsgInit);
    }
}

/*************************************************************************************************/
/****       $Function: ProcessSENT_EGR_DeltPres_MessageData                                   ****/
/****       $Content: Process received EGR SENT message data                                  ****/
/****                - Check message faults                                                    ****/
/****                - Process delta pressure value                                           ****/
/****                - Apply signal scaling                                                    ****/
/****                                                                                         ****/
/****       $argument: None                                                                   ****/
/****       $return value: None                                                               ****/
/*************************************************************************************************/
void ProcessSENT_EGR_DeltPres_MessageData(void)
{
    WORD LfSENT_EGR_DeltPres_Value;

    /* Only process if EGR type is Sensata */
    if (CeSENT_EGR_Sensata == KeSENT_EGR_Type)
    {
        /* Get message data and fault status */
        VsSENT_EGR_MsgFaults.Byte = (BYTE)GetHWIO_SENT_Msg(SENT_SENSOR_EGR, 
                                                          &VsSENT_EGR_MsgStructure);
        
        /* Extract error flag from fast nibbles */
        VbSENT_EGR_DeltPres_Ch1ErrFlag = (TbBOOLEAN)(VsSENT_EGR_MsgStructure.FastNibbles[0] 
                                                     & SENT_ERROR_MASK1);
        
        /* Combine fast nibbles to form delta pressure value */
        LfSENT_EGR_DeltPres_Value = ((((WORD)VsSENT_EGR_MsgStructure.FastNibbles[1]) << 8) +
                                    (VsSENT_EGR_MsgStructure.FastNibbles[2] << 4) +
                                     VsSENT_EGR_MsgStructure.FastNibbles[3]);

        /* Process delta pressure value if no error */
        if (!VbSENT_EGR_DeltPres_Ch1ErrFlag)
        {
            /* Apply scaling to raw delta pressure value */
            VfSENT_EGR_DeltPres = ssADD_us_ssp(
                usMUL_us_usp(LfSENT_EGR_DeltPres_Value,
                            KsSENT_EGR_DeltPres_SignalScalling.KfSENT_EGR_DeltPresSlope,
                            S_COUNT_WORD,
                            S_RATIO_0_1,
                            S_KPA),
                KsSENT_EGR_DeltPres_SignalScalling.KfSENT_EGR_DeltPresOffset,
                S_KPA,
                S_KPA_N128_128,
                S_KPA_N128_128);
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