/* generated configuration header file - do not edit */
#ifndef BSP_PIN_CFG_H_
#define BSP_PIN_CFG_H_
#include "r_ioport.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

#define V_ADC (BSP_IO_PORT_00_PIN_10)
#define I_ADC (BSP_IO_PORT_00_PIN_11)
#define VO_ADC (BSP_IO_PORT_00_PIN_12)
#define ON_OFF (BSP_IO_PORT_00_PIN_13)
#define FREQ_SEL (BSP_IO_PORT_01_PIN_00)
#define PIN_AC_FAULT (BSP_IO_PORT_01_PIN_01)
#define PIN_AC_SLEEP (BSP_IO_PORT_01_PIN_02)
#define PIN_5V_EN (BSP_IO_PORT_01_PIN_09)
#define PIN_AC_IN1 (BSP_IO_PORT_01_PIN_10)
#define PIN_AC_IN2 (BSP_IO_PORT_01_PIN_12)
#define AUX1 (BSP_IO_PORT_02_PIN_00)
#define AUX2 (BSP_IO_PORT_02_PIN_13)
extern const ioport_cfg_t g_bsp_pin_cfg; /* R7FA0E1073CFJ.pincfg */

void BSP_PinConfigSecurityInit();

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER
#endif /* BSP_PIN_CFG_H_ */
