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

#include "lcd_hal/io.h"
#ifdef SDL_EMULATION
#include "sdl_core.h"
#endif
#include "nano_gfx_types.h"

static const PROGMEM uint8_t s_ssd1331_oled96x64_initData[] =
{
#ifdef SDL_EMULATION
    SDL_LCD_SSD1331_X8,
    0x00,
#endif
    0xAE,          // display off
    0xA0, 0x00 | 0x20 | 0x10 | 0x02 | 0x01, /* 8-bit rgb color mode */
    0xA1, 0x00,    // First line to start scanning from
    0xA2, 0x00,    // Set display offset
    0xA4,          // Normal display
    0xA8, 63,      // Reset to default MUX. See datasheet
    0xAD, 0x8E,    // Set master mode
    0xB0, 0x0B,    // Disable power-safe mode
    0xB1, 0x31,    // Precharge Phase 1 and Phase 2 periods
    0xB3, 0xF0,    // CLOCKDIV 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
    0x8A, 0x64,    // Precharge A
    0x8B, 0x78,    // Precharge B
    0xBB, 0x3A,    // Precharge level
    0xBE, 0x3E,    // VCOM
    0x87, 0x09,    // Master current
    0x81, 0x91,    // RED
    0x82, 0x50,    // GREEN
    0x83, 0x7D,    // BLUE
    0xAF,
};

static const PROGMEM uint8_t s_ssd1331_oled96x64_initData16[] =
{
#ifdef SDL_EMULATION
    SDL_LCD_SSD1331_X16,
    0x00,
#endif
    0xAE,          // display off
    0xA0, 0x40 | 0x20 | 0x10 | 0x02 | 0x01, /* 16-bit rgb color mode */
    0xA1, 0x00,    // First line to start scanning from
    0xA2, 0x00,    // Set display offset
    0xA4,          // Normal display
    0xA8, 63,      // Reset to default MUX. See datasheet
    0xAD, 0x8E,    // Set master mode
    0xB0, 0x0B,    // Disable power-safe mode
    0xB1, 0x31,    // Phase 1 and Phase 2 periods
    0xB3, 0xF0,    // 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
    0x8A, 0x64,    // Precharge A
    0x8B, 0x78,    // Precharge B
    0xBB, 0x3A,    // Precharge level
    0xBE, 0x3E,    // VCOM
    0x87, 0x09,    // Master current
    0x81, 0x91,    // RED
    0x82, 0x50,    // GREEN
    0x83, 0x7D,    // BLUE
    0xAF,
};

template <class I>
void InterfaceSSD1331<I>::spiDataMode(uint8_t mode)
{
    if ( m_dc >= 0 )
    {
        lcd_gpioWrite( m_dc, mode ? LCD_HIGH : LCD_LOW );
    }
}

template <class I>
void InterfaceSSD1331<I>::startBlock(lcduint_t x, lcduint_t y, lcduint_t w)
{
    uint8_t rx = w ? (x + w - 1) : (m_base.width() - 1);
    this->start();
    spiDataMode(0);
    this->send((m_rotation & 1) ? 0x75: 0x15);
    this->send(x);
    this->send(rx < m_base.width() ? rx : (m_base.width() - 1));
    this->send((m_rotation & 1) ? 0x15: 0x75);
    this->send(y);
    this->send(m_base.height() - 1);
    spiDataMode(1);
}

template <class I>
void InterfaceSSD1331<I>::nextBlock()
{
}

template <class I>
void InterfaceSSD1331<I>::endBlock()
{
    this->stop();
}

template <class I>
void InterfaceSSD1331<I>::setRotation(uint8_t rotation)
{
    uint8_t ram_mode;
    if ((rotation^m_rotation) & 0x01)
    {
        m_base.swapDimensions();
    }
    m_rotation = rotation & 0x03;
    this->start();
    spiDataMode(0);
    this->send( 0xA0 );
    switch (m_rotation)
    {
    // NORMAL FULL COLOR MODE
    case 0:
        ram_mode = 0b00110010;
        break;
    case 1: // 90 degree CW
        ram_mode = 0b00110001;
        break;
    case 2: // 180 degree CW
        ram_mode = 0b00100000;
        break;
    case 3: // 270 degree CW
    default:
        ram_mode = 0b00100011;
        break;
    }
    this->send( ram_mode | (m_bits == 16 ? 0x40: 0x00) );
    this->stop();
}

template <class I>
void InterfaceSSD1331<I>::drawLine(lcdint_t x1, lcdint_t y1, lcdint_t x2, lcdint_t y2, uint16_t color)
{
    this->start();
    spiDataMode(0);
    this->send( 0x21 );
    this->send(x1);
    this->send(y1);
    this->send(x2);
    this->send(y2);
    this->send( (color & 0xF800) >> 10 );
    this->send( (color & 0x07E0) >> 5 );
    this->send( (color & 0x001F) << 1 );
    this->stop();
}

template <class I>
void InterfaceSSD1331<I>::copyBlock(uint8_t left, uint8_t top,
     uint8_t right, uint8_t bottom, uint8_t newLeft, uint8_t newTop)
{
    this->start();
    spiDataMode(0);
    this->send(0x23);
    this->send(left);
    this->send(top);
    this->send(right);
    this->send(bottom);
    this->send(newLeft);
    this->send(newTop);
    this->stop();
}


////////////////////////////////////////////////////////////////////////////////
//             SSD1331 basic 8-bit implementation
////////////////////////////////////////////////////////////////////////////////

template <class I>
void DisplaySSD1331<I>::begin()
{
    ssd1306_resetController2( m_rstPin, 10 );
    this->m_w = 96;
    this->m_h = 64;
    for( uint8_t i=0; i < sizeof(s_ssd1331_oled96x64_initData); i++)
    {
        this->m_intf.start();
        this->m_intf.spiDataMode(0);
        this->m_intf.send(pgm_read_byte(&s_ssd1331_oled96x64_initData[i]));
        this->m_intf.stop();
    }
}

template <class I>
void DisplaySSD1331<I>::end()
{
}

////////////////////////////////////////////////////////////////////////////////
//             SSD1331 basic 16-bit implementation
////////////////////////////////////////////////////////////////////////////////

template <class I>
void DisplaySSD1331x16<I>::begin()
{
    ssd1306_resetController2( m_rstPin, 10 );
    this->m_w = 96;
    this->m_h = 64;
    for( uint8_t i=0; i < sizeof(s_ssd1331_oled96x64_initData16); i++)
    {
        this->m_intf.start();
        this->m_intf.spiDataMode(0);
        this->m_intf.send(pgm_read_byte(&s_ssd1331_oled96x64_initData16[i]));
        this->m_intf.stop();
    }
}

template <class I>
void DisplaySSD1331x16<I>::end()
{
}

