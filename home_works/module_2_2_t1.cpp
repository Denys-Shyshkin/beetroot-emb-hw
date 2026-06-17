/**
 *  Модуль 2.2.  
 *  Домашнє завдання
 *  Завдання 1: Час спрацювання реле
 */
#include <Arduino.h>

constexpr uint8_t RELAY_CONTROL_PIN = 4;
constexpr uint8_t RELAY_STATUS_PIN = 1;
constexpr uint8_t BUTTON_PIN = 5;

constexpr uint16_t BUTTON_DEBOUNCE = 50;

bool current_button_state = 1;
bool current_relay_state = 0;
bool prev_relay_state = 0;
bool last_button_state = 1;
uint32_t last_button_press_time = 0;

uint16_t relay_switch_count = 0;

uint32_t switch_start_time = 0;
volatile uint32_t switch_finish_time = 0;
uint32_t prev_switch_finish_time = 0;

void serial_monitor_init();
void IRAM_ATTR relay_status_ISR();
void print_measurements();

void setup() {
    serial_monitor_init();

    pinMode(RELAY_CONTROL_PIN, OUTPUT);
    pinMode(RELAY_STATUS_PIN, INPUT_PULLUP);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(RELAY_STATUS_PIN), relay_status_ISR, CHANGE);
}

void loop() {
    bool button_reading = digitalRead(BUTTON_PIN);

    if (last_button_state != button_reading) {
        last_button_press_time = millis();
    }
    
    if (millis() - last_button_press_time > BUTTON_DEBOUNCE) {
        if (button_reading != current_button_state) {
            current_button_state = !current_button_state;

            if (button_reading == LOW) {
                current_relay_state = !current_relay_state;
            }
        }
    }

    if (current_relay_state != prev_relay_state) {
        relay_switch_count++;
        switch_start_time = millis();
        digitalWrite(RELAY_CONTROL_PIN, current_relay_state);
    }
    
    if (prev_switch_finish_time != switch_finish_time) {
        print_measurements();
    }

    last_button_state = button_reading;
    prev_relay_state = current_relay_state;
    prev_switch_finish_time = switch_finish_time;
}

void serial_monitor_init() {
    Serial.begin(115200);
    delay(1000);
}

void IRAM_ATTR relay_status_ISR() {
    switch_finish_time = millis();
}

void print_measurements() {
    Serial.printf("%2d | Start time: %d | Finish time: %d | Duration: %d \n",
                  relay_switch_count, switch_start_time, switch_finish_time, switch_finish_time - switch_start_time);
}