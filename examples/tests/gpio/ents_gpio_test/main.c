/**
 * @file main.c
 * @brief GPIO test for the Seeed Studio LoRa-E5 mini (ENTS board)
 *
 * Tests the 8 userspace GPIO pins exposed by the kernel GPIO capsule:
 *
 *   Pin 0 => PA0  (ESP32 wakeup signal)
 *   Pin 1 => PA9  (peripheral power-down, default HIGH)
 *   Pin 2 => PB4  (general purpose)
 *   Pin 3 => PB5  (user LED, also used by LED capsule)
 *   Pin 4 => PB10 (ESP32 enable)
 *   Pin 5 => PB13 (battery voltage monitor)
 *   Pin 6 => PB14 (charge indicator CHG)
 *   Pin 7 => PC0  (ADC data ready)
 *
 * Tests performed:
 *   1. Verify the GPIO driver exists and reports 8 pins.
 *   2. Walk through output pins 0,2,3,4 with a 500 ms pulse each.
 *   3. Read input pins 5,6,7 and print their state.
 *   4. Confirm PA9 (pin 1) stays HIGH (peripherals powered).
 *
 * @author Pritish Mahato
 * @date   2026-04-29
 *
 * Copyright (c) 2026 jLab, UCSC
 */

#include <stdbool.h>
#include <stdio.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/peripherals/gpio.h>
#include <libtock/tock.h>

/* ---------------------------------------------------------
 * Userspace GPIO pin indices (must match kernel mapping)
 * --------------------------------------------------------- */
#define GPIO_PA0_ESP32_WAKEUP  0
#define GPIO_PA9_PERIPH_PWR    1
#define GPIO_PB4_GP            2
#define GPIO_PB5_LED           3
#define GPIO_PB10_ESP32_EN     4
#define GPIO_PB13_BATT_MON     5
#define GPIO_PB14_CHG          6
#define GPIO_PC0_ADC_DRDY      7

#define NUM_GPIO_PINS          8

/* Pins we drive as output in the walking test */
static const uint32_t output_pins[] = {
  GPIO_PA0_ESP32_WAKEUP,
  GPIO_PB4_GP,
  GPIO_PB5_LED,
  GPIO_PB10_ESP32_EN,
};
#define NUM_OUTPUT_PINS (sizeof(output_pins) / sizeof(output_pins[0]))

/* Pins we sample as input */
static const uint32_t input_pins[] = {
  GPIO_PB13_BATT_MON,
  GPIO_PB14_CHG,
  GPIO_PC0_ADC_DRDY,
};
#define NUM_INPUT_PINS  (sizeof(input_pins) / sizeof(input_pins[0]))

static const char* pin_names[NUM_GPIO_PINS] = {
  "PA0  (ESP32 wakeup)",
  "PA9  (periph pwr)",
  "PB4  (general purpose)",
  "PB5  (user LED)",
  "PB10 (ESP32 enable)",
  "PB13 (batt monitor)",
  "PB14 (CHG indicator)",
  "PC0  (ADC drdy)",
};

/* =========================================================
 * Test 1 – Driver existence + pin count
 * ========================================================= */
static bool test_driver_exists(void) {
  printf("[TEST 1] GPIO driver exists? ");
  if (!libtock_gpio_exists()) {
    printf("FAIL - driver not found!\n");
    return false;
  }
  printf("OK\n");

  int count       = 0;
  returncode_t rc = libtock_gpio_count(&count);
  printf("[TEST 1] GPIO pin count: %d  (expected %d)  %s\n",
         count, NUM_GPIO_PINS,
         (rc == RETURNCODE_SUCCESS && count == NUM_GPIO_PINS) ? "OK" : "FAIL");

  return rc == RETURNCODE_SUCCESS && count == NUM_GPIO_PINS;
}

/* =========================================================
 * Test 2 – Walking output pulse
 * ========================================================= */
static void test_output_walk(void) {
  printf("\n[TEST 2] Walking output pulse (500 ms each)...\n");
  for (uint32_t i = 0; i < NUM_OUTPUT_PINS; i++) {
    uint32_t pin = output_pins[i];
    libtock_gpio_enable_output(pin);
    libtock_gpio_clear(pin);   /* start LOW */
  }

  for (uint32_t rep = 0; rep < 2; rep++) {
    for (uint32_t i = 0; i < NUM_OUTPUT_PINS; i++) {
      uint32_t pin = output_pins[i];
      printf("  -> Pulsing pin %lu (%s) HIGH\n", (unsigned long)pin, pin_names[pin]);
      libtock_gpio_set(pin);
      libtocksync_alarm_delay_ms(500);
      libtock_gpio_clear(pin);
      libtocksync_alarm_delay_ms(100);
    }
  }
  printf("[TEST 2] Done\n");
}

/* =========================================================
 * Test 3 – Read input pins
 * ========================================================= */
static void test_input_read(void) {
  printf("\n[TEST 3] Reading input pins...\n");
  for (uint32_t i = 0; i < NUM_INPUT_PINS; i++) {
    uint32_t pin = input_pins[i];
    libtock_gpio_enable_input(pin, libtock_pull_none);
    int val         = 0;
    returncode_t rc = libtock_gpio_read(pin, &val);
    printf("  Pin %lu (%s) = %d  %s\n",
           (unsigned long)pin, pin_names[pin], val,
           rc == RETURNCODE_SUCCESS ? "OK" : "FAIL");
  }
  printf("[TEST 3] Done\n");
}

/* =========================================================
 * Test 4 – PA9 (peripheral power) stays HIGH
 * ========================================================= */
static void test_periph_power_pin(void) {
  printf("\n[TEST 4] PA9 (periph pwr, pin 1) should read HIGH...\n");
  /* The kernel already drives PA9 HIGH before handing it to userspace.
   * Configure as input momentarily to sample the line. */
  libtock_gpio_enable_input(GPIO_PA9_PERIPH_PWR, libtock_pull_none);
  int val         = 0;
  returncode_t rc = libtock_gpio_read(GPIO_PA9_PERIPH_PWR, &val);
  printf("  PA9 = %d  %s\n", val,
         (rc == RETURNCODE_SUCCESS && val == 1) ? "OK (HIGH)" : "UNEXPECTED");
  /* Restore as output HIGH so peripherals stay powered */
  libtock_gpio_enable_output(GPIO_PA9_PERIPH_PWR);
  libtock_gpio_set(GPIO_PA9_PERIPH_PWR);
}

/* =========================================================
 * Main
 * ========================================================= */
int main(void) {
  printf("\n======================================\n");
  printf(" ENTS LoRa-E5 Mini GPIO Test\n");
  printf("======================================\n\n");

  bool ok = test_driver_exists();
  if (!ok) {
    printf("Aborting: GPIO driver unavailable.\n");
    return 1;
  }

  test_output_walk();
  test_input_read();
  test_periph_power_pin();

  printf("\n======================================\n");
  printf(" All GPIO tests complete.\n");
  printf("======================================\n");
  return 0;
}
