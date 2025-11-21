#include <stdio.h>
#include <string.h>

#include <libtock-sync/services/alarm.h>

#include "ads1219.h"

int main(void) {
  printf("Example ADS1219\n");

  // TODO Need to add support for userspace gpio
  // setup gpio pin (PA9)

  //bool gpio_exists = libtock_gpio_exists();
  //printf("[Test] GPIO exists: %s\n", gpio_exists ? "true" : "false");

  //int gpio_count = 0;
  //libtock_gpio_count(&gpio_count);
  //printf("[Test] GPIO count: %d\n", gpio_count);

  //uint32_t powerdown = 0;
  //libtock_gpio_enable_output(powerdown);
  //libtock_gpio_clear(powerdown);


  ads1219_status_t ret = ADS1219_SUCCESS;

  ret = ads1219_reset();
  if (ret != ADS1219_SUCCESS) {
    printf("ADS1219 reset failed\n");
  }

  for (int i = 0; ; i++) {
    uint32_t voltage = 0;
    ret = ads1219_voltage_raw(&voltage);
    if (ret != ADS1219_SUCCESS) {
      printf("ADS1219 voltage read failed\n");
      continue;
    }

    uint32_t current = 0;
    ret = ads1219_current_raw(&current);
    if (ret != ADS1219_SUCCESS) {
      printf("ADS1219 current read failed\n");
      continue;
    }

    printf("v: %ld, i: %ld\n", voltage, current);

    libtocksync_alarm_delay_ms(1000);
  }
}
