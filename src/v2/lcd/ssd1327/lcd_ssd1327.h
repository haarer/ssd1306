/*
    MIT License

    Copyright (c) 2019, Alexey Dynda

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
 * @file lcd_ssd1327.h support for LCD SSD1327 display
 */

#ifndef _LCD_SSD1327_V2_H_
#define _LCD_SSD1327_V2_H_

#include "lcd_hal/io.h"
#include "v2/lcd/lcd_common.h"
#include "v2/lcd/base/display.h"

/**
 * @ingroup LCD_INTERFACE_API_V2
 * @{
 */

/**
 * Class implements interface functions to SSD1327 displays
 */
template <class I>
class InterfaceSSD1327: public I
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
    InterfaceSSD1327(NanoDisplayBase<InterfaceSSD1327<I>> &base, int8_t dc, Args&&... data)
        : I(data...)
        , m_dc( dc )
        , m_base(base)
    {
    }

    /**
     * @brief Sets block in RAM of lcd display controller to write data to.
     *
     * Sets block in RAM of lcd display controller to write data to.
     * For SSD1327 it uses horizontal addressing mode, while for
     * sh1106 the function uses page addressing mode.
     * Width can be specified as 0, thus the library will set the right boundary to
     * region of RAM block to the right column of the display.
     * @param x - column (left region)
     * @param y - row (top region)
     * @param w - width of the block in pixels to control
     *
     * @warning - this function initiates session (i2c or spi) and does not close it.
     *            To close session, please, call endBlock().
     */
    void startBlock(lcduint_t x, lcduint_t y, lcduint_t w);

    /**
     * Switches to the start of next RAM page for the block, specified by
     * startBlock().
     * For SSD1327 it does nothing, while for sh1106 the function moves cursor to
     * next page.
     */
    void nextBlock();

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

private:
    const int8_t m_dc = -1; ///< data/command pin for SPI, -1 for i2c
    NanoDisplayBase<InterfaceSSD1327<I>> &m_base; ///< basic lcd display support interface
};


/**
 * Class implements basic functions for 4-bit mode of SSD1327-based displays
 */
template <class I>
class DisplaySSD1327: public NanoDisplayOps<NanoDisplayOps4<I>,I>
{
public:
    /**
     * Creates instance of SSD1327 controller class for 4-bit mode
     *
     * @param intf interface to use
     * @param rstPin pin to use as HW reset pin for LCD display
     */
    DisplaySSD1327(I &intf, int8_t rstPin)
        : NanoDisplayOps<NanoDisplayOps4<I>, I>(intf)
        , m_rstPin( rstPin ) { }

protected:
    int8_t m_rstPin; ///< indicates hardware reset pin used, -1 if it is not required

    /**
     * Basic SSD1327 initialization
     */
    void begin() override;

    /**
     * Basic SSD1327 deinitialization
     */
    void end() override;
};

/**
 * Class implements basic functions for 4-bit mode of SSD1327-based displays
 */
template <class I>
class DisplaySSD1327_128x128: public DisplaySSD1327<I>
{
public:
    /**
     * Creates instance of SSD1327 128x128 controller class for 4-bit mode
     *
     * @param intf interface to use
     * @param rstPin pin to use as HW reset pin for LCD display
     */
    DisplaySSD1327_128x128(I &intf, int8_t rstPin)
        : DisplaySSD1327<I>(intf, rstPin) { }

protected:

    /**
     * Basic SSD1327 128x128 initialization
     */
    void begin() override;

    /**
     * Basic SSD1327 deinitialization
     */
    void end() override;
};

/**
 * Class implements SSD1327 128x128 lcd display in 4 bit mode over SPI
 */
class DisplaySSD1327_128x128_SPI: public DisplaySSD1327_128x128<InterfaceSSD1327<PlatformSpi>>
{
public:
    /**
     * @brief Inits 128x128 lcd display over spi (based on SSD1327 controller): 4-bit mode.
     *
     * Inits 128x128 lcd display over spi (based on SSD1327 controller): 4-bit mode
     * @param rstPin pin controlling LCD reset (-1 if not used)
     * @param config platform spi configuration. Please refer to SPlatformI2cConfig.
     */
    DisplaySSD1327_128x128_SPI( int8_t rstPin, const SPlatformSpiConfig &config = { -1, -1, -1, 0, -1, -1 } )
        : DisplaySSD1327_128x128(m_spi, rstPin)
        , m_spi( *this, config.dc,
                 SPlatformSpiConfig{ config.busId,
                                     config.cs,
                                     config.dc,
                                     config.frequency ?: 4400000,
                                     config.scl,
                                     config.sda } ) {}

    /**
     * Initializes SSD1327 lcd in 4-bit mode
     */
    void begin() override;

    /**
     * Closes connection to display
     */
    void end() override;

private:
    InterfaceSSD1327<PlatformSpi> m_spi;
};

#include "lcd_ssd1327.inl"

/**
 * @}
 */

// ----------------------------------------------------------------------------

#endif // _LCD_SSD1327_V2_H_
