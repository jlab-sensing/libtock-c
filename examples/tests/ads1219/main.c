#include <stdio.h>
#include <string.h>

#include <libtock/tock.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/peripherals/i2c_master.h>

int main(void) {
  printf("[Test] Hello, World!\n");

  for (int i=0; ; i++) {
    printf("[Test] Loop count: %d\n", i);
    libtocksync_alarm_delay_ms(1000);
  }
}
