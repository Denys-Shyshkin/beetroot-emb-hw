/**
 *  Модуль 2.2.  
 *  Домашнє завдання
 *  Завдання 2 (опційне): Програмний PWM
 */
#include <Arduino.h>

constexpr uint8_t POTENTIOMETER = 4;
constexpr uint8_t TRANSISTOR = 5;
constexpr uint8_t PERIOD = 20; // 50 Hz
constexpr uint16_t POT_MAX_VALUE = 4095;

bool current_transistor_state = 0;
uint32_t last_recorded_on_state = 0;
uint32_t last_recorded_off_state = 0;

void serial_monitor_init();
uint8_t calculate_on_duration(uint16_t pot_reading);

void setup() {
    serial_monitor_init();

    pinMode(POTENTIOMETER, INPUT);
    pinMode(TRANSISTOR, OUTPUT);
}

void loop() {
    uint16_t pot_reading = analogRead(POTENTIOMETER);

    uint8_t on_duration = calculate_on_duration(pot_reading);
    uint8_t off_duration = PERIOD - on_duration;

    if (current_transistor_state == HIGH && millis() - last_recorded_on_state > on_duration) {
        current_transistor_state = LOW;
        last_recorded_off_state = millis();
    }

    if (current_transistor_state == LOW && millis() - last_recorded_off_state > off_duration) {
        current_transistor_state = HIGH;
        last_recorded_on_state = millis();
    }

    digitalWrite(TRANSISTOR, current_transistor_state);
}

void serial_monitor_init() {
    Serial.begin(115200);
    delay(1000);
}

uint8_t calculate_on_duration(uint16_t pot_reading) {
    return (pot_reading * PERIOD) / POT_MAX_VALUE;
}