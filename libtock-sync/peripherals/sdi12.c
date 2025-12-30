#include "sdi12.h"

struct sdi12_receive_data {
    bool fired;
    returncode_t ret;
    uint32_t length;
};

struct sdi12_write_data {
    bool fired;
    returncode_t ret;
};

static struct sdi12_receive_data rx_result = { .fired = false };
static struct sdi12_write_data tx_result = { .fired = false };

static void sdi12_receive_cb(returncode_t ret, uint32_t length) {
    rx_result.fired = true;
    rx_result.ret = ret;
    rx_result.length = length;
}

static void sdi12_tx_done_cb(returncode_t ret) {
    tx_result.fired = true;
    tx_result.ret = ret;
}

bool libtocksync_sdi12_exists(void) {
    return libtock_sdi12_exists();
}

returncode_t libtocksync_sdi12_receive(uint8_t* rx_buffer, uint32_t len) {
    returncode_t ret;

    rx_result.fired = false;

    ret = libtock_sdi12_receive(sdi12_receive_cb, rx_buffer, len);
    if (ret != RETURNCODE_SUCCESS) return ret;

    yield_for(&rx_result.fired);
    return rx_result.ret;
}

returncode_t libtocksync_sdi12_write(uint8_t* tx_buffer, uint32_t len) {
    returncode_t ret; 

    tx_result.fired = false;
    printf("A");
    ret = libtock_sdi12_write(sdi12_tx_done_cb, tx_buffer, len);
    printf("B");
    if (ret != RETURNCODE_SUCCESS) return tx_result.ret;

    yield_for(&tx_result.fired);
    return tx_result.ret;   
}