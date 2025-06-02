/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
#ifdef __cplusplus
        extern "C" {
        #endif
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (2)
#endif
/* ISR prototypes */
void tau_tmi_isr(void);
void sau_i2c_tei_isr(void);

/* Vector table allocations */
#define VECTOR_NUMBER_TAU0_TMI00 ((IRQn_Type) 19) /* TAU0 TMI00 (End of timer channel 00 count or capture) */
#define TAU0_TMI00_IRQn          ((IRQn_Type) 19) /* TAU0 TMI00 (End of timer channel 00 count or capture) */
#define VECTOR_NUMBER_SAU0_UART_RXI1 ((IRQn_Type) 23) /* SAU0 IIC TXRXI11 (SAU UART RX 1/I2C 11/SPI 11) */
#define SAU0_UART_RXI1_IRQn          ((IRQn_Type) 23) /* SAU0 IIC TXRXI11 (SAU UART RX 1/I2C 11/SPI 11) */
#ifdef __cplusplus
        }
        #endif
#endif /* VECTOR_DATA_H */
