/*
 * ac_control.c
 *
 *  Created on: Jan. 25, 2025
 *      Author: fabiop
 */

#include <ac_control.h>
#include <utils.h>

#define TIMING_ADJ      (23)  // microseconds

static uint32_t passive_period, active_period;
static uint8_t frequency, duty_cycle;
static psu_state_t ac_state;
static uint32_t ac_fsm_timing;

void ac_set_duty_cycle(uint8_t duty);
void ac_set_frequency(uint8_t freq);
void ac_init(uint8_t freq, uint8_t duty);
void enable_drv8874(bool state);
bool read_drv8874_status(void);

void ac_set_duty_cycle(uint8_t duty) {

    uint32_t half_period = (500000 / (uint32_t)frequency) - TIMING_ADJ;  // this gives us microseconds
    if (duty > 0 && duty < 100) {
        duty_cycle = duty;
        active_period = (half_period * (uint32_t)duty) / 100UL;
        passive_period = half_period - active_period;
    }
}
void ac_set_frequency(uint8_t freq) {
    if (freq >= 50 && freq <= 65) {
        frequency = freq;
        ac_set_duty_cycle(duty_cycle);
    }
}

void ac_init(uint8_t freq, uint8_t duty) {
    duty_cycle = duty;
    ac_set_frequency(freq);
    R_TAU_Open(&g_timer0_ctrl, &g_timer0_cfg);
    R_TAU_PeriodSet(&g_timer0_ctrl, 1000);
    R_TAU_Start(&g_timer0_ctrl);
}

// true = enabled
void enable_drv8874(bool drv_state) {
    R_IOPORT_PinWrite(&g_ioport_ctrl, PIN_AC_SLEEP, !drv_state);
}

// Returns true if there a fault
bool read_drv8874_status(void) {
    bsp_io_level_t pin_state;
    R_IOPORT_PinRead(&g_ioport_ctrl, PIN_AC_FAULT, &pin_state);
    return !pin_state;
}

/* Callback function */
void callback60Hz(timer_callback_args_t *p_args) {
    FSP_PARAMETER_NOT_USED(p_args);
    static uint8_t pos = 0;
    uint32_t period = 0;

    if (ac_state != STATE_ON) {
        pos = 0;
        enable_drv8874(false);
    } else {
        enable_drv8874(true);
    }

    switch (pos) {
        case 0:    // initial phase off
            period = passive_period/2;
            R_IOPORT_PinWrite(&g_ioport_ctrl, PIN_AC_IN1, 0);
            R_IOPORT_PinWrite(&g_ioport_ctrl, PIN_AC_IN2, 0);
            break;
        case 1: // active phase
            period = active_period;
            R_IOPORT_PinWrite(&g_ioport_ctrl, PIN_AC_IN1, 1);
            break;
        case 2: // final phase
            period = passive_period/2;
            R_IOPORT_PinWrite(&g_ioport_ctrl, PIN_AC_IN1, 0);
            break;
        case 3:    // initial phase off
            period = passive_period/2;
            break;
        case 4: // active phase
            period = active_period;
            R_IOPORT_PinWrite(&g_ioport_ctrl, PIN_AC_IN2, 1);
            break;
        case 5: // final phase
            period = passive_period/2;
            R_IOPORT_PinWrite(&g_ioport_ctrl, PIN_AC_IN2, 0);
            break;
    }
    R_TAU_Stop(&g_timer0_ctrl);
    R_TAU_PeriodSet(&g_timer0_ctrl, period);
    R_TAU_Start(&g_timer0_ctrl);
    pos++;
    if (pos > 5) pos = 0;
}

psu_state_t ac_control_fsm(psu_state_t new_state) {
    switch (ac_state) {
        case STATE_INIT:   // Initialize the AC section
            ac_init(AC_FREQUENCY, AC_DUTY_CYCLE);
            ac_state = STATE_OFF;
            break;
        case STATE_OFF:    // when the AC is off, we check if there is a command to turn it on
            if (new_state == STATE_ON) {
                ac_state = new_state;
                ac_fsm_timing = utils_get_time();
            }
            enable_drv8874(false);
            break;
        case STATE_ON:     // while AC is on, we monitor if there is a fault
            if (new_state == STATE_OFF) {
                ac_state = new_state;
                enable_drv8874(false);
            } else if (read_drv8874_status()) {
                if (utils_get_time() > ac_fsm_timing + DRV8874_FLT_DELAY) {
                    ac_state = STATE_FAULT;
                    enable_drv8874(false);
                    ac_fsm_timing = utils_get_time();
                }
            }
            break;
        case STATE_FAULT:  // in AC fault state we wait and retry to enable it
            if (new_state == STATE_OFF) {
                ac_state = new_state;
            } else if (utils_get_time() > ac_fsm_timing + DRV8874_RETRY) {
                //ac_state = STATE_ON;
            }
            break;
        default:
            break;
    }
    return ac_state;
}
