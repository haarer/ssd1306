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
 * @file oled_ssd1306.h support for OLED ssd1306-based displays
 */


#ifndef _OLED_SSD1306_V2_H_
#define _OLED_SSD1306_V2_H_

#include "lcd_hal/io.h"
#include "v2/lcd/base/display.h"
#include "v2/lcd/lcd_common.h"

/**
 * @ingroup LCD_INTERFACE_API_V2
 * @{
 */

/**
 * Class implements interface functions to SSD1306 displays
 */
template <class I>
class InterfaceSSD1306: public I
{
public:
    /**
     * Creates instance of interface to LCD display.
     *
     * @param base Reference to base class, which represents Display
     * @param dc Data/command control pin number, for i2c communication should be -1
     * @param data variable argument list, accepted by platform interface (PlatformI2c, PlatformSpi)
     */
    template <typename... Args>
    InterfaceSSD1306(NanoDisplayBase<InterfaceSSD1306<I>> &base, int8_t dc, Args&&... data)
        : I(data...)
        , m_dc( dc )
        , m_base(base)
    {
    }

    /**
     * @brief Sets block in RAM of lcd display controller to write data to.
     *
     * Sets block in RAM of lcd display controller to write data to.
     * For ssd1306 it uses horizontal addressing mode, while for
     * sh1106 the function uses page addressing mode.
     * Width can be specified as 0, thus the library will set the right boundary to
     * region of RAM block to the right column of the display.
     * @param x - column (left region)
     * @param y - page (top page of the block)
     * @param w - width of the block in pixels to control
     *
     * @warning - this function initiates session (i2c or spi) and does not close it.
     *            To close session, please, call endBlock().
     */
    void startBlock(lcduint_t x, lcduint_t y, lcduint_t w) __attribute__ ((noinline));

    /**
     * Switches to the start of next RAM page for the block, specified by
     * startBlock().
     * For ssd1306 it does nothing, while for sh1106 the function moves cursor to
     * next page.
     */
    void nextBlock() {}

    /**
     * Closes data send operation to lcd display.
     */
    void endBlock();

    /**
     * Enables either data or command mode on SPI bus
     * @param mode 1 to enable data mode, or 0 to enable command mode
     */
    void spiDataMode(uint8_t mode);

    /**
     * Starts communication with LCD display in command mode.
     * To stop communication use m_intf.end().
     */
    void commandStart();

    /**
     * Sets start line in gdram to start display content with
     *
     * @param line start line in range 0 - 63
     */
    void setStartLine(uint8_t line);

    /**
     * returns start line in gdram.
     */
    uint8_t getStartLine();

    /**
     * Switches display to normal mode. This feature is specific
     * for ssd1306 based controllers.
     */
    void normalMode();

    /**
     * Switches display to normal mode. This feature is specific
     * for ssd1306 based controllers.
     */
    void invertMode();

    /**
     * Set display contrast, ie light intensity
     * @param contrast - contrast value to see, refer to ssd1306 datasheet
     */
    void setContrast(uint8_t contrast);

    /**
     * Turns off display
     */
    void displayOff();

    /**
     * Turns on display
     */
    void displayOn();

    /**
     * @brief performs horizontal flip
     *
     * Performs horizontal flip. If you need to turn display by 180 degree,
     * please use both flipHorizontal() and flipVertical().
     *
     * @param mode - 0 to disable horizontal flip
     *               1 to enable horizontal flip
     */
    void flipHorizontal(uint8_t mode);

    /**
     * @brief performs vertical flip
     *
     * Performs vertical flip. If you need to turn display by 180 degree,
     * please use both flipHorizontal() and flipVertical().
     *
     * @param mode - 0 to disable vertical flip
     *               1 to enable vertical flip
     */
    void flipVertical(uint8_t mode);

private:
    int8_t m_dc = -1; ///< data/command pin for SPI, -1 for i2c
    NanoDisplayBase<InterfaceSSD1306<I>> &m_base; ///< basic lcd display support interface
    uint8_t m_startLine = 0;
};

/**
 * Generic interface to ssd1306-based controllers
 */
template <class I>
class DisplaySSD1306: public NanoDisplayOps<NanoDisplayOps1<I>, I>
{
public:
    /**
     * Created object instance to control ssd1306-based displays
     *
     * @param intf reference to communication interface to use
     * @param rstPin reset pin number, or -1 if not used
     */
    DisplaySSD1306(I &intf, int8_t rstPin = -1)
        : NanoDisplayOps<NanoDisplayOps1<I>, I>(intf)
        , m_rstPin( rstPin ) { }

protected:
    int8_t m_rstPin; ///< Reset pin number if used

};

/**
 * Class implements interface to 128x64 ssd1306 monochrome display.
 */
template <class I>
class DisplaySSD1306_128x64: public DisplaySSD1306<I>
{
public:
    using DisplaySSD1306<I>::DisplaySSD1306;

    /**
     * Initializes 128x64 lcd display. Interface should be ready prior to
     * this function call
     */
    void begin() override;

    /**
     * Closes display connection
     */
    void end() override;
};

/**
 * Class implements interface to 128x32 ssd1306 monochrome display.
 */
template <class I>
class DisplaySSD1306_128x32: public DisplaySSD1306<I>
{
public:
    using DisplaySSD1306<I>::DisplaySSD1306;

    /**
     * Initializes 128x32 lcd display. Interface should be ready prior to
     * this function call
     */
    void begin() override;

    /**
     * Closes display connection
     */
    void end() override;
};

/**
 * Class implements interface to 128x64 ssd1306 i2c monochrome display.
 */
class DisplaySSD1306_128x64_I2C: public DisplaySSD1306_128x64<InterfaceSSD1306<PlatformI2c>>
{
public:
    /**
     * @brief Inits 128x64 OLED display over i2c (based on SSD1306 controller).
     *
     * Inits 128x64 OLED display over i2c (based on SSD1306 controller)
     * This function uses hardcoded pins for i2c communication, depending on your hardware.
     *
     * @param config platform i2c configuration. Please refer to SPlatformI2cConfig.
     */
    DisplaySSD1306_128x64_I2C( const SPlatformI2cConfig &config = { -1, 0x3C, -1, -1, 0 } )
        : DisplaySSD1306_128x64(m_i2c, -1)
        , m_i2c( *this, -1,
                 SPlatformI2cConfig{ config.busId,
                                     config.addr ?: (uint8_t)0x3C,
                                     config.scl,
                                     config.sda,
                                     config.frequency ?: 400000 } ) {}

    /**
     * Initializes LCD display over I2C.
     */
    void begin() override;

    /**
     * Closes display connection
     */
    void end() override;

private:
    InterfaceSSD1306<PlatformI2c> m_i2c;
};

/**
 * Class implements interface to 128x32 ssd1306 i2c monochrome display.
 */
class DisplaySSD1306_128x32_I2C: public DisplaySSD1306_128x32<InterfaceSSD1306<PlatformI2c>>
{
public:
    /**
     * @brief Inits 128x32 OLED display over i2c (based on SSD1306 controller).
     *
     * Inits 128x32 OLED display over i2c (based on SSD1306 controller)
     * This function uses hardcoded pins for i2c communication, depending on your hardware.
     *
     * @param config platform i2c configuration. Please refer to SPlatformI2cConfig.
     */
    DisplaySSD1306_128x32_I2C( const SPlatformI2cConfig &config = { -1, 0x3C, -1, -1, 0 } )
        : DisplaySSD1306_128x32(m_i2c, -1)
        , m_i2c( *this, -1,
                 SPlatformI2cConfig{ config.busId,
                                     config.addr ?: (uint8_t)0x3C,
                                     config.scl,
                                     config.sda,
                                     config.frequency ?: 400000 } ) {}

    void begin() override;

    void end() override;

private:
    InterfaceSSD1306<PlatformI2c> m_i2c;
};

/**
 * Class implements interface to 128x64 ssd1306 spi monochrome display.
 */
class DisplaySSD1306_128x64_SPI: public DisplaySSD1306_128x64<InterfaceSSD1306<PlatformSpi>>
{
public:
    /**
     * @brief Inits 128x64 OLED display over spi (based on SSD1306 controller).
     *
     * Inits 128x64 OLED display over spi (based on SSD1306 controller)
     * @param rstPin pin controlling LCD reset (-1 if not used)
     * @param config platform spi configuration. Please refer to SPlatformI2cConfig.
     */
    DisplaySSD1306_128x64_SPI( int8_t rstPin, const SPlatformSpiConfig &config = { -1, -1, -1, 0, -1, -1 } )
        : DisplaySSD1306_128x64( m_spi, rstPin )
        , m_spi( *this, config.dc,
                 SPlatformSpiConfig{ config.busId,
                                     config.cs,
                                     config.dc,
                                     config.frequency ?: 10000000,
                                     config.scl,
                                     config.sda } ) {}

    void begin() override;

    void end() override;

private:
    InterfaceSSD1306<PlatformSpi> m_spi;
};

/**
 * Class implements interface to 128x32 ssd1306 spi monochrome display.
 */
class DisplaySSD1306_128x32_SPI: public DisplaySSD1306_128x32<InterfaceSSD1306<PlatformSpi>>
{
public:
    /**
     * @brief Inits 128x32 OLED display over spi (based on SSD1306 controller).
     *
     * Inits 128x32 OLED display over spi (based on SSD1306 controller)
     * @param rstPin pin controlling LCD reset (-1 if not used)
     * @param config platform spi configuration. Please refer to SPlatformI2cConfig.
     */
    DisplaySSD1306_128x32_SPI( int8_t rstPin, const SPlatformSpiConfig &config = { -1, -1, -1, 0, -1, -1 } )
        : DisplaySSD1306_128x32( m_spi, rstPin )
        , m_spi( *this, config.dc,
                 SPlatformSpiConfig{ config.busId,
                                     config.cs,
                                     config.dc,
                                     config.frequency ?: 10000000,
                                     config.scl,
                                     config.sda } ) {}

    void begin() override;

    void end() override;

private:
    InterfaceSSD1306<PlatformSpi> m_spi;
};

#include "lcd_ssd1306.inl"

/**
 * @}
 */

// ----------------------------------------------------------------------------
#endif // _OLED_SSD1306_V2_H_
