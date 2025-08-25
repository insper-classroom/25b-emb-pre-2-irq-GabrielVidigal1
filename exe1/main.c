#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;

volatile int event_flag = 0;

#define NO_EVENT 0
#define FALL_EVENT 1
#define RISE_EVENT 2

void btn_callback(uint gpio, uint32_t events) {
  if (events == GPIO_IRQ_EDGE_FALL) { 
    event_flag = FALL_EVENT;
  } else if (events == GPIO_IRQ_EDGE_RISE) { 
    event_flag = RISE_EVENT;
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_set_irq_enabled_with_callback(
      BTN_PIN_R, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

  while (true) {
    if (event_flag == FALL_EVENT) {
      printf("fall \n");
      event_flag = NO_EVENT; 
    } else if (event_flag == RISE_EVENT) {
      printf("rise \n");
      event_flag = NO_EVENT; 
    }
  }
}
