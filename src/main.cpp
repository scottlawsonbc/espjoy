#include <Arduino.h>

#include "display.h"
#include "button.h"
#include "joystick.h"
#include "battery.h"
#include "ble.h"

button btn0 = { .pin = 18 };
button btn1 = { .pin = 23 };
button btn2 = { .pin = 19 };
button btn3 = { .pin = 22 };
button btn4 = { .pin = 21 };
button* btn[] = { &btn0, &btn1, &btn2, &btn3, &btn4 };
const int N_BUTTONS = sizeof(btn) / sizeof(btn[0]);

joystick axisX = { .pin = 34, .min = -445, .max = 495 };
joystick axisY = { .pin = 37, .min = -430, .max = 485 };
joystick axisZ = { .pin = 38, .min = -680, .max = 730 };
joystick* axis[] = { &axisX, &axisY, &axisZ };
const int N_JOYSTICK_AXES = sizeof(axis) / sizeof(axis[0]);

battery bat = { .pin = 35, .R1 = 10e3, .R2 = 10e3 };

void setup() {
  Serial.begin(115200);
  initDisplay();
  initBattery(&bat);
  drawBattery(&bat, 10, 0);

  for (int i = 0; i < N_BUTTONS; i++) {
    initButton(btn[i]);
    readButton(btn[i]);
    drawButton(btn[i], i, 0);
  }

  for (int i = 0; i < N_JOYSTICK_AXES; i++) {
    initJoystick(axis[i]);
    readButton(btn[i]);
    drawJoystick(axis[i], 4*i, 2);
  }

  initBLE();
}

void loop() {
  readBattery(&bat);
  if (bat.state != bat.prevState) {
    drawBattery(&bat, 10, 0);
  }

  for (int i = 0; i < N_BUTTONS; i++) {
    readButton(btn[i]);
  }
  
  for (int i = 0; i < N_JOYSTICK_AXES; i++) {
    readJoystick(axis[i]);
  }
  applyDeadzone(axis[0], axis[1], 20);

  bool stateChange;

  for (int i = 0; i < N_BUTTONS; i++) {
    if (btn[i]->state != btn[i]->prevState) {
      stateChange = true;
      drawButton(btn[i], i, 0);
      Serial.printf("%lu\tbtn %i\t%i\n", millis(), i, btn[i]->state);
    }
  }

  for (int i = 0; i < N_JOYSTICK_AXES; i++) {
    if (axis[i]->state != axis[i]->prevState) {
      stateChange = true;
      drawJoystick(axis[i], 4*i, 2);
      Serial.printf("%lu\taxis %i\t%i\n", millis(), i, axis[i]->state);
    }
  }

  // Send BLE update.
  if (connected && stateChange) {
    int8_t x = axis[0]->state;
    int8_t y = axis[1]->state;
    int8_t z = axis[2]->state;
    uint8_t b = 0;
    for (int i = 0; i < N_BUTTONS; i++) {
      b |= btn[i]->state << i;
    }
    uint8_t a[] = {x, y, z, b};
    input->setValue(a, sizeof(a));
    input->notify();
  }

  delay(5);
}