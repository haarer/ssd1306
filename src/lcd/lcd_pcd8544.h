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
 * @file lcd_pcd8544.h support for Monochrome lcd 84x48 display
 */


#ifndef _LCD_PCD8544_H_
#define _LCD_PCD8544_H_

#include "ssd1306_hal/io.h"
#include "lcd/lcd_common.h"
#include "base/display.h"

/**
 * @ingroup LCD_INTERFACE_API
 * @{
 */

/**
 * Class implements interface functions to PCD8544 displays
 */
template <class I>
class InterfacePCD8544: public I
{
public:
    /**
     * Creates instance of interface to LCD display.
     *
     * @param bits display bit mode: 8 or 16
     * @param base Reference to base class, which represents Display
     * @param dc Data/command control pin number, for i2c communication should be -1
     * @param data variable argument list, accepted by platform interface (PlatformI2c, PlatformSpi)
     */
    template <typename... Args>
    InterfacePCD8544(NanoDisplayBase<InterfacePCD8544<I>> &base, int8_t dc, Args&&... data)
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
    void startBlock(lcduint_t x, lcduint_t y, lcduint_t w);

    /**
     * Switches to the start of next RAM page for the block, specified by
     * startBlock().
     * For ssd1306 it does nothing, while for sh1106 the function moves cursor to
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

private:
    const int8_t m_dc = -1; ///< data/command pin for SPI, -1 for i2c
    NanoDisplayBase<InterfacePCD8544<I>> &m_base; ///< basic lcd display support interface

    uint8_t m_width;
    uint8_t m_column;
    uint8_t m_page;
};

/**
 * Class implements basic functions for 16-bit mode of pcd8544-based displays
 */
template <class I>
class DisplayPCD8544: public NanoDisplayOps<NanoDisplayOps1<I>,I>
{
public:
    /**
     * Creates instance of PCD8544 controller class for 8-bit mode
     *
     * @param intf interface to use
     * @param rstPin pin to use as HW reset pin for LCD display
     */
    DisplayPCD8544(I &intf, int8_t rstPin)
        : NanoDisplayOps<NanoDisplayOps1<I>, I>(intf)
        , m_rstPin( rstPin ) { }

protected:
    int8_t m_rstPin; ///< indicates hardware reset pin used, -1 if it is not required

    /**
     * Basic PCD8544 initialization
     */
    void begin() override;

    /**
     * Basic PCD8544 deinitialization
     */
    void end() override;
};

/**
 * Class implements pcd8544 84x48 oled display in 16 bit mode over SPI
 */
class DisplayPCD8544_84x48_SPI: public DisplayPCD8544<InterfacePCD8544<PlatformSpi>>
{
public:
    /**
     * @brief Inits 84x48 OLED display over spi (based on PCD8544 controller): 16-bit mode.
     *
     * Inits 84x48 OLED display over spi (based on PCD8544 controller): 16-bit mode
     * @param rstPin pin controlling LCD reset (-1 if not used)
     * @param config platform spi configuration. Please refer to SPlatformI2cConfig.
     */
    DisplayPCD8544_84x48_SPI( int8_t rstPin, const SPlatformSpiConfig &config = { -1, -1, -1, 0, -1, -1 } )
        : DisplayPCD8544(m_spi, rstPin)
        , m_spi( *this, config.dc,
                 SPlatformSpiConfig{ config.busId,
                                     config.cs,
                                     config.dc,
                                     config.frequency ?: 4000000,
                                     config.scl,
                                     config.sda } ) {}

    /**
     * Initializes pcd8544 lcd in 8-bit mode
     */
    void begin() override;

    /**
     * Closes connection to display
     */
    void end() override;

private:
    InterfacePCD8544<PlatformSpi> m_spi;
};

#include "lcd_pcd8544.inl"

/**
 * @}
 */

// ----------------------------------------------------------------------------
#endif // _LCD_PCD8544_H_
