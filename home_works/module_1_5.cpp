/**
 *  Модуль 1.5.  
 *  Домашнє завдання
 *  Дослідження брязкоту контактів кнопки за допомогою логічного аналізатора та мікроконтролера
 */ 
#include <Arduino.h>

#define BUTTON 4

int button_state = HIGH;
int last_button_state = HIGH;

int button_press_count = 0;

unsigned long last_debounce_time = 0;
unsigned long debounce_delay = 10;

void init_logs();

void setup() {
  pinMode(BUTTON, INPUT);

  init_logs();
}

void loop() {
  int reading = digitalRead(BUTTON);

  if (reading != last_button_state) {
    last_debounce_time = millis();
  }

  if ((millis() - last_debounce_time) > debounce_delay) {
    if (reading != button_state) {
      button_state = reading;

      if (button_state == LOW) {
        button_press_count++;
      }
    }
  }

  Serial.println(button_press_count);

  last_button_state = reading;
}

void init_logs() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Setup complete");
}