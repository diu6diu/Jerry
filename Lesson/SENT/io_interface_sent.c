
#include "io_config.h"
#include "dd_ir.h"
#include "dd_ir_interface.h"
#include "dd_sent.h"
#include "dd_sent_init.h"
#include "sent_device_driver.h"
#include "sent_port_interface.h"

HWI_SENT_CFG_CHANNEL_STRUCT_TYPE MTSA_SENT0_CFG =
{
   SENT0_CLOCK_PERIOD,              /* clock period in nanoseconds */
   HWI_SENT_ALT_INP1,
   HWI_SENT_INPULSE_ACTIVE_HIGH,    /* SENT signal polarity                      */
   HWI_SENT_SHORT_ID,               /* type of serial message, short or enhanced */
   true,                            /* automatic processing of serial msg enabled*/
   true,                             /* pause pulse enabled                       */
   HWI_SENT_CRC_AUGMENTATION_SELECTED,
   10000
};

HWI_SENT_CFG_CHANNEL_STRUCT_TYPE MTSA_SENT1_CFG =
{
   SENT1_CLOCK_PERIOD,              /* clock period in nanoseconds */
   HWI_SENT_ALT_INP1,
   HWI_SENT_INPULSE_ACTIVE_HIGH,    /* SENT signal polarity                      */
   HWI_SENT_SHORT_ID,               /* type of serial message, short or enhanced */
   true,                            /* automatic processing of serial msg enabled*/
   true,                             /* pause pulse enabled                       */
   HWI_SENT_CRC_AUGMENTATION_SELECTED,
   10000
};

HWI_SENT_CFG_CHANNEL_STRUCT_TYPE MTSA_SENT2_CFG =
{
   SENT2_CLOCK_PERIOD,              /* clock period in nanoseconds */
   HWI_SENT_ALT_INP1,
   HWI_SENT_INPULSE_ACTIVE_HIGH,    /* SENT signal polarity                      */
   HWI_SENT_SHORT_ID,               /* type of serial message, short or enhanced */
   true,                            /* automatic processing of serial msg enabled*/
   true,                             /* pause pulse enabled                       */
   HWI_SENT_CRC_AUGMENTATION_SELECTED,
   10000
};

HWI_SENT_CFG_CHANNEL_STRUCT_TYPE MTSA_SENT3_CFG =
{
   SENT3_CLOCK_PERIOD,              /* clock period in nanoseconds */
   HWI_SENT_ALT_INP1,
   HWI_SENT_INPULSE_ACTIVE_HIGH,    /* SENT signal polarity                      */
   HWI_SENT_SHORT_ID,               /* type of serial message, short or enhanced */
   true,                            /* automatic processing of serial msg enabled*/
   true,                             /* pause pulse enabled                       */
   HWI_SENT_CRC_AUGMENTATION_SELECTED,
   10000
};

HWI_SENT_CFG_CHANNEL_STRUCT_TYPE MTSA_SENT4_CFG =
{
   SENT4_CLOCK_PERIOD,              /* clock period in nanoseconds */
   HWI_SENT_ALT_INP1,
   HWI_SENT_INPULSE_ACTIVE_HIGH,    /* SENT signal polarity                      */
   HWI_SENT_SHORT_ID,               /* type of serial message, short or enhanced */
   true,                            /* automatic processing of serial msg enabled*/
   true,                             /* pause pulse enabled                       */
   HWI_SENT_CRC_AUGMENTATION_SELECTED,
   10000
};

HWI_SENT_CFG_CHANNEL_STRUCT_TYPE MTSA_SENT5_CFG =
{
   SENT5_CLOCK_PERIOD,              /* clock period in nanoseconds */
   HWI_SENT_ALT_INP1,
   HWI_SENT_INPULSE_ACTIVE_HIGH,    /* SENT signal polarity                      */
   HWI_SENT_SHORT_ID,               /* type of serial message, short or enhanced */
   true,                            /* automatic processing of serial msg enabled*/
   true,                             /* pause pulse enabled                       */
   HWI_SENT_CRC_AUGMENTATION_SELECTED,
   10000
};

const HWI_SENT_CHANNEL_STRUCT_TYPE MTSA_SENT_A =
{
   HWI_SENT_CHANNEL_0,
   &MTSA_SENT0_CFG,
   HWI_SENT_V2010
};

const HWI_SENT_CHANNEL_STRUCT_TYPE MTSA_SENT_B =
{
   HWI_SENT_CHANNEL_1,
   &MTSA_SENT1_CFG,
   HWI_SENT_V2010
};

const HWI_SENT_CHANNEL_STRUCT_TYPE MTSA_SENT_C =
{
   HWI_SENT_CHANNEL_2,
   &MTSA_SENT2_CFG,
   HWI_SENT_V2010
};

const HWI_SENT_CHANNEL_STRUCT_TYPE MTSA_SENT_D =
{
   HWI_SENT_CHANNEL_6,
   &MTSA_SENT3_CFG,
   HWI_SENT_V2010
};

const HWI_SENT_CHANNEL_STRUCT_TYPE MTSA_SENT_E =
{
   HWI_SENT_CHANNEL_7,
   &MTSA_SENT4_CFG,
   HWI_SENT_V2010
};

const HWI_SENT_CHANNEL_STRUCT_TYPE MTSA_SENT_F =
{
   HWI_SENT_CHANNEL_8,
   &MTSA_SENT5_CFG,
   HWI_SENT_V2010
};

const IO_Interrupt_T MTSA_INTERRUPT_SENT0RX =
{
   MTSA_CONFIG_INTERRUPT_SENT0RX,
   NULL,
   &INT_ROUTER
};

const IO_Interrupt_T MTSA_INTERRUPT_SENT1RX =
{
   MTSA_CONFIG_INTERRUPT_SENT1RX,
   NULL,
   &INT_ROUTER
};

const IO_Interrupt_T MTSA_INTERRUPT_SENT2RX =
{
   MTSA_CONFIG_INTERRUPT_SENT2RX,
   NULL,
   &INT_ROUTER
};

const IO_Interrupt_T MTSA_INTERRUPT_SENT3RX =
{
   MTSA_CONFIG_INTERRUPT_SENT3RX,
   NULL,
   &INT_ROUTER
};

const IO_Interrupt_T MTSA_INTERRUPT_SENT4RX =
{
   MTSA_CONFIG_INTERRUPT_SENT4RX,
   NULL,
   &INT_ROUTER
};

const IO_Interrupt_T MTSA_INTERRUPT_SENT5RX =
{
   MTSA_CONFIG_INTERRUPT_SENT5RX,
   NULL,
   &INT_ROUTER
};

const SENT_Port_C_T SENT_PORT_A =
{
   SENT_CH0,
   &MTSA_SENT_A,
   &SENT_PORT_INTERFACE,
   &SENT_PORT_CALLBACK,
};

const SENT_Port_C_T SENT_PORT_B =
{
   SENT_CH1,
   &MTSA_SENT_B,
   &SENT_PORT_INTERFACE,
   &SENT_PORT_CALLBACK,
};

const SENT_Port_C_T SENT_PORT_C =
{
   SENT_CH2,
   &MTSA_SENT_C,
   &SENT_PORT_INTERFACE,
   &SENT_PORT_CALLBACK,
};
const SENT_Port_C_T SENT_PORT_D =
{
   SENT_CH6,
   &MTSA_SENT_D,
   &SENT_PORT_INTERFACE,
   &SENT_PORT_CALLBACK,
};
const SENT_Port_C_T SENT_PORT_E =
{
   SENT_CH7,
   &MTSA_SENT_E,
   &SENT_PORT_INTERFACE,
   &SENT_PORT_CALLBACK,
};
const SENT_Port_C_T SENT_PORT_F =
{
   SENT_CH8,
   &MTSA_SENT_F,
   &SENT_PORT_INTERFACE,
   &SENT_PORT_CALLBACK,
};