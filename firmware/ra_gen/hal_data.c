/* generated HAL source file - do not edit */
#include "hal_data.h"
tau_instance_ctrl_t g_timer0_ctrl;
const tau_extended_cfg_t g_timer0_extend =
{ .opirq = TAU_INTERRUPT_OPIRQ_BIT_RESET,
  .tau_func = TAU_FUNCTION_INTERVAL,
  .bit_mode = TAU_BIT_MODE_16BIT,
  .initial_output = TAU_PIN_OUTPUT_CFG_DISABLED,
  .input_source = TAU_INPUT_SOURCE_NONE,
  .tau_filter = TAU_INPUT_NOISE_FILTER_DISABLE,
  .trigger_edge = TAU_TRIGGER_EDGE_RISING,
  .operation_clock = TAU_OPERATION_CK01,
  /* Not used for 16-bit or lower 8-bit mode */
  .period_higher_8bit_counts = (uint16_t) 0x100,
  .higher_8bit_cycle_end_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_TAU0_TMI00H)
    .higher_8bit_cycle_end_irq       = VECTOR_NUMBER_TAU0_TMI00H,
#else
  .higher_8bit_cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
const timer_cfg_t g_timer0_cfg =
{ .mode = (timer_mode_t) 0,
/* Actual Period: 0.0166670000 seconds. */
/* Minimum Period ~ Maximum Period: 0.0000010000 ~ 0.06553600 seconds. */.period_counts = (uint32_t) 0x411b,
  .duty_cycle_counts = 0, .source_div = (timer_source_div_t) BSP_CFG_TAU_CK01, .channel = 0, .p_callback = callback60Hz,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = &g_timer0_extend,
  .cycle_end_ipl = (0),
#if defined(VECTOR_NUMBER_TAU0_TMI00)
    .cycle_end_irq       = VECTOR_NUMBER_TAU0_TMI00,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer0 =
{ .p_ctrl = &g_timer0_ctrl, .p_cfg = &g_timer0_cfg, .p_api = &g_timer_on_tau };
#include "r_sau_i2c_cfg.h"
sau_i2c_instance_ctrl_t g_i2c0_ctrl;

#if SAU_I2C_CFG_DTC_SUPPORT_ENABLE
transfer_info_t RA_NOT_DEFINED_info[2] DTC_TRANSFER_INFO_ALIGNMENT =
{
    { .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
      .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
      .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
      .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_EACH,
      .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
      .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
      .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
      .p_dest = (void*) NULL,
      .p_src = (void const*) NULL,
      .num_blocks = 0,
      .length = 0, },
    { .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
      .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
      .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
      .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
      .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
      .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
      .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
      .p_dest = (void*) NULL,
      .p_src = (void const*) NULL,
      .num_blocks = 0,
      .length = 0, }
};

const transfer_cfg_t RA_NOT_DEFINED_cfg_sau_i2c =
{
  .p_info              = RA_NOT_DEFINED_info,
  .p_extend = &RA_NOT_DEFINED_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t RA_NOT_DEFINED_sau_i2c =
{
    .p_ctrl        = &RA_NOT_DEFINED_ctrl,
    .p_cfg         = &RA_NOT_DEFINED_cfg_sau_i2c,
    .p_api         = &g_transfer_on_dtc
};

#endif

const sau_i2c_extended_cfg_t g_i2c0_cfg_extend =
{
/* Actual calculated bitrate: 100000 */
.clock_settings.stclk = 79,
  .clock_settings.operation_clock = SAU_I2C_MASTER_OPERATION_CLOCK_CK0, .i2c_unit = 0, .delay_time = 5, };

const i2c_master_cfg_t g_i2c0_cfg =
{ .channel = 3, .rate = I2C_MASTER_RATE_STANDARD, .slave = 0x3c, .p_callback = sau_i2c_master_callback, .p_context =
          NULL,
#if defined(VECTOR_NUMBER_SAU0_UART_RXI1)
    .tei_irq             = VECTOR_NUMBER_SAU0_UART_RXI1,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
  .ipl = (2),
  .p_extend = &g_i2c0_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_tx = NULL,
#else
                .p_transfer_tx   = &RA_NOT_DEFINED_sau_i2c,
#endif
#undef RA_NOT_DEFINED
        };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c0 =
{ .p_ctrl = &g_i2c0_ctrl, .p_cfg = &g_i2c0_cfg, .p_api = &g_i2c_master_on_sau };
adc_d_instance_ctrl_t g_adc0_ctrl;

/* ADC conversion time 3.1875 us, conversion start time: 1.0625us */
const adc_d_extended_cfg_t g_adc0_cfg_extend =
{ .channel_mode = ADC_D_CHANNEL_MODE_SELECT, .operation_voltage = ADC_D_VOLTAGE_MODE_NORMAL_1, .conversion_clockdiv =
          ADC_D_CLOCK_DIV_1,
  .trigger_source = ADC_D_TRIGGER_SOURCE_SOFTWARE, .operation_trigger = ADC_D_TRIGGER_MODE_WAIT, .conversion_operation =
          ADC_D_CONVERSION_MODE_ONESHOT,
  .upper_lower_bound = ADC_D_BOUNDARY_IN_RANGE, .negative_vref = ADC_D_NEGATIVE_VREF_VSS, .positive_vref =
          ADC_D_POSITIVE_VREF_VCC,
  .upper_bound_limit = 255, .lower_bound_limit = 0, };
const adc_cfg_t g_adc0_cfg =
{ .unit = 0, .mode = (adc_mode_t) 0, // Unused
  .resolution = ADC_RESOLUTION_12_BIT,
  .alignment = (adc_alignment_t) 0, // Unused
  .trigger = (adc_trigger_t) 0, // Unused
  .p_callback = NULL,
  .p_context = NULL,
  .p_extend = &g_adc0_cfg_extend,
#if defined(VECTOR_NUMBER_ADC0_SCAN_END)
    .scan_end_irq        = VECTOR_NUMBER_ADC0_SCAN_END,
#else
  .scan_end_irq = FSP_INVALID_VECTOR,
#endif
  .scan_end_ipl = (BSP_IRQ_DISABLED),
  .scan_end_b_irq = FSP_INVALID_VECTOR,
  .scan_end_b_ipl = BSP_IRQ_DISABLED, };

const adc_d_channel_cfg_t g_adc0_channel_cfg =
{ .channel_input = ADC_CHANNEL_0 };
/* Instance structure to use this module. */
const adc_instance_t g_adc0 =
{ .p_ctrl = &g_adc0_ctrl, .p_cfg = &g_adc0_cfg, .p_channel_cfg = &g_adc0_channel_cfg, .p_api = &g_adc_on_adc_d };
void g_hal_init(void)
{
    g_common_init ();
}
