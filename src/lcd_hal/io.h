/*
    MIT License

    Copyright (c) 2018-2019, Alexey Dynda

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
 * @file lcd_hal/io.h SSD1306 HAL IO communication functions
 */

#ifndef _LCD_HAL_IO_H_
#define _LCD_HAL_IO_H_

// ARDUINO_COMPAT

/**
 * @defgroup SSD1306_HAL_API HAL: ssd1306 library hardware abstraction layer
 * @{
 *
 * @brief i2c/spi ssd1306 library hardware abstraction layer
 *
 * @details ssd1306 library hardware abstraction layer
 */

#include "UserSettings.h"
#include "interface.h"

#if defined(ARDUINO)
#include "arduino/io.h"
#ifdef __cplusplus
#include "arduino/arduino_spi.h"
#include "arduino/arduino_wire.h"
#include "avr/avr_spi.h"
#include "avr/avr_twi.h"
#include "avr/ssd1306_spi_usi.h"
#include "avr/ssd1306_i2c_embedded.h"
#include "esp/esp32_i2c.h"
#include "esp/esp32_spi.h"
#endif
#elif defined(__AVR__) && !defined(ARDUINO)
#include "avr/io.h"
#ifdef __cplusplus
#include "avr/avr_spi.h"
#include "avr/avr_twi.h"
#include "avr/ssd1306_spi_usi.h"
#include "avr/ssd1306_i2c_embedded.h"
#endif
#elif defined(__XTENSA__) && !defined(ARDUINO)
#include "esp/io.h"
#ifdef __cplusplus
#include "esp/esp32_i2c.h"
#include "esp/esp32_spi.h"
#endif
#elif defined(STM32F1) || defined(STM32F2) || defined(STM32F4)
#include "stm32/io.h"
#elif defined(__linux__)
#include "linux/io.h"
#ifdef __cplusplus
#include "linux/linux_i2c.h"
#include "linux/linux_spi.h"
#include "linux/sdl_i2c.h"
#include "linux/sdl_spi.h"
#endif
#elif defined(__MINGW32__)
#include "mingw/io.h"
#ifdef __cplusplus
#include "linux/sdl_i2c.h"
#include "linux/sdl_spi.h"
#endif
#else
#warning "Platform is not supported. Use template to add support"
#endif

#ifndef LCDINT_TYPES_DEFINED
/** Macro informs if lcdint_t type is defined */
#define LCDINT_TYPES_DEFINED
/** internal int type, used by ssd1306 library. Important for uC with low SRAM */
typedef int lcdint_t;
/** internal int type, used by ssd1306 library. Important for uC with low SRAM */
typedef unsigned int lcduint_t;
#endif

/** swaps content of a and b variables of type type */
#define ssd1306_swap_data(a, b, type)  { type t = a; a = b; b = t; }

#ifdef __cplusplus

////////////////////////////////////////////////////////////////////////////////
//                    I2C PLATFORM
////////////////////////////////////////////////////////////////////////////////

#if defined(CONFIG_ARDUINO_I2C_AVAILABLE) && defined(CONFIG_ARDUINO_I2C_ENABLE)
/**
 * PlatformI2c implementation for current platform.
 */
class PlatformI2c: public ArduinoI2c
{
public:
    /**
     * Creates instance of i2c implementation for current platform.
     * @param config i2c platform configuration. Refer to SPlatformI2cConfig.
     */
     PlatformI2c(const SPlatformI2cConfig &config)
         : ArduinoI2c(config.scl, config.sda, config.addr) {}
};

#elif defined(CONFIG_TWI_I2C_AVAILABLE) && defined(CONFIG_TWI_I2C_ENABLE)

/**
 * PlatformI2c implementation for current platform.
 */
class PlatformI2c: public TwiI2c
{
public:
    /**
     * Creates instance of i2c implementation for current platform.
     * @param config i2c platform configuration. Refer to SPlatformI2cConfig.
     */
    PlatformI2c(const SPlatformI2cConfig &config)
        : TwiI2c(config.addr) {}
};

#elif defined(CONFIG_LINUX_I2C_AVAILABLE) && defined(CONFIG_LINUX_I2C_ENABLE)

#if defined(SDL_EMULATION)
/**
 * PlatformI2c implementation for current platform.
 */
class PlatformI2c: public SdlI2c
{
public:
    /**
     * Creates instance of i2c implementation for current platform.
     * @param config i2c platform configuration. Refer to SPlatformI2cConfig.
     */
    PlatformI2c(const SPlatformI2cConfig &config)
        : SdlI2c(config.scl, config.sda, config.addr) {}
};
#else
/**
 * PlatformI2c implementation for current platform.
 */
class PlatformI2c: public LinuxI2c
{
public:
    /**
     * Creates instance of i2c implementation for current platform.
     * @param config i2c platform configuration. Refer to SPlatformI2cConfig.
     */
    PlatformI2c(const SPlatformI2cConfig &config)
        : LinuxI2c( config.busId, config.addr ) {}
};
#endif

#elif defined(CONFIG_ESP32_I2C_AVAILABLE) && defined(CONFIG_ESP32_I2C_ENABLE)

/**
 * PlatformI2c implementation for current platform.
 */
class PlatformI2c: public EspI2c
{
public:
    /**
     * Creates instance of i2c implementation for current platform.
     * @param config i2c platform configuration. Refer to SPlatformI2cConfig.
     */
    PlatformI2c(const SPlatformI2cConfig &config)
        : EspI2c( config.busId, config.addr, config.scl, config.sda, 400000) {}
};

#elif defined(CONFIG_SOFTWARE_I2C_AVAILABLE) && defined(CONFIG_SOFTWARE_I2C_ENABLE)

/**
 * PlatformI2c implementation for current platform.
 */
class PlatformI2c: public SoftwareI2c
{
public:
    /**
     * Creates instance of i2c implementation for current platform.
     * @param config i2c platform configuration. Refer to SPlatformI2cConfig.
     */
     PlatformI2c(const SPlatformI2cConfig &config)
         : SoftwareI2c(config.scl, config.sda, config.addr) {}
};

#else

#error "Platform not supported"

#endif

////////////////////////////////////////////////////////////////////////////////
//                    SPI PLATFORM
////////////////////////////////////////////////////////////////////////////////

#if defined(CONFIG_AVR_SPI_AVAILABLE) && defined(CONFIG_AVR_SPI_ENABLE)

/**
 * PlatformSpi implementation for current platform
 */
class PlatformSpi: public AvrSpi
{
public:
    /**
     * Creates instance of PlatformSpi implementation for current platform
     * @param config spi platform configuration. Refer to SPlatformSpiConfig.
     */
    PlatformSpi(const SPlatformSpiConfig &config)
        : AvrSpi( config.cs, config.dc, config.frequency ) {}
};

#elif defined(CONFIG_ARDUINO_SPI_AVAILABLE) && defined(CONFIG_ARDUINO_SPI_ENABLE)

/**
 * PlatformSpi implementation for current platform
 */
class PlatformSpi: public ArduinoSpi
{
public:
    /**
     * Creates instance of PlatformSpi implementation for current platform
     * @param config spi platform configuration. Refer to SPlatformSpiConfig.
     */
    PlatformSpi(const SPlatformSpiConfig &config)
        : ArduinoSpi(config.cs, config.dc, config.frequency) {}
};

#elif defined(CONFIG_LINUX_SPI_AVAILABLE) && defined(CONFIG_LINUX_SPI_ENABLE)

#if defined(SDL_EMULATION)
/**
 * PlatformSpi implementation for current platform
 */
class PlatformSpi: public SdlSpi
{
public:
    /**
     * Creates instance of PlatformSpi implementation for current platform
     * @param config spi platform configuration. Refer to SPlatformSpiConfig.
     */
    PlatformSpi(const SPlatformSpiConfig &config)
        : SdlSpi(config.dc) {}
};
#else
/**
 * PlatformSpi implementation for current platform
 */
class PlatformSpi: public LinuxSpi
{
public:
    /**
     * Creates instance of PlatformSpi implementation for current platform
     * @param config spi platform configuration. Refer to SPlatformSpiConfig.
     */
    PlatformSpi(const SPlatformSpiConfig &config)
        : LinuxSpi( config.busId, config.devId, config.dc, config.frequency ) {}
};
#endif

#elif defined(CONFIG_ESP32_SPI_AVAILABLE) && defined(CONFIG_ESP32_SPI_ENABLE)
/**
 * PlatformSpi implementation for current platform
 */
class PlatformSpi: public EspSpi
{
public:
    /**
     * Creates instance of PlatformSpi implementation for current platform
     * @param config spi platform configuration. Refer to SPlatformSpiConfig.
     */
    PlatformSpi(const SPlatformSpiConfig &config)
        : EspSpi( config.busId, config.cs, config.dc, config.frequency ) {}
};

#elif defined(CONFIG_USI_SPI_AVAILABLE) && defined(CONFIG_USI_SPI_ENABLE)

/**
 * PlatformSpi implementation for current platform
 */
class PlatformSpi: public UsiSpi
{
public:
    /**
     * Creates instance of PlatformSpi implementation for current platform
     * @param config spi platform configuration. Refer to SPlatformSpiConfig.
     */
    PlatformSpi(const SPlatformSpiConfig &config)
        : UsiSpi( config.cs, config.dc ) {}
};

#else

#error "Platform not supported"

#endif



extern "C" {
#endif // end ifdef __cplusplus

// !!! PLATFORM I2C IMPLEMENTATION OPTIONAL !!!

/**
 * Structure describes i2c pins
 */
typedef struct
{
    int8_t sda; ///< data pin
    int8_t scl; ///< clock pin
} ssd1306_platform_i2cConfig_t;

/**
 * @brief Initializes i2c interface for platform being used.
 *
 * Initializes i2c interface for platform being used. i2c implementation
 * depends on platform.
 *
 * @param busId i2c bus number. Some platforms have several i2c buses. so, this
 *        argument identifies bus to use. For several platforms busId is not used.
 *        If you want to use default i2c bus for specific platform, please pass -1.
 * @param addr i2c address of oled driver, connected to i2c bus. If you want to use default
 *        i2c display address, please, pass 0.
 * @param cfg Specify scl and sda for the platform. If you want to use default pin numbers,
 *        please pass -1 for both members.
 */
void ssd1306_platform_i2cInit(int8_t busId, uint8_t addr, ssd1306_platform_i2cConfig_t * cfg);

// !!! PLATFORM SPI IMPLEMENTATION OPTIONAL !!!

/**
 * @brief Initializes spi interface for platform being used.
 *
 * Initializes spi interface for platform being used. spi implementation
 * depends on platform.
 *
 * @param busId spi bus number if device has several spi buses. For most AVR platforms
 *        busId is not used. If you want to use default spi bus for specific platform, please
 *        pass -1.
 * @param cesPin chip select (chip enable) pin number. If you want to use default pin,
 *        hard coded by ssd1306 library, please, pass -1. For Linux platform cesPin means
 *        second number in spidev device.
 * @param dcPin data/command pin number. For most oled displays this pin number is used to
 *        select data or command mode for the bus. If you want to use default pin number, please
 *        pass -1.
 */
void ssd1306_platform_spiInit(int8_t busId, int8_t cesPin, int8_t dcPin);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif


