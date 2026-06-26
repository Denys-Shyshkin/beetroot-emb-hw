/**
 *  Модуль 2.5.  
 *  Домашнє завдання
 *  Таймери
 */
#include <Arduino.h>
#include <esp_task_wdt.h>

constexpr uint8_t FAN_CONTROL = 4;
constexpr uint8_t FAN_ON_LED = 5;
constexpr uint8_t SYSTEM_ON_LED = 6;

constexpr uint32_t FAN_ON_TIME = 15 * 60 * 1000 * 1000; // min * sec * millisec * microsec
constexpr uint32_t FAN_OFF_TIME = 60U * 60 * 1000 * 1000; // min * sec * millisec * microsec

hw_timer_t *timer = NULL;

volatile bool fan_state = 0;
bool last_fan_state = 0;

void serial_monitor_init();
void IRAM_ATTR toggle_fan_state();

void setup() {
    serial_monitor_init();

    pinMode(FAN_CONTROL, OUTPUT);
    pinMode(FAN_ON_LED, OUTPUT);
    pinMode(SYSTEM_ON_LED, OUTPUT);

    digitalWrite(SYSTEM_ON_LED, HIGH);

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &toggle_fan_state, true);
    timerAlarmWrite(timer, FAN_OFF_TIME, true);
    timerAlarmEnable(timer);

    // watchdog setup, reload MCU if stuck for more than 2 seconds
    esp_task_wdt_init(2, true);
    esp_task_wdt_add(NULL);
}

void loop() {
    // watchdog reset during normal operation
    esp_task_wdt_reset();

    bool current_fan_state = fan_state;
    
    if (current_fan_state != last_fan_state) {
        uint32_t now = millis();
        timerAlarmWrite(timer, current_fan_state ? FAN_ON_TIME : FAN_OFF_TIME, true);

        digitalWrite(FAN_ON_LED, current_fan_state);
        digitalWrite(FAN_CONTROL, current_fan_state);

        Serial.printf("Time since start: %d mins | FAN state: %s\n", now / 60000,
                      current_fan_state ? "ON" : "OFF");

        last_fan_state = current_fan_state;
    }
}

void serial_monitor_init() {
    Serial.begin(115200);
    delay(1000);
}

void IRAM_ATTR toggle_fan_state() { 
    fan_state = !fan_state; 
}
