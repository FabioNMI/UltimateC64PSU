/*
Partial port and adaptation of Adafruit GFX library
Fabio Pereira


This is the core graphics library for all our displays, providing a common
set of graphics primitives (points, lines, circles, etc.).  It needs to be
paired with a hardware-specific library for each display device we carry
(to handle the lower-level functions).

Adafruit invests time and resources providing this open source code, please
support Adafruit & open-source hardware by purchasing products from Adafruit!

Copyright (c) 2013 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "glcdfont.h"
#include "Adafruit_GFX.h"
#include "gfxfont.h"
#include "display.h"

static int16_t cursor_x;     ///< x location to start print()ing text
static int16_t cursor_y;     ///< y location to start print()ing text
static uint16_t textcolor;   ///< 16-bit background color for print()
static uint16_t textbgcolor; ///< 16-bit text color for print()
static uint8_t textsize_x;   ///< Desired magnification in X-axis of text to print()
static uint8_t textsize_y;   ///< Desired magnification in Y-axis of text to print()
static bool wrap;            ///< If set, 'wrap' text at right edge of display
static bool _cp437;          ///< If set, use correct CP437 charset (default is off)
static GFXfont *gfxFont;     ///< Pointer to special font
static tDisplay * display;


#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

static void Adafruit_GFX_fillCircleHelper(int16_t x0, int16_t y0, int16_t r,uint8_t corners, int16_t delta,uint16_t color);

/**************************************************************************/
/*!
   @brief    Instantiate a GFX context for graphics! Can only be done by a
   superclass
   @param    w   Display width, in pixels
   @param    h   Display height, in pixels
*/
/**************************************************************************/
void Adafruit_GFX_init(tDisplay * disp) {
    display = disp;
    cursor_y = cursor_x = 0;
    textsize_x = textsize_y = 1;
    textcolor = textbgcolor = 0xFFFF;
    wrap = true;
    _cp437 = false;
    gfxFont = NULL;
    int16_t * width = display->getWidthPointer();
    *width = display->WIDTH;
    int16_t * height = display->getHeightPointer();
    *height = display->HEIGHT;
    display->init();
}

/**************************************************************************/
/*!
   @brief    Write a line.  Bresenham's algorithm - thx wikipedia
    @param    x0  Start point x coordinate
    @param    y0  Start point y coordinate
    @param    x1  End point x coordinate
    @param    y1  End point y coordinate
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_GFX_writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }

    if (x0 > x1) {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0 <= x1; x0++) {
        if (steep) {
            display->drawPixel(y0, x0, color);
        } else {
            display->drawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

/**************************************************************************/
/*!
   @brief    Draw a pixel
    @param   x   x coordinate
    @param   y   y coordinate
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void Adafruit_GFX_drawPixel(uint16_t x, uint16_t y, uint16_t color) {
    display->drawPixel(x, y, color);
}

/**************************************************************************/
/*!
   @brief    Draw a perfectly vertical line
    @param    x   Top-most x coordinate
    @param    y   Top-most y coordinate
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void Adafruit_GFX_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    // Can be just writeLine(x, y, x, y+h-1, color);
    // or writeFillRect(x, y, 1, h, color);
    display->drawFastVLine(x, y, h, color);
}

/**************************************************************************/
/*!
   @brief    Draw a perfectly horizontal line
    @param    x   Left-most x coordinate
    @param    y   Left-most y coordinate
    @param    w   Width in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void Adafruit_GFX_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    // Example: writeLine(x, y, x+w-1, y, color);
    // or writeFillRect(x, y, w, 1, color);
    display->drawFastHLine(x, y, w, color);
}

/**************************************************************************/
/*!
   @brief    Fill a rectangle completely with one color.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void Adafruit_GFX_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    for (int16_t i = x; i < x + w; i++) {
        Adafruit_GFX_drawFastVLine(i, y, h, color);
    }
}

/**************************************************************************/
/*!
   @brief    Fill the screen completely with one color.
    @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void Adafruit_GFX_fillScreen(uint16_t color) {
    int16_t width = *display->getWidthPointer();
    int16_t height = *display->getHeightPointer();
    Adafruit_GFX_fillRect(0, 0, width, height, color);
}

/**************************************************************************/
/*!
   @brief    Draw a line
    @param    x0  Start point x coordinate
    @param    y0  Start point y coordinate
    @param    x1  End point x coordinate
    @param    y1  End point y coordinate
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_GFX_drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    // Update in subclasses if desired!
    if (x0 == x1) {
        if (y0 > y1)_swap_int16_t(y0, y1);
        Adafruit_GFX_drawFastVLine(x0, y0, y1 - y0 + 1, color);
    } else if (y0 == y1) {
        if (x0 > x1)_swap_int16_t(x0, x1);
        Adafruit_GFX_drawFastHLine(x0, y0, x1 - x0 + 1, color);
    } else {
        Adafruit_GFX_writeLine(x0, y0, x1, y1, color);
    }
}

/**************************************************************************/
/*!
   @brief    Draw a circle outline
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_GFX_drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    display->drawPixel(x0, y0 + r, color);
    display->drawPixel(x0, y0 - r, color);
    display->drawPixel(x0 + r, y0, color);
    display->drawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        display->drawPixel(x0 + x, y0 + y, color);
        display->drawPixel(x0 - x, y0 + y, color);
        display->drawPixel(x0 + x, y0 - y, color);
        display->drawPixel(x0 - x, y0 - y, color);
        display->drawPixel(x0 + y, y0 + x, color);
        display->drawPixel(x0 - y, y0 + x, color);
        display->drawPixel(x0 + y, y0 - x, color);
        display->drawPixel(x0 - y, y0 - x, color);
    }
}

/**************************************************************************/
/*!
    @brief    Quarter-circle drawer, used to do circles and roundrects
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    cornername  Mask bit #1 or bit #2 to indicate which quarters of
   the circle we're doing
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_GFX_drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    if (cornername & 0x4) {
        display->drawPixel(x0 + x, y0 + y, color);
        display->drawPixel(x0 + y, y0 + x, color);
    }
    if (cornername & 0x2) {
        display->drawPixel(x0 + x, y0 - y, color);
        display->drawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
        display->drawPixel(x0 - y, y0 + x, color);
        display->drawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
        display->drawPixel(x0 - y, y0 - x, color);
        display->drawPixel(x0 - x, y0 - y, color);
    }
  }
}

/**************************************************************************/
/*!
   @brief    Draw a circle with filled color
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void Adafruit_GFX_fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    Adafruit_GFX_drawFastVLine(x0, y0 - r, 2 * r + 1, color);
    Adafruit_GFX_fillCircleHelper(x0, y0, r, 3, 0, color);
}

/**************************************************************************/
/*!
    @brief  Quarter-circle drawer with fill, used for circles and roundrects
    @param  x0       Center-point x coordinate
    @param  y0       Center-point y coordinate
    @param  r        Radius of circle
    @param  corners  Mask bits indicating which quarters we're doing
    @param  delta    Offset from center-point, used for round-rects
    @param  color    16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
static void Adafruit_GFX_fillCircleHelper(int16_t x0, int16_t y0, int16_t r,uint8_t corners, int16_t delta,uint16_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  int16_t px = x;
  int16_t py = y;

  delta++; // Avoid some +1's in the loop

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    // These checks avoid double-drawing certain lines, important
    // for the SSD1306 library which has an INVERT drawing mode.
    if (x < (y + 1)) {
      if (corners & 1) Adafruit_GFX_drawFastVLine(x0 + x, y0 - y, 2 * y + delta, color);
      if (corners & 2) Adafruit_GFX_drawFastVLine(x0 - x, y0 - y, 2 * y + delta, color);
    }
    if (y != py) {
      if (corners & 1) Adafruit_GFX_drawFastVLine(x0 + py, y0 - px, 2 * px + delta, color);
      if (corners & 2) Adafruit_GFX_drawFastVLine(x0 - py, y0 - px, 2 * px + delta, color);
      py = y;
    }
    px = x;
  }
}

/**************************************************************************/
/*!
   @brief   Draw a rectangle with no fill color
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_GFX_drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    Adafruit_GFX_drawFastHLine(x, y, w, color);
    Adafruit_GFX_drawFastHLine(x, y + h - 1, w, color);
    Adafruit_GFX_drawFastVLine(x, y, h, color);
    Adafruit_GFX_drawFastVLine(x + w - 1, y, h, color);
}

/**************************************************************************/
/*!
   @brief   Draw a rounded rectangle with no fill color
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
    @param    r   Radius of corner rounding
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_GFX_drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
    int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if (r > max_radius) r = max_radius;
    // smarter version
    Adafruit_GFX_drawFastHLine(x + r, y, w - 2 * r, color);         // Top
    Adafruit_GFX_drawFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
    Adafruit_GFX_drawFastVLine(x, y + r, h - 2 * r, color);         // Left
    Adafruit_GFX_drawFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right
    // draw four corners
    Adafruit_GFX_drawCircleHelper(x + r, y + r, r, 1, color);
    Adafruit_GFX_drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
    Adafruit_GFX_drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
    Adafruit_GFX_drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

/**************************************************************************/
/*!
   @brief   Draw a rounded rectangle with fill color
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
    @param    r   Radius of corner rounding
    @param    color 16-bit 5-6-5 Color to draw/fill with
*/
/**************************************************************************/
void Adafruit_GFX_fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
  int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
  if (r > max_radius) r = max_radius;
  // smarter version
  Adafruit_GFX_fillRect(x + r, y, w - 2 * r, h, color);
  // draw four corners
  Adafruit_GFX_fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
  Adafruit_GFX_fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}

/**************************************************************************/
/*!
   @brief   Draw a triangle with no fill color
    @param    x0  Vertex #0 x coordinate
    @param    y0  Vertex #0 y coordinate
    @param    x1  Vertex #1 x coordinate
    @param    y1  Vertex #1 y coordinate
    @param    x2  Vertex #2 x coordinate
    @param    y2  Vertex #2 y coordinate
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_GFX_drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    Adafruit_GFX_drawLine(x0, y0, x1, y1, color);
    Adafruit_GFX_drawLine(x1, y1, x2, y2, color);
    Adafruit_GFX_drawLine(x2, y2, x0, y0, color);
}

/**************************************************************************/
/*!
   @brief     Draw a triangle with color-fill
    @param    x0  Vertex #0 x coordinate
    @param    y0  Vertex #0 y coordinate
    @param    x1  Vertex #1 x coordinate
    @param    y1  Vertex #1 y coordinate
    @param    x2  Vertex #2 x coordinate
    @param    y2  Vertex #2 y coordinate
    @param    color 16-bit 5-6-5 Color to fill/draw with
*/
/**************************************************************************/
void Adafruit_GFX_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {

  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    _swap_int16_t(y0, y1);
    _swap_int16_t(x0, x1);
  }
  if (y1 > y2) {
    _swap_int16_t(y2, y1);
    _swap_int16_t(x2, x1);
  }
  if (y0 > y1) {
    _swap_int16_t(y0, y1);
    _swap_int16_t(x0, x1);
  }

  if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a) a = x1;
    else if (x1 > b)b = x1;
    if (x2 < a) a = x2;
    else if (x2 > b) b = x2;
    Adafruit_GFX_drawFastHLine(a, y0, b - a + 1, color);
    return;
  }

  int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
          dx12 = x2 - x1, dy12 = y2 - y1;
  int32_t sa = 0, sb = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if (y1 == y2) last = y1; // Include y1 scanline
  else last = y1 - 1; // Skip it

  for (y = y0; y <= last; y++) {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b)_swap_int16_t(a, b);
    Adafruit_GFX_drawFastHLine(a, y, b - a + 1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = (int32_t)dx12 * (y - y1);
  sb = (int32_t)dx02 * (y - y0);
  for (; y <= y2; y++) {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b)_swap_int16_t(a, b);
    Adafruit_GFX_drawFastHLine(a, y, b - a + 1, color);
  }
}

// BITMAP / XBITMAP / GRAYSCALE / RGB BITMAP FUNCTIONS ---------------------

/**************************************************************************/
/*!
   @brief      Draw a 1-bit image at the specified (x,y) position.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with monochrome bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
    @param    fg 16-bit 5-6-5 Color to draw with
    @param    bg 16-bit 5-6-5 background color
    @param    t bool transparency
*/
/**************************************************************************/
void Adafruit_GFX_drawBitmap(int16_t x, int16_t y, uint8_t bitmap[], int16_t w, int16_t h, uint16_t fg, uint16_t bg, bool t) {

  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t b = 0;

  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7) b <<= 1;
      else b = bitmap[j * byteWidth + i / 8];
      if (t) {
          if (b & 0x80) display->drawPixel(x + i, y, (b & 0x80) ? fg : bg);
      } else {
          if (b & 0x80) display->drawPixel(x + i, y, fg);
      }
    }
  }
}



/**************************************************************************/
/*!
   @brief      Draw XBitMap Files (*.xbm), exported from GIMP.
   Usage: Export from GIMP to *.xbm, rename *.xbm to *.c and open in editor.
   C Array can be directly used with this function.
   There is no RAM-resident version of this function; if generating bitmaps
   in RAM, use the format defined by drawBitmap() and call that instead.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with monochrome bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
    @param    color 16-bit 5-6-5 Color to draw pixels with
*/
/**************************************************************************/
void Adafruit_GFX_drawXBitmap(int16_t x, int16_t y, uint8_t bitmap[], int16_t w, int16_t h, uint16_t color) {

  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t b = 0;

  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7) b >>= 1;
      else b = bitmap[j * byteWidth + i / 8];
      // Nearly identical to drawBitmap(), only the bit order
      // is reversed here (left-to-right = LSB to MSB):
      if (b & 0x01) display->drawPixel(x + i, y, color);
    }
  }
}

/**************************************************************************/
/*!
   @brief   Draw a RAM-resident 8-bit image (grayscale) at the specified (x,y)
   pos. Specifically for 8-bit display devices such as IS31FL3731; no color
   reduction/expansion is performed.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with grayscale bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
*/
/**************************************************************************/
void Adafruit_GFX_drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h) {
  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
        display->drawPixel(x + i, y, bitmap[j * w + i]);
    }
  }
}

/**************************************************************************/
/*!
   @brief   Draw an 8-bit image (grayscale) with a 1-bit mask
   (set bits = opaque, unset bits = clear) at the specified (x,y) position.
   Specifically for 8-bit display devices such as IS31FL3731; no color
   reduction/expansion is performed.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with grayscale bitmap
    @param    mask  byte array with mask bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
*/
/**************************************************************************/
void Adafruit_GFX_drawGrayscaleBitmapWithMask(int16_t x, int16_t y, uint8_t bitmap[], uint8_t mask[], int16_t w, int16_t h) {
  int16_t bw = (w + 7) / 8; // Bitmask scanline pad = whole byte
  uint8_t b = 0;
  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7) b <<= 1;
      else b = mask[j * bw + i / 8];
      if (b & 0x80) {
          display->drawPixel(x + i, y, (uint8_t)bitmap[j * w + i]);
      }
    }
  }
}

/**************************************************************************/
/*!
   @brief   Draw a 16-bit image (RGB 5/6/5) at the specified
   (x,y) position. For 16-bit display devices; no color reduction performed.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with 16-bit color bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
*/
/**************************************************************************/
void Adafruit_GFX_drawRGBBitmap(int16_t x, int16_t y, uint16_t bitmap[], int16_t w, int16_t h) {
  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
        display->drawPixel(x + i, y, bitmap[j * w + i]);
    }
  }
}


/**************************************************************************/
/*!
   @brief   Draw a PROGMEM-resident 16-bit image (RGB 5/6/5) with a 1-bit mask
   (set bits = opaque, unset bits = clear) at the specified (x,y) position. BOTH
   buffers (color and mask) must be PROGMEM-resident. For 16-bit display
   devices; no color reduction performed.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with 16-bit color bitmap
    @param    mask  byte array with monochrome mask bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
*/
/**************************************************************************/
void Adafruit_GFX_drawRGBBitmapWithMask(int16_t x, int16_t y, uint16_t bitmap[], uint8_t mask[], int16_t w, int16_t h) {
  int16_t bw = (w + 7) / 8; // Bitmask scanline pad = whole byte
  uint8_t b = 0;
  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7) b <<= 1;
      else b = mask[j * bw + i / 8];
      if (b & 0x80) display->drawPixel(x + i, y, bitmap[j * w + i]);
    }
  }
}

// TEXT- AND CHARACTER-HANDLING FUNCTIONS ----------------------------------

// Draw a character
/**************************************************************************/
/*!
   @brief   Draw a single character
    @param    x   Bottom left corner x coordinate
    @param    y   Bottom left corner y coordinate
    @param    c   The 8-bit font-indexed character (likely ascii)
    @param    color 16-bit 5-6-5 Color to draw chraracter with
    @param    bg 16-bit 5-6-5 Color to fill background with (if same as color,
   no background)
    @param    size_x  Font magnification level in X-axis, 1 is 'original' size
    @param    size_y  Font magnification level in Y-axis, 1 is 'original' size
*/
/**************************************************************************/
void Adafruit_GFX_drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y) {
  if (!gfxFont) { // 'Classic' built-in font
    int16_t width = *display->getWidthPointer();
    int16_t height = *display->getHeightPointer();

    if ((x >= width) ||              // Clip right
        (y >= height) ||             // Clip bottom
        ((x + 6 * size_x - 1) < 0) || // Clip left
        ((y + 8 * size_y - 1) < 0))   // Clip top
      return;

    if (!_cp437 && (c >= 176))
      c++; // Handle 'classic' charset behavior

    for (int8_t i = 0; i < 5; i++) { // Char bitmap = 5 columns
      uint8_t line = font[c * 5 + i];
      for (int8_t j = 0; j < 8; j++, line >>= 1) {
        if (line & 1) {
          if (size_x == 1 && size_y == 1) display->drawPixel(x + i, y + j, color);
          else Adafruit_GFX_fillRect(x + i * size_x, y + j * size_y, size_x, size_y, color);
        } else if (bg != color) {
          if (size_x == 1 && size_y == 1) display->drawPixel(x + i, y + j, bg);
          else Adafruit_GFX_fillRect(x + i * size_x, y + j * size_y, size_x, size_y, bg);
        }
      }
    }
    if (bg != color) { // If opaque, draw vertical line for last column
      if (size_x == 1 && size_y == 1)
          Adafruit_GFX_drawFastVLine(x + 5, y, 8, bg);
      else
          Adafruit_GFX_fillRect(x + 5 * size_x, y, size_x, 8 * size_y, bg);
    }

  } else { // Custom font

    // Character is assumed previously filtered by write() to eliminate
    // newlines, returns, non-printable characters, etc.  Calling
    // drawChar() directly with 'bad' characters of font may cause mayhem!

    c -= (uint8_t)gfxFont->first;
    GFXglyph *glyph = gfxFont->glyph + c;
    uint8_t *bitmap = gfxFont->bitmap;

    uint16_t bo = glyph->bitmapOffset;
    uint8_t w = glyph->width, h = glyph->height;
    int8_t xo = glyph->xOffset,
           yo = glyph->yOffset;
    uint8_t xx, yy, bits = 0, bit = 0;
    int16_t xo16 = 0, yo16 = 0;

    if (size_x > 1 || size_y > 1) {
      xo16 = xo;
      yo16 = yo;
    }

    // Todo: Add character clipping here

    // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
    // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
    // has typically been used with the 'classic' font to overwrite old
    // screen contents with new data.  This ONLY works because the
    // characters are a uniform size; it's not a sensible thing to do with
    // proportionally-spaced fonts with glyphs of varying sizes (and that
    // may overlap).  To replace previously-drawn text when using a custom
    // font, use the getTextBounds() function to determine the smallest
    // rectangle encompassing a string, erase the area with fillRect(),
    // then draw new text.  This WILL infortunately 'blink' the text, but
    // is unavoidable.  Drawing 'background' pixels will NOT fix this,
    // only creates a new set of problems.  Have an idea to work around
    // this (a canvas object type for MCUs that can afford the RAM and
    // displays supporting setAddrWindow() and pushColors()), but haven't
    // implemented this yet.

    for (yy = 0; yy < h; yy++) {
      for (xx = 0; xx < w; xx++) {
        if (!(bit++ & 7)) {
          bits = bitmap[bo++];
        }
        if (bits & 0x80) {
          if (size_x == 1 && size_y == 1) {
              display->drawPixel(x + xo + xx, y + yo + yy, color);
          } else {
              Adafruit_GFX_fillRect(x + (xo16 + xx) * size_x, y + (yo16 + yy) * size_y,
                          size_x, size_y, color);
          }
        }
        bits <<= 1;
      }
    }

  } // End classic vs custom font
}
/**************************************************************************/
/*!
    @brief  Print one byte/character of data, used to support print()
    @param  c  The 8-bit ascii character to write
*/
/**************************************************************************/
size_t Adafruit_GFX_write(uint8_t c) {
  int16_t width = *display->getWidthPointer();
  if (!gfxFont) { // 'Classic' built-in font
    if (c == '\n') {              // Newline?
      cursor_x = 0;               // Reset x to zero,
      cursor_y += textsize_y * 8; // advance y one line
    } else if (c != '\r') {       // Ignore carriage returns
      if (wrap && ((cursor_x + textsize_x * 6) > width)) { // Off right?
        cursor_x = 0;                                       // Reset x to zero,
        cursor_y += textsize_y * 8; // advance y one line
      }
      Adafruit_GFX_drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x, textsize_y);
      cursor_x += textsize_x * 6; // Advance x one char
    }
  } else { // Custom font
    if (c == '\n') {
      cursor_x = 0;
      cursor_y += (int16_t)textsize_y * (uint8_t)gfxFont->yAdvance;
    } else if (c != '\r') {
      uint8_t first = gfxFont->first;
      if ((c >= first) && (c <= (uint8_t)gfxFont->last)) {
        GFXglyph *glyph = gfxFont->glyph + (c - first);
        uint8_t w = glyph->width, h = glyph->height;
        if ((w > 0) && (h > 0)) { // Is there an associated bitmap?
          int16_t xo = (int8_t)glyph->xOffset; // sic
          if (wrap && ((cursor_x + textsize_x * (xo + w)) > width)) {
            cursor_x = 0;
            cursor_y += (int16_t)textsize_y * (uint8_t)gfxFont->yAdvance;
          }
          Adafruit_GFX_drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x, textsize_y);
        }
        cursor_x += (uint8_t)glyph->xAdvance * (int16_t)textsize_x;
      }
    }
  }
  return 1;
}

void Adafruit_GFX_printAt(unsigned char x, unsigned char y, char * data) {
    Adafruit_GFX_setCursor(x, y);
    while (*data) {
        Adafruit_GFX_write(*data);
        data++;
    }
}

/**************************************************************************/
/*!
    @brief   Set text 'magnification' size. Each increase in s makes 1 pixel
   that much bigger.
    @param  s_x  Desired text width magnification level in X-axis. 1 is default
    @param  s_y  Desired text width magnification level in Y-axis. 1 is default
*/
/**************************************************************************/
void Adafruit_GFX_setTextSize(uint8_t s_x, uint8_t s_y) {
  textsize_x = (s_x > 0) ? s_x : 1;
  textsize_y = (s_y > 0) ? s_y : 1;
}

/**********************************************************************/
/*!
  @brief  Set text cursor location
  @param  x    X coordinate in pixels
  @param  y    Y coordinate in pixels
*/
/**********************************************************************/
void Adafruit_GFX_setCursor(int16_t x, int16_t y) {
  cursor_x = x;
  cursor_y = y;
}

/**********************************************************************/
/*!
  @brief   Set text font color with custom background color
  @param   c   16-bit 5-6-5 Color to draw text with
  @param   bg  16-bit 5-6-5 Color to draw background/fill with
*/
/**********************************************************************/
void Adafruit_GFX_setTextColor(uint16_t c, uint16_t bg) {
  textcolor = c;
  textbgcolor = bg;
}

/**********************************************************************/
/*!
@brief  Set whether text that is too long for the screen width should
        automatically wrap around to the next line (else clip right).
@param  w  true for wrapping, false for clipping
*/
/**********************************************************************/
void Adafruit_GFX_setTextWrap(bool w) {
    wrap = w;
}

/**********************************************************************/
/*!
  @brief  Enable (or disable) Code Page 437-compatible charset.
          There was an error in glcdfont.c for the longest time -- one
          character (#176, the 'light shade' block) was missing -- this
          threw off the index of every character that followed it.
          But a TON of code has been written with the erroneous
          character indices. By default, the library uses the original
          'wrong' behavior and old sketches will still work. Pass
          'true' to this function to use correct CP437 character values
          in your code.
  @param  x  true = enable (new behavior), false = disable (old behavior)
*/
/**********************************************************************/
void Adafruit_GFX_cp437(bool x) {
    _cp437 = x;
}

// get current cursor position (get rotation safe maximum values,
// using: width() for x, height() for y)
/************************************************************************/
/*!
  @brief  Get text cursor X location
  @returns    X coordinate in pixels
*/
/************************************************************************/
uint16_t getCursorX(void) {
    return cursor_x;
}

/************************************************************************/
/*!
  @brief      Get text cursor Y location
  @returns    Y coordinate in pixels
*/
/************************************************************************/
uint16_t getCursorY(void) {
    return cursor_y;
};

/**************************************************************************/
/*!
    @brief      Set rotation setting for display
    @param  x   0 thru 3 corresponding to 4 cardinal rotations
*/
/**************************************************************************/
void Adafruit_GFX_setRotation(displayRotation r) {
    displayRotation * rotation = display->getRotationPointer();
    *rotation = r;
    int16_t * width = display->getWidthPointer();
    int16_t * height = display->getHeightPointer();
    switch (r) {
        case DISPLAY_ROTATE_0:
        case DISPLAY_ROTATE_180:
            *width = display->WIDTH;
            *height = display->HEIGHT;
            break;
        case DISPLAY_ROTATE_90:
        case DISPLAY_ROTATE_270:
            *width = display->HEIGHT;
            *height = display->WIDTH;
            break;
        default:
    }
}

/************************************************************************/
/*!
  @brief      Get rotation setting for display
  @returns    0 thru 3 corresponding to 4 cardinal rotations
*/
/************************************************************************/
displayRotation getRotation(void) {
    return *display->getRotationPointer();
}

/**************************************************************************/
/*!
    @brief Set the font to display when print()ing, either custom or default
    @param  f  The GFXfont object, if NULL use built in 6x8 font
*/
/**************************************************************************/
void Adafruit_GFX_setFont(const GFXfont *f) {
  if (f) {          // Font struct pointer passed in?
    if (!gfxFont) { // And no current font struct?
      // Switching from classic to new font behavior.
      // Move cursor pos down 6 pixels so it's on baseline.
      cursor_y += 6;
    }
  } else if (gfxFont) { // NULL passed.  Current font struct defined?
    // Switching from new to classic font behavior.
    // Move cursor pos up 6 pixels so it's at top-left of char.
    cursor_y -= 6;
  }
  gfxFont = (GFXfont *)f;
}

/**************************************************************************/
/*!
    @brief  Helper to determine size of a character with current font/size.
            Broke this out as it's used by both the PROGMEM- and RAM-resident
            getTextBounds() functions.
    @param  c     The ASCII character in question
    @param  x     Pointer to x location of character. Value is modified by
                  this function to advance to next character.
    @param  y     Pointer to y location of character. Value is modified by
                  this function to advance to next character.
    @param  minx  Pointer to minimum X coordinate, passed in to AND returned
                  by this function -- this is used to incrementally build a
                  bounding rectangle for a string.
    @param  miny  Pointer to minimum Y coord, passed in AND returned.
    @param  maxx  Pointer to maximum X coord, passed in AND returned.
    @param  maxy  Pointer to maximum Y coord, passed in AND returned.
*/
/**************************************************************************/
void Adafruit_GFX_charBounds(unsigned char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy) {
  int16_t width = *display->getWidthPointer();
  if (gfxFont) {

    if (c == '\n') { // Newline?
      *x = 0;        // Reset x to zero, advance y by one line
      *y += textsize_y * (uint8_t)gfxFont->yAdvance;
    } else if (c != '\r') { // Not a carriage return; is normal char
      uint8_t first = gfxFont->first,
              last = gfxFont->last;
      if ((c >= first) && (c <= last)) { // Char present in this font?
        GFXglyph *glyph = gfxFont->glyph + (c - first);
        uint8_t gw = glyph->width,
                gh = glyph->height,
                xa = glyph->xAdvance;
        int8_t xo = glyph->xOffset,
               yo = glyph->yOffset;
        if (wrap && ((*x + (((int16_t)xo + gw) * textsize_x)) > width)) {
          *x = 0; // Reset x to zero, advance y by one line
          *y += textsize_y * (uint8_t)gfxFont->yAdvance;
        }
        int16_t tsx = (int16_t)textsize_x, tsy = (int16_t)textsize_y,
                x1 = *x + xo * tsx, y1 = *y + yo * tsy, x2 = x1 + gw * tsx - 1,
                y2 = y1 + gh * tsy - 1;
        if (x1 < *minx)
          *minx = x1;
        if (y1 < *miny)
          *miny = y1;
        if (x2 > *maxx)
          *maxx = x2;
        if (y2 > *maxy)
          *maxy = y2;
        *x += xa * tsx;
      }
    }

  } else { // Default font

    if (c == '\n') {        // Newline?
      *x = 0;               // Reset x to zero,
      *y += textsize_y * 8; // advance y one line
      // min/max x/y unchanged -- that waits for next 'normal' character
    } else if (c != '\r') { // Normal char; ignore carriage returns
      if (wrap && ((*x + textsize_x * 6) > width)) { // Off right?
        *x = 0;                                       // Reset x to zero,
        *y += textsize_y * 8;                         // advance y one line
      }
      int x2 = *x + textsize_x * 6 - 1, // Lower-right pixel of char
          y2 = *y + textsize_y * 8 - 1;
      if (x2 > *maxx)
        *maxx = x2; // Track max x, y
      if (y2 > *maxy)
        *maxy = y2;
      if (*x < *minx)
        *minx = *x; // Track min x, y
      if (*y < *miny)
        *miny = *y;
      *x += textsize_x * 6; // Advance x one char
    }
  }
}

/**************************************************************************/
/*!
    @brief  Helper to determine size of a string with current font/size.
            Pass string and a cursor position, returns UL corner and W,H.
    @param  str  The ASCII string to measure
    @param  x    The current cursor X
    @param  y    The current cursor Y
    @param  x1   The boundary X coordinate, returned by function
    @param  y1   The boundary Y coordinate, returned by function
    @param  w    The boundary width, returned by function
    @param  h    The boundary height, returned by function
*/
/**************************************************************************/
void Adafruit_GFX_getTextBounds(const char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {

  uint8_t c; // Current character
  int16_t minx = 0x7FFF, miny = 0x7FFF, maxx = -1, maxy = -1; // Bound rect
  // Bound rect is intentionally initialized inverted, so 1st char sets it

  *x1 = x; // Initial position is value passed in
  *y1 = y;
  *w = *h = 0; // Initial size is zero

  while ((c = *str++)) {
    // charBounds() modifies x/y to advance for each character,
    // and min/max x/y are updated to incrementally build bounding rect.
      Adafruit_GFX_charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy);
  }

  if (maxx >= minx) {     // If legit string bounds were found...
    *x1 = minx;           // Update x1 to least X coord,
    *w = maxx - minx + 1; // And w to bound rect width
  }
  if (maxy >= miny) { // Same for height
    *y1 = miny;
    *h = maxy - miny + 1;
  }
}

/**************************************************************************/
/*!
    @brief      Invert the display (ideally using built-in hardware command)
    @param   i  True if you want to invert, false to make 'normal'
*/
/**************************************************************************/
void Adafruit_GFX_invertDisplay(bool i) {
  // Do nothing, must be subclassed if supported by hardware
  (void)i; // disable -Wunused-parameter warning
}
