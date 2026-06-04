/**
 *  Модуль 1.3.
 *  Домашнє завдання
 *  Поліцейська мигалка
 */
#include <Arduino.h>

#define LED_RED 4
#define LED_WHITE 5
#define LED_BLUE 6

#define DELAY_TIME_1 1000
#define DELAY_TIME_2 200
#define DELAY_TIME_3 100

#define REPEAT_TIMES 3

void init_logs();

void red_on();
void white_on();
void blue_on();

void all_leds_on();
void all_leds_off();

void blue_red_pattern(int time, int repeat);
void all_lights_flash(int repeat);

void setup() {
  init_logs();

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_WHITE, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
}

void loop() {
  blue_red_pattern(DELAY_TIME_1, 3);
  
  blue_red_pattern(DELAY_TIME_2, 3);

  all_lights_flash(5);
  
  delay(DELAY_TIME_2);
}

void init_logs() {
  Serial.begin(115200);
  delay(1000);
}

void red_on() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_WHITE, LOW);
  digitalWrite(LED_BLUE, LOW);

  Serial.println("Red LED is ON");
}

void white_on() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_WHITE, HIGH);
  digitalWrite(LED_BLUE, LOW);

  Serial.println("White LED is ON");
}

void blue_on() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_WHITE, LOW);
  digitalWrite(LED_BLUE, HIGH);

  Serial.println("Blue LED is ON");
}

void all_leds_on() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_WHITE, HIGH);
  digitalWrite(LED_BLUE, HIGH);

  Serial.println("All LEDs are ON");
}

void all_leds_off() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_WHITE, LOW);
  digitalWrite(LED_BLUE, LOW);
}

void blue_red_pattern(int time, int repeat) {
  for (int i = 0; i < repeat; i++) {
    blue_on();
    delay(time);
    red_on();
    delay(time);
  }
}

void all_lights_flash(int repeat) {
  for (int i = 0; i < repeat; i++) {
    all_leds_on();
    delay(DELAY_TIME_3);
    all_leds_off();
    delay(DELAY_TIME_3);
  }
}