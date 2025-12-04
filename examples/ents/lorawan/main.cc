/*
   RadioLib Non-Arduino Tock Library test application

   Licensed under the MIT or Apache License

   Copyright (c) 2023 Alistair Francis <alistair@alistair23.me>
 */

// include the library
// #include <RadioLib.h>

// include the hardware abstraction layer
#include "config.h"
// Include some libtock-c helpers
#define BUFFER_LEN 64

// the entry point for the program
int main(void) {
  char buffer[BUFFER_LEN];

  printf("[SX1261] Initialising Radio ... \n");

  // create a new instance of the HAL class
  // TockRadioLibHal* hal = new TockRadioLibHal();

  TockRadioLibHal* hal = new TockRadioLibHal();
  SX1262 tock_module = new Module(hal, RADIOLIB_RADIO_NSS, RADIOLIB_RADIO_DIO_1, RADIOLIB_RADIO_RESET,
                                 RADIOLIB_RADIO_BUSY);

                                 // create the LoRaWAN node
  // print const LoRaWANBand_t US915
  printf("reg: %d\n", Region->txSpans->numChannels);
  printf("subBand: %d\n", Region->freqMin);
                                 
  LoRaWANNode node(&tock_module, Region, subBand);
  // now we can create the radio module
  // pinout corresponds to the SparkFun LoRa Thing Plus - expLoRaBLE
  // NSS pin:   0
  // DIO1 pin:  2
  // NRST pin:  4
  // BUSY pin:  1

  
  printf("[SX1261] Setting up Radio ... \n");
  // Setup the radio
  // The settings here work for the SparkFun LoRa Thing Plus - expLoRaBLE
  // radio->XTAL = true;
  assert(&tock_module != nullptr);
  // receiver is s1262 with parameters:
  // int16_t state = radio->begin(
  //     915.0,   // MHz
  //     125.0,   // kHz
  //     7,       // spreading factor
  //     5,       // coding rate 4/5
  //     0x34,    // sync word (LoRaWAN)
  //     14,      // power in dBm (max 14 for STM32WL/SX1261)
  //     8        // preamble length
  // );


  int state = tock_module.begin();

  // tock_module.setDio2AsRfSwitch(false);
  // tock_module.setCurrentLimit(60.0);
  // tock_module.explicitHeader();
  // tock_module.setCRC(2);
  
  assert(&node != nullptr);

  printf("[SX1261] Radio setup\n");
  if (state != RADIOLIB_ERR_NONE) {
    printf("failed, code %d\r\n", state);
    return 1;
  }
  printf("success!\r\n");

  //while (1) {
  //  uint32_t t1 = hal->millis();
  //  hal->delay(2000); // 100 s
  //  uint32_t t2 = hal->millis();
  //  printf("elapsed = %lu ms\n", t2 - t1);
  //}

  static const uint8_t devEUI_bytes[8] = FORMAT32_KEY(RADIOLIB_LORAWAN_DEV_EUI);
  static const uint64_t devEUI  = bytes_to_u64_be(devEUI_bytes);
  uint8_t appKey[16] = FORMAT_KEY(RADIOLIB_LORAWAN_APP_KEY);
 
  static const uint8_t joinEUI_bytes[8] = FORMAT32_KEY(RADIOLIB_LORAWAN_JOIN_EUI);
  static const uint64_t joinEUI = bytes_to_u64_be(joinEUI_bytes);

  static const uint8_t nwkKey[16] = FORMAT_KEY(RADIOLIB_LORAWAN_NWK_KEY);

  node.scanGuard = 100;

  printf("joinEUI: ");
  for (int i = 0; i < 8; i++) {
    printf("%02X, ", joinEUI_bytes[i]);
  }
  printf("\r\n");

  printf("devEUI: ");
  for (int i = 0; i < 8; i++) {
    printf("%02X, ", devEUI_bytes[i]);
  }
  printf("\r\n");

  printf("appkey: ");
  for (int i = 0; i < 16; i++) {
    printf("%02X, ", appKey[i]);
  }
  printf("\r\n");

  printf("nwkey: ");
  for (int i = 0; i < 16; i++) {
    printf("%02X, ", nwkKey[i]);
  }
  printf("\r\n");

  // Setup the OTAA session info
  state = node.beginOTAA(joinEUI, devEUI, nwkKey, appKey);

  if (state != RADIOLIB_ERR_NONE) {
    printf("LoRaWAN OTAA setup failed, code %d\r\n", state);
    return 1;
  }
  printf("LoRaWAN OTAA setup success!\r\n");

  state = node.activateOTAA();
  if (state != RADIOLIB_ERR_NONE && state != RADIOLIB_LORAWAN_NEW_SESSION) {
    printf("LoRaWAN OTAA activation failed, code %d\r\n", state);
    return 1;
  }
  printf("LoRaWAN OTAA activation success!\r\n");

  // loop forever
  for ( ;;) {
    // Ensure there are no pending callbacks
    yield_no_wait();

    // Form payload to send
    snprintf(buffer, BUFFER_LEN, "Hello!");
    printf("Sending uplink of %lu bytes: %s\r\n", strlen(buffer), buffer);

    state = node.sendReceive(buffer, strlen(buffer));
    if (state != RADIOLIB_ERR_NONE) {
      printf("LoRaWAN send/receive failed, code %d\r\n", state);
    } else {
      printf("LoRaWAN send/receive success!\r\n");
    }



    // Wait until next uplink - observing legal & TTN FUP constraints
    hal->delay(uplinkIntervalSeconds * 1000UL);
  }

  return 0;
}
