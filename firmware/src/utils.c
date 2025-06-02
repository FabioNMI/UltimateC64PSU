/*
 * utils.c
 *
 *  Created on: Sep. 1, 2024
 *      Author: fabiop
 */
#include "hal_data.h"
#include "utils.h"

void SysTick_Handler(void);

static uint32_t timer;

void SysTick_Handler(void) {
    timer++;
}

void utils_init_time(uint32_t freq) {
    SysTick_Config(SystemCoreClock / freq);
    timer = 0;
}

uint32_t utils_get_time(void) {
    return timer;
}


