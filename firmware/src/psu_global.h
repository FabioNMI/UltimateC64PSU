/*
 * c64_psu.h
 *
 *  Created on: Jan. 25, 2025
 *      Author: fabiop
 */

#ifndef PSU_GLOBAL_H_
#define PSU_GLOBAL_H_

#include <hal_data.h>

#define AC_FREQUENCY        (60)      // AC output frequency in Hz
#define AC_DUTY_CYCLE       (75)      // The duty-cycle to use on the pseudo-AC waveform

#define SPLASH_X            (15)
#define SPLASH_Y            (5)
#define V_PX                (5)
#define V_PY                (8)
#define I_PX                (60)
#define I_PY                (8)
#define P_PX                (25)
#define P_PY                (29)
#define AC_ERR_PX           (80)
#define DC_ERR_PX           (10)
#define ERR_PY              (20)
#define BYE_PX              (30)
#define BYE_PY              (23)
#define GUI_UPDATE_TIME     (250LU)   // milliseconds
#define GUI_SPLASH_TIME     (3000LU)  // milliseconds
#define GUI_SAVER_TIME      (5000LU)  // milliseconds

#define V5_MAX              (510LU)   // Maximum voltage allowed on the 5V DC bus (multiplied by 100)
#define V5_MIN              (485LU)   // Minimum voltage allowed on the 5V DC bus (multiplied by 100)
#define I5_MAX              (2000LU)  // Maximum current allowed on the 5V rail
#define AUTO_OFF_TIME       (5000LU)  // Time for automatically turning off (milliseconds)
#define I5_MIN              (100LU)   // Minimum current (in mA) on 5V bus to turn off AC output and switch off PSU
#define SMP_INT             (10)      // perform analog readings at this interval (ms)
#define DRV8874_FLT_DELAY   (250LU)   // Delay before checking fault status (ms)
#define DRV8874_RETRY       (200LU)   // Retry time in ms on a DRV8874fault
#define R3                  (10000LU) // upper V5 resistor divider
#define R4                  (10000LU) // lower V5 resistor divider
#define PRECISION_FACTOR    (100LU)
#define ADC_MAX_COUNT       (4095)    // use 4095 -> 12 bits ADCs!
#define ADC_MAX_VOLTAGE     (500)     // 500 = 5.00V
#define ADC_AVERAGING       (3)       // 2 to the power of this number (ie 2^3 -> average is 8 samples)
#define V5_DIVIDER          (((R3+R4)*PRECISION_FACTOR)/R4) // this is multiplied by the precision factor in order to keep some precision
#define I5_DIVIDER          ((uint32_t)(1.00*PRECISION_FACTOR))
#define V5_MAX_RAW          ((((V5_MAX * PRECISION_FACTOR)/V5_DIVIDER)*ADC_MAX_COUNT)/ADC_MAX_VOLTAGE)
#define V5_MIN_RAW          ((((V5_MIN * PRECISION_FACTOR)/V5_DIVIDER)*ADC_MAX_COUNT)/ADC_MAX_VOLTAGE)
#define I5_MAX_RAW          ((((I5_MAX * PRECISION_FACTOR)/I5_DIVIDER/10)*ADC_MAX_COUNT)/ADC_MAX_VOLTAGE)
#define I5_MIN_RAW          ((((I5_MIN * PRECISION_FACTOR)/I5_DIVIDER/10)*ADC_MAX_COUNT)/ADC_MAX_VOLTAGE)

typedef enum {
  ERR_NONE,
  ERR_V5_HIGH,
  ERR_V5_LOW,
  ERR_V5_CURRENT,
  ERR_V12_FAULT
} error_code_t;

typedef enum {
    STATE_INIT,
    STATE_OFF,
    STATE_ON,
    STATE_FAULT,
    STATE_OVERVOLTAGE,
    STATE_UNDERVOLTAGE,
    STATE_OVERCURRENT,
    STATE_UNDERCURRENT,
    STATE_NONE
} psu_state_t;

#endif /* PSU_GLOBAL_H_ */
