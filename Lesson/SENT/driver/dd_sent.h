/* CONFIDENTIAL */
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_*/
/*_/_/_/        $DWG-No.: NONE                                                                _/_/_/_*/
/*_/_/_/        $Content: SENT Device Driver Header File                                      _/_/_/_*/
/*_/_/_/        $Category: Driver                                                            _/_/_/_*/
/*_/_/_/        $Date: 2024                                                                  _/_/_/_*/
/*_/_/_/        $Design: Device Driver Interface                                             _/_/_/_*/
/*_/_/_/        $Check: NONE                                                                _/_/_/_*/
/*_/_/_/        $Header: dd_sent.h                                                          _/_/_/_*/
/*_/_/_/        $Copyright(C) 2024                                                          _/_/_/_*/
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_*/

#ifndef DD_SENT_H
#define DD_SENT_H

/*###############################################################################################*/
/*###                                     $INCLUDE FILES$                                     ####*/
/*###############################################################################################*/
#include "io_type.h"
#include "hw_sent.h"
#include "reuse.h"
#include "io_scale.h"
#include "dd_scu.h"        /* for ENDINIT protect and unprotect */
#include "dd_port.h"

/*###############################################################################################*/
/*###                                     $SECTION DEFINITION$                                ####*/
/*###############################################################################################*/
#define SECTION_SENT_REGISTER
#include "section.h"
extern SENT_T SENT;
#define SECTION_END
#include "section.h"

/*###############################################################################################*/
/*###                                     $TYPE DEFINITIONS$                                  ####*/
/*###############################################################################################*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Channel Types                                         +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
typedef enum
{
    HWI_SENT_CHANNEL_0,
    HWI_SENT_CHANNEL_1,
    HWI_SENT_CHANNEL_2,
    HWI_SENT_CHANNEL_3,
    HWI_SENT_CHANNEL_4,
    HWI_SENT_CHANNEL_5,
    HWI_SENT_CHANNEL_6,
    HWI_SENT_CHANNEL_7,
    HWI_SENT_CHANNEL_8,
    HWI_SENT_CHANNEL_9,
    HWI_SENT_CHANNEL_MAX
} HWI_SENT_CHANNEL_TYPE;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Protocol Version                                      +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
typedef enum
{
    HWI_SENT_V2008,                  /* NOT SUPPORTED */
    HWI_SENT_V2010,
    HWI_SENT_VERSION_STUB
} HWI_SENT_VERSION_ENUM_TYPE;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Frame Types                                          +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
typedef enum
{
    HWI_SENT_SHORT_ID,               /* 4 bits ID - 8 bits datas             */
    HWI_SENT_LONG_ID,                /* 4 or 8 bits ID - 12 or 16 bits datas */
    HWI_SENT_SENT_SERIAL_FRAME_STUB
} HWI_SENT_SERIAL_FRAME_ENUM_TYPE;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Diagnostic Types                                      +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
typedef enum
{
    HWI_SENT_DG_NO_ERROR = 0,        /* Lowest priority : no error          */
    HWI_SENT_DG_NO_REFRESHED_FRAME,  /* WDI > HWI_SENT_MSG_LOST_CPV        */
    HWI_SENT_DG_ERROR,
    HWI_SENT_DG_CRC_ERROR,           /* CRC Nibble error                    */
    HWI_SENT_DG_NIBBLE_DATA_ERROR,   /* Nibble data value out of range     */
    HWI_SENT_DG_N_NIBBLE_ERROR,      /* Number of nibbles out of range     */
    HWI_SENT_DG_CLOCK_DRIFT,         /* Consecutive CAL pulse > 1.56%      */
    HWI_SENT_DG_CALPULSE_DRIFT,      /* CAL pulse > +/- 25%                */
    HWI_SENT_DG_LOST_SIGNAL          /* highest priority : no signal       */
} HWI_SENT_DG_ENUM_TYPE;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Input Selection                                       +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
typedef enum
{
    HWI_SENT_ALT_INP0 = 0,           /* select alternate input 0 */
    HWI_SENT_ALT_INP1,
    HWI_SENT_ALT_INP2,
    HWI_SENT_ALT_INP3
} HWI_SENT_ALT_INSEL_ENUM_TYPE;

typedef enum
{
    HWI_SENT_INPULSE_ACTIVE_LOW = 0, /* input pulse active low */
    HWI_SENT_INPULSE_ACTIVE_HIGH
} HWI_SENT_IN_POLARITY_ENUM_TYPE;

typedef enum
{
    HWI_SENT_CRC_AUGMENTATION_SELECTED = 0,
    HWI_SENT_CRC_AUGMENTATION_NOT_SELECTED
} HWI_SENT_CRC_AUGMENTATION_ENUM_TYPE;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Configuration Structures                              +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
typedef struct
{
    uint32_t                                  HWI_SENT_CLOCK_TICK_DURATION;  /* NANOSECOND */
    HWI_SENT_ALT_INSEL_ENUM_TYPE             HWI_SENT_ALT_IN;
    HWI_SENT_IN_POLARITY_ENUM_TYPE           HWI_SENT_POLARITY;
    HWI_SENT_SERIAL_FRAME_ENUM_TYPE          HWI_SENT_SERIAL_FRAME_METHOD;
    bool                                      HWI_SENT_SERIAL_FRAME_REQ;
    bool                                      HWI_SENT_ENDPULSE_IGNORE;
    HWI_SENT_CRC_AUGMENTATION_ENUM_TYPE      HWI_SENT_CRC_IMPLEMENT_TYPE;
    uint32_t                                  HWI_SENT_WDT_MAX_COUNTER;
} HWI_SENT_CFG_CHANNEL_STRUCT_TYPE;

typedef struct
{
    HWI_SENT_CHANNEL_TYPE                     HWI_SENT_CHANNEL_NUMBER;
    const HWI_SENT_CFG_CHANNEL_STRUCT_TYPE*   HWI_SENT_CHANNEL_CONFIG;
    HWI_SENT_VERSION_ENUM_TYPE                HWI_SENT_VERSION;
} HWI_SENT_CHANNEL_STRUCT_TYPE;

/*###############################################################################################*/
/*###                                     $API FUNCTIONS$                                     ####*/
/*###############################################################################################*/
extern void SENT_Initialize_Device(void);
extern void SENT_Initialize_Channel(const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr, 
                                  uint8_t sent_channel);

extern HWI_SENT_DG_ENUM_TYPE HWI_Sent_Slow_Read(const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr,
                                                uint16_t* hwi_sent_id_ptr,
                                                uint16_t* hwi_sent_data_ptr);

extern HWI_SENT_DG_ENUM_TYPE HWI_Sent_Fast_Read(const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr,
                                                uint16_t* hwi_sent_data1_ptr,
                                                uint16_t* hwi_sent_data2_ptr);

extern uint16_t* SENT_Get_Data(const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr);
extern uint16_t* SENT_Get_SerialData(const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr);
extern uint8_t SENT_Get_CRC(const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr);
extern uint8_t SENT_Get_Sensor_Status(const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr);
extern HWI_SENT_DG_ENUM_TYPE SENT_Get_Channel_Status(const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr);
extern uint32_t SENT_Get_Interrupt_Status(const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr);
extern void SENT_Clear_Interrupt_Status(const HWI_SENT_CHANNEL_STRUCT_TYPE* hwi_sent_channel_ptr);

/*###############################################################################################*/
/*###                                     $REGISTER DEFINITIONS$                              ####*/
/*###############################################################################################*/
#define HWI_BITS_MASK(pos, len) (uint32_t)(((1<<len)-1)<<pos)

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     RCR Register                                          +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define HWI_SENT_RCR_CEN(val)   (((uint32_t)(val << BIT_0 )) & HWI_BITS_MASK(BIT_0 ,1))
#define HWI_SENT_RCR_IEP(val)   (((uint32_t)(val << BIT_1 )) & HWI_BITS_MASK(BIT_1 ,1))
#define HWI_SENT_RCR_ACE(val)   (((uint32_t)(val << BIT_2 )) & HWI_BITS_MASK(BIT_2 ,1))
#define HWI_SENT_RCR_SNI(val)   (((uint32_t)(val << BIT_3 )) & HWI_BITS_MASK(BIT_3 ,1))
#define HWI_SENT_RCR_SDP(val)   (((uint32_t)(val << BIT_4 )) & HWI_BITS_MASK(BIT_4 ,1))
#define HWI_SENT_RCR_SCDIS(val) (((uint32_t)(val << BIT_5 )) & HWI_BITS_MASK(BIT_5 ,1))
#define HWI_SENT_RCR_CDIS(val)  (((uint32_t)(val << BIT_6 )) & HWI_BITS_MASK(BIT_6 ,1))
#define HWI_SENT_RCR_CFC(val)   (((uint32_t)(val << BIT_7 )) & HWI_BITS_MASK(BIT_7 ,1))
#define HWI_SENT_RCR_FRL(val)   (((uint32_t)(val << BIT_8 )) & HWI_BITS_MASK(BIT_8 ,8))
#define HWI_SENT_RCR_CRZ(val)   (((uint32_t)(val << BIT_16)) & HWI_BITS_MASK(BIT_16,1))
#define HWI_SENT_RCR_ESF(val)   (((uint32_t)(val << BIT_17)) & HWI_BITS_MASK(BIT_17,1))
#define HWI_SENT_RCR_IDE(val)   (((uint32_t)(val << BIT_18)) & HWI_BITS_MASK(BIT_18,1))
#define HWI_SENT_RCR_SUSEN(val) (((uint32_t)(val << BIT_19)) & HWI_BITS_MASK(BIT_19,1))
#define HWI_SENT_RCR_FDFL(val)  (((uint32_t)(val << BIT_20)) & HWI_BITS_MASK(BIT_20,1))

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Other Registers                                       +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* CPDR */
#define HWI_SENT_CPDR_GEN_CONF(val)   (((uint32_t)(val << BIT_0 )) & HWI_BITS_MASK(BIT_0 ,12))

/* CFDR */
#define HWI_SENT_CFDR_DIV(val)        (((uint32_t)(val << BIT_0 )) & HWI_BITS_MASK(BIT_0 ,16))
#define HWI_SENT_CFDR_DEF_CONF        HWI_SENT_CFDR_DIV(HWI_SENT_CPDR_DIV_MIN)

/* RDR */
#define HWI_SENT_READ_DATA_MSB(val)   (uint16_t)((val & HWI_BITS_MASK(BIT_16, 16))>> BIT_16)
#define HWI_SENT_READ_DATA_LSB(val)   (uint16_t)(val & HWI_BITS_MASK(BIT_0, 16))

/* SDS */
#define HWI_SENT_SDS_CON_MASK         HWI_BITS_MASK(BIT_31, 1)
#define HWI_SENT_READ_S_DATA(val)     (uint16_t)(val & HWI_BITS_MASK(BIT_0, 16))
#define HWI_SENT_READ_S_ID(val)       (uint16_t)((val & HWI_BITS_MASK(BIT_16, 8))>> BIT_16)
#define HWI_SENT_READ_S_IDCRC(val)    (uint16_t)((val & HWI_BITS_MASK(BIT_16, 16))>> BIT_16)

/* INTEN */
#define HWI_SENT_RSI_EN(val)          (((uint32_t)(val << BIT_0 )) & HWI_BITS_MASK(BIT_0 ,1))
#define HWI_SENT_RDI_EN(val)          (((uint32_t)(val << BIT_1 )) & HWI_BITS_MASK(BIT_1 ,1))
#define HWI_SENT_RBI_EN(val)          (((uint32_t)(val << BIT_2 )) & HWI_BITS_MASK(BIT_2 ,1))
#define HWI_SENT_TDI_EN(val)          (((uint32_t)(val << BIT_3 )) & HWI_BITS_MASK(BIT_3 ,1))
#define HWI_SENT_TBI_EN(val)          (((uint32_t)(val << BIT_4 )) & HWI_BITS_MASK(BIT_4 ,1))
#define HWI_SENT_FRI_EN(val)          (((uint32_t)(val << BIT_5 )) & HWI_BITS_MASK(BIT_5 ,1))
#define HWI_SENT_FDI_EN(val)          (((uint32_t)(val << BIT_6 )) & HWI_BITS_MASK(BIT_6 ,1))
#define HWI_SENT_NNI_EN(val)          (((uint32_t)(val << BIT_7 )) & HWI_BITS_MASK(BIT_7 ,1))
#define HWI_SENT_NVI_EN(val)          (((uint32_t)(val << BIT_8 )) & HWI_BITS_MASK(BIT_8 ,1))
#define HWI_SENT_CRCI_EN(val)         (((uint32_t)(val << BIT_9 )) & HWI_BITS_MASK(BIT_9 ,1))
#define HWI_SENT_WSI_EN(val)          (((uint32_t)(val << BIT_10)) & HWI_BITS_MASK(BIT_10,1))
#define HWI_SENT_SDI_EN(val)          (((uint32_t)(val << BIT_11)) & HWI_BITS_MASK(BIT_11,1))
#define HWI_SENT_SCRI_EN(val)         (((uint32_t)(val << BIT_12)) & HWI_BITS_MASK(BIT_12,1))
#define HWI_SENT_WDI_EN(val)          (((uint32_t)(val << BIT_13)) & HWI_BITS_MASK(BIT_13,1))

/* RSR */
#define HWI_SENT_READ_RSR_CRC(val)    (uint8_t)(val & HWI_BITS_MASK(BIT_0, 4))
#define HWI_SENT_READ_RSR_SCN(val)    (uint8_t)((val & HWI_BITS_MASK(BIT_8, 4))>>BIT_8)

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++                                     Constants                                             +++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define HWI_SENT_DIS_ALL_ISR          (uint32_t)(0)
#define HWI_SENT_CLEAR_ISR_STATUS     (uint32_t)(0x00003FFF)
#define HWI_SENT_INP0_WDI_ERRI        (uint32_t)(0x10100000)
#define HWI_SENT_NIBBLE_ORDER         (uint32_t)(0x76543210)

/* Time calculation macro */
#define HWI_TIME_SUB(a,b)             (((a)-(b))&0x00ffffffuL)

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

#endif /* end DD_SENT_H */