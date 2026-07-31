// Positive test for AppSoftwareWatchdog.
//
// Registers this process with a 4096-tick watchdog window but never tickles
// it. The kernel will restart this process once the deadline expires.
// Each restart increments a persistent counter so the console output confirms
// the restarts are happening.

#include <stdio.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/kernel/app_software_watchdog.h>
#include <libtock/tock.h>

int main(void) {
  uint32_t time_window_sec = 5;
  
  printf("[FIRE APP] starting...\n");

  returncode_t err = libtock_app_software_watchdog_register(time_window_sec);

  if (err != RETURNCODE_SUCCESS) {
   printf("[FIRE APP]  ERROR: register returned %d\n", err);
   return 1;
  }
  printf("[FIRE APP] Watchdog registered with window size of %d sec). NOT tickling — expect restart.\n", time_window_sec);

  // Spin forever printing a heartbeat.  The watchdog will cut this short.
    uint32_t now;
    
  for (int i = 0; ; i++) {
    libtocksync_alarm_delay_ms(1000);
    printf("[FIRE APP] Iteration %d (no watchdog tickle) \n", i);
  }

  return 0;
}
