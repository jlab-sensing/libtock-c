/**
 * @file mb85rc1mt.h
 * @brief FRAM driver for MB85RC1MT over TockOS I2C master syscalls
 *
 * Ported from the baremetal STM32 HAL implementation in ENTS-node-firmware.
 * Uses libtock-c I2C master API instead of HAL_I2C_Mem_Write/Read.
 *
 * Datasheet: https://www.fujitsu.com/uk/Images/MB85RC1MT.pdf
 *
 * @author Pritish Mahato
 * @date 2026-02-25
 *
 * Copyright (c) 2026 jLab, UCSC
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

/**
 * @defgroup fram FRAM
 * @brief Library for interfacing with the MB85RC1MT FRAM chip via TockOS
 * @{
 */

/** Status codes for the FRAM library */
typedef enum {
  FRAM_OK           =  0,
  FRAM_ERROR        = -1,
  FRAM_OUT_OF_RANGE = -2,
} FramStatus;

/** Address type (flat address space) */
typedef uint32_t FramAddr;

/**
 * @brief Write bytes to FRAM at the given address
 *
 * @param addr  Starting address in FRAM (0 to fram_size()-1)
 * @param data  Pointer to data to write
 * @param len   Number of bytes to write
 * @return FRAM_OK on success, FRAM_OUT_OF_RANGE or FRAM_ERROR on failure
 */
FramStatus fram_write(FramAddr addr, const uint8_t *data, size_t len);

/**
 * @brief Read bytes from FRAM at the given address
 *
 * @param addr  Starting address in FRAM (0 to fram_size()-1)
 * @param len   Number of bytes to read
 * @param data  Buffer to read into (must be at least len bytes)
 * @return FRAM_OK on success, FRAM_OUT_OF_RANGE or FRAM_ERROR on failure
 */
FramStatus fram_read(FramAddr addr, size_t len, uint8_t *data);

/**
 * @brief Get the total size of FRAM in bytes
 *
 * @return Number of bytes available (131072 for MB85RC1MT)
 */
FramAddr fram_size(void);

/** @} */

#ifdef __cplusplus
}
#endif
