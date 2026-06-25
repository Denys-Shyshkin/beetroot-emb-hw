#include <Arduino.h>

constexpr uint8_t BUTTON_1 = 4;
constexpr uint8_t BUTTON_2 = 6;
constexpr uint8_t LED_1 = 5;
constexpr uint8_t LED_2 = 7;

hw_timer_t *timer = NULL;
volatile bool led_1_state = false;
volatile bool is_alarm = false;

void serial_monitor_init();
void IRAM_ATTR led_1_timer_ISR();
void IRAM_ATTR button_1_press_ISR();
void IRAM_ATTR button_2_press_ISR();

void setup() {
    serial_monitor_init();

    pinMode(BUTTON_1, INPUT_PULLUP);
    pinMode(BUTTON_2, INPUT_PULLUP);
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &led_1_timer_ISR, true);
    timerAlarmWrite(timer, 500000, true);
    timerAlarmEnable(timer);

    attachInterrupt(BUTTON_2, button_2_press_ISR, FALLING);
    attachInterrupt(BUTTON_1, button_1_press_ISR, FALLING);
}

void loop() {
    if (is_alarm) {
        digitalWrite(LED_2, is_alarm);
        digitalWrite(LED_1, !is_alarm);
        detachInterrupt(BUTTON_1);
        while(1) {
            delay(20);
        }
    } else {
        digitalWrite(LED_2, is_alarm);
    }
}

void serial_monitor_init() {
    Serial.begin(115200);
    delay(1000);
}

void IRAM_ATTR led_1_timer_ISR() {
    led_1_state = !led_1_state;
    digitalWrite(LED_1, led_1_state);
}

void IRAM_ATTR button_1_press_ISR() {
    timerAlarmEnable(timer);
    is_alarm = false; 
}

void IRAM_ATTR button_2_press_ISR() { 
    timerAlarmDisable(timer);
    is_alarm = true; 
}