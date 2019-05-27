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

#include "ssd1351_commands.h"
#include "lcd_hal/io.h"
#ifdef SDL_EMULATION
#include "sdl_core.h"
#endif
#include "nano_gfx_types.h"

#ifndef CMD_ARG
#define CMD_ARG     0xFF
#endif

static const PROGMEM uint8_t s_ssd1351_oled128x128_initData[] =
{
#ifdef SDL_EMULATION
    SDL_LCD_SSD1351,
    0x00,
#endif
    SSD1351_UNLOCK, CMD_ARG, 0x12,
    SSD1351_UNLOCK, CMD_ARG, 0xB1,
    SSD1351_SLEEP_ON,
    SSD1351_CLOCKDIV, CMD_ARG, 0xF1,         // 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
    SSD1351_SETMULTIPLEX, CMD_ARG, 127,      // Reset to default MUX. See datasheet
    SSD1351_SEGREMAP, CMD_ARG, 0B00110100,   // 16-bit rgb color mode
    SSD1351_SETSTARTLINE, CMD_ARG, 0x00,     // First line to start scanning from
    SSD1351_SETDISPLAYOFFSET, CMD_ARG, 0x00, // Set display offset
    SSD1351_SETGPIO, CMD_ARG, 0x00,          // GPIO OFF
    SSD1351_SETFUNCTION, CMD_ARG, 0x01,
    SSD1351_SETPRECHARGE, CMD_ARG, 0x32,     // Phase 1 and Phase 2 periods
    SSD1351_VCOMH, CMD_ARG, 0x05,            //
    SSD1351_PRECHARGELEVEL, CMD_ARG, 0x17,
    SSD1351_NORMALDISPLAY,
    SSD1351_CONTRAST,  CMD_ARG, 0xC8,        // RED
                       CMD_ARG, 0x80,        // GREEN
                       CMD_ARG, 0xC8,        // BLUE
    SSD1351_MASTERCURRENT, CMD_ARG, 0x0F,    //
    SSD1351_EXTVSL, CMD_ARG, 0xA0, CMD_ARG, 0xB5, CMD_ARG, 0x55,
    SSD1351_PRECHARGESECOND, CMD_ARG, 0x01,  //
    SSD1351_SLEEP_OFF,                    // Disable power-safe mode
    SSD1351_NORMALDISPLAY,
};


template <class I>
void InterfaceSSD1351<I>::startBlock(lcduint_t x, lcduint_t y, lcduint_t w)
{
    lcduint_t rx = w ? (x + w - 1) : (m_base.width() - 1);
    this->start();
    spiDataMode(0);
    this->send(SSD1351_COLUMNADDR);
    spiDataMode(1);  // According to datasheet all args must be passed in data mode
    this->send(x);
    this->send(rx < m_base.width() ? rx : (m_base.width() - 1));
    spiDataMode(0);
    this->send(SSD1351_ROWADDR);
    spiDataMode(1);  // According to datasheet all args must be passed in data mode
    this->send(y);
    this->send(m_base.height() - 1);
    spiDataMode(0);
    this->send(SSD1351_WRITEDATA);
    spiDataMode(1);
}

template <class I>
void InterfaceSSD1351<I>::nextBlock()
{
}

template <class I>
void InterfaceSSD1351<I>::endBlock()
{
    this->stop();
}

template <class I>
void InterfaceSSD1351<I>::spiDataMode(uint8_t mode)
{
    if ( m_dc >= 0 )
    {
        digitalWrite( m_dc, mode ? HIGH : LOW );
    }
}


////////////////////////////////////////////////////////////////////////////////
//             SSD1351 basic 8-bit implementation
////////////////////////////////////////////////////////////////////////////////

template <class I>
void DisplaySSD1351<I>::begin()
{
    ssd1306_resetController2( m_rstPin, 20 );
    this->m_w = 128;
    this->m_h = 128;
    this->m_intf.start();
    this->m_intf.spiDataMode(0);
    for( uint8_t i=0; i<sizeof(s_ssd1351_oled128x128_initData); i++)
    {
        uint8_t data = pgm_read_byte(&s_ssd1351_oled128x128_initData[i]);
        if (data == CMD_ARG)
        {
            data = pgm_read_byte(&s_ssd1351_oled128x128_initData[++i]);
            this->m_intf.spiDataMode(1);
            this->m_intf.send(data);
            this->m_intf.spiDataMode(0);
        }
        else
        {
            this->m_intf.send(data);
        }
    }
    this->m_intf.stop();
}

template <class I>
void DisplaySSD1351<I>::end()
{
}
