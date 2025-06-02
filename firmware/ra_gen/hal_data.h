/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_tau.h"
#include "r_timer_api.h"
#include "r_sau_i2c.h"
#include "r_i2c_master_api.h"
#include "r_adc_d.h"
#include "r_adc_api.h"
FSP_HEADER
/** TAU Timer Instance */
extern const timer_instance_t g_timer0;

/** Access the TAU instance using these structures when calling API functions directly (::p_api is not used). */
extern tau_instance_ctrl_t g_timer0_ctrl;
extern const timer_cfg_t g_timer0_cfg;

#ifndef callback60Hz
void callback60Hz(timer_callback_args_t *p_args);
#endif
extern const i2c_master_cfg_t g_i2c0_cfg;
/* I2C on SAU Instance. */
extern const i2c_master_instance_t g_i2c0;
#ifndef sau_i2c_master_callback
void sau_i2c_master_callback(i2c_master_callback_args_t *p_args);
#endif

extern const sau_i2c_extended_cfg_t g_i2c0_cfg_extend;
extern sau_i2c_instance_ctrl_t g_i2c0_ctrl;
/** ADC on ADC Instance. */
extern const adc_instance_t g_adc0;

/** Access the ADC instance using these structures when calling API functions directly (::p_api is not used). */
extern adc_d_instance_ctrl_t g_adc0_ctrl;
extern const adc_cfg_t g_adc0_cfg;
extern const adc_d_channel_cfg_t g_adc0_channel_cfg;

#ifndef NULL
void NULL(adc_callback_args_t *p_args);
#endif
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */
