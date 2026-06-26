/**
 *  Модуль 2.4.
 *  Домашнє завдання
 *  Завдання 4: Polling + debounce (без interrupts)
 */
#include <Arduino.h>

constexpr uint8_t BUTTON = 4;

uint16_t serial_delay = 500;
uint32_t last_serial_output = 0;

uint8_t button_debounce_delay = 50;
uint32_t last_button_press = 0;

uint8_t button_check_delay = 10;
uint32_t last_button_check = 0;

bool button_state = 1;
bool last_button_state = 1;

uint8_t counter = 0;

enum ButtonState { CHECK, PRESSED, DEBOUNCE };

uint8_t current_state = ButtonState::CHECK;

void serial_monitor_init();

void setup() {
    serial_monitor_init();

    pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
    uint32_t now = millis();

    

    switch (current_state) {
    case ButtonState::CHECK:
        if (now - last_button_check >= button_check_delay) {
            last_button_check = now;

            button_state = digitalRead(BUTTON);

            if (button_state != last_button_state && button_state == LOW) {
                current_state = ButtonState::PRESSED;
            }
        }
        break;

    case ButtonState::PRESSED:
        last_button_press = now;
        current_state = ButtonState::DEBOUNCE;
        break;

    case ButtonState::DEBOUNCE:
        if (now - last_button_press >= button_debounce_delay) {
            if (button_state == LOW) {
                counter++;
            }
            current_state = ButtonState::CHECK;
        }
        break;
    }

    if (now - last_serial_output >= serial_delay) {
        last_serial_output = now;

        Serial.println(counter);
    }

    last_button_state = button_state;
}

void serial_monitor_init() {
    Serial.begin(115200);
    delay(1000);
}