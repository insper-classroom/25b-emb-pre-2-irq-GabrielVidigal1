#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include <stdbool.h>
#include <stdio.h>

const int BTN_PIN_R = 28;
const uint DEBOUNCE_DELAY_MS = 200;

volatile bool press_event_flag = false;
volatile bool release_event_flag = false;
volatile uint64_t last_event_time = 0;

void btn_callback(uint gpio, uint32_t events) {
    uint64_t current_time = time_us_64();

    if ((current_time - last_event_time) > (DEBOUNCE_DELAY_MS * 1000)) {
        if (events & GPIO_IRQ_EDGE_FALL) {
            press_event_flag = true;
        } else if (events & GPIO_IRQ_EDGE_RISE) {
            release_event_flag = true;
        }
        last_event_time = current_time;
    }
}

int main() {
    stdio_init_all();

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &btn_callback);


    while (1) {
        if (press_event_flag) {
            press_event_flag = false;
            printf("btn pressed\n");
        }
        
        if (release_event_flag) {
            release_event_flag = false;
            printf("btn released\n");
        }
        
        tight_loop_contents();
    }
}