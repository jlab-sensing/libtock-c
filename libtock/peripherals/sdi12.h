#pragma once

#include "../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for sdi12 receive callback.
//
// - `arg1` (`int`): Returncode indicating status from sampling the sensor.
// - `arg2` (`uint32_t`): Length of the data received placed into the provide read/write buffer.
typedef void (*libtock_sdi12_receive_callback)(returncode_t, uint32_t);
typedef void (*libtock_sdi12_write_done_callback)(returncode_t);

// Check if the driver exists.
bool libtock_sdi12_exists(void);

// Start a sdi12 receive operation. The reading will be provided via the callback.
returncode_t libtock_sdi12_receive(libtock_sdi12_receive_callback cb, uint8_t* rx_buffer, uint32_t len);

returncode_t libtock_sdi12_get_measurement(libtock_sdi12_receive_callback cb, uint8_t* tx_buffer, uint8_t* rx_buffer);

// Write data to the sdi12 bus.
returncode_t libtock_sdi12_write(libtock_sdi12_write_done_callback cb, uint8_t* tx_buffer, uint32_t len);

#ifdef __cplusplus
}
#endif
