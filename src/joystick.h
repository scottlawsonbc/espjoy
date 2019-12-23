#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <Arduino.h>

struct joystick {
  int pin;
  int min;
  int max;
  int zero;
  int state;
  int prevState;
};

void initJoystick(joystick* axis) {
  pinMode(axis->pin, INPUT);
  const int n = 32;
  int offset = 0;
  for (int i = 0; i < n; i++) {
    delay(10);
    offset += analogRead(axis->pin);
  }
  offset /= n;
  axis->zero = offset;
}

void readJoystick(joystick* axis) {
  int val = 0;
  int n = 64;
  for (int i = 0; i < n; i++) {
    val += analogRead(axis->pin);
  }
  val -= n * axis->zero;
  val /= n;

  if (val > 0) {
    val = map(val, 0, axis->max, 0, 127);
  } else if (val < 0) {
    val = map(val, axis->min, 0, -127, 0);
  }
  val = min(val, 127);
  val = max(val, -127);

  axis->prevState = axis->state;
  axis->state = val;
}

void applyDeadzone(joystick* x, joystick* y, int threshold) {
  int dist = x->state*x->state + y->state*y->state;
  if (dist < threshold) {
    x->state = 0;
    y->state = 0;
  }
}

#endif