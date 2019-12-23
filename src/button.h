#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

struct button {
  int pin;
  int state;
  int prevState;
};

void initButton(button* btn) {
  pinMode(btn->pin, INPUT_PULLUP);
  delay(5);
}

void readButton(button* btn) {
  btn->prevState = btn->state;
  btn->state = !digitalRead(btn->pin);
}

#endif