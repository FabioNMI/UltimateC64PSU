

#include "ssd1306.h"
#include "hal_data.h"
#include "display.h"

static void ssd1306_drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color);
static void ssd1306_drawFastVLineInternal(int16_t x, int16_t __y, int16_t __h, uint16_t color);
static displayRotation * ssd1306_getRotationPointer(void);
static int16_t * ssd1306_getWidthPointer(void);
static int16_t * ssd1306_getHeightPointer(void);

#define ssd1306_swap(a, b)                                                     \
  (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation

static uint8_t lcdBuffer[LCD_BUFFER_SIZE];
static volatile bool i2c_busy = false;
displayRotation rotation;
static int16_t _width;       ///< Display width as modified by current rotation
static int16_t _height;      ///< Display height as modified by current rotation
const tDisplay ssd1306 = {  .WIDTH = DISP_WIDTH,
                            .HEIGHT = DISP_HEIGHT,
                            .drawPixel =  ssd1306_drawPixel,
                            .drawFastHLine = ssd1306_drawFastHLine,
                            .drawFastVLine = ssd1306_drawFastVLine,
                            .inverseScreen = ssd1306_inverseScreen,
                            .init = ssd1306_init,
                            .show = ssd1306_updateScreen,
                            .getRotationPointer = ssd1306_getRotationPointer,
                            .getWidthPointer = ssd1306_getWidthPointer,
                            .getHeightPointer = ssd1306_getHeightPointer
};

/* Callback function */
void sau_i2c_master_callback(i2c_master_callback_args_t *p_args) {
    FSP_PARAMETER_NOT_USED(p_args);
    i2c_busy = false;
}

static fsp_err_t wait_i2c_busy(void) {
    fsp_err_t   status  = FSP_SUCCESS;
    uint16_t    timeout = 20;
    /* Wait for any I2C event */
    while (i2c_busy) {
        /* Check time elapsed to avoid infinite loop */
        if(timeout == 0) {
            status = FSP_ERR_TIMEOUT;
            break;
        }
        /* Decrease timeout value then software delay */
        timeout --;
        R_BSP_SoftwareDelay(8000U, BSP_DELAY_UNITS_MICROSECONDS);
    }
    // Set i2c_busy since this function should always precede an i2c operation
    i2c_busy = true;
    return status;
}

/* Initialize the SSD1306 display */
void ssd1306_init(void) {
    // Initialization commands for SSD1306
    const uint8_t init_commands[] = {
         SSD1306_DISPLAY_OFF,                                         // 0xAE = Set Display OFF
         SSD1306_SET_OSC_FREQ, 0x80,                                  // 0xD5, 0x80 => D=1; DCLK = Fosc / D <=> DCLK = Fosc
         SSD1306_SET_MUX_RATIO, 0x1F,                                 // 0xA8 - 0x3F for 128 x 64 version (64MUX)
                                                                         //      - 0x1F for 128 x 32 version (32MUX)
         SSD1306_DISPLAY_OFFSET, 0x00,                                // 0xD3
         SSD1306_SET_START_LINE,                                      // 0x40
         SSD1306_SET_CHAR_REG, 0x14,                                  // 0x8D, Enable charge pump during display on
         SSD1306_MEMORY_ADDR_MODE, 0x00,                              // 0x20 = Set Memory Addressing Mode
                                                                         // 0x00 - Horizontal Addressing Mode
                                                                         // 0x01 - Vertical Addressing Mode
                                                                         // 0x02 - Page Addressing Mode (RESET)
         SSD1306_SEG_REMAP_OP,                                        // 0xA0 / remap 0xA1
         SSD1306_COM_SCAN_DIR_OP,                                     // 0xC0 / remap 0xC8
         SSD1306_COM_PIN_CONF, 0x02,                                  // 0xDA, 0x12 - Disable COM Left/Right remap, Alternative COM pin configuration
                                                                         //       0x12 - for 128 x 64 version
                                                                         //       0x02 - for 128 x 32 version
         SSD1306_SET_CONTRAST, 0x8F,                                  // 0x81, 0x7F - reset value (max 0xFF)
         SSD1306_SET_PRECHARGE, 0xC2,                                 // 0xD9, higher value less blinking
                                                                         // 0xC2, 1st phase = 2 DCLK,  2nd phase = 13 DCLK
         SSD1306_VCOM_DESELECT, 0x40,                                 // Set V COMH Deselect, reset value 0x22 = 0,77xUcc
         SSD1306_DIS_ENT_DISP_ON,                                     // 0xA4
         SSD1306_DIS_NORMAL,                                         // 0xA6
         SSD1306_DEACT_SCROLL,                                        // 0x2E
         SSD1306_DISPLAY_ON,                                           // 0xAF = Set Display ON
    };

    R_SAU_I2C_Open(&g_i2c0_ctrl, &g_i2c0_cfg);
    R_SAU_I2C_SlaveAddressSet(&g_i2c0_ctrl, SSD1306_ADDR, I2C_MASTER_ADDR_MODE_7BIT);
    // Send display init commands
    for (uint32_t i = 0; i < sizeof(init_commands); i++) {
        ssd1306_write_command(init_commands[i]);
    }
    ssd1306_clearScreen();
    ssd1306_updateScreen();
}

/* Write a command to the SSD1306 */
void ssd1306_write_command(uint8_t command) {
    static uint8_t buffer[2];
    //buffer[0] = SSD1306_COMMAND;
    //buffer[1] = command;

    //i2c_busy = true;
    wait_i2c_busy();
    buffer[0] = SSD1306_COMMAND;
    buffer[1] = command;
    R_BSP_SoftwareDelay(2, BSP_DELAY_UNITS_MICROSECONDS);
    R_SAU_I2C_Write(&g_i2c0_ctrl, buffer, 2, false);
    //wait_i2c_busy();
}

/* Write data to the SSD1306 */
void ssd1306_write_data(uint8_t *data, uint32_t length) {
    static uint8_t * buffer = NULL;
    //uint8_t buffer[length + 1];
    //if (length == 1)  buffer[0] = SSD1306_DATA; else buffer[0] = SSD1306_DATA_STREAM;
    //memcpy(&buffer[1], data, length);
    wait_i2c_busy();
    if (buffer != NULL) {
        free(buffer);
        buffer = NULL;
    }
    buffer = malloc(length + 1);
    if (length == 1)  buffer[0] = SSD1306_DATA; else buffer[0] = SSD1306_DATA_STREAM;
    memcpy(&buffer[1], data, length);
    //i2c_busy = true;
    R_SAU_I2C_Write(&g_i2c0_ctrl, buffer, length+1, false);
    //wait_i2c_busy();
}

/**
 * +------------------------------------------------------------------------------------+
 * |== PUBLIC FUNCTIONS ================================================================|
 * +------------------------------------------------------------------------------------+
 */

/**
 * @brief   invert screen colors
 * @param   bool
 * @return  void
 */
void ssd1306_inverseScreen(bool inverse) {
    if (inverse) {
        ssd1306_write_command(SSD1306_DIS_INVERSE);
    } else {
        ssd1306_write_command(SSD1306_DIS_NORMAL);
    }
}


void ssd1306_updateScreen(void) {
    const uint8_t commands[] = {
        SSD1306_SET_PAGE_ADDR, 0, 0xff,
        SSD1306_SET_COLUMN_ADDR,0,
        127
    };
    for (uint32_t i = 0; i < sizeof(commands); i++) {
        ssd1306_write_command(commands[i]);
    }
    ssd1306_write_data(lcdBuffer, LCD_BUFFER_SIZE);
}

/**
 * @brief   ssd1306 Clear screen
 * @param   void
 * @return  void
 */
void ssd1306_clearScreen (void) {
    memset (lcdBuffer, 0x00, LCD_BUFFER_SIZE);
}

// This is based on Adafruit's SSD1306 driver
void ssd1306_drawPixel(int16_t x, int16_t y, uint16_t color) {
    if ((x >= 0) && (x < _width) && (y >= 0) && (y < _height)) {
        // Pixel is in-bounds. Rotate coordinates if needed.
        switch (rotation) {
            case DISPLAY_ROTATE_90:
                ssd1306_swap(x, y);
                x = DISP_WIDTH - x - 1;
                break;
            case DISPLAY_ROTATE_180:
                x = DISP_WIDTH - x - 1;
                y = DISP_HEIGHT - y - 1;
                break;
            case DISPLAY_ROTATE_270:
                ssd1306_swap(x, y);
                y = DISP_HEIGHT - y - 1;
                break;
            case DISPLAY_ROTATE_0:
                break;  // do nothing
        }
        switch (color) {
            case CL_WHITE:
                lcdBuffer[x + (y / 8) * DISP_WIDTH] |= (1 << (y & 7));
                break;
            case CL_BLACK:
                lcdBuffer[x + (y / 8) * DISP_WIDTH] &= ~(1 << (y & 7));
                break;
            case CL_INVERSE:
                lcdBuffer[x + (y / 8) * DISP_WIDTH] ^= (1 << (y & 7));
                break;
        }
    }
}

void ssd1306_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    bool bSwap = false;
    switch (rotation) {
        case DISPLAY_ROTATE_90:
            // 90 degree rotation, swap x & y for rotation, then invert x
            bSwap = true;
            ssd1306_swap(x, y);
            x = DISP_WIDTH - x - 1;
            break;
        case DISPLAY_ROTATE_180:
            // 180 degree rotation, invert x and y, then shift y around for height.
            x = DISP_WIDTH - x - 1;
            y = DISP_HEIGHT - y - 1;
            x -= (w - 1);
            break;
        case DISPLAY_ROTATE_270:
            // 270 degree rotation, swap x & y for rotation,
            // then invert y and adjust y for w (not to become h)
            bSwap = true;
            ssd1306_swap(x, y);
            y = DISP_HEIGHT - y - 1;
            y -= (w - 1);
            break;
        case DISPLAY_ROTATE_0:
            break;  // do nothing
    }

    if (bSwap) {
        ssd1306_drawFastVLineInternal(x, y, w, color);
    } else {
        ssd1306_drawFastHLineInternal(x, y, w, color);
    }
}

void ssd1306_drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) {
    if ((y >= 0) && (y < DISP_HEIGHT)) { // Y coord in bounds?
        if (x < 0) {                  // Clip left
            w += x;
            x = 0;
        }
        if ((x + w) > DISP_WIDTH) { // Clip right
            w = (DISP_WIDTH - x);
        }
        if (w > 0) { // Proceed only if width is positive
            uint8_t *pBuf = &lcdBuffer[(y / 8) * DISP_WIDTH + x], mask = 1 << (y & 7);
            switch (color) {
                case CL_WHITE:
                    while (w--) {
                        *pBuf++ |= mask;
                    };
                    break;
                case CL_BLACK:
                    mask = ~mask;
                    while (w--) {
                        *pBuf++ &= mask;
                    };
                    break;
                case CL_INVERSE:
                    while (w--) {
                      *pBuf++ ^= mask;
                    };
                    break;
            }
        }
    }
}

void ssd1306_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    bool bSwap = false;
    switch (rotation) {
        case DISPLAY_ROTATE_90:
            // 90 degree rotation, swap x & y for rotation,
            // then invert x and adjust x for h (now to become w)
            bSwap = true;
            ssd1306_swap(x, y);
            x = DISP_WIDTH - x - 1;
            x -= (h - 1);
            break;
        case DISPLAY_ROTATE_180:
            // 180 degree rotation, invert x and y, then shift y around for height.
            x = DISP_WIDTH - x - 1;
            y = DISP_HEIGHT - y - 1;
            y -= (h - 1);
            break;
        case DISPLAY_ROTATE_270:
            // 270 degree rotation, swap x & y for rotation, then invert y
            bSwap = true;
            ssd1306_swap(x, y);
            y = DISP_HEIGHT - y - 1;
            break;
        case DISPLAY_ROTATE_0:
            break;  // do nothing
    }

    if (bSwap) {
        ssd1306_drawFastHLineInternal(x, y, h, color);
    }else {
        ssd1306_drawFastVLineInternal(x, y, h, color);
    }
}

static void ssd1306_drawFastVLineInternal(int16_t x, int16_t __y, int16_t __h, uint16_t color) {
    if ((x >= 0) && (x < DISP_WIDTH)) { // X coord in bounds?
        if (__y < 0) {               // Clip top
            __h += __y;
            __y = 0;
        }
        if ((__y + __h) > DISP_HEIGHT) { // Clip bottom
            __h = (DISP_HEIGHT - __y);
        }
        if (__h > 0) { // Proceed only if height is now positive
            // this display doesn't need ints for coordinates,
            // use local byte registers for faster juggling
            uint8_t y = __y, h = __h;
            uint8_t *pBuf = &lcdBuffer[(y / 8) * DISP_WIDTH + x];

            // do the first partial byte, if necessary - this requires some masking
            uint8_t mod = (y & 7);
            if (mod) {
                // mask off the high n bits we want to set
                mod = 8 - mod;
                // note - lookup table results in a nearly 10% performance
                // improvement in fill* functions
                // uint8_t mask = ~(0xFF >> mod);
                static const uint8_t premask[8] = {0x00, 0x80, 0xC0, 0xE0,
                                                       0xF0, 0xF8, 0xFC, 0xFE};
                uint8_t mask = premask[mod];
                // adjust the mask if we're not going to reach the end of this byte
                if (h < mod) {
                    mask &= (0XFF >> (mod - h));
                }
                switch (color) {
                    case CL_WHITE:
                        *pBuf |= mask;
                        break;
                    case CL_BLACK:
                        *pBuf &= ~mask;
                        break;
                    case CL_INVERSE:
                        *pBuf ^= mask;
                        break;
                }
                pBuf += DISP_WIDTH;
            }

            if (h >= mod) { // More to go?
                h -= mod;
                // Write solid bytes while we can - effectively 8 rows at a time
                if (h >= 8) {
                    if (color == CL_INVERSE) {
                        // separate copy of the code so we don't impact performance of
                        // black/white write version with an extra comparison per loop
                        do {
                            *pBuf ^= 0xFF; // Invert byte
                            pBuf += DISP_WIDTH; // Advance pointer 8 rows
                            h -= 8;        // Subtract 8 rows from height
                        } while (h >= 8);
                    } else {
                        // store a local value to work with
                        uint8_t val = (color != CL_BLACK) ? 255 : 0;
                        do {
                            *pBuf = val;   // Set byte
                            pBuf += DISP_WIDTH; // Advance pointer 8 rows
                            h -= 8;        // Subtract 8 rows from height
                        } while (h >= 8);
                    }
                }

                if (h) { // Do the final partial byte, if necessary
                    mod = h & 7;
                    // this time we want to mask the low bits of the byte,
                    // vs the high bits we did above
                    // uint8_t mask = (1 << mod) - 1;
                    // note - lookup table results in a nearly 10% performance
                    // improvement in fill* functions
                    static const uint8_t postmask[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F};
                    uint8_t mask = postmask[mod];
                    switch (color) {
                        case CL_WHITE:
                            *pBuf |= mask;
                            break;
                        case CL_BLACK:
                            *pBuf &= ~mask;
                            break;
                        case CL_INVERSE:
                            *pBuf ^= mask;
                            break;
                    }
                }
            }
        } // endif positive height
    }   // endif x in bounds
}

/*!
    @brief  Return color of a single pixel in display buffer.
    @param  x
            Column of display -- 0 at left to (screen width - 1) at right.
    @param  y
            Row of display -- 0 at top to (screen height -1) at bottom.
    @return true if pixel is set (usually SSD1306_WHITE, unless display invert
   mode is enabled), false if clear (SSD1306_BLACK).
    @note   Reads from buffer contents; may not reflect current contents of
            screen if display() has not been called.
*/
bool ssd1306_getPixel(int16_t x, int16_t y) {
    if ((x >= 0) && (x < _width) && (y >= 0) && (y < _height)) {
        // Pixel is in-bounds. Rotate coordinates if needed.
        switch (rotation) {
            case DISPLAY_ROTATE_90:
                ssd1306_swap(x, y);
                x = DISP_WIDTH - x - 1;
                break;
            case DISPLAY_ROTATE_180:
                x = DISP_WIDTH - x - 1;
                y = DISP_HEIGHT - y - 1;
                break;
            case DISPLAY_ROTATE_270:
                ssd1306_swap(x, y);
                y = DISP_HEIGHT - y - 1;
                break;
            case DISPLAY_ROTATE_0:
                break;  // do nothing
        }
        return (lcdBuffer[x + (y / 8) * DISP_WIDTH] & (1 << (y & 7)));
    }
    return false; // Pixel out of bounds
}

static displayRotation * ssd1306_getRotationPointer(void) {
    return &rotation;
}

static int16_t * ssd1306_getWidthPointer(void) {
    return &_width;
}

static int16_t * ssd1306_getHeightPointer(void) {
    return &_height;
}
