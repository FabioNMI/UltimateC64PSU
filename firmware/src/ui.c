/*
 * ui.c
 *
 *  Created on: Jan. 25, 2025
 *      Author: fabiop
 */

#include <stdio.h>
#include <psu_global.h>
#include <utils.h>
#include <ssd1306.h>
#include <Adafruit_GFX.h>
#include <FreeMono9p7b.h>
#include <Org_01.h>
#include <logo.h>
#include <ac_control.h>
#include <dc_control.h>
#include <ui.h>

typedef enum {
    UI_POWER_ON,
    UI_SPLASH,
    UI_SHOW_DATA,
    UI_ERROR,
    UI_POWERING_OFF,
    UI_BYE,
    UI_OFF
} ui_state_t;

extern tDisplay ssd1306;
extern uint16_t v5raw, i5raw;
uint32_t ui_timer, ui_screen_saver_timer;
ui_state_t ui_state;
uint8_t x_offset = 0, y_offset = 0;
uint8_t ui_count_down;

void display_data(uint8_t x, uint8_t y, uint16_t data, char unit);
void ui_show_data(void);

void display_data(uint8_t x, uint8_t y, uint16_t data, char unit) {
    char string[10];
    sprintf(string,"%d.%02d%c", data/100, data%100, unit);
    Adafruit_GFX_printAt(x, y, string);
}

void ui_show_data(void) {
    uint16_t voltage, current;
    uint32_t power;
    voltage = ((uint32_t)v5raw * ADC_MAX_VOLTAGE)/ADC_MAX_COUNT;
    voltage = (voltage * V5_DIVIDER)/PRECISION_FACTOR;
    current = ((uint32_t)i5raw * ADC_MAX_VOLTAGE)/ADC_MAX_COUNT;
    current = (current * I5_DIVIDER)/PRECISION_FACTOR;
    power = ((uint32_t)voltage * (uint32_t)current)/PRECISION_FACTOR;
    if (utils_get_time() > ui_screen_saver_timer + GUI_SAVER_TIME) {
        if (y_offset == 2) {
            x_offset = 0; y_offset = 0;
        } else if (x_offset == 2) {
            y_offset = 2;
        } else {
            x_offset = 2;
        }
        ui_screen_saver_timer = utils_get_time();
    }
    Adafruit_GFX_fillScreen(0);
    Adafruit_GFX_setTextSize(2,2);
    display_data(V_PX + x_offset,V_PY + y_offset, voltage, 'V');
    display_data(I_PX + x_offset, I_PY + y_offset, current, 'A');
    Adafruit_GFX_setTextSize(3,3);
    display_data(P_PX - x_offset, P_PY - y_offset, power, 'W');
}

void ui_fsm(void) {
    static ui_state_t previous_state;
    ui_state_t new_state = UI_POWER_ON;
    switch (ui_state) {
        case UI_POWER_ON:   // If we just powered on, wait 100ms and init display and show splash screen
            if (utils_get_time() > ui_timer + 100) {
                Adafruit_GFX_init(&ssd1306);
                Adafruit_GFX_setFont(&Org_01);
                Adafruit_GFX_setTextColor(CL_WHITE,CL_BLACK);
                Adafruit_GFX_setRotation(DISPLAY_ROTATE_0);
                Adafruit_GFX_fillScreen(0);
                Adafruit_GFX_setTextSize(3,3);
                ssd1306.show();
                new_state = UI_SPLASH;
            }
            break;
        case UI_SPLASH:     // Show splash screen
            if (previous_state != ui_state) {
                Adafruit_GFX_drawBitmap(SPLASH_X, SPLASH_Y, (uint8_t *) logo, LOGO_WIDTH, LOGO_HEIGHT, 1, 0, 0);
                Adafruit_GFX_printAt(SPLASH_X+LOGO_WIDTH+12,12,"C64");
                Adafruit_GFX_printAt(SPLASH_X+LOGO_WIDTH+12,29,"PSU");
                ssd1306.show();
                ui_timer = utils_get_time();
            }
            if (utils_get_time() > ui_timer + GUI_SPLASH_TIME) {
                new_state = UI_SHOW_DATA;
                ui_screen_saver_timer = utils_get_time();
            }
            break;
        case UI_SHOW_DATA:  // Show 5V data
            // Check if there is a fault on one of the outputs
            if (ac_control_fsm(STATE_NONE) == STATE_FAULT || dc_check_status() == STATE_FAULT) {
                new_state = UI_ERROR;
            } else if (dc_check_status() == STATE_OFF) {
                ui_count_down = 31;
                new_state = UI_POWERING_OFF;
            } else if (utils_get_time() > ui_timer + GUI_UPDATE_TIME) {
                ui_show_data();
                ssd1306.show();
                ui_timer = utils_get_time();
            }
            break;
        case UI_ERROR:  // Decode and show error messages
            if (previous_state != ui_state) {
                Adafruit_GFX_fillScreen(0);
            }
            if (ac_control_fsm(STATE_NONE) == STATE_FAULT) {
                Adafruit_GFX_setTextSize(3,3);
                Adafruit_GFX_printAt(AC_ERR_PX, ERR_PY, "9V");
                Adafruit_GFX_setTextSize(2,2);
                Adafruit_GFX_printAt(AC_ERR_PX, ERR_PY+10, "Fault");
                ssd1306.show();
            }
            switch (dc_check_status()) {
                case STATE_OVERVOLTAGE:
                    Adafruit_GFX_setTextSize(3,3);
                    Adafruit_GFX_printAt(DC_ERR_PX, ERR_PY, "5V");
                    Adafruit_GFX_setTextSize(2,2);
                    Adafruit_GFX_printAt(AC_ERR_PX, ERR_PY+10, "High");
                    ssd1306.show();
                    break;
                case STATE_UNDERVOLTAGE:
                    Adafruit_GFX_setTextSize(3,3);
                    Adafruit_GFX_printAt(DC_ERR_PX, ERR_PY, "5V");
                    Adafruit_GFX_setTextSize(2,2);
                    Adafruit_GFX_printAt(AC_ERR_PX, ERR_PY+10, "Low");
                    ssd1306.show();
                    break;
                case STATE_OVERCURRENT:
                    Adafruit_GFX_setTextSize(3,3);
                    Adafruit_GFX_printAt(DC_ERR_PX, ERR_PY, "5V");
                    Adafruit_GFX_setTextSize(2,2);
                    Adafruit_GFX_printAt(AC_ERR_PX, ERR_PY+10, "Current");
                    ssd1306.show();
                    break;
                case STATE_OFF:
                    new_state = UI_BYE;
                    break;
                case STATE_ON:
                    if (ac_control_fsm(STATE_NONE) != STATE_FAULT) new_state = UI_SHOW_DATA;
                    break;
                default:
                    break;
            }
            break;
        case UI_POWERING_OFF:
            if (dc_check_status() == STATE_ON) {
                new_state = UI_SHOW_DATA;
            } else if (utils_get_time() > ui_timer + GUI_UPDATE_TIME) {
                ui_show_data();
                Adafruit_GFX_drawLine(0,31,0,31 - ui_count_down, 1);
                ssd1306.show();
                ui_timer = utils_get_time();
                if (ui_count_down) ui_count_down--; else new_state = UI_BYE;
            }
            break;
        case UI_BYE:
            if (previous_state != ui_state) {
                Adafruit_GFX_fillScreen(0);
                Adafruit_GFX_setTextSize(3,3);
                Adafruit_GFX_printAt(BYE_PX, BYE_PY, "BYE!");
                ssd1306.show();
            }
            if (dc_check_status() == STATE_ON) {
                new_state = UI_POWER_ON;
            } else if (utils_get_time() > ui_timer + GUI_SPLASH_TIME) {
                new_state = UI_OFF;
            }
            break;
        case UI_OFF:
            if (previous_state != ui_state) {
                Adafruit_GFX_fillScreen(0);
                ssd1306.show();
            }
            if (dc_check_status() == STATE_ON) {
                new_state = UI_SHOW_DATA;
            }
            break;
        default:
            break;
    }
    previous_state = ui_state;
    if (new_state != UI_POWER_ON) ui_state = new_state;
}
