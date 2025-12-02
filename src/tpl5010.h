#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "boards.h"

#ifdef WDT_ENABLED
// ------------------------------------------------------------------
// CHANGE THESE FOR YOUR BOARD
// ------------------------------------------------------------------
#ifndef DONE_PIN
#define DONE_PIN   8   // Example: P0.08 – change in your board.h
#endif

#ifndef WAKE_PIN
#define WAKE_PIN   9   // Example: P0.09 – change in your board.h
#endif

// ------------------------------------------------------------------
// Public API
// ------------------------------------------------------------------
void tpl5010_init(void);
void tpl5010_kick_if_needed(void);
void tpl5010_done_pulse(void);

// Convenience macro for main loop
#define TPL5010_KICK()   tpl5010_kick_if_needed()

// Optional: auto-kick on every boot (recommended)
#define TPL5010_KICK_ON_START()  do { tpl5010_kick_if_needed(); } while(0)
#endif