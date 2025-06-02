/*
 * dc_control.c
 *
 *  Created on: Jan. 25, 2025
 *      Author: fabiop
 */

#include <dc_control.h>
#include <hal_data.h>
#include <utils.h>

#define ADC_V               ADC_CHANNEL_0
#define ADC_I               ADC_CHANNEL_1
#define ADC_VO              ADC_CHANNEL_4

uint16_t v5raw, i5raw;
uint32_t v5avg, i5avg;
psu_state_t dc_state;
uint32_t analog_sample_timer;
uint8_t adc_num_samples;

void adc_init(void);
uint16_t adc_get_reading(adc_channel_t in);
void enable_dc_power(bool state);
void read_analog_data(void);

void adc_init(void) {
    R_ADC_D_Open(&g_adc0_ctrl, &g_adc0_cfg);
    adc_num_samples = 0;
}

uint16_t adc_get_reading(adc_channel_t adc_channel) {
    uint16_t result = 0;
    //R_ADC_D->ADS = (uint8_t)adc_channel;
    R_ADC_D_ScanCfg(&g_adc0_ctrl, &adc_channel);
    R_ADC_D_ScanStart (&g_adc0_ctrl);
    // Wait for the ADC conversion to complete
    while (R_ADC_D->ADM0_b.ADCS);
    R_ADC_D_Read(&g_adc0_ctrl, adc_channel, &result);
    return result;
}

void enable_dc_power(bool state) {
    R_IOPORT_PinWrite(&g_ioport_ctrl, PIN_5V_EN, state);
}

void read_analog_data(void) {
    if (utils_get_time() > analog_sample_timer + SMP_INT) {
        if (analog_sample_timer == 0) {
            v5avg = adc_get_reading(ADC_V) << ADC_AVERAGING;
            i5avg = adc_get_reading(ADC_I) << ADC_AVERAGING;
        } else {
            v5avg += adc_get_reading(ADC_V) - (v5avg >> ADC_AVERAGING);
            i5avg += adc_get_reading(ADC_I) - (i5avg >> ADC_AVERAGING);
            v5raw = v5avg >> ADC_AVERAGING;
            i5raw = i5avg >> ADC_AVERAGING;
        }
        analog_sample_timer = utils_get_time();
        if (adc_num_samples<100) adc_num_samples++;
    }
}

// In this function we check the voltage and current of the DC section
psu_state_t dc_check_status(void) {
    if (adc_num_samples < 10) return STATE_OFF;
    if (v5raw > V5_MAX_RAW) {
        return STATE_OVERVOLTAGE;
    } else if (i5raw > I5_MAX_RAW) {
        return STATE_OVERCURRENT;
    } else if (v5raw < V5_MIN_RAW) {
        return STATE_UNDERVOLTAGE;
    } else if (i5raw < I5_MIN) {
        return STATE_OFF;
    }
    return STATE_ON;
}

psu_state_t dc_control_fsm(psu_state_t new_state) {
    read_analog_data();
    switch (dc_state) {
        case STATE_INIT:    // Just initialize ADC and move on
            adc_init();
            dc_state = STATE_ON;
            break;
        case STATE_OFF:     // DC is always on!
            dc_state = STATE_ON;
            break;
        case STATE_ON:      // DC power is on, monitor parameters and check if we should power off due to a fault
            enable_dc_power(true);
            psu_state_t status = dc_check_status();
            if (status != STATE_ON && status != STATE_OFF) {
                enable_dc_power(false);
                dc_state = STATE_FAULT;
            }
            break;
        default:
            if (new_state == STATE_ON) {
                dc_state = STATE_ON;
            }
            enable_dc_power(false);
            break;
    }
    return dc_state;
}
