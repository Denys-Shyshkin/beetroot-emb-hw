/**
 *  Модуль 2.3.  
 *  Домашнє завдання
 *  Superloop та незалежні задачі
 */
#include <Arduino.h>

constexpr uint8_t GREEN_LED = 4;
constexpr uint8_t YELLOW_LED = 5;
constexpr uint8_t BLUE_LED = 6;

constexpr uint16_t DELAY_GREEN = 200;
constexpr uint16_t DELAY_YELLOW = 500;
constexpr uint16_t DELAY_BLUE = 1000;

class Led {
  private:
    uint8_t pin;
    bool current_state = 0;
    uint32_t last_state_change = 0;
    uint32_t delay_time;

  public:
    Led(uint8_t led_pin, uint32_t led_delay_time) {
        pin = led_pin;
        delay_time = led_delay_time;
    }

    void init() { 
        pinMode(pin, OUTPUT);
    }

    void update_state() {
        uint32_t now = millis();

        if (now - last_state_change > delay_time) {
            last_state_change = now;
            current_state = !current_state;
            
            digitalWrite(pin, current_state);
        }
    }
};

Led green_led(GREEN_LED, DELAY_GREEN);
Led yellow_led(YELLOW_LED, DELAY_YELLOW);
Led blue_led(BLUE_LED, DELAY_BLUE);

void setup() {
    green_led.init();
    yellow_led.init();
    blue_led.init();
}

void loop() {
    green_led.update_state();
    yellow_led.update_state();
    blue_led.update_state();
}