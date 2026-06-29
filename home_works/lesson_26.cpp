/**
 *  Урок №26.  
 *  Домашнє завдання
 *  Міні-проєкт: Світлофор
 */
#include <Arduino.h>

constexpr uint8_t BUTTON_PIN = 7;
constexpr uint8_t RED_LED_PIN = 6;
constexpr uint8_t YELLOW_LED_PIN = 5;
constexpr uint8_t GREEN_LED_PIN = 4;

constexpr uint32_t LIGHT_DELAY_MICROSEC = 2000 * 1000;
constexpr uint16_t PRINT_DELAY = 50;
constexpr uint16_t BLINK_DELAY = 500;
constexpr uint16_t BUTTON_DELAY = 50;

uint32_t last_print = 0;
uint32_t last_green_light_blink = 0;
uint32_t last_yellow_light_blink = 0;

volatile uint32_t last_button_press = 0;

bool current_green_blink_state = 1;
bool current_yellow_blink_state = 1;

bool current_button_state = 1;
bool last_button_state = 1;

hw_timer_t *timer = NULL;

class Led {
  private:
    uint8_t pin;

  public:
    Led(uint8_t led_pin) { pin = led_pin; }

    void init() { pinMode(pin, OUTPUT); }

    void update_state(uint8_t state) { digitalWrite(pin, state); }
};

Led red_led(RED_LED_PIN);
Led yellow_led(YELLOW_LED_PIN);
Led green_led(GREEN_LED_PIN);

class Button {
  private:
    uint8_t pin;

  public:
    Button(uint8_t led_pin) { pin = led_pin; }

    void init() { pinMode(pin, INPUT_PULLUP); }

    bool read_state() { return digitalRead(pin); }
};

Button change_mode_button(BUTTON_PIN);

enum LIGHT_STATE {
    GREEN,
    GREEN_BLINK,
    YELLOW,
    RED,
    RED_YELLOW,
    YELLOW_BLINK,
};

constexpr uint8_t total_light_states = LIGHT_STATE::YELLOW_BLINK;
const uint32_t light_delays[total_light_states] = {15000000, 3000000, 3000000, 15000000,
                                                   2000000}; // in microsec

volatile uint8_t current_light_state = LIGHT_STATE::GREEN;
uint8_t last_light_state = LIGHT_STATE::GREEN;

void serial_monitor_init();
void IRAM_ATTR update_light_state();
void reset_lights();
void green_light();
void green_blink_light(uint32_t now);
void yellow_light();
void red_light();
void red_yellow_light();
void yellow_blink(uint32_t now);
void button_reading(uint32_t now);
void print_state(uint32_t now, uint8_t light_state);

void setup() {
    serial_monitor_init();

    green_led.init();
    yellow_led.init();
    red_led.init();
    change_mode_button.init();

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &update_light_state, true);
    timerAlarmWrite(timer, light_delays[0], true);
    timerAlarmEnable(timer);
}

void loop() {
    uint32_t now = millis();

    button_reading(now);

    uint8_t light_state = current_light_state;

    if (light_state != last_light_state) {
        reset_lights();
        timerAlarmWrite(timer, light_delays[light_state], true);
    }

    switch (light_state) {
    case LIGHT_STATE::GREEN:
        green_light();
        break;
    case LIGHT_STATE::GREEN_BLINK:
        green_blink_light(now);
        break;
    case LIGHT_STATE::YELLOW:
        yellow_light();
        break;
    case LIGHT_STATE::RED:
        red_light();
        break;
    case LIGHT_STATE::RED_YELLOW:
        red_yellow_light();
        break;
    case LIGHT_STATE::YELLOW_BLINK:
        yellow_blink(now);
        break;

    default:
        yellow_blink(now);
        break;
    }

    print_state(now, light_state);
}

void serial_monitor_init() {
    Serial.begin(115200);
    delay(1000);
}

void IRAM_ATTR update_light_state() {
    if (current_light_state == LIGHT_STATE::RED_YELLOW) {
        current_light_state = LIGHT_STATE::GREEN;
    } else {
        current_light_state++;
    }
}

void reset_lights() {
    red_led.update_state(0);
    yellow_led.update_state(0);
    green_led.update_state(0);
}

void green_light() { green_led.update_state(1); }

void green_blink_light(uint32_t now) {
    if (now - last_green_light_blink >= BLINK_DELAY) {
        last_green_light_blink = now;

        current_green_blink_state = !current_green_blink_state;

        green_led.update_state(current_green_blink_state);
    }
}

void yellow_light() { yellow_led.update_state(1); }

void red_light() { red_led.update_state(1); }

void red_yellow_light() {
    red_light();
    yellow_light();
}

void yellow_blink(uint32_t now) {
    if (now - last_yellow_light_blink >= BLINK_DELAY) {
        last_yellow_light_blink = now;

        current_yellow_blink_state = !current_yellow_blink_state;

        yellow_led.update_state(current_yellow_blink_state);
    }
}

void button_reading(uint32_t now) {
    bool reading_button_state = change_mode_button.read_state();

    if (reading_button_state != last_button_state) {
        last_button_press = now;
    }
    if (now - last_button_press >= BUTTON_DELAY) {
        if (current_button_state != reading_button_state) {
            current_button_state = reading_button_state;

            if (current_button_state == 0) {

                if (current_light_state == LIGHT_STATE::YELLOW_BLINK) {
                    current_light_state = LIGHT_STATE::YELLOW;

                    timerWrite(timer, 0);
                    timerAlarmWrite(timer, light_delays[LIGHT_STATE::YELLOW], true);
                    timerAlarmEnable(timer);
                } else {
                    current_light_state = LIGHT_STATE::YELLOW_BLINK;

                    timerAlarmDisable(timer);
                }
            }
        }
    }

    last_button_state = reading_button_state;
}

void print_state(uint32_t now, uint8_t light_state) {
    if (now - last_print >= PRINT_DELAY) {
        last_print = now;

        Serial.println(light_state);
    }

    last_light_state = light_state;
}
