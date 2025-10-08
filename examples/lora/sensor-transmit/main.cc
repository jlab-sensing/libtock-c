/*
   RadioLib Non-Arduino Tock Library test application

   Licensed under the MIT or Apache License

   Copyright (c) 2023 Alistair Francis <alistair@alistair23.me>
 */

// include the library
#include <RadioLib.h>

// include the hardware abstraction layer
#include "libtockHal.h"

// Include some libtock-c helpers

#define BUFFER_LEN 64

// the entry point for the program
int main(void) {
  char buffer[BUFFER_LEN];

  printf("[SX1261] Initialising Radio ... \n");

  // create a new instance of the HAL class
  TockRadioLibHal* hal = new TockRadioLibHal();

  // now we can create the radio module
  // pinout corresponds to the SparkFun LoRa Thing Plus - expLoRaBLE
  // NSS pin:   0
  // DIO1 pin:  2
  // NRST pin:  4
  // BUSY pin:  1
  Module* tock_module = new Module(hal, RADIOLIB_RADIO_NSS, RADIOLIB_RADIO_DIO_1, RADIOLIB_RADIO_RESET,
                                   RADIOLIB_RADIO_BUSY);
  SX1262* radio = new SX1262(tock_module);


  printf("[SX1261] Setting up Radio ... \n");
  // Setup the radio
  // The settings here work for the SparkFun LoRa Thing Plus - expLoRaBLE
  // radio->XTAL = true;

  // receiver is s1262 with parameters:
int16_t state = radio->begin(
    915.0,   // MHz
    125.0,   // kHz
    7,       // spreading factor
    5,       // coding rate 4/5
    0x34,    // sync word (LoRaWAN)
    14,      // power in dBm (max 14 for STM32WL/SX1261)
    8        // preamble length
);

radio->setDio2AsRfSwitch(false);
// radio->setDio3AsTcxoCtrl(RADIOLIB_SX126X_TCXO_CTRL_1_8V);
radio->setCurrentLimit(60.0);
radio->explicitHeader();
radio->setCRC(2);

  
  printf("[SX1261] Radio setup\n");
  if (state != RADIOLIB_ERR_NONE) {
    printf("failed, code %d\r\n", state);
    return 1;
  }
  printf("success!\r\n");

  int temp = 0;
  int humi = 0;

  // loop forever
  for ( ;;) {
    // Ensure there are no pending callbacks
    yield_no_wait();

    snprintf(buffer, BUFFER_LEN, "Hello from ents over LoRa!\r\n");

    // send a packet
    printf("[SX1261] Transmitting hello from ents over LoRa!\r\n");

    state = radio->transmit((const uint8_t*)buffer, strlen(buffer));

    if (state == RADIOLIB_ERR_NONE) {
      // the packet was successfully transmitted
      printf("success!\r\n");

      // wait for a second before transmitting again
      hal->delay(1000);
    } else {
      printf("failed, code %d\r\n", state);
    }
  }

  return 0;
}
