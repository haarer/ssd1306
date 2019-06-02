/*
    MIT License

    Copyright (c) 2018, Alexey Dynda

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

/*
 * @file lcd_hal/avr/io.h SSD1306 AVR IO communication functions
 */

#ifndef _SSD1306V2_AVR_IO_H_
#define _SSD1306V2_AVR_IO_H_

#include "../UserSettings.h"
#include "../interface.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef LOW
#define LOW  0
#endif

#ifndef HIGH
#define HIGH 1
#endif

#ifndef INPUT
#define INPUT 0
#endif

#ifndef OUTPUT
#define OUTPUT 1
#endif

#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || \
    defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    /** The macro is defined when software i2c implementation is available */
    #define CONFIG_SOFTWARE_I2C_AVAILABLE
    /** The macro is defined when USI module is available for use */
    #define CONFIG_USI_SPI_AVAILABLE
    /* Define lcdint as smallest types to reduce memo usage on tiny controllers. *
     * Remember, that this can cause issues with large lcd displays, i.e. 320x240*/
    #define LCDINT_TYPES_DEFINED
    typedef int8_t lcdint_t;
    typedef uint8_t lcduint_t;
    /** The macro is defined when micro controller doesn't support multiplication operation */
    #define CONFIG_MULTIPLICATION_NOT_SUPPORTED

#elif defined(__AVR_ATmega328P__)
    /** The macro is defined when software i2c implementation is available */
    #define CONFIG_SOFTWARE_I2C_AVAILABLE
    /** The macro is defined when TWI module is available */
    #define CONFIG_TWI_I2C_AVAILABLE
    /** The macro is defined when SPI module is available */
    #define CONFIG_AVR_SPI_AVAILABLE
    /** The macro is defined when UART module is available */
    #define CONFIG_AVR_UART_AVAILABLE
    /** The macro is defined when VGA monitor control is available directly from controller */
    #define CONFIG_VGA_AVAILABLE

#else
    /** The macro is defined when software i2c implementation is available */
    #define CONFIG_SOFTWARE_I2C_AVAILABLE
    /** The macro is defined when TWI module is available */
    #define CONFIG_TWI_I2C_AVAILABLE
    /** The macro is defined when SPI module is available */
    #define CONFIG_AVR_SPI_AVAILABLE

#endif

#ifdef __cplusplus
extern "C" {
#endif

void pinMode(int pin, int mode);
static inline int  digitalRead(int pin) { return LOW; };
void digitalWrite(int pin, int level);
static inline int  analogRead(int pin) { return 0; };
static inline uint32_t millis() { return 0; };
static inline void randomSeed(int seed) { };
static inline void attachInterrupt(int pin, void (*interrupt)(), int level) { };
void delay(unsigned long ms);
static inline void delayMicroseconds(unsigned long us) { _delay_us(us); };


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
static inline int random(int max) { return 0; };
static inline int random(int min, int max) { return 0; };
#endif

#endif

