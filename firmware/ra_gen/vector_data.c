/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [19] = tau_tmi_isr, /* TAU0 TMI00 (End of timer channel 00 count or capture) */
            [23] = sau_i2c_tei_isr, /* SAU0 IIC TXRXI11 (SAU UART RX 1/I2C 11/SPI 11) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
            [19] = BSP_PRV_VECT_ENUM(EVENT_TAU0_TMI00,GROUP3), /* TAU0 TMI00 (End of timer channel 00 count or capture) */
            [23] = BSP_PRV_VECT_ENUM(EVENT_SAU0_UART_RXI1,GROUP7), /* SAU0 IIC TXRXI11 (SAU UART RX 1/I2C 11/SPI 11) */
        };
        #endif
        #endif
