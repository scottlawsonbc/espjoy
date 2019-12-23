#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>

// Battery sense circuit:
//   VIN
//    |
//    R1 
//    |____VADC
//    |
//    R2
//    |
//   GND

struct battery {
  int pin;
  float R1; /* Resistance in Ohms */
  float R2; /* Resistance in Ohms */
  float state;
  float prevState;
};

void initBattery(battery* b) {
  pinMode(b->pin, INPUT);
}

void readBattery(battery* b) {
  const static int n = 16;
  const static int resolution = 4096;
  const static float attenuation = 3.6;

  int raw = analogRead(b->pin);
  for (int i = 0; i < n; i++) {
    raw += analogRead(b->pin);    
  }

  float avg = float(raw) / float(n);
  float val = avg * attenuation * (b->R1 + b->R2) / (b->R2 * resolution);
  val = roundf(val * 100) / 100;
  b->prevState = b->state;
  b->state = val;
}

#endif