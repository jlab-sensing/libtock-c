#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_APP_SOFTWARE_WATCHDOG 0x10003

// Returns true if the driver is present.
bool libtock_app_software_watchdog_exists(void);

// Command 1: register this process and set the watchdog window size in ticks.
returncode_t libtock_app_software_watchdog_command_register(uint32_t window_ticks);

// Command 2: tickle the watchdog, resetting the deadline.
returncode_t libtock_app_software_watchdog_command_tickle(void);

#ifdef __cplusplus
}
#endif
