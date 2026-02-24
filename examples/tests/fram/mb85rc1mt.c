/**
 * @file mb85rc1mt.c
 * @brief FRAM driver for MB85RC1MT using TockOS I2C master syscalls
 *
 * Ported from stm32/lib/storage/src/mb85rc1mt.c.
 * Replaces HAL_I2C_Mem_Write/Read with libtock-c i2c_master_write_sync
 * and i2c_master_write_read_sync.
 *
 * The MB85RC1MT is a 128KB (1 Mbit) FRAM with I2C interface.
 * Memory is accessed via a 17-bit address:
 *   - Bit 16 is encoded in the device address (A0 bit position)
 *   - Bits 15:0 are sent as a 16-bit memory address
 *
 * @author Pritish Mahato
 * @date 2026-02-25
 *
 * Copyright (c) 2026 jLab, UCSC
 */

#include <libtock/peripherals/i2c_master.h>

#include "mb85rc1mt.h"

/** Base I2C address of the MB85RC1MT (8-bit format: 0xA0) */
#define MB85RC1MT_BASE_ADDR 0xA0

/** Total size of the chip in bytes (128 KB = 1 Mbit) */
#define MB85RC1MT_SIZE (1 << 17)

/** I2C buffer size: 2 bytes address + 1 byte data (for byte-at-a-time writes) */
#define I2C_BUF_SIZE 3

/**
 * @brief Internal representation of the MB85RC1MT I2C address
 */
typedef struct {
  /** 8-bit device address (includes upper address bit and R/W) */
  uint8_t dev;
  /** 16-bit memory address within the device */
  uint16_t mem;
} Mb85rc1mtAddress;

/**
 * @brief Convert a flat FRAM address to MB85RC1MT I2C address format
 *
 * The 17-bit address is split:
 *   - Bit 16 -> device address bit A0 (shifted into position)
 *   - Bits 15:0 -> 16-bit memory address
 *
 * @param addr Flat FRAM address (0 to MB85RC1MT_SIZE-1)
 * @return Formatted I2C address
 */
static Mb85rc1mtAddress convert_address(FramAddr addr) {
  Mb85rc1mtAddress i2c_addr;

  // Upper address bits go into device address.
  // Shift right by 15 to get bit 16 into position, mask to keep only
  // relevant bits, OR with base address.
  i2c_addr.dev = MB85RC1MT_BASE_ADDR | ((addr >> 15) & 0x0E);
  i2c_addr.mem = addr & 0xFFFF;

  return i2c_addr;
}

FramStatus fram_write(FramAddr addr, const uint8_t* data, size_t len) {
  // Bounds check
  if (addr + len > MB85RC1MT_SIZE) {
    return FRAM_OUT_OF_RANGE;
  }

  // Write data byte-by-byte.
  // The MB85RC1MT supports sequential writes within a page, but for
  // simplicity and correctness across page boundaries we write one byte
  // at a time, matching the baremetal implementation.
  for (size_t i = 0; i < len; i++) {
    Mb85rc1mtAddress i2c_addr = convert_address(addr + i);

    // Build I2C write buffer: [mem_addr_hi, mem_addr_lo, data_byte]
    uint8_t buf[I2C_BUF_SIZE];
    buf[0] = (uint8_t)(i2c_addr.mem >> 8);
    buf[1] = (uint8_t)(i2c_addr.mem & 0xFF);
    buf[2] = data[i];

    int ret = i2c_master_write_sync(i2c_addr.dev, buf, I2C_BUF_SIZE);
    if (ret != RETURNCODE_SUCCESS) {
      return FRAM_ERROR;
    }
  }

  return FRAM_OK;
}

FramStatus fram_read(FramAddr addr, size_t len, uint8_t* data) {
  // Bounds check
  if (addr + len > MB85RC1MT_SIZE) {
    return FRAM_OUT_OF_RANGE;
  }

  // Read data byte-by-byte to handle address boundary crossings correctly.
  for (size_t i = 0; i < len; i++) {
    Mb85rc1mtAddress i2c_addr = convert_address(addr + i);

    // Build buffer with memory address for the write phase of write-read.
    uint8_t buf[2];
    buf[0] = (uint8_t)(i2c_addr.mem >> 8);
    buf[1] = (uint8_t)(i2c_addr.mem & 0xFF);

    // write_read: write 2 bytes (memory address), then read 1 byte (data).
    // After completion, the read byte is placed at buf[0].
    int ret = i2c_master_write_read_sync(i2c_addr.dev, buf, 2, 1);
    if (ret != RETURNCODE_SUCCESS) {
      return FRAM_ERROR;
    }

    data[i] = buf[0];
  }

  return FRAM_OK;
}

FramAddr fram_size(void) {
  return MB85RC1MT_SIZE;
}
