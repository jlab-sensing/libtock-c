#pragma once

#include "../tock.h"
#include "syscalls/app_software_watchdog_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Register this process with the software watchdog.
//
// - `window_ticks`: number of kernel ticks before the watchdog fires if not
//   tickled. Must be called once before libtock_app_software_watchdog_tickle().
//
// Returns RETURNCODE_SUCCESS or an error.
returncode_t libtock_app_software_watchdog_register(uint32_t window_ticks);

// Tickle the watchdog, resetting the deadline.
//
// Returns RETURNCODE_SUCCESS or an error.
returncode_t libtock_app_software_watchdog_tickle(void);

#ifdef __cplusplus
}
#endif
