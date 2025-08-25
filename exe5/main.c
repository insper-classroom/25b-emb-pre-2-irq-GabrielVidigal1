#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include <stdbool.h>
#include <stdio.h>

const int BTN_PIN = 28;
const uint32_t LONG_PRESS_THRESHOLD_MS = 800;

volatile uint64_t press_start_time = 0;
volatile bool event_occurred = false;
volatile bool is_long_press = false;

void btn_callback(uint gpio, uint32_t events) {
  if (events & GPIO_IRQ_EDGE_FALL) {
    press_start_time = time_us_64();
  } else if (events & GPIO_IRQ_EDGE_RISE) {
    uint64_t press_duration_us = time_us_64() - press_start_time;
    if (press_duration_us > LONG_PRESS_THRESHOLD_MS * 1000) {
      is_long_press = true;
    } else {
      is_long_press = false;
    }
    event_occurred = true;
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN);
  gpio_set_dir(BTN_PIN, GPIO_IN);
  gpio_pull_up(BTN_PIN);

  gpio_set_irq_enabled_with_callback(BTN_PIN,
                                     GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,
                                     true, &btn_callback);

  while (true) {
    if (event_occurred) {
      event_occurred = false;
      if (is_long_press) {
        printf("Aperto longo!\n");
      } else {
        printf("Aperto curto!\n");
      }
    }
  }
}