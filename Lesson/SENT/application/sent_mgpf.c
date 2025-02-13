/******************************************************************************
 * Filename:          sent_mgpf.c
 *
 * Description:       SENT(Single Edge Nibble Transmission)communication protocol for GPF.
 * Global Functions Defined:
 *                  InitSENT_GPF_ResetToKeyOn
 *                  ProcessSENT_GPF_Press_MessageData
 *                  ProcessSENT_GPF_Temp_MessageData
 *                  MngSENT_1msTasks
 * Static Functions Defined:
 *                  UpdateSENT_GPF_DeltaPres
 *                  UpdateSENT_GPF_OutletPres
 *                  UpdateSENT_GPF_InletAbsPres
 *                  UpdateSENT_GPF_OutletAbsPres
 * Inline Functions Defined:
 *                  None
 *
 ******************************************************************************/

/******************************************************************************
 *  Subsystem Input Interface File
 *****************************************************************************/
#include "t_gentypes.h" /* For Global Data Types                */
#include "sent_fexi.h"  /* For external resource declarations */

/******************************************************************************
 *  Subsystem Include Files
 *****************************************************************************/
#include "sent_io.h"
#include "sent_cald.h"
#include "sent_nvmd.h"

/******************************************************************************
 * Global Data Define
 ******************************************************************************/
SENT_MsgParams_T     VOL0ADDR VsSENT_GPF_MsgStructure;
SENT_MsgParams_T     VaSENT_GPF_MsgStructure[15];
SENT_MsgParams_T     VaSENT_GPF_OP_MsgStructure[15];

T_KPA_N128_128       VOL0ADDR VfSENT_GPF_DeltPres;
SENT_MsgFault_T      VOL0ADDR VsSENT_GPF_DeltPres_MsgFaults;
TbBOOLEAN            VOL0ADDR VbSENT_GPF_DeltPres_SensorFaults;
TbBOOLEAN            VOL0ADDR VbSENT_GPF_DeltPres_CommuFault;
TbBOOLEAN            VOL0ADDR VbSENT_GPF_DeltPresValid;

TbBOOLEAN            VOL0ADDR VbSENT_GPF_Temp_Ch1ErrFlag;
T_DEG_Cb             VOL0ADDR VfSENT_GPF_Temp;
SENT_MsgFault_T      VOL0ADDR VsSENT_GPF_Temp_MsgFaults;
SENT_MsgParams_T     VOL0ADDR VsSENT_GPF_Temp_MsgStructure;

T_KPA_N256_256       VOL0ADDR VfSENT_GPF_OutletPres;
T_KPA_N256_256       VOL0ADDR VfSENT_GPF_OutletAbsPres;
SENT_MsgFault_T      VOL0ADDR VsSENT_GPF_OutletPres_MsgFaults;
TbBOOLEAN            VOL0ADDR VbSENT_GPF_OutletPres_SensorFaults;
TbBOOLEAN            VOL0ADDR VbSENT_GPF_OutletPres_CommuFault;
TbBOOLEAN            VOL0ADDR VbSENT_GPF_OutletPresValid;

T_KPA_N256_256       VOL0ADDR VfSENT_GPF_InletAbsPres;
SENT_MsgFault_T      VOL0ADDR VsSENT_GPF_InletPres_MsgFaults;
TbBOOLEAN            VOL0ADDR VbSENT_GPF_InletPres_SensorFaults;
TbBOOLEAN            VOL0ADDR VbSENT_GPF_InletPres_CommuFault;
TbBOOLEAN            VOL0ADDR VbSENT_GPF_InletPresValid;

/******************************************************************************
 * Static Data Define
 ******************************************************************************/
static T_COUNT_BYTE ScSENT_GPF_CRCFailCnt;
static T_COUNT_BYTE ScSENT_GPF_DeltPres_CommuFault_Counter;
static T_COUNT_BYTE ScSENT_GPF_InletPres_CommuFault_Counter;
static T_COUNT_BYTE ScSENT_GPF_OutletPres_CommuFault_Counter;
static T_COUNT_BYTE ScSENT_GPF_FastSamplingCount;

/******************************************************************************
 *  Function Definitions
 *****************************************************************************/

/******************************************************************************
 *
 * Function:    InitSENT_GPF_ResetToKeyOn
 * Description: This function initial SENT
 *              GPF parameters.
 * Parameters:  None
 * Return:      None
 *****************************************************************************/
void InitSENT_GPF_ResetToKeyOn(void)
{
  if((CeSENT_Sensata_DeltPresOnly == KeSENT_GPF_PressSensor)
   ||(CeSENT_Sensata_Delt_OutletPres == KeSENT_GPF_PressSensor))
  {
    SetHWIO_SENTParameters(SENT_SENSOR_GPF, KsSENT_GPF_MsgInit);
  }
  if(CeSENT_Bosch_Inlet_OutletPres == KeSENT_GPF_PressSensor)
  {
    SetHWIO_SENTParameters(SENT_SENSOR_GPF_Outlet, KsSENT_GPF_OutLetPres_MsgInit);
    SetHWIO_SENTParameters(SENT_SENSOR_GPF_Inlet, KsSENT_GPF_InletPres_MsgInit);
  }
  if(CeSENT_Sensata == KeSENT_GPF_Temp_Type)
  {
    SetHWIO_SENTParameters(SENT_SENSOR_GPF_TEMP, KsSENT_GPF_Temp_MsgInit);
  }

  ScSENT_GPF_FastSamplingCount = V_COUNT_BYTE(0);
}

/******************************************************************************
 *
 * Function:    UpdateSENT_GPF_DeltaPres
 * Description:
 * Parameters:  None
 * Return:      None
 *****************************************************************************/
static void UpdateSENT_GPF_DeltaPres(void)
{
   WORD     LfSENT_GPF_DeltPres_Value;

   VsSENT_GPF_DeltPres_MsgFaults.Byte = (BYTE)GetHWIO_SENT_Msg(SENT_SENSOR_GPF, &VsSENT_GPF_MsgStructure);
   VbSENT_GPF_DeltPres_SensorFaults = (TbBOOLEAN)(VsSENT_GPF_MsgStructure.FastNibbles[0]& SENT_ERROR_MASK1);
   LfSENT_GPF_DeltPres_Value = ( (((WORD)VsSENT_GPF_MsgStructure.FastNibbles[1]) << 8) +
                                     (VsSENT_GPF_MsgStructure.FastNibbles[2] << 4) +
                                      VsSENT_GPF_MsgStructure.FastNibbles[3] );
   if( CbTRUE == VsSENT_GPF_DeltPres_MsgFaults.bits.TotalMsgFault)
   {
      if(ScSENT_GPF_DeltPres_CommuFault_Counter >= KfSENT_GPF_DeltPres_CommuFaultThrsh)
      {
         VbSENT_GPF_DeltPres_CommuFault = CbTRUE;
      }
      else
      {
         ScSENT_GPF_DeltPres_CommuFault_Counter = INCusp(ScSENT_GPF_DeltPres_CommuFault_Counter);
      }
   }
   else
   {
      VbSENT_GPF_DeltPres_CommuFault = CbFALSE;
      ScSENT_GPF_DeltPres_CommuFault_Counter = V_COUNT_BYTE(0);
   }
  
   VbSENT_GPF_DeltPresValid = (( CbFALSE == VbSENT_GPF_DeltPres_SensorFaults )
                                &&( CbFALSE ==VbSENT_GPF_DeltPres_CommuFault ));
   if( CbTRUE == VbSENT_GPF_DeltPresValid )
   {
      VfSENT_GPF_DeltPres = ssADD_us_ssp( usMUL_us_usp( LfSENT_GPF_DeltPres_Value,
                                                        KsSENT_GPF_DeltaPres_SignalScalling.KfSENT_GPFSlope,
                                                        S_COUNT_WORD,
                                                        S_RATIO_0_1,
                                                        S_KPA ),
                                          KsSENT_GPF_DeltaPres_SignalScalling.KfSENT_GPFOffset,
                                          S_KPA,
                                          S_KPA_N128_128,
                                          S_KPA_N128_128 );
   }
}

/******************************************************************************
 *
 * Function:    UpdateSENT_GPF_OutletPres
 * Description:
 * Parameters:  None
 * Return:      None
 *****************************************************************************/
static void UpdateSENT_GPF_OutletPres(void)
{
   WORD     LfSENT_GPF_OutletPres_Value;

   VsSENT_GPF_OutletPres_MsgFaults.Byte = (BYTE)GetHWIO_SENT_Msg(SENT_SENSOR_GPF, &VsSENT_GPF_MsgStructure);
   VbSENT_GPF_OutletPres_SensorFaults = (TbBOOLEAN)((VsSENT_GPF_MsgStructure.FastNibbles[0]& SENT_ERROR_MASK2)>> 1);
   LfSENT_GPF_OutletPres_Value = ( (((WORD)VsSENT_GPF_MsgStructure.FastNibbles[6]) << 8) +
                                  (VsSENT_GPF_MsgStructure.FastNibbles[5] << 4) +
                                   VsSENT_GPF_MsgStructure.FastNibbles[4] );
   if( CbTRUE == VsSENT_GPF_OutletPres_MsgFaults.bits.TotalMsgFault)
   {
      if(ScSENT_GPF_OutletPres_CommuFault_Counter >= KcSENT_GPF_OutletPres_CommuFaultThrsh)
      {
         VbSENT_GPF_OutletPres_CommuFault = CbTRUE;
      }
      else
      {
         ScSENT_GPF_OutletPres_CommuFault_Counter =INCusp(ScSENT_GPF_OutletPres_CommuFault_Counter);
      }
   }
   else
   {
     VbSENT_GPF_OutletPres_CommuFault = CbFALSE;
     ScSENT_GPF_OutletPres_CommuFault_Counter = V_COUNT_BYTE(0);
   }
  
   VbSENT_GPF_OutletPresValid = (( CbFALSE == VbSENT_GPF_OutletPres_SensorFaults )
                                &&( CbFALSE ==VbSENT_GPF_OutletPres_CommuFault ));
   if( CbTRUE == VbSENT_GPF_OutletPresValid )
   {
      VfSENT_GPF_OutletPres = ssADD_us_ssp( usMUL_us_usp( LfSENT_GPF_OutletPres_Value,
                                                       KsSENT_GPF_OutletPres_SignalScalling.KfSENT_GPFSlope,
                                                       S_COUNT_WORD,
                                                       S_RATIO_0_1,
                                                       S_KPA_0_512),
                                           KsSENT_GPF_OutletPres_SignalScalling.KfSENT_GPFOffset,
                                           S_KPA_0_512,
                                           S_KPA_N256_256,
                                           S_KPA_N256_256);
   }
}

/******************************************************************************
 *
 * Function:    UpdateSENT_GPF_InletAbsPres
 * Description:
 * Parameters:  None
 * Return:      None
 *****************************************************************************/
static void UpdateSENT_GPF_InletAbsPres(void)
{
   WORD               LfSENT_GPF_InletPres_Value;
   SENT_MsgParams_T   LsSENT_GPF_MsgStructure; 
   T_COUNT_BYTE       LySent_Index;
   
   LfSENT_GPF_InletPres_Value = V_COUNT_WORD(0);

   VsSENT_GPF_InletPres_MsgFaults.Byte = 
                      (BYTE)GetHWIO_SENT_Msg(SENT_SENSOR_GPF_Inlet, &LsSENT_GPF_MsgStructure);
   
   VbSENT_GPF_InletPres_SensorFaults = (TbBOOLEAN)( LsSENT_GPF_MsgStructure.FastNibbles[0]& SENT_ERROR_MASK1); 

   if( CbTRUE == VsSENT_GPF_InletPres_MsgFaults.bits.TotalMsgFault)
   {
      if(ScSENT_GPF_InletPres_CommuFault_Counter >= KcSENT_GPF_InletPres_CommuFaultThrsh)
      {
         VbSENT_GPF_InletPres_CommuFault = CbTRUE;
      }
      else
      {
         ScSENT_GPF_InletPres_CommuFault_Counter = INCusp(ScSENT_GPF_InletPres_CommuFault_Counter);
      }
   }
   else
   {
      VbSENT_GPF_InletPres_CommuFault = CbFALSE;
      ScSENT_GPF_InletPres_CommuFault_Counter = V_COUNT_BYTE(0);
   }

   for(LySent_Index = 0;LySent_Index < KcSENT_FastSamplingAvgCountThrsh;LySent_Index++)
   {
     LfSENT_GPF_InletPres_Value += ( (((WORD)VaSENT_GPF_MsgStructure[LySent_Index].FastNibbles[1]) << 8) +
                                    (VaSENT_GPF_MsgStructure[LySent_Index].FastNibbles[2] << 4) +
                                     VaSENT_GPF_MsgStructure[LySent_Index].FastNibbles[3] );
   }

   LfSENT_GPF_InletPres_Value = usDIV_us_usp(LfSENT_GPF_InletPres_Value,
                                            KcSENT_FastSamplingAvgCountThrsh,
                                            S_COUNT_WORD,
                                            S_COUNT_BYTE,
                                            S_COUNT_WORD );

   VbSENT_GPF_InletPresValid = ((CbFALSE == VbSENT_GPF_InletPres_SensorFaults)
                             &&(CbFALSE == VbSENT_GPF_InletPres_CommuFault));
               
   if(CbTRUE == VbSENT_GPF_InletPresValid)
   {
      VfSENT_GPF_InletAbsPres = 
        ssADD_us_ssp(usMUL_us_usp(LfSENT_GPF_InletPres_Value,
                                 KsSENT_GPF_InletPres_SignalScalling.KfSENT_GPFSlope,
                                 S_COUNT_WORD,
                                 S_RATIO_0_1,
                                 S_KPA_0_512),
                    KsSENT_GPF_InletPres_SignalScalling.KfSENT_GPFOffset,
                    S_KPA_0_512,
                    S_KPA_N256_256,
                    S_KPA_N256_256);
   }

   VbSENT_GPF_DeltPres_SensorFaults = ( (CbTRUE == VbSENT_GPF_OutletPres_SensorFaults)
                                     ||(CbTRUE == VbSENT_GPF_InletPres_SensorFaults) );

   VbSENT_GPF_DeltPres_CommuFault = ( (CbTRUE == VbSENT_GPF_OutletPres_CommuFault)
                                   ||(CbTRUE == VbSENT_GPF_InletPres_CommuFault) );

   VbSENT_GPF_DeltPresValid = ( (CbTRUE == VbSENT_GPF_OutletPresValid)
                             &&(CbTRUE == VbSENT_GPF_InletPresValid) );

   if(CbTRUE == VbSENT_GPF_DeltPresValid)
   {
      VfSENT_GPF_DeltPres =
               ssSUB_ss_ssp(VfSENT_GPF_InletAbsPres,
                           VfSENT_GPF_OutletAbsPres,
                           S_KPA_N256_256,
                           S_KPA_N256_256,
                           S_KPA_N128_128);
   }
}

/******************************************************************************
 *
 * Function:    UpdateSENT_GPF_OutletAbsPres
 * Description:
 * Parameters:  None
 * Return:      None
 *****************************************************************************/
static void UpdateSENT_GPF_OutletAbsPres(void)
{
   WORD               LfSENT_GPF_OutletPres_Value;
   SENT_MsgParams_T   LsSENT_GPF_MsgStructure;
   T_COUNT_BYTE       LySent_Index;

   LfSENT_GPF_OutletPres_Value = V_COUNT_WORD(0);

   VsSENT_GPF_OutletPres_MsgFaults.Byte = (BYTE)GetHWIO_SENT_Msg(SENT_SENSOR_GPF_Outlet, &LsSENT_GPF_MsgStructure);
   VbSENT_GPF_OutletPres_SensorFaults = (TbBOOLEAN)( LsSENT_GPF_MsgStructure.FastNibbles[0]& SENT_ERROR_MASK1);

   if( CbTRUE == VsSENT_GPF_OutletPres_MsgFaults.bits.TotalMsgFault)
   {
      if(ScSENT_GPF_OutletPres_CommuFault_Counter >= KcSENT_GPF_OutletPres_CommuFaultThrsh)
      {
         VbSENT_GPF_OutletPres_CommuFault = CbTRUE;
      }
      else
      {
         ScSENT_GPF_OutletPres_CommuFault_Counter = INCusp(ScSENT_GPF_OutletPres_CommuFault_Counter);
      }
   }
   else
   {
      VbSENT_GPF_OutletPres_CommuFault = CbFALSE;
      ScSENT_GPF_OutletPres_CommuFault_Counter = V_COUNT_BYTE(0);
   }

   VbSENT_GPF_OutletPresValid = ((CbFALSE == VbSENT_GPF_OutletPres_SensorFaults)
                               &&(CbFALSE == VbSENT_GPF_OutletPres_CommuFault)
                               &&(CbTRUE == GetEPSE_BaroValid()));

   for(LySent_Index =0 ;LySent_Index < KcSENT_FastSamplingAvgCountThrsh;LySent_Index++) 
   {   
     LfSENT_GPF_OutletPres_Value += ( (((WORD)VaSENT_GPF_OP_MsgStructure[LySent_Index].FastNibbles[1]) << 8) +
                                     (VaSENT_GPF_OP_MsgStructure[LySent_Index].FastNibbles[2] << 4) +
                                      VaSENT_GPF_OP_MsgStructure[LySent_Index].FastNibbles[3] );   
   }

   LfSENT_GPF_OutletPres_Value = usDIV_us_usp(LfSENT_GPF_OutletPres_Value,
                                             KcSENT_FastSamplingAvgCountThrsh,
                                             S_COUNT_WORD,
                                             S_COUNT_BYTE,
                                             S_COUNT_WORD);

   if(CbTRUE == VbSENT_GPF_OutletPresValid)
   {
      VfSENT_GPF_OutletAbsPres = 
           ssADD_us_ssp(usMUL_us_usp(LfSENT_GPF_OutletPres_Value,
                                    KsSENT_GPF_OutletPres_SignalScalling.KfSENT_GPFSlope,
                                    S_COUNT_WORD,
                                    S_RATIO_0_1,
                                    S_KPA_0_512),
                       KsSENT_GPF_OutletPres_SignalScalling.KfSENT_GPFOffset,
                       S_KPA_0_512,
                       S_KPA_N256_256,
                       S_KPA_N256_256);

     VfSENT_GPF_OutletPres = ssSUB_ss_usp(VfSENT_GPF_OutletAbsPres,
                                         GetEPSE_p_Baro(),
                                         S_KPA_N256_256,
                                         S_KPAa,
                                         S_KPA_N256_256);
  }
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: ProcessSENT_GPF_Press_MessageData                                     ****/
/****       $Content: Process SENT GPF Pressure Message Data                                 ****/
/****                                                                                         ****/
/****       $argument: void                                                                   ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void ProcessSENT_GPF_Press_MessageData(void)
{
   TeSENT_GPF_PresSensorType  LeSENT_GPF_PresSensorType;
   LeSENT_GPF_PresSensorType = KeSENT_GPF_PressSensor;
   switch( LeSENT_GPF_PresSensorType )
   {
      case CeSENT_Sensata_DeltPresOnly:
        UpdateSENT_GPF_DeltaPres();
      break;

      case  CeSENT_Sensata_Delt_OutletPres:
        UpdateSENT_GPF_DeltaPres();
        UpdateSENT_GPF_OutletPres();
      break;

      case  CeSENT_Bosch_Inlet_OutletPres:
        UpdateSENT_GPF_OutletAbsPres();
        UpdateSENT_GPF_InletAbsPres();
      break;

      default:
      break;
   }
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: ProcessSENT_GPF_Temp_MessageData                                      ****/
/****       $Content: Process SENT GPF Temperature Message Data                              ****/
/****                                                                                         ****/
/****       $argument: void                                                                   ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
void ProcessSENT_GPF_Temp_MessageData(void)
{
   WORD       LfSENT_GPF_Temp_Value;
   TbBOOLEAN  LbSENT_GPF_Temp_Valid;
   if( CeSENT_Sensata == KeSENT_GPF_Temp_Type )
   {
      VsSENT_GPF_Temp_MsgFaults.Byte = (BYTE)GetHWIO_SENT_Msg(SENT_SENSOR_GPF_TEMP, &VsSENT_GPF_Temp_MsgStructure);
      VbSENT_GPF_Temp_Ch1ErrFlag = (TbBOOLEAN)(VsSENT_GPF_Temp_MsgStructure.FastNibbles[0]& SENT_ERROR_MASK1);
      LfSENT_GPF_Temp_Value = ( (((WORD)VsSENT_GPF_Temp_MsgStructure.FastNibbles[1]) << 8) +
                                     (VsSENT_GPF_Temp_MsgStructure.FastNibbles[2] << 4) +
                                      VsSENT_GPF_Temp_MsgStructure.FastNibbles[3] );

      if( ( LfSENT_GPF_Temp_Value <= KcSENT_GPF_Temp_InvalidMax ) && ( LfSENT_GPF_Temp_Value >= KcSENT_GPF_Temp_InvalidMin ) )
      {
         LbSENT_GPF_Temp_Valid = CbTRUE;
      }
      else
      {
         LbSENT_GPF_Temp_Valid = CbFALSE;
      }

      if( ( !VbSENT_GPF_Temp_Ch1ErrFlag ) && LbSENT_GPF_Temp_Valid )
      {
         VfSENT_GPF_Temp = ssADD_us_ssp( usMUL_us_usp( LfSENT_GPF_Temp_Value,
                                                          KsSENT_GPF_Temp_SignalScalling.KfSENT_GPFSlope,
                                                          S_COUNT_WORD,
                                                          S_RATIO_0_1,
                                                          S_DEG_Cd ),
                                            KsSENT_GPF_Temp_SignalScalling.KfSENT_GPFOffset,
                                            S_DEG_Cd,
                                            S_DEG_Cb,
                                            S_DEG_Cb );
      }
   }
}

/*************************************************************************************************/
/*************************************************************************************************/
/****       $Function: MngSENT_1msTasks                                                      ****/
/****       $Content: Manage SENT 1ms Tasks                                                  ****/
/****                                                                                         ****/
/****       $argument: void                                                                   ****/
/****       $return value: void                                                              ****/
/*************************************************************************************************/
/*************************************************************************************************/
FARFUNC void MngSENT_1msTasks(void)  
{ 
  T_COUNT_BYTE LfSENT_GPF_MsgStructure;

  if(KeSENT_GPF_PressSensor == CeSENT_Bosch_Inlet_OutletPres)
  {
     LfSENT_GPF_MsgStructure = GetHWIO_SENT_Msg(SENT_SENSOR_GPF_Outlet,&VaSENT_GPF_OP_MsgStructure[ScSENT_GPF_FastSamplingCount]);
     LfSENT_GPF_MsgStructure = GetHWIO_SENT_Msg(SENT_SENSOR_GPF_Inlet, &VaSENT_GPF_MsgStructure[ScSENT_GPF_FastSamplingCount]);

     if(ScSENT_GPF_FastSamplingCount >= usSUB_us_usp(KcSENT_FastSamplingAvgCountThrsh,
                                                    V_COUNT_BYTE(1),
                                                    S_COUNT_BYTE,
                                                    S_COUNT_BYTE,
                                                    S_COUNT_BYTE))
     {
        ScSENT_GPF_FastSamplingCount = V_COUNT_BYTE(0);
     }
     else
     {
        ScSENT_GPF_FastSamplingCount = INCusp(ScSENT_GPF_FastSamplingCount);
     } 
  }
  
  if(V_COUNT_WORD(65534) < NsSENT_BaseData.wCounter)
  {
    NsSENT_BaseData.wCounter = V_COUNT_WORD(0);
    NsSENT_BaseData.bFlag = !(NsSENT_BaseData.bFlag);
  }
  else
  {
    /* Do Nothing */
  }
  NsSENT_BaseData.wCounter = INCusp(NsSENT_BaseData.wCounter);
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