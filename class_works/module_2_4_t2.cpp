#include <Arduino.h>

constexpr uint8_t BUTTON = 4;

volatile uint32_t last_interrupt_time = 0;
uint8_t button_debounce_time = 50;

uint32_t last_report_time = 0;
uint16_t report_debounce_time = 1000;

volatile uint8_t pulse_count = 0;

void serial_monitor_init();
void IRAM_ATTR button_press_ISR();

void setup() {
    serial_monitor_init();

    pinMode(BUTTON, INPUT_PULLUP);

    attachInterrupt(BUTTON, button_press_ISR, FALLING);
    Serial.println("Start: ");
}

void loop() {
    uint32_t now = millis();

    if (now - last_report_time > report_debounce_time) {
        last_report_time = now;
        uint8_t current = pulse_count;
        pulse_count = 0;

        if (current > 0) {
            Serial.printf("Pulses/s: %d \n", current);
        }
    }
}

void serial_monitor_init() {
    Serial.begin(115200);
    delay(1000);
}

void IRAM_ATTR button_press_ISR() {
    uint32_t now = millis();

    if (now - last_interrupt_time > button_debounce_time) {
        last_interrupt_time = now;

        pulse_count++;
    }
}