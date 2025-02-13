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