#pragma once

#include <libtock/peripherals/sdi12.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

bool libtocksync_sdi12_exists(void);

returncode_t libtocksync_sdi12_receive(uint8_t* rx_buffer, uint32_t len);

returncode_t libtocksync_sdi12_write_and_receive(uint8_t* tx_buffer, uint32_t tx_len, uint8_t* rx_buffer,
                                                 uint32_t rx_len);

returncode_t libtocksync_sdi12_write(uint8_t* tx_buffer, uint32_t len);
#ifdef __cplusplus
}
#endif
