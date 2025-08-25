#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdbool.h>
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;
const int LED_PIN_R = 4;
const int LED_PIN_G = 6;

volatile bool red_btn_event = false;
volatile bool green_btn_event = false;

void btn_callback(uint gpio, uint32_t events) {
  if (gpio == BTN_PIN_R && events == GPIO_IRQ_EDGE_FALL) {
    red_btn_event = true;
  } else if (gpio == BTN_PIN_G && events == GPIO_IRQ_EDGE_RISE) {
    green_btn_event = true;
  }
}

int main() {
  stdio_init_all();

  bool red_led_state = false;
  bool green_led_state = false;

  gpio_init(LED_PIN_R);
  gpio_set_dir(LED_PIN_R, GPIO_OUT);

  gpio_init(LED_PIN_G);
  gpio_set_dir(LED_PIN_G, GPIO_OUT);

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_init(BTN_PIN_G);
  gpio_set_dir(BTN_PIN_G, GPIO_IN);
  gpio_pull_up(BTN_PIN_G);

  gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
                                     &btn_callback);
  gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_RISE, true);

  while (true) {
    if (red_btn_event) {
      red_btn_event = false;
      red_led_state = !red_led_state;
      gpio_put(LED_PIN_R, red_led_state);
    }

    if (green_btn_event) {
      green_btn_event = false;
      green_led_state = !green_led_state;
      gpio_put(LED_PIN_G, green_led_state);
    }
  }
}