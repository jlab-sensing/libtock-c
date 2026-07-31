#include "app_software_watchdog_syscalls.h"

bool libtock_app_software_watchdog_exists(void) {
  return driver_exists(DRIVER_NUM_APP_SOFTWARE_WATCHDOG);
}

returncode_t libtock_app_software_watchdog_command_register(uint32_t window_ticks) {
  syscall_return_t rval = command(DRIVER_NUM_APP_SOFTWARE_WATCHDOG, 1, window_ticks, 0);
  return tock_command_return_novalue_to_returncode(rval);
}

returncode_t libtock_app_software_watchdog_command_tickle(void) {
  syscall_return_t rval = command(DRIVER_NUM_APP_SOFTWARE_WATCHDOG, 2, 0, 0);
  return tock_command_return_novalue_to_returncode(rval);
}
