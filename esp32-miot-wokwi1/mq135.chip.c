#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  pin_t pin_a0;
  uint32_t ppm_attr;
} chip_state_t;

static void chip_timer_callback(void *user_data) {
  chip_state_t *chip = (chip_state_t*)user_data;
  uint32_t ppm = attr_read(chip->ppm_attr);
  
  // Konversi PPM ke Voltase (0-3.3V untuk ESP32)
  float voltage = (float)ppm * 3.3 / 1000.0;
  pin_dac_write(chip->pin_a0, voltage);
}

void chip_init() {
  chip_state_t *chip = malloc(sizeof(chip_state_t));
  chip->pin_a0 = pin_init("A0", ANALOG);
  chip->ppm_attr = attr_init("ppm", 100);

  const timer_config_t config = {
    .callback = chip_timer_callback,
    .user_data = chip,
    .period = 100, // update setiap 100ms
  };
  timer_t timer_id = timer_init(&config);
  timer_start(timer_id, true);
}