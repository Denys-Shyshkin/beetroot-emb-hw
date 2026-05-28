/* 
 *  Модуль 1.4. 
 *  Домашнє завдання
 *  Проєкт: Два світлодіоди, зовнішня кнопка та BOOT
 */

#include <Arduino.h>

// ===== Constants declaration =====
#define GREEN_LED 4
#define BLUE_LED 5

#define EXT_BUTTON 2
#define INT_BUTTON 0

#define DELAY_TIME_BASE 50
#define MAX_MODS_QTY 2

// ===== Global variables declaration =====
int current_mode = 1;
int prev_mode = 0;

// ===== Functions declaration =====
void init_logs();
void mode_logging();
void leds_blink(int delay_time);
char* get_mode_name(int current_mode);

// ===== Pins initialization and logs setup =====
void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  pinMode(EXT_BUTTON, INPUT_PULLUP);
  pinMode(INT_BUTTON, INPUT_PULLUP);

  init_logs();
}

// ===== Main loop =====
void loop() {
  int ext_button_state = digitalRead(EXT_BUTTON);
  int int_button_state = digitalRead(INT_BUTTON);

  if (!ext_button_state && !int_button_state) {
    current_mode = MAX_MODS_QTY + 1;
  } else if (current_mode > 1 && !ext_button_state) {
    current_mode--;
  } else if (current_mode < MAX_MODS_QTY && !int_button_state) {
    current_mode++;
  }
  
  mode_logging();
  leds_blink(DELAY_TIME_BASE * current_mode);
  
  delay(50);
}

// ===== Functions definition =====
void init_logs() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Setup complete");
}

void mode_logging() {
  if (current_mode != prev_mode) {
    Serial.print("Current mode: ");
    Serial.println(get_mode_name(current_mode));

    prev_mode = current_mode;
  }
}

void leds_blink(int delay_time) {
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, LOW);
  delay(delay_time);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, HIGH);
  delay(delay_time);
}

char* get_mode_name(int current_mode) {
  switch(current_mode) {
    case 1: return "FAST";
    case 2: return "SLOW";
    case 3: return "EXTRA";
    default: return "";
  }
}