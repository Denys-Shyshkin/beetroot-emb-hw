/**
 *  Урок №17.  
 *  Домашнє завдання
 *  Міні-проєкт
 */
#include <Arduino.h>

// Defining macro
#define RELAY 4
#define BLUE_LED 5
#define RED_LED 6
#define THERMISTOR 7
#define THRESHOLD 1000

// Defining variables
unsigned long last_time = 0;
unsigned long delay_time = 200;

int red_led_state = HIGH;

// Functions declaration
void led_alarm_on();
void led_alarm_off();

void setup() {
  // Serial monitor setup
  Serial.begin(115200);
  delay(1000);

  // All pin modes setup
  pinMode(RELAY, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(THERMISTOR, INPUT);

  // Initial states setup
  digitalWrite(RELAY, HIGH);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(RED_LED, LOW);
}

void loop() {
  // Reading raw data from the pin
  int raw_reading = analogRead(THERMISTOR);

  // Reading voltage value from the pin
  float voltage_reading = analogReadMilliVolts(THERMISTOR) / (float) 1000;

  // Raw data and voltage readings output to serial monitor
  Serial.printf(
    "Raw: %d | Reading: %.2f V \n", 
    raw_reading, voltage_reading
  );

  // Main logic block - turn off relay and turn alarm on when temperature readings reaches certain threshold value
  if (raw_reading < THRESHOLD) {
    led_alarm_on();
    digitalWrite(RELAY, LOW);
  } else {
    led_alarm_off();
    digitalWrite(RELAY, HIGH);
  }

  delay(50);
}

// Functions definition
void led_alarm_on() {
  if ((millis() - last_time) > delay_time) {
    last_time = millis();

    if (red_led_state) {
      red_led_state = LOW;

      digitalWrite(BLUE_LED, HIGH);
      digitalWrite(RED_LED, red_led_state);
    } else {
      red_led_state = HIGH;
      
      digitalWrite(BLUE_LED, LOW);
      digitalWrite(RED_LED, red_led_state);
    }
  }
}

void led_alarm_off() {
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(RED_LED, LOW);
}