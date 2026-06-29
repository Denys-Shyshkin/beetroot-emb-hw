#include <Arduino.h>
#include <esp_task_wdt.h>

constexpr uint8_t BUTTON = 4;
constexpr uint8_t LED = 5;

volatile uint8_t counter = 0;
volatile bool led_state = 0;

hw_timer_t *timer_led = NULL;
hw_timer_t *timer_counter = NULL;

uint32_t last_serial_print = 0;
uint16_t serial_print_delay = 2000;

void serial_monitor_init();
void panic_function();
void IRAM_ATTR led_on_ISR();
void IRAM_ATTR counter_ISR();

void setup() {
    serial_monitor_init();

    pinMode(BUTTON, INPUT_PULLUP);
    pinMode(LED, OUTPUT);

    attachInterrupt(BUTTON, counter_ISR, FALLING);

    timer_led = timerBegin(0, 80, true);
    timerAttachInterrupt(timer_led, &led_on_ISR, true);
    timerAlarmWrite(timer_led, 500000, true);
    timerAlarmEnable(timer_led);

    timer_counter = timerBegin(1, 80, true);
    timerAttachInterrupt(timer_counter, &counter_ISR, true);
    timerAlarmWrite(timer_counter, 1000000, true);
    timerAlarmEnable(timer_counter);

    esp_task_wdt_init(5, false);
    esp_task_wdt_add(NULL);
}

void loop() {
    esp_task_wdt_reset();

    uint32_t now = millis();

    if (led_state) {
        led_state = false;
        digitalWrite(LED, !digitalRead(LED));
    }

    if (now - last_serial_print >= serial_print_delay) {
        last_serial_print = now;

        uint8_t snapshot = counter;

        Serial.println(snapshot);
    }

    if (counter > 10) {
        while(true) {
            Serial.println("STUCK!");
        }
    }
}

void serial_monitor_init() {
    Serial.begin(115200);
    delay(1000);
}

void IRAM_ATTR led_on_ISR() {
    led_state = true;
}

void IRAM_ATTR counter_ISR() {
    if (digitalRead(BUTTON) == LOW) {
        counter++;
    }
}
