#include <stdio.h>
#include <string.h>

#include <libtock-sync/peripherals/sdi12.h>
#include <libtock-sync/services/alarm.h>

#define RESPONSE_LEN 60
#define MEASUREMENT_LEN 60

uint8_t response_buf[RESPONSE_LEN]       = {};
uint8_t measurement_buf[MEASUREMENT_LEN] = {};

int main(void) {
  returncode_t ret = 0;

  printf("[TEST] SDI12\r\n");

  if (!libtocksync_sdi12_exists()) {
    printf("[FAIL] No SDI12 driver.\n");
    return -2;
  } else {
    printf("[PASS] SDI12 driver exists.\n");
  }

  //
  // Send measure command and wait for service request
  //
  // The read length is hardcoded to the length of the BREAK, Command, and
  // response length.
  //
  // \00M!00013\r\n0\r\n
  //

  printf("Sending measure command...\n");
  ret = libtocksync_sdi12_write_and_receive((uint8_t*) "0M!", 3, response_buf, 14);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[FAIL] Measure failed with code %d.\n", ret);
    return -1;
  }

  // clear parity bit
  for (int i = 0; i < RESPONSE_LEN; i++) {
    response_buf[i] &= 0x7F;
  }

  printf("Response:\n===%s===\n\n", response_buf + 1);

  //
  // Read measurement data
  // The read length is hardcoded to the length of the BREAK, Command, and
  // response length.
  //
  // \00D0!0+1837.02+19.1+0\r\n
  //
  //

  printf("Reading data...\n");
  ret = libtocksync_sdi12_write_and_receive((uint8_t*) "0D0!", 4, response_buf, 23);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[FAIL] 0D0! failed with code %d.\n", ret);
    return -1;
  }

  // clear parity bit
  for (int i = 0; i < RESPONSE_LEN; i++) {
    response_buf[i] &= 0x7F;
  }

  printf("Data:\n===%s===\n\n", response_buf + 1);

  printf("[SUCCESS] All SDI12 tests have passed.\n");
  return 0;
}
