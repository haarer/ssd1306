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

#include "lcd_common.h"
#include "pcd8544_commands.h"
#include "ssd1306_hal/io.h"
#ifdef SDL_EMULATION
#include "sdl_core.h"
#endif
#include "nano_gfx_types.h"

static const uint8_t PROGMEM s_pcd8544_lcd84x48_initData[] =
{
#ifdef SDL_EMULATION
    SDL_LCD_PCD8544,
    0x00,
#endif
    PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION, // switch to extented commands
    PCD8544_SETVOP | 0x16,  // Set vop contrast
    PCD8544_SETTEMP,
    PCD8544_SETBIAS | 0x04, // Set bias mode
    PCD8544_FUNCTIONSET, // switch to basic commands
    PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL
};


template <class I>
void InterfacePCD8544<I>::spiDataMode(uint8_t mode)
{
    if ( m_dc >= 0 )
    {
        digitalWrite( m_dc, mode ? HIGH : LOW );
    }
}

template <class I>
void InterfacePCD8544<I>::startBlock(lcduint_t x, lcduint_t y, lcduint_t w)
{
    m_width = w;
    m_column = x;
    m_page = y;
    this->start();
    spiDataMode(0);
    if (w == 1) this->send( 0x22 ); else this->send( 0x20 );
    this->send(0x80 | x);
    this->send(0x40 | y);
    spiDataMode(1);
}

template <class I>
void InterfacePCD8544<I>::nextBlock()
{
    if ( m_width != 1)
    {
        this->stop();
        startBlock(m_column, m_page+1, m_width);
    }
}

template <class I>
void InterfacePCD8544<I>::endBlock()
{
    spiDataMode(0);
    this->send( 0x00 ); // Send NOP command to allow last data byte to pass (bug in PCD8544?)
                        // ssd1306 E3h is NOP command
    this->stop();
}


////////////////////////////////////////////////////////////////////////////////
//             PCD8544 basic 8-bit implementation
////////////////////////////////////////////////////////////////////////////////

template <class I>
void DisplayPCD8544<I>::begin()
{
    ssd1306_resetController( m_rstPin, 20 );

    this->m_w = 84;
    this->m_h = 48;
    this->m_intf.start();
    this->m_intf.spiDataMode(0);

    for( uint8_t i=0; i<sizeof(s_pcd8544_lcd84x48_initData); i++)
    {
        this->m_intf.send(pgm_read_byte(&s_pcd8544_lcd84x48_initData[i]));
    }
    this->m_intf.stop();
}

template <class I>
void DisplayPCD8544<I>::end()
{
}
