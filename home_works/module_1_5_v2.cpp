#include <Arduino.h>

#define BUTTON 4

int button_state = HIGH;
int measured_button_state = HIGH;

int button_press_count = 0;

unsigned long last_debounce_time = 0;
unsigned long debounce_delay = 20;

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(BUTTON, INPUT);
}

void loop() {
  int reading = digitalRead(BUTTON);

  if (reading != measured_button_state) {
    measured_button_state = reading;
    last_debounce_time = millis();
  }

  if ((millis() - last_debounce_time) > debounce_delay) {
    if (measured_button_state != button_state) {
      button_state = measured_button_state;

      if (button_state == LOW) {
        button_press_count++;
      }
    }
  }

  Serial.println(button_press_count);
}