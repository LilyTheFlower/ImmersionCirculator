#include "OLED_Driver.h"
#include "GUI_paint.h"
#include "DEV_Config.h"
#include "Debug.h"
#ifndef ARDUINO_ARCH_AVR
#include <Arduino.h>
#endif
void drawTargetTemp(int targetTemp, int ForC);
void drawCurrentTemp(int CurrentTemp, int ForC);
void drawTimer(int hours, int minutes);
void drawWattage();

void DrawNumber(int x, int y, int h, int w, int num);
void DrawCharFromHex(int x, int y, int h, int w, const uint8_t Arr[]);
const uint8_t zero [] PROGMEM = {
  0x0, 0x0, 0x0, 
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x80, 0xb8, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xff, 0x1,
0x80, 0xff, 0x3,
0xc0, 0xff, 0x7,
0xc0, 0xff, 0x7,
0xe0, 0xc7, 0xf,
0xe0, 0x83, 0xf,
0xf0, 0x83, 0x1f,
0xf0, 0x83, 0x1f,
0xf0, 0x1, 0x1f,
0xf8, 0x1, 0x3f,
0xf8, 0x1, 0x3f,
0xf8, 0x1, 0x3f,
0xf8, 0x0, 0x3e,
0xf8, 0x0, 0x3e,
0xf8, 0x0, 0x3e,
0xf8, 0x0, 0x3e,
0xf8, 0x0, 0x3e,
0xf8, 0x0, 0x3e, 
0xf8, 0x0, 0x3e,
0xf8, 0x1, 0x3f,
0xf8, 0x1, 0x3f,
0xf8, 0x1, 0x3f,
0xf0, 0x1, 0x1f,
0xf0, 0x83, 0x1f,
0xf0, 0x83, 0x1f,
0xe0, 0x83, 0xf,
0xe0, 0xc7, 0xf,
0xc0, 0xff, 0x7,
0xc0, 0xff, 0x7,
0x80, 0xff, 0x3,
0x0, 0xff, 0x1,
0x0, 0xfe, 0x0,
0x0, 0x38, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0
};

const uint8_t one [] PROGMEM = {
  0x0, 0x0, 0x0, 
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0xf0, 0x0,
0x0, 0xf8, 0x0,
0x0, 0xfc, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xff, 0x0,
0x80, 0xff, 0x0,
0xc0, 0xff, 0x0,
0xe0, 0xff, 0x0,
0xf0, 0xff, 0x0,
0xf8, 0xff, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0, 
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xfe, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0
};

const uint8_t two [] PROGMEM = {
  0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 
0x0, 0xfc, 0x0,
0x0, 0xfe, 0x1,
0x0, 0xff, 0x3,
0x80, 0xff, 0x7,
0xc0, 0xff, 0xf,
0xc0, 0xff, 0xf,
0xe0, 0x87, 0xf,
0xe0, 0x83, 0xf,
0xe0, 0x81, 0xf,
0xe0, 0xc1, 0xf,
0x0, 0xc0, 0xf,
0x0, 0xe0, 0x7,
0x0, 0xe0, 0x7,
0x0, 0xf0, 0x3,
0x0, 0xf0, 0x3,
0x0, 0xf8, 0x1,
0x0, 0xf8, 0x1,
0x0, 0xfc, 0x0,
0x0, 0xfc, 0x0,
0x0, 0x7e, 0x0,
0x0, 0x7e, 0x0,
0x0, 0x3f, 0x0,
0x0, 0x3f, 0x0,
0x80, 0x1f, 0x0,
0x80, 0x1f, 0x0,
0xc0, 0xf, 0x0,
0xc0, 0xf, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0xff, 0x7,
0xe0, 0xff, 0x7,
0xe0, 0xff, 0x7,
0xe0, 0xff, 0x7,
0xe0, 0xff, 0x7,
0xe0, 0xff, 0x7, 
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0
};

const uint8_t three [] PROGMEM = {
  0x0, 0x0, 0x0, 
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x3c, 0x0,
0x0, 0xff, 0x0,
0xc0, 0xff, 0x1,
0xe0, 0xff, 0x3,
0xf0, 0xff, 0x7,
0xf0, 0xff, 0x7,
0xf8, 0xe3, 0xf,
0xf8, 0x80, 0xf,
0xf8, 0x80, 0xf,
0xf8, 0x80, 0xf,
0x0, 0xc0, 0xf,
0x0, 0xc0, 0x7,
0x0, 0xe0, 0x7, 
0x0, 0xe0, 0x3,
0x0, 0xf0, 0x3,
0x0, 0xf8, 0x1,
0x0, 0xfc, 0x0,
0x0, 0x7c, 0x0,
0x0, 0xfc, 0x0,
0x0, 0xf8, 0x1,
0x0, 0xf0, 0x3,
0x0, 0xe0, 0x3,
0x0, 0xe0, 0x7,
0x0, 0xc0, 0x7,
0x0, 0xc0, 0xf,
0xf8, 0x80, 0xf,
0xf8, 0x80, 0xf,
0xf8, 0x80, 0xf,
0xf8, 0x80, 0xf,
0xf0, 0xe3, 0x7,
0xf0, 0xff, 0x7,
0xe0, 0xff, 0x3,
0xc0, 0xff, 0x1,
0x80, 0xff, 0x0,
0x0, 0x7f, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0
};

const uint8_t four [] PROGMEM = {
  0x0, 0x0, 0x0, 
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x80, 0x3,
0x0, 0xc0, 0x3,
0x0, 0xc0, 0x3,
0x0, 0xe0, 0x3,
0x0, 0xf0, 0x3,
0x0, 0xf0, 0x1,
0x0, 0xf8, 0x0,
0x0, 0xfc, 0x0,
0x0, 0x7c, 0x0,
0x0, 0x3e, 0x0,
0x0, 0x3f, 0x0,
0x0, 0x1f, 0x0,
0x80, 0xf, 0x0,
0xc0, 0xef, 0x3,
0xc0, 0xe7, 0x3,
0xe0, 0xe3, 0x3,
0xf0, 0xe3, 0x3,
0xf0, 0xe1, 0x3,
0xf0, 0xe0, 0x3,
0xf0, 0xff, 0x1f,
0xf0, 0xff, 0x1f,
0xf0, 0xff, 0x1f,
0xf0, 0xff, 0x1f,
0x0, 0xe0, 0x3,
0x0, 0xe0, 0x3, 
0x0, 0xe0, 0x3,
0x0, 0xe0, 0x3,
0x0, 0xe0, 0x3,
0x0, 0xe0, 0x3,
0x0, 0xe0, 0x3,
0x0, 0xe0, 0x3,
0x0, 0xe0, 0x3,
0x0, 0xe0, 0x3,
0x0, 0xe0, 0x3,
0x0, 0xe0, 0x3,
0x0, 0xe0, 0x3,
0x0, 0xe0, 0x3,
0x0, 0xe0, 0x3,
0x0, 0xe0, 0x3,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0
};

const uint8_t five [] PROGMEM = {
  0x0, 0x0, 0x0, 
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0xf0, 0xff, 0x7,
0xf0, 0xff, 0x7,
0xf0, 0xff, 0x7,
0xf0, 0xff, 0x7,
0xf0, 0x0, 0x0,
0xf0, 0x0, 0x0,
0xf0, 0x0, 0x0,
0xf0, 0x0, 0x0,
0xf0, 0x0, 0x0,
0xf0, 0x0, 0x0,
0xf0, 0x0, 0x0,
0xf0, 0x0, 0x0,
0xf0, 0x0, 0x0,
0xf0, 0x7f, 0x0,
0xf0, 0xff, 0x0, 
0xf0, 0xff, 0x1,
0xf0, 0xff, 0x3,
0x0, 0xf0, 0x7,
0x0, 0xe0, 0x7,
0x0, 0xc0, 0xf,
0x0, 0x80, 0xf,
0x0, 0x0, 0xf,
0x0, 0x0, 0xf,
0x0, 0x0, 0xf,
0x0, 0x0, 0xf,
0x78, 0x0, 0xf,
0x78, 0x0, 0xf,
0xf8, 0x80, 0xf,
0xf8, 0x80, 0xf,
0xf0, 0xe3, 0x7,
0xf0, 0xff, 0x7,
0xe0, 0xff, 0x3,
0xc0, 0xff, 0x1,
0x80, 0xff, 0x0,
0x0, 0x7f, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0
};

const uint8_t six [] PROGMEM = {
  0x0, 0x0, 0x0, 
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0xf0, 0x1,
0x0, 0xf0, 0x1,
0x0, 0xf8, 0x0,
0x0, 0xf8, 0x0,
0x0, 0x7c, 0x0,
0x0, 0x7c, 0x0,
0x0, 0x3e, 0x0,
0x0, 0x3e, 0x0,
0x0, 0x1f, 0x0,
0x0, 0x1f, 0x0,
0x80, 0xf, 0x0, 
0x80, 0xf, 0x0,
0xc0, 0x7, 0x0,
0xc0, 0x7f, 0x0,
0xe0, 0xff, 0x0,
0xe0, 0xff, 0x1,
0xf0, 0xff, 0x3,
0xf0, 0xff, 0x7,
0xf8, 0xe3, 0x7,
0xf8, 0xc1, 0xf,
0xf8, 0x80, 0xf,
0x78, 0x0, 0xf,
0x78, 0x0, 0xf,
0x78, 0x0, 0xf,
0x78, 0x0, 0xf,
0x78, 0x0, 0xf,
0x78, 0x0, 0xf,
0xf8, 0x80, 0xf,
0xf8, 0x80, 0xf,
0xf0, 0xe3, 0x7,
0xf0, 0xff, 0x7,
0xe0, 0xff, 0x3,
0xc0, 0xff, 0x1,
0x80, 0xff, 0x0,
0x0, 0x7f, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0
};

const uint8_t seven [] PROGMEM = {
  0x0, 0x0, 0x0, 
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0xe0, 0xff, 0xf,
0xe0, 0xff, 0xf,
0xe0, 0xff, 0xf,
0xe0, 0xff, 0xf,
0xe0, 0xff, 0xf,
0xe0, 0xff, 0xf,
0x0, 0xc0, 0xf,
0x0, 0xc0, 0xf,
0x0, 0xe0, 0xf,
0x0, 0xe0, 0x7,
0x0, 0xf0, 0x7,
0x0, 0xf0, 0x3,
0x0, 0xf8, 0x3,
0x0, 0xf8, 0x1,
0x0, 0xfc, 0x1,
0x0, 0xfc, 0x0,
0x0, 0xfe, 0x0,
0x0, 0x7e, 0x0,
0x0, 0x7f, 0x0,
0x0, 0x3f, 0x0,
0x80, 0x3f, 0x0,
0x80, 0x1f, 0x0,
0xc0, 0x1f, 0x0,
0xc0, 0xf, 0x0,
0xe0, 0xf, 0x0,
0xe0, 0x7, 0x0,
0xf0, 0x7, 0x0,
0xf0, 0x3, 0x0,
0xf0, 0x3, 0x0,
0xf0, 0x1, 0x0,
0xf0, 0x1, 0x0,
0xf0, 0x0, 0x0,
0xf0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0
};

const uint8_t eight [] PROGMEM = {
  0x0, 0x0, 0x0, 
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x80, 0xff, 0x0,
0xc0, 0xff, 0x1,
0xe0, 0xff, 0x3,
0xf0, 0xff, 0x7,
0xf8, 0xe3, 0x7,
0xf8, 0xd1, 0xf,
0xf8, 0x80, 0xf,
0x78, 0x0, 0xf,
0xf8, 0x80, 0xf,
0x78, 0x0, 0xf,
0x78, 0x0, 0xf, 
0x78, 0x0, 0xf,
0x78, 0x0, 0xf,
0xf0, 0x80, 0x7,
0xf0, 0xc1, 0x7,
0xe0, 0xe3, 0x13,
0xc0, 0xff, 0x11,
0x80, 0xff, 0x8,
0x80, 0xff, 0x8,
0xc0, 0xe3, 0x5,
0xe0, 0xc1, 0x3,
0xf0, 0x80, 0x7,
0x70, 0x0, 0x7,
0x78, 0x0, 0xf,
0x78, 0x0, 0xf,
0x78, 0x0, 0xf,
0x78, 0x0, 0xf,
0x78, 0x0, 0xf,
0x78, 0x0, 0xf,
0xf0, 0x80, 0x7,
0xf0, 0xc1, 0x7,
0xe0, 0xe3, 0x3,
0xc0, 0xff, 0x1,
0x80, 0xff, 0x0,
0x0, 0x7f, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0
};

const uint8_t nine [] PROGMEM = {
  0x0, 0x0, 0x0, 
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x80, 0xff, 0x0,
0xc0, 0xff, 0x1,
0xe0, 0xff, 0x3,
0xf0, 0xff, 0x7,
0xf8, 0xe3, 0x7,
0xf8, 0xd1, 0xf,
0xf8, 0x80, 0xf,
0x78, 0x0, 0xf,
0xf8, 0x80, 0xf,
0x78, 0x0, 0xf,
0x78, 0x0, 0xf,
0x78, 0x0, 0xf,
0xf8, 0x80, 0xf,
0xf0, 0xc1, 0xf,
0xf0, 0xe3, 0xf,
0xe0, 0xff, 0x1f,
0xc0, 0xff, 0x1f,
0x80, 0xff, 0x7,
0x0, 0xff, 0x7,
0x0, 0xc0, 0x7,
0x0, 0xc0, 0x3,
0x0, 0xe0, 0x3,
0x0, 0xe0, 0x3,
0x0, 0xf0, 0x1,
0x0, 0xf0, 0x1,
0x0, 0xf8, 0x0,
0x0, 0xf8, 0x0,
0x0, 0x7c, 0x0,
0x0, 0x7c, 0x0,
0x0, 0x3e, 0x0,
0x0, 0x3e, 0x0,
0x0, 0x1f, 0x0,
0x0, 0x1f, 0x0,
0x0, 0xf, 0x0,
0x0, 0xf, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 
0x0, 0x0, 0x0
};

const uint8_t C48 [] PROGMEM = {
  0x0, 0x0, 0x0, 
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x80, 0xb8, 0x0,
0x0, 0xfe, 0x0,
0x0, 0xff, 0x1,
0x80, 0xff, 0x3,
0xc0, 0xff, 0x7,
0xc0, 0xff, 0x7,
0xe0, 0xc7, 0xf,
0xe0, 0x83, 0xf,
0xf0, 0x83, 0xf,
0xf0, 0x1, 0xf, 
0xf0, 0x1, 0x0,
0xf8, 0x1, 0x0,
0xf8, 0x0, 0x0,
0xf8, 0x0, 0x0,
0xf8, 0x0, 0x0,
0xf8, 0x0, 0x0,
0xf8, 0x0, 0x0,
0xf8, 0x0, 0x0,
0xf8, 0x0, 0x0,
0xf8, 0x0, 0x0,
0xf8, 0x0, 0x0,
0xf8, 0x0, 0x0,
0xf8, 0x0, 0x0,
0xf8, 0x1, 0x0,
0xf0, 0x1, 0x0,
0xf0, 0x1, 0xf,
0xf0, 0x83, 0xf,
0xe0, 0x83, 0xf,
0xe0, 0xc7, 0xf,
0xc0, 0xff, 0x7,
0xc0, 0xff, 0x7,
0x80, 0xff, 0x3,
0x0, 0xff, 0x1,
0x0, 0xfe, 0x0,
0x0, 0x38, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0
};

const uint8_t F [] PROGMEM = {
  0x0, 0x0, 0x0, 
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0xe0, 0xff, 0x1f,
0xe0, 0xff, 0x1f,
0xe0, 0xff, 0x1f,
0xe0, 0xff, 0x1f,
0xe0, 0xff, 0x1f,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0xff, 0x3,
0xe0, 0xff, 0x3,
0xe0, 0xff, 0x3,
0xe0, 0xff, 0x3,
0xe0, 0xff, 0x3,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0xe0, 0x7, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0
};
/*
int[] C24 = {
  
}

int[] u = {
  
}

int[] r = {
  
}

int[] e = {
  
}

int[] n = {
  
}

int[] a = {
  
}

int[] r = {
  
}

int[] g = {
  
}

int[] t = {
  
}

int[] W = {
  
}
*/
const uint8_t degree [] PROGMEM = {
  0x0, 0x0, 0x0, 
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x80, 0x3, 0x0,
0xe0, 0xf, 0x0,
0x70, 0x1c, 0x0,
0x30, 0x18, 0x0,
0x18, 0x30, 0x0,
0x18, 0x30, 0x0,
0x18, 0x30, 0x0,
0x30, 0x18, 0x0,
0x70, 0x1c, 0x0,
0xe0, 0xf, 0x0,
0x80, 0x3, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0
};

const uint8_t colon [] PROGMEM = {
  0x0, 0x0, 0x0, 
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x7c, 0x0,
0x0, 0x7c, 0x0,
0x0, 0x7c, 0x0,
0x0, 0x7c, 0x0,
0x0, 0x7c, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x7c, 0x0,
0x0, 0x7c, 0x0,
0x0, 0x7c, 0x0,
0x0, 0x7c, 0x0,
0x0, 0x7c, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0,
0x0, 0x0, 0x0
};