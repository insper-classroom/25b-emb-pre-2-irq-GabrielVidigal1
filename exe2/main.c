#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdbool.h>

const int BTN_PIN_R = 28;
const int LED_PIN = 4;

volatile bool btn_pressed_flag = false;

void btn_callback(uint gpio, uint32_t events) {
  btn_pressed_flag = true;
}

int main() {
  stdio_init_all();

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_set_irq_enabled_with_callback(
      BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);

  bool led_is_on = false;

  while (true) {
    if (btn_pressed_flag) {
      btn_pressed_flag = false;
      led_is_on = !led_is_on;
      gpio_put(LED_PIN, led_is_on);
    }
  }
}