#ifndef _ADAFRUIT_GFX_H
#define _ADAFRUIT_GFX_H

#include "gfxfont.h"
#include <stdint.h>
#include "display.h"

void Adafruit_GFX_init(tDisplay * disp);
void Adafruit_GFX_writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void Adafruit_GFX_drawPixel(uint16_t x, uint16_t y, uint16_t color);
void Adafruit_GFX_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void Adafruit_GFX_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void Adafruit_GFX_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void Adafruit_GFX_fillScreen(uint16_t color);
void Adafruit_GFX_drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void Adafruit_GFX_drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void Adafruit_GFX_drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
void Adafruit_GFX_fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void Adafruit_GFX_drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void Adafruit_GFX_drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void Adafruit_GFX_fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void Adafruit_GFX_drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void Adafruit_GFX_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void Adafruit_GFX_drawBitmap(int16_t x, int16_t y, uint8_t bitmap[], int16_t w, int16_t h, uint16_t fg, uint16_t bg, bool t);
void Adafruit_GFX_drawXBitmap(int16_t x, int16_t y, uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
void Adafruit_GFX_drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h);
void Adafruit_GFX_drawGrayscaleBitmapWithMask(int16_t x, int16_t y, uint8_t bitmap[], uint8_t mask[], int16_t w, int16_t h);
void Adafruit_GFX_drawRGBBitmap(int16_t x, int16_t y, uint16_t bitmap[], int16_t w, int16_t h);
void Adafruit_GFX_drawRGBBitmapWithMask(int16_t x, int16_t y, uint16_t bitmap[], uint8_t mask[], int16_t w, int16_t h);
void Adafruit_GFX_drawChar(int16_t x, int16_t y, unsigned char c, uint16_t fg, uint16_t bg, uint8_t size_x, uint8_t size_y);
size_t Adafruit_GFX_write(uint8_t c);
void Adafruit_GFX_printAt(unsigned char x, unsigned char y, char * data);
void Adafruit_GFX_cp437(bool x);
void Adafruit_GFX_setCursor(int16_t x, int16_t y);
void Adafruit_GFX_setTextColor(uint16_t fg, uint16_t bg);
void Adafruit_GFX_setTextSize(uint8_t s_x, uint8_t s_y);
void Adafruit_GFX_setTextWrap(bool w);
void Adafruit_GFX_setRotation(displayRotation x);
void Adafruit_GFX_setFont(const GFXfont *f);
void Adafruit_GFX_charBounds(unsigned char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy);
uint16_t getCursorX(void);
uint16_t getCursorY(void);
displayRotation getRotation(void);
void Adafruit_GFX_getTextBounds(const char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
void Adafruit_GFX_invertDisplay(bool i);

#endif // _ADAFRUIT_GFX_H
