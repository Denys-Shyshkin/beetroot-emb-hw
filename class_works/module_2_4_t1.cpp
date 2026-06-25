#include <Arduino.h>

constexpr uint8_t BUTTON = 4;
constexpr uint8_t LED = 5;

volatile bool toggle_flag = false;

uint32_t last_interrupt_time = 0;
uint8_t debounce_time = 50;

void serial_monitor_init();
void IRAM_ATTR button_press_ISR();

void setup() {
    serial_monitor_init();

    pinMode(BUTTON, INPUT_PULLUP);
    pinMode(LED, OUTPUT);

    attachInterrupt(BUTTON, button_press_ISR, FALLING);
}

void loop() {
    if (toggle_flag) {
        digitalWrite(LED, !digitalRead(LED));
        toggle_flag = false;
    }
}

void serial_monitor_init() {
    Serial.begin(115200);
    delay(1000);
}

void IRAM_ATTR button_press_ISR() {
    uint32_t now = millis();

    if (now - last_interrupt_time > debounce_time) {
        last_interrupt_time = now;
        toggle_flag = true;
    }
}