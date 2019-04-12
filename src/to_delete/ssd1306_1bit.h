/*
    MIT License

    Copyright (c) 2016-2019, Alexey Dynda

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
/**
 * @file ssd1306_1bit.h SSD1306 basic draw functions
 */


#ifndef _SSD1306_1BIT_H_
#define _SSD1306_1BIT_H_

#include "nano_gfx_types.h"

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////
//                 DIRECT GRAPH FUNCTIONS
///////////////////////////////////////////////////////////////////////

#if 0

/**
 * @defgroup LCD_1BIT_GRAPHICS DIRECT DRAW: 1-bit graphic functions for ssd1306 compatible mode.
 * @{
 * @brief LCD direct draw functions for all display types: color and monochrome.
 *
 * @details LCD direct draw functions are applicable for all display types. These functions will work
 *        both for monochrome and 8-bit/16-bit color OLED displays. You need remember, that for RGB
 *        oled displays these functions work only in special ssd1306 compatible mode. If you're going to
 *        combine NanoEngine capabilities with these functions, don't forget to switch addressing
 *        mode via ssd1306_setMode().
 *        Direct draw functions draw directly in GDRAM and do not use any double-buffering.
 */

#ifndef DOXYGEN_SHOULD_SKIP_THIS
uint8_t ssd1306_printFixed_oldStyle(uint8_t xpos, uint8_t y, const char *ch, EFontStyle style);
#endif


/**
 * @brief Prints single character to display at current cursor position
 *
 * Prints single character to display at current cursor position
 * @param ch - character to print to the display. 'LF' and 'CR' are skipped
 * @return returns number of printed characters.
 */
size_t      ssd1306_write(uint8_t ch);

/**
 * @brief Prints null-terminated string to display at current cursor position
 *
 * Prints null-terminated string to display at current cursor position
 * @param ch - string to print to the display. 'LF' and 'CR' are skipped
 * @return returns number of printed characters.
 */
size_t      ssd1306_print(const char ch[]);

/**
 * Put single pixel on the LCD.
 *
 * @warning Please, take into account that there is no way
 * to read data from ssd1306, thus since each byte contains
 * 8 pixels, all other pixels in the same byte will be cleared
 * on the display. Use ssd1306_putPixels() instead.
 * If you need to have buffered output, please, refer to NanoCanvas.
 *
 * @param x - horizontal position in pixels
 * @param y - vertical position in pixels
 */
void         ssd1306_putPixel(uint8_t x, uint8_t y);

/**
 * Puts eight vertical pixels on the LCD at once.
 *
 * ~~~~~~~~~~~~~~~{.c}
 * // Draw 8 vertical pixels, starting at [10,16] position
 * ssd1306_putPixels(10,2,0xFF);
 * // Draw 4 vertical pixels, starting at [32,28] position
 * ssd1306_putPixels(32,3,0x0F);
 * ~~~~~~~~~~~~~~~
 *
 * @param x - horizontal position in pixels
 * @param y - vertical position pixels. Should be multiply of 8.
 * @param pixels - bit-pixels to draw on display
 */
void         ssd1306_putPixels(uint8_t x, uint8_t y, uint8_t pixels);

/**
 * Draws rectangle
 * @param x1 - left boundary in pixel units
 * @param y1 - top boundary in pixel units
 * @param x2 - right boundary in pixel units
 * @param y2 - bottom boundary int pixel units
 */
void         ssd1306_drawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

/**
 * Draws line
 * @param x1 - x position in pixels of start point
 * @param y1 - y position in pixels of start point
 * @param x2 - x position in pixels of end point
 * @param y2 - y position in pixels of end point
 *
 * @warning Remember that this function draws line directly in GDRAM of oled controller.
 *          Since there is no way to detect pixels already being displayed, some pixels
 *          can be overwritten by black color. If you use RGB oled, based on ssd1331 controller,
 *          use ssd1331_drawLine() function.
 */
void         ssd1306_drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

/**
 * Draws horizontal line
 * @param x1 - left boundary in pixels
 * @param y1 - position Y in pixels
 * @param x2 - right boundary in pixels
 */
void         ssd1306_drawHLine(uint8_t x1, uint8_t y1, uint8_t x2);

/**
 * Draws vertical line
 * @param x1 - position X in pixels
 * @param y1 - top boundary in pixels
 * @param y2 - bottom boundary in pixels
 */
void         ssd1306_drawVLine(uint8_t x1, uint8_t y1, uint8_t y2);

/**
 * Draws bitmap, located in SRAM, on the display
 * Each byte represents 8 vertical pixels.
 *
 * ~~~~~~~~~~~~~~~{.c}
 * // Draw small rectangle 3x8 at position 10,8
 * uint8_t buffer[3] = { 0xFF, 0x81, 0xFF };
 * ssd1306_drawBuffer(10, 1, 3, 8, buffer);
 * ~~~~~~~~~~~~~~~
 *
 * @param x - horizontal position in pixels
 * @param y - vertical position in blocks (pixels/8)
 * @param w - width of bitmap in pixels
 * @param h - height of bitmap in pixels (must be divided by 8)
 * @param buf - pointer to data, located in SRAM: each byte represents 8 vertical pixels.
 */
void         ssd1306_drawBuffer(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *buf);

/**
 * Draws bitmap, located in SRAM, on the display
 * Each byte represents 8 vertical pixels.
 *
 * ~~~~~~~~~~~~~~~{.c}
 * // Draw small rectangle 3x8 at position 10,8
 * uint8_t buffer[3] = { 0xFF, 0x81, 0xFF };
 * ssd1306_drawBuffer(10, 1, 3, 8, buffer);
 * ~~~~~~~~~~~~~~~
 *
 * @param x - horizontal position in pixels
 * @param y - vertical position in pixels (must be devided by 8)
 * @param w - width of bitmap in pixels
 * @param h - height of bitmap in pixels (must be divided by 8)
 * @param buf - pointer to data, located in SRAM: each byte represents 8 vertical pixels.
 *
 * @note ssd1306_drawBufferFast() doesn't support negative draw mode. Use ssd1306_drawBuffer() instead.
 */
void         ssd1306_drawBufferFast(lcdint_t x, lcdint_t y, lcduint_t w, lcduint_t h, const uint8_t *buf);

/**
 * @copydoc ssd1306_drawBuffer
 */
#define      ssd1306_drawCanvas(x, y, w, h, buf)   ssd1306_drawBuffer(x, y, w, h, buf)

/**
 * Draws bitmap, located in Flash, on the display
 *
 * @param x - horizontal position in pixels
 * @param y - vertical position in blocks (pixels/8)
 * @param w - width of bitmap in pixels
 * @param h - height of bitmap in pixels (must be divided by 8)
 * @param buf - pointer to data, located in Flash: each byte represents 8 vertical pixels.
 */
void         ssd1306_drawBitmap(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *buf);

/**
 * Draws bitmap, located in Flash, on the display
 *
 * @param x - horizontal position in pixels
 * @param y - vertical position in pixels
 * @param w - width of bitmap in pixels
 * @param h - height of bitmap in pixels (must be divided by 8)
 * @param buf - pointer to data, located in Flash: each byte represents 8 vertical pixels.
 */
void         gfx_drawMonoBitmap(lcdint_t x, lcdint_t y, lcduint_t w, lcduint_t h, const uint8_t *buf);

/**
 * Fills block with black pixels
 * @param x - horizontal position in pixels
 * @param y - vertical position in blocks (pixels/8)
 * @param w - width of block in pixels
 * @param h - height of block in pixels (must be divided by 8)
 * @note usually this method is used to erase bitmap on the screen.
 */
void         ssd1306_clearBlock(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

#endif

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // _SSD1306_1BIT_H_
