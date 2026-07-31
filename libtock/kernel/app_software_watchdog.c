#include "app_software_watchdog.h"

returncode_t libtock_app_software_watchdog_register(uint32_t window_ticks) {
  return libtock_app_software_watchdog_command_register(window_ticks);
}

returncode_t libtock_app_software_watchdog_tickle(void) {
  return libtock_app_software_watchdog_command_tickle();
}
