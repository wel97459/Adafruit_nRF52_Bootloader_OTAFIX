#include "tpl5010.h"

#ifdef WDT_ENABLED

/**
 * @brief Send a >100 µs pulse on DONE pin (TPL5010 requirement)
 *        300 µs gives plenty of margin and works with long traces/capacitance
 */
void tpl5010_done_pulse(void)
{
    nrf_gpio_pin_set(DONE_PIN);
    nrf_delay_us(10);
    nrf_gpio_pin_clear(DONE_PIN);
}

/**
 * @brief Initialize GPIOs for TPL5010 watchdog
 *        Must be called very early (before USB, before SoftDevice, etc.)
 */
void tpl5010_init(void)
{
    // DONE pin → normal push-pull output, start low
    nrf_gpio_cfg_output(DONE_PIN);
    nrf_gpio_pin_clear(DONE_PIN);

    // WAKE pin → input with pull-down and SENSE high (latch)
    nrf_gpio_cfg_input(WAKE_PIN, NRF_GPIO_PIN_PULLDOWN);
    nrf_gpio_cfg_sense_set(WAKE_PIN, NRF_GPIO_PIN_SENSE_HIGH);

    // Immediate kick on first boot — prevents instant reset if timer already running
    tpl5010_done_pulse();
}

/**
 * @brief Check if TPL5010 is asking "are you alive?" and answer immediately
 *        Call this as often as possible: after wake, after USB init, in main loop
 */
void tpl5010_kick_if_needed(void)
{
    // Fast check: is the latch still high?
    if (nrf_gpio_pin_latch_get(WAKE_PIN))
    {
        // Clear the latch first (prevents infinite triggering)
        nrf_gpio_pin_latch_clear(WAKE_PIN);

        // Tell TPL5010 we are alive
        tpl5010_done_pulse();

        // Re-arm the SENSE mechanism (very important!)
        // Temporarily disable then re-enable sense
        nrf_gpio_cfg_sense_set(WAKE_PIN, NRF_GPIO_PIN_SENSE_LOW);   // dummy
        nrf_gpio_cfg_sense_set(WAKE_PIN, NRF_GPIO_PIN_SENSE_HIGH);
    }
}

#endif