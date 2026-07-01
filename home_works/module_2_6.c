/**
 *  Модуль 2.6.  
 *  Домашнє завдання
 *  Домашнє завдання: Усунення брязкоту контактів (ESP-IDF)
 */
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

const gpio_num_t BUTTON_GPIO = GPIO_NUM_5;

const TickType_t debounce_time = pdMS_TO_TICKS(30);

TickType_t last_press = 0;

uint8_t counter = 0;
volatile bool button_pressed = false;

void IRAM_ATTR button_press_isr(void *arg);

void app_main(void) {
    // Configure BUTTON Pin
    gpio_reset_pin(BUTTON_GPIO);
    gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
    gpio_pullup_en(BUTTON_GPIO);

    // Configure Interrupt to trigger on the Falling Edge
    gpio_set_intr_type(BUTTON_GPIO, GPIO_INTR_NEGEDGE);

    // Set global GPIO ISR service
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_GPIO, button_press_isr, NULL);

    while (1) {
        if (button_pressed) {
            TickType_t now = xTaskGetTickCount();

            if (now - last_press >= debounce_time) {
                last_press = xTaskGetTickCount();

                if (gpio_get_level(BUTTON_GPIO) == 0) {
                    counter++;
                    printf("%d \n", counter);
                }
            }

            button_pressed = false;
        }

        // Prevent watchdog trigger
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void button_press_isr(void *arg) { button_pressed = true; }
