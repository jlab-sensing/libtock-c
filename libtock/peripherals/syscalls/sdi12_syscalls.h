#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_SDI12 0x20008

#define SDI12_COMMAND_TX 1
#define SDI12_COMMAND_RX 2
#define SDI12_GET_MEASUREMENT 3

#define SDI12_ALLOW_RW_RX_BUFFER 0
#define SDI12_ALLOW_RO_TX_BUFFER 1

#define SDI12_SUBSCRIBE_RX 0
#define SDI12_SUBSCRIBE_TX_DONE 1

// Check if the RTC driver exists.
bool libtock_sdi12_driver_exists(void);

// Set the upcall for get and set complete callbacks.
returncode_t libtock_sdi12_set_receive_upcall(subscribe_upcall callback, void* opaque);
returncode_t libtock_sdi12_set_write_done_upcall(subscribe_upcall callback, void* opaque);

// Command to write data over the sdi12 interface using the previously provided buffer.
// A `readonly` buffer must be provided to the kernel driver prior to calling this function.
returncode_t libtock_sdi12_command_write(uint8_t* tx_buffer, uint32_t len);

// Waits asynchronously to receive data over the sdi12 interface. A `readwrite`
// buffer must be provided to the kernel driver prior to calling this function.
returncode_t libtock_sdi12_command_receive(uint8_t* rx_buffer, uint32_t len);

// Sends a get measurement command to the sensor at the specified address.
// Waits asynchronously to receive data over the sdi12 interface. A `readwrite`
// buffer must be provided to the kernel driver prior to calling this function.
returncode_t libtock_sdi12_command_get_measurement(uint8_t* tx_buffer, uint8_t* rx_buffer);

returncode_t libtock_sdi12_set_readwrite_allow_rx(uint8_t* buffer, uint32_t len);
returncode_t libtock_sdi12_set_readonly_allow_tx(const uint8_t* buffer, uint32_t len);
#ifdef __cplusplus
}
#endif
