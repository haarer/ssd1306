/*
    MIT License

    Copyright (c) 2017-2019, Alexey Dynda

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
 * @file lcd_common.h global lcd settings
 */

#ifndef _LCD_COMMON_H_
#define _LCD_COMMON_H_

#include "ssd1306_hal/io.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup LCD_INTERFACE_API OLEDs: initialization and service functions
 * @{
 * @brief LCD initialization and service functions
 *
 * @details This group contains API functions for OLED displays initialization and
 *          direct programming of GDRAM. This API can be used to create your own
 *          graphics functions.
 */

/**
 * @brief Does hardware reset for oled controller.
 *
 * Does hardware reset for oled controller. The function pulls up rstPin
 * for 10 milliseconds, then pulls down rstPin for delayMs milliseconds
 * and then finally pulls rstPin up.
 *
 * @param rstPin reset pin number. If -1, then reset will not be performed
 * @param delayMs delay in milliseconds to hold rstPin in low state
 */
void ssd1306_resetController(int8_t rstPin, uint8_t delayMs);

#if 0

/**
 * Macro CONTROLLER_NATIVE_SPI_BLOCK_8BIT_CMDS() generates 2 static functions,
 * applicable for many oled controllers with 8-bit commands:
 * set_block_native(), next_page_native(). These functions are to be used
 * when working in oled controller native mode.
 * @param column_cmd command opcode for setting column address according to
 *        oled controller datasheet
 * @param row_cmd command opcode for setting row address according to
 *        oled controller datasheet
 * @note It is assumed that column and row commands accept 2 single byte
 *       arguments: start and end of region
 */
#define CONTROLLER_NATIVE_SPI_BLOCK_8BIT_CMDS(column_cmd, row_cmd) \
    static void set_block_native(lcduint_t x, lcduint_t y, lcduint_t w) \
    { \
        uint8_t rx = w ? (x + w - 1) : (ssd1306_lcd.width - 1); \
        ssd1306_intf.start(); \
        ssd1306_spiDataMode(0); \
        ssd1306_intf.send(column_cmd); \
        ssd1306_intf.send(x); \
        ssd1306_intf.send(rx < ssd1306_lcd.width ? rx : (ssd1306_lcd.width - 1)); \
        ssd1306_intf.send(row_cmd); \
        ssd1306_intf.send(y); \
        ssd1306_intf.send(ssd1306_lcd.height - 1); \
        ssd1306_spiDataMode(1); \
    } \
    static void next_page_native(void) \
    { \
    } \

/**
 * Macro SSD1306_COMPAT_SEND_PIXELS_RGB8_CMDS() generates 2 static functions,
 * applicable for many oled controllers in 8-bit RGB mode:
 * send_pixels_compat(), send_pixels_buffer_compat(). These functions are to be used
 * when working in ssd1306 compatible mode.
 */
#define SSD1306_COMPAT_SEND_PIXELS_RGB8_CMDS() \
    extern uint16_t ssd1306_color; \
    static void send_pixels_compat(uint8_t data) \
    { \
        for (uint8_t i=8; i>0; i--) \
        { \
            if ( data & 0x01 ) \
            { \
                ssd1306_intf.send( (uint8_t)ssd1306_color ); \
            } \
            else \
            { \
                ssd1306_intf.send( 0B00000000 ); \
            } \
            data >>= 1; \
        } \
    } \
    static void send_pixels_buffer_compat(const uint8_t *buffer, uint16_t len) \
    { \
        while(len--) \
        { \
            send_pixels_compat(*buffer); \
            buffer++; \
        } \
    }

/**
 * Macro SSD1306_COMPAT_SEND_PIXELS_RGB16_CMDS() generates 2 static functions,
 * applicable for many oled controllers in 16-bit RGB mode:
 * send_pixels_compat16(), send_pixels_buffer_compat16(). These functions are to be used
 * when working in ssd1306 compatible mode.
 */
#define SSD1306_COMPAT_SEND_PIXELS_RGB16_CMDS() \
    extern uint16_t ssd1306_color; \
    static void send_pixels_compat16(uint8_t data) \
    { \
        for (uint8_t i=8; i>0; i--) \
        { \
            if ( data & 0x01 ) \
            { \
                ssd1306_intf.send( (uint8_t)(ssd1306_color >> 8 ) ); \
                ssd1306_intf.send( (uint8_t)(ssd1306_color & 0xFF) ); \
            } \
            else \
            { \
                ssd1306_intf.send( 0B00000000 ); \
                ssd1306_intf.send( 0B00000000 ); \
            } \
            data >>= 1; \
        } \
    } \
    static void send_pixels_buffer_compat16(const uint8_t *buffer, uint16_t len) \
    { \
        while(len--) \
        { \
            send_pixels_compat16(*buffer); \
            buffer++; \
        } \
    }

#endif

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _LCD_COMMON_H_ */
