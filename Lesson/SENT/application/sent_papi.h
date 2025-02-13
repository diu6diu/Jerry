/* CONFIDENTIAL */
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_*/
/*_/_/_/        $DWG-No.: NONE                                                                _/_/_/_*/
/*_/_/_/        $Content: Protected output interface header file for SENT subsystem           _/_/_/_*/
/*_/_/_/        $Category: Header                                                            _/_/_/_*/
/*_/_/_/        $Date: 2024                                                                  _/_/_/_*/
/*_/_/_/        $Design: Protected Application Programming Interface                         _/_/_/_*/
/*_/_/_/        $Check: NONE                                                                _/_/_/_*/
/*_/_/_/        $Header: sent_papi.h                                                        _/_/_/_*/
/*_/_/_/        $Copyright(C) 2024                                                          _/_/_/_*/
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_*/

#ifndef SENT_PAPI_H
#define SENT_PAPI_H

/*###############################################################################################*/
/*###                                     $INCLUDE FILES$                                     ####*/
/*###############################################################################################*/
#include "t_gentypes.h"       /* For global data types                */
#include "sent_nvmd.h"
#include "sent_styp.h"        /* For TeSENT_GPF_PresSensorType       */

/*###############################################################################################*/
/*###                                  $FUNCTION DECLARATIONS$                                ####*/
/*###############################################################################################*/
FARFUNC void InitSENT_ResetToKeyOn(void);
FARFUNC void MngSENT_BaseLoopTasks(void);
FARFUNC void MngSENT_6X_EventTasks(void);
FARFUNC void MngSENT_12X_EventTasks(void);
FARFUNC void MngSENT_125msTasks(void);
FARFUNC void MngSENT_1msTasks(void);

/*###############################################################################################*/
/*###                                  $VARIABLE EXPORTS$                                     ####*/
/*###############################################################################################*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     GPF Delta Pressure                                    +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
extern TbBOOLEAN            VOL0ADDR         VbSENT_GPF_DeltPres_SensorFaults; 
#define GetSENT_GPF_DP_SensorFaults()       (VbSENT_GPF_DeltPres_SensorFaults)

extern TbBOOLEAN            VOL0ADDR         VbSENT_GPF_DeltPres_CommuFault;
#define GetSENT_GPF_DP_CommuFaults()        (VbSENT_GPF_DeltPres_CommuFault)

extern TbBOOLEAN            VOL0ADDR         VbSENT_GPF_DeltPresValid;
#define GetSENT_GPF_DeltPresValid()         (VbSENT_GPF_DeltPresValid)

extern T_KPA_N128_128       VOL0ADDR         VfSENT_GPF_DeltPres;
#define GetSENT_p_GPF_DeltPres()           (VfSENT_GPF_DeltPres)

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     ECT Actual Position                                   +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
extern TbBOOLEAN            VOL0ADDR         VbSENT_ECT_ActPstn_CommuFault;
#define GetSENT_ECT_ActPstn_CommuFaults()   (VbSENT_ECT_ActPstn_CommuFault)

extern TbBOOLEAN            VOL0ADDR         VbSENT_ECT_ActPstn_Ch1ErrFlag;
#define GetSENT_ECT_ActPstn_SensorFaults()  (VbSENT_ECT_ActPstn_Ch1ErrFlag)

extern T_PERCENT_MULTb      VOL0ADDR         VfSENT_Pct_ECT_ActPstn;
#define GetSENT_Pct_ECT_ActPstn()           (VfSENT_Pct_ECT_ActPstn)

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     GPF Temperature                                       +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
extern T_DEG_Cb             VOL0ADDR         VfSENT_GPF_Temp;
#define GetSENT_T_GPF_Temp()                (VfSENT_GPF_Temp)

extern TbBOOLEAN            VOL0ADDR         VbSENT_GPF_Temp_Ch1ErrFlag;
#define GetSENT_GPF_Temp_ChErrFlag()        (VbSENT_GPF_Temp_Ch1ErrFlag)

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     EGR Delta Pressure                                    +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
extern TbBOOLEAN            VOL0ADDR         VbSENT_EGR_DeltPres_Ch1ErrFlag; 
#define GetSENT_EGR_DeltPres_ChErrFlag()    (VbSENT_EGR_DeltPres_Ch1ErrFlag)

extern T_KPA_N128_128       VOL0ADDR         VfSENT_EGR_DeltPres;
#define GetSENT_p_EGR_DeltPres()            (VfSENT_EGR_DeltPres)

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     GPF Outlet Pressure                                   +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
extern TbBOOLEAN            VOL0ADDR         VbSENT_GPF_OutletPres_SensorFaults;
#define GetSENT_GPF_OP_SensorFaults()       (VbSENT_GPF_OutletPres_SensorFaults)

extern TbBOOLEAN            VOL0ADDR         VbSENT_GPF_OutletPres_CommuFault;
#define GetSENT_GPF_OP_CommuFaults()        (VbSENT_GPF_OutletPres_CommuFault)

extern T_KPA_N256_256       VOL0ADDR         VfSENT_GPF_OutletPres;
#define GetSENT_p_GPF_OutletPres()          (VfSENT_GPF_OutletPres)

extern TbBOOLEAN            VOL0ADDR         VbSENT_GPF_OutletPresValid;
#define GetSENT_GPF_OutletPresValid()       (VbSENT_GPF_OutletPresValid)

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     GPF Inlet Pressure                                    +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
extern TbBOOLEAN            VOL0ADDR         VbSENT_GPF_InletPres_SensorFaults;
#define GetSENT_GPF_IP_SensorFaults()       (VbSENT_GPF_InletPres_SensorFaults)

#define GetSENT_BaseCounterData()           (NsSENT_BaseData)

extern T_KPA_N256_256       VOL0ADDR         VfSENT_GPF_InletAbsPres;
#define GetSENT_p_GPF_B1_InletAbsPres()     (VfSENT_GPF_InletAbsPres)

extern T_KPA_N256_256       VOL0ADDR         VfSENT_GPF_OutletAbsPres;
#define GetSENT_p_GPF_B1_OutletAbsPres()    (VfSENT_GPF_OutletAbsPres)

extern const TeSENT_GPF_PresSensorType      KeSENT_GPF_PressSensor;

#define GetSENT_GPF_PressSensor_Bosch_Inlet_OutletPres() \
    ((KeSENT_GPF_PressSensor == CeSENT_Bosch_Inlet_OutletPres) ? CbTRUE : CbFALSE)

#define GetSENT_GPF_PressSensor_Sensata_Delt_OutletPres() \
    ((KeSENT_GPF_PressSensor == CeSENT_Sensata_Delt_OutletPres) ? CbTRUE : CbFALSE)

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

#endif /* end SENT_PAPI_H */