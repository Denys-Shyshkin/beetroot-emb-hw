/**
 *  Модуль 2.1.  
 *  Домашнє завдання
 *  Embedded C++ на ESP32
 */
#include <Arduino.h>

constexpr uint8_t GREEN_LED_PIN = 4;
constexpr uint8_t BUTTON_PIN = 5;
constexpr uint16_t BLINK_DELAY = 500;

uint32_t last_blink_time = 0;
uint32_t last_iteration_time = 0;
uint32_t last_button_press_time = 0;
uint32_t iteration_count = 0;
bool blinking_led_state = 0;

enum class Led_State {
  OFF,
  ON,
  BLINKING,
};

Led_State current_led_state = Led_State::OFF;

enum class Button_State {
  PRESSED,
  RELEASED,
};

volatile Button_State current_button_state = Button_State::RELEASED;

class Led {
  private:
    uint8_t pin;

  public:
    Led(uint8_t ledPin) {
      pin = ledPin;
    }

    void init() {
      pinMode(pin, OUTPUT);
    }

    void set(bool state) {
      digitalWrite(pin, state);
    }
};

Led green_led(GREEN_LED_PIN);

class Button {
  private:
    uint8_t pin;

  public:
    Button(uint8_t ledPin) {
      pin = ledPin;
    }

    void init() {
      pinMode(pin, INPUT_PULLUP);
    }
};

Button button(BUTTON_PIN);

void serial_monitor_init();
void print_iteration_info(uint32_t start_time);
void IRAM_ATTR button_state_ISR();
void process_button_press();
void led_state_handler();
void led_blinking();

void setup() {
  serial_monitor_init();

  green_led.init();
  button.init();

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), button_state_ISR, FALLING);
}

void loop() {
  uint32_t iteration_start_time = micros();
  iteration_count++;

  process_button_press();
  led_state_handler();

  print_iteration_info(iteration_start_time);
}

void serial_monitor_init() {
  Serial.begin(115200);
  delay(1000);
}

void IRAM_ATTR button_state_ISR() {
  current_button_state = Button_State::PRESSED;
}

void process_button_press() {
  if (current_button_state == Button_State::PRESSED) {
    switch (current_led_state){
      case Led_State::ON:
        current_led_state = Led_State::OFF;
        break;
      case Led_State::OFF:
        current_led_state = Led_State::BLINKING;
        break;
      default:
        current_led_state = Led_State::ON;
        break;
    }

    current_button_state = Button_State::RELEASED;
  }
}

void led_state_handler() {
  if (current_led_state == Led_State::BLINKING) {
    led_blinking();
  } else {
    green_led.set((bool)current_led_state);
  }
}

void led_blinking() {
  if (millis() - last_blink_time > BLINK_DELAY) {
    last_blink_time = millis();
    blinking_led_state = !blinking_led_state;

    green_led.set(blinking_led_state);
  }
}

void print_iteration_info(uint32_t start_time) {
  static const uint16_t output_iterations_count = 1000;

  if (iteration_count % output_iterations_count == 0) {
    Serial.print("Iteration count: ");
    Serial.print(iteration_count);

    Serial.print(" | Time of this iteration in micro sec: ");
    Serial.println(micros() - start_time);
  }
}