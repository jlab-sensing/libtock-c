// Negative test for AppSoftwareWatchdog.
//
// Registers this process with a 32768-tick watchdog window and tickles it
// every ~500 ms.  The watchdog should never fire; the app runs indefinitely
// printing a counter to confirm it is alive.

#include <stdio.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/kernel/app_software_watchdog.h>
#include <libtock/tock.h>

int main(void) {
  printf("[PASS APP] starting...\n");

  // Register with a generous 32768-tick window.
  returncode_t err = libtock_app_software_watchdog_register(5);
  if (err != RETURNCODE_SUCCESS) {
    printf("[PASS APP] ERROR: register returned %d\n", err);
    return 1;
  }
  printf("[PASS APP] Watchdog registered. Tickling every 3 sec.\n");

  // Loop forever, tickling before each sleep so the deadline is always reset.
  for (int i = 0; ; i++) {
    err = libtock_app_software_watchdog_tickle();
    if (err != RETURNCODE_SUCCESS) {
      printf("[PASS APP] ERROR: tickle returned %d\n", err);
      return 1;
    }
    libtocksync_alarm_delay_ms(3000);
    printf("[PASS APP]  alive %d (watchdog tickled)\n", i);
  }

  return 0;
}
