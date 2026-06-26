/**
 *  Модуль 2.4.  
 *  Домашнє завдання
 *  Завдання 3: Debounce через перевірку рівня (state-based)
 */
#include <Arduino.h>

constexpr uint8_t BUTTON = 4;

uint16_t serial_delay = 500;
uint32_t last_serial_output = 0;

volatile uint8_t counter = 0;

bool is_button_pressed = false;

void serial_monitor_init();
void IRAM_ATTR button_press_ISR();

void setup() {
    serial_monitor_init();

    pinMode(BUTTON, INPUT_PULLUP);

    attachInterrupt(BUTTON, button_press_ISR, FALLING);
}

void loop() {
    uint32_t now = millis();

    if (is_button_pressed && digitalRead(BUTTON) == LOW) {
        is_button_pressed = false;

        counter++;
    }


    if (now - last_serial_output >= serial_delay) {
        last_serial_output = now;

        Serial.println(counter);
    }
}

void serial_monitor_init() {
    Serial.begin(115200);
    delay(1000);
}

void IRAM_ATTR button_press_ISR() { 
    is_button_pressed = true;
}