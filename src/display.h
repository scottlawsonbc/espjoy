#ifndef DISPLAY_H
#define DISPLAY_H

#include <U8x8lib.h>
#include "button.h"
#include "joystick.h"
#include "battery.h"

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

void initDisplay() {
  u8x8.begin();
  u8x8.setFlipMode(1);
  u8x8.setFont(u8x8_font_8x13B_1x2_f);
}

void drawButton(button* btn, uint8_t x, uint8_t y) {
  u8x8.setCursor(x, y);
  u8x8.printf("%i", btn->state);
}

void drawJoystick(joystick* axis, uint8_t x, uint8_t y) {
  u8x8.setCursor(x, y);
  u8x8.printf("%4i", axis->state);
}

void drawBattery(battery* b, uint8_t x, uint8_t y) {
  u8x8.setCursor(x, y);
  u8x8.printf("%4.2f", b->state);
}

#endif