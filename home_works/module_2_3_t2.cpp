/**
 *  Модуль 2.3.  
 *  Домашнє завдання
 *  Superloop та незалежні задачі (завдання №2)
 */
#include <Arduino.h>

constexpr uint8_t LED_1 = 4;
constexpr uint8_t LED_2 = 5;
constexpr uint8_t LED_3 = 6;
constexpr uint8_t BUTTON = 1;

constexpr uint8_t LEDS_QTY = 3;

const uint8_t LED[LEDS_QTY] = {LED_1, LED_2, LED_3};

const uint16_t INTERVAL = 500;

bool direction = 1;
bool last_button_state = 0;

int8_t current_index_on = 0;
uint32_t last_change = 0;

void setup() {
    for (uint8_t i = 0; i < LEDS_QTY; i++) {
        pinMode(LED[i], OUTPUT);
    }

    pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
    uint32_t now = millis();

    if (now - last_change >= INTERVAL) {
        last_change = now;

        for (uint8_t i = 0; i < LEDS_QTY; i++) {
            digitalWrite(LED[i], i == current_index_on);
        }

        direction ? current_index_on++ : current_index_on--;

        if (current_index_on >= LEDS_QTY) {
            current_index_on = 0;
        }
        if (current_index_on < 0) {
            current_index_on = LEDS_QTY - 1;
        }
    }

    bool button_read = digitalRead(BUTTON);

    if (last_button_state != button_read) {
        if (button_read == LOW) {
            direction = !direction;
        }
    }

    last_button_state = button_read;
}