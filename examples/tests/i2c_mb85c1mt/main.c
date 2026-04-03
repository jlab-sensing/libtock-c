/* vim: set sw=2 expandtab tw=80: */


/**
 * Helper file for debugging i2c tests with the onboard FRAM chip. (MB85RC1MT).
 *
 * @author John Madden (jmadden173@pm.me)
 */

#include <stdio.h>
#include <stdint.h>

#include "libtock/peripherals/i2c_master.h"


// i2c address
uint8_t mb85c1mt_addr = 0b10100000;
uint8_t ads1219_addr = 0x40 << 1;


int main(void) {

  printf("test i2c mb85rc1mt\n");


  const size_t buffer_size = 32;
  uint8_t buffer[buffer_size] = {};

  int status = 0;



  // ADS1219

  // RREG command
  buffer[0] = 0b00100000;

  status = i2c_master_write_sync(ads1219_addr, buffer, 1);
  if (status < 0) {
    printf("could not write\n");
  } else {
    printf("success\n"); 
  }

  status = i2c_master_read_sync(ads1219_addr, buffer, 1);
  if (status < 0) {
    printf("could not read\n");
  } else {
    printf("success: %x\n", buffer[0]); 
  }


  // repeat the same call with write_read
  
  buffer[0] = 0b00100000;
  
  status = i2c_master_write_read_sync(ads1219_addr, buffer, 1, 1);
  if (status < 0) {
    printf("could not write_read\n");
  } else {
    printf("success: %x\n", buffer[0]); 
  }

  return 0;


  // single byte write

  buffer[0] = 0;
  buffer[1] = 0;
  buffer[2] = 1;

  printf("first try\n");
  status = i2c_master_write_sync(mb85c1mt_addr, buffer, 3);
  if (status < 0) {
    printf("could not write\n");
  } else {
    printf("success\n"); 
  }


  // retry to see what happens

  printf("second try\n");
  status = i2c_master_write_sync(mb85c1mt_addr, buffer, 3);
  if (status < 0) {
    printf("could not write\n");
  } else {
    printf("success\n"); 
  }






  //// read chip id
  // Unsure how this works with multiple devices on the same bus
 
  //// device id cmd -> chip address

  //buffer[0] = addr;
  //status = i2c_master_write_sync(0xf8, buffer, 2);
  //if (status) {
  //  printf("could not write\n");
  //}

  //// device id cmd -> 3 bytes

  //status = i2c_master_read_sync(0xf8, buffer, 3);
  //if (status) {
  //  printf("could not read");
  //}

  //printf("deviceid: %x %x %x\n", buffer[0], buffer[1], buffer[2]);




  printf("Done!\n");

  return 0;
}
