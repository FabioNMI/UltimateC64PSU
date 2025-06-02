/*
 * display.h
 *
 *  Created on: Aug. 10, 2024
 *      Author: fabiop
 */

#ifndef __DISPLAY_H_
#define __DISPLAY_H_

typedef enum {
    DISPLAY_ROTATE_0,
    DISPLAY_ROTATE_90,
    DISPLAY_ROTATE_180,
    DISPLAY_ROTATE_270
} displayRotation;

typedef struct {
    uint16_t WIDTH;
    uint16_t HEIGHT;
    void (*init)(void);
    void (*drawPixel)(int16_t x, int16_t y, uint16_t color);
    void (*drawFastVLine)(int16_t x, int16_t y, int16_t h, uint16_t color);
    void (*drawFastHLine)(int16_t x, int16_t y, int16_t w, uint16_t color);
    void (*inverseScreen)(bool inverse);
    void (*show)(void);
    displayRotation *(*getRotationPointer)(void);
    int16_t *(*getWidthPointer)(void);
    int16_t *(*getHeightPointer)(void);
} tDisplay;

#endif /* __DISPLAY_H_ */
