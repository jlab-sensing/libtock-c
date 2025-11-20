#include <stdio.h>
#include <string.h>

#include <libtock/tock.h>
#include <libtock-sync/services/alarm.h>
#include <libtock/peripherals/i2c_master.h>
#include <libtock/peripherals/gpio.h>


void measure(void);


void measure(void) {
  uint8_t buffer[16];

  // start conversion
  buffer[0] = 0x08; 

  printf("Writing\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_write_sync(0x40 << 1, buffer, 1));

  // arbitrary delay for testing
  libtocksync_alarm_delay_ms(100);

  printf("Reading\n");
  TOCK_EXPECT(RETURNCODE_SUCCESS, i2c_master_read_sync(0x40 << 1, buffer, 3));

  for (int i = 0; i < 3; i++) {
    printf("Byte %d: 0x%02x\n", i, buffer[i]);
  }
}


int main(void) {
  printf("[Test] Hello, World!\n");

  // setup gpio pin (PA9)


  bool gpio_exists = libtock_gpio_exists();
  printf("[Test] GPIO exists: %s\n", gpio_exists ? "true" : "false");

  //int gpio_count = 0;
  //libtock_gpio_count(&gpio_count);
  //printf("[Test] GPIO count: %d\n", gpio_count);

  //uint32_t powerdown = 0;
  //libtock_gpio_enable_output(powerdown);
  //libtock_gpio_clear(powerdown);

  // take a measurement
  measure();

  while (true) {
    yield();
  }

  
  //for (int i=0; ; i++) {
  //  printf("[Test] Loop count: %d\n", i);
  //  libtocksync_alarm_delay_ms(1000);
  //}
}
