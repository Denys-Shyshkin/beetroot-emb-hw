/**
 *  Модуль 1.6.  
 *  Домашнє завдання
 *  Вимірювання освітленості за допомогою LDR та ESP32-S3
 */
#include <Arduino.h>
#include <cmath>

#define LDR_PIN 4
#define RESOLUTION 12

const adc_attenuation_t attenuation = ADC_11db;
const int max_reading_value = std::pow(2, RESOLUTION);

float get_max_voltage(adc_attenuation_t attenuation);

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LDR_PIN, INPUT);

  analogReadResolution(RESOLUTION);
  analogSetPinAttenuation(LDR_PIN, attenuation);
}

void loop() {
  int raw_reading = analogRead(LDR_PIN);

  float voltage_reading = analogReadMilliVolts(LDR_PIN) / (float) 1000;
  float voltage_calc = ((float) raw_reading / max_reading_value) * get_max_voltage(attenuation);

  int deviation_percentage = ((voltage_reading - voltage_calc) / voltage_reading) * 100;
  
  Serial.printf(
    "Raw: %d | Reading: %.2f V | Calculated: %.2f V | Deviation: %d %% \n", 
    raw_reading, voltage_reading, voltage_calc, deviation_percentage
  );

  delay(100);
}

float get_max_voltage(adc_attenuation_t attenuation) {
    switch(attenuation) {
        case ADC_0db:   return 0.97;
        case ADC_2_5db: return 1.29;
        case ADC_6db:   return 1.81;
        case ADC_11db:  return 3.14;
        default:        return 3.30;
    }
}