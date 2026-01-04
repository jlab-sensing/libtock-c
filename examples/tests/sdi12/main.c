#include <stdio.h>
#include <string.h>

#include <libtock-sync/peripherals/sdi12.h>

#define DATA_LEN 3

uint8_t data_buf[DATA_LEN]; 

#define READ_LEN 16

uint8_t read_buf[READ_LEN];

int main(void) {
  returncode_t ret;
  printf("[TEST] SDI12\r\n");

  data_buf[0] = '0';
  data_buf[1] = 'M';
  data_buf[2] = '!';

  if (!libtocksync_sdi12_exists()) {
    printf("[FAIL] No SDI12 driver.\n");
    return -2;
  } else {
    printf("[PASS] SDI12 driver exists.\n");
  }

  // WRITE TEST //
  printf("Beginning SDI12 write test...");
  printf("Data to write: ");
  for(int i = 0; i < DATA_LEN; i++) {
    printf("%c", data_buf[i]);
  }
  ret = libtocksync_sdi12_write(data_buf, DATA_LEN);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[FAIL] Unable to write SDI12 data.\n");
    return -1;
  } else {
    printf("[PASS] Completed SDI12 write.\n");
  }

  // READ TEST //
  printf("Beginning read test...");
  ret = libtocksync_sdi12_receive(read_buf, READ_LEN);
  if (ret != RETURNCODE_SUCCESS) {
    printf("[FAIL] Unable to read SDI12 data.\n");
    return -1;
  } else {
    printf("[PASS] Completed SDI12 receive -- (note this test does not confirm the accuracy of the received data).\n");
  }
  // Print as string
  printf("Recieved Buffer as string: '");
  for (int i = 0; i < 7; i++) {
    char c = read_buf[i];
    if (c == '\n') {
        printf("\\n");
    } else if (c == '\r') {
        printf("\\r");
    } else if (c == '\t') {
        printf("\\t");
    } else if (c < 32 || c > 126) {
        printf("\\x%02X", (unsigned char)c);
    } else {
        printf("%c", c);
    }
  }
  printf("'\n");

  printf("[SUCCESS] All SDI12 tests have passed.");
  return 0;
}
