// include the hardware abstraction layer
#include "config.h"
#define BUFFER_LEN 64
#include <libtock/kernel/ipc.h>
#include <libtock/tock.h>

struct lorawan_service {
  bool pending_data;
  int8_t length;
  char buf[BUFFER_LEN];
};

lorawan_service lorawan_service_data;

// Callback a client process can call to transmit data over lorawan interface.
static void lorawan_callback(int pid, int len, int buf, __attribute__ ((unused)) void* ud) {
  if (lorawan_service_data.pending_data) {
    // Transmitted 0 bytes.
    reinterpret_cast<uint8_t*>(buf)[0] = 0;

    // Busy - notify client to try later
    ipc_notify_client(pid);
    return;
  }

  lorawan_service_data.pending_data = true;

  if (len > BUFFER_LEN) {
    len = BUFFER_LEN;
  }
  lorawan_service_data.length = len;
  memcpy(lorawan_service_data.buf, reinterpret_cast<uint8_t*>(buf), len);
}

int counter = 0;
// the entry point for the program
int main(void) {

  printf("[SX1261] Initialising Radio ... \n");

  TockRadioLibHal* hal = new TockRadioLibHal();
  SX1262 tock_module   = new Module(hal, RADIOLIB_RADIO_NSS, RADIOLIB_RADIO_DIO_1, RADIOLIB_RADIO_RESET,
                                    RADIOLIB_RADIO_BUSY);

  // create the LoRaWAN node
  // print const LoRaWANBand_t US915
  printf("reg: %d\n", Region->txSpans->numChannels);
  printf("subBand: %ld\n", Region->freqMin);

  LoRaWANNode node(&tock_module, Region, subBand);
  // now we can create the radio module
  // pinout corresponds to the SparkFun LoRa Thing Plus - expLoRaBLE
  // NSS pin:   0
  // DIO1 pin:  2
  // NRST pin:  4
  // BUSY pin:  1

  // Setup the radio
  assert(&tock_module != nullptr);
  int state = tock_module.begin();

  assert(&node != nullptr);

  printf("[SX1261] Radio setup\n");
  if (state != RADIOLIB_ERR_NONE) {
    printf("failed, code %d\r\n", state);
    return 1;
  }

  static const uint8_t devEUI_bytes[8] = FORMAT32_KEY(RADIOLIB_LORAWAN_DEV_EUI);
  static const uint64_t devEUI         = bytes_to_u64_be(devEUI_bytes);
  uint8_t appKey[16] = FORMAT_KEY(RADIOLIB_LORAWAN_APP_KEY);

  static const uint8_t joinEUI_bytes[8] = FORMAT32_KEY(RADIOLIB_LORAWAN_JOIN_EUI);
  static const uint64_t joinEUI         = bytes_to_u64_be(joinEUI_bytes);

  static const uint8_t nwkKey[16] = FORMAT_KEY(RADIOLIB_LORAWAN_NWK_KEY);

  node.scanGuard = 100;

  // Setup the OTAA session info
  state = node.beginOTAA(joinEUI, devEUI, nwkKey, appKey);

  if (state != RADIOLIB_ERR_NONE) {
    printf("LoRaWAN OTAA setup failed, code %d\r\n", state);
    return 1;
  }
  printf("LoRaWAN OTAA setup success!\r\n");

  state = node.activateOTAA();
  while (state != RADIOLIB_ERR_NONE && state != RADIOLIB_LORAWAN_NEW_SESSION) {
    printf("LoRaWAN OTAA activation failed, code %d. Delaying 5 seconds, then retrying.\r\n", state);
    hal->delay(5000);
  }

  printf("LoRaWAN OTAA activation success!\r\n");

  ipc_register_service_callback("ents.lorwan", lorawan_callback,
                                NULL);

  // loop forever
  for ( ;;) {
    // Ensure there are no pending callbacks
    yield_no_wait();

    // Check if there is data to send from a client process.
    if (lorawan_service_data.pending_data) {
      // Send the data
      state = node.sendReceive(lorawan_service_data.buf, lorawan_service_data.length);

      printf("LoRaWAN uplink send code %d\r\n", state);

      // Notify the client process that sent the data.
      lorawan_service_data.pending_data = false;
      ipc_notify_client(lorawan_service_data.length);
    }

    printf("LoRaWAN send/receive code %d\r\n", state);

    // Wait until next uplink - observing legal & TTN FUP constraints
    hal->delay(uplinkIntervalSeconds * 1000UL);
    counter++;
  }

  return 0;
}
