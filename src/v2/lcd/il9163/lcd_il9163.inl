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

#ifndef CMD_ARG
#define CMD_ARG     0xFF
#endif


template <class I>
void InterfaceIL9163<I>::startBlock(lcduint_t x, lcduint_t y, lcduint_t w)
{
    lcduint_t rx = w ? (x + w - 1) : (m_base.width() - 1);
    this->start();
    spiDataMode(0);
    this->send(0x2A);
    spiDataMode(1);  // According to datasheet all args must be passed in data mode
    this->send(0);
    // Take into account bug in il9163 controller
    this->send(x + (m_rotation == 3 ? 32 : 0));
    this->send(0);
    this->send((rx < m_base.width() ? rx : (m_base.width() - 1))
                       + (m_rotation == 3 ? 32 : 0));
    spiDataMode(0);
    this->send(0x2B);
    spiDataMode(1);  // According to datasheet all args must be passed in data mode
    this->send(0);
    // Take into account bug in il9163 controller
    this->send(y + (m_rotation == 2 ? 32: 0));
    this->send(0);
    this->send(m_base.height() - 1 + (m_rotation == 2 ? 32: 0));
    spiDataMode(0);
    this->send(0x2C);
    spiDataMode(1);
}

template <class I>
void InterfaceIL9163<I>::nextBlock()
{
}

template <class I>
void InterfaceIL9163<I>::endBlock()
{
    this->stop();
}

template <class I>
void InterfaceIL9163<I>::spiDataMode(uint8_t mode)
{
    if ( m_dc >= 0 )
    {
        lcd_gpioWrite( m_dc, mode ? LCD_HIGH : LCD_LOW );
    }
}

template <class I>
void InterfaceIL9163<I>::commandStart()
{
    this->start();
    spiDataMode(0);
}

template <class I>
void InterfaceIL9163<I>::setRotation(uint8_t rotation)
{
    uint8_t ram_mode;
    if ((rotation^m_rotation) & 0x01)
    {
        m_base.swapDimensions();
    }
    m_rotation = (rotation & 0x03);
    this->start();
    spiDataMode(0);
    this->send(0x28);
    this->send(0x36);
    spiDataMode(1);
    switch (m_rotation)
    {
    case 0:
        ram_mode = 0b00000000;
        break;
    case 1: // 90 degree CW
        ram_mode = 0b01100000;
        break;
    case 2: // 180 degree CW
        ram_mode = 0b11000000;
        break;
    default: // 270 degree CW
        ram_mode = 0b10100000;
        break;
    }
    this->send( ram_mode | m_rgb_bit );
    spiDataMode(0);
    this->send(0x29);
    this->stop();
}

////////////////////////////////////////////////////////////////////////////////
//             IL9163 basic 16-bit implementation
////////////////////////////////////////////////////////////////////////////////

template <class I>
void DisplayIL9163x16<I>::begin()
{
}

template <class I>
void DisplayIL9163x16<I>::end()
{
}

static const PROGMEM uint8_t s_IL9163_lcd128x128x16_initData[] =
{
#ifdef SDL_EMULATION
    SDL_LCD_IL9163,
    0x00,
#endif
//    0x01,                     // sw reset. not needed, we do hardware reset
    0x11,                       // exit sleep mode
    0x3A, CMD_ARG, 0x05,        // set 16-bit pixel format
    0x26, CMD_ARG, 0x04,        // set gamma curve: valid values 1, 2, 4, 8
//    0xF2, CMD_ARG, 0x01,        // enable gamma adjustment, 0 - to disable
//    0xE0, CMD_ARG, 0x3F, CMD_ARG, 0x25, CMD_ARG, 0x1C,
//          CMD_ARG, 0x1E, CMD_ARG, 0x20, CMD_ARG, 0x12,
//          CMD_ARG, 0x2A, CMD_ARG, 0x90, CMD_ARG, 0x24,
//          CMD_ARG, 0x11, CMD_ARG, 0x00, CMD_ARG, 0x00,
//          CMD_ARG, 0x00, CMD_ARG, 0x00, CMD_ARG, 0x00, // positive gamma correction
//    0xE1, CMD_ARG, 0x20, CMD_ARG, 0x20, CMD_ARG, 0x20,
//          CMD_ARG, 0x20, CMD_ARG, 0x05, CMD_ARG, 0x00,
//          CMD_ARG, 0x15, CMD_ARG, 0xA7, CMD_ARG, 0x3D,
//          CMD_ARG, 0x18, CMD_ARG, 0x25, CMD_ARG, 0x2A,
//          CMD_ARG, 0x2B, CMD_ARG, 0x2B, CMD_ARG, 0x3A, // negative gamma correction
//    0xB1,  CMD_ARG,  0x08, CMD_ARG, 0x08, // frame rate control 1, use by default
//    0xB4,  CMD_ARG, 0x07,                 // display inversion, use by default
    0xC0,  CMD_ARG,  0x0A, CMD_ARG, 0x02, // power control 1
    0xC1,  CMD_ARG,  0x02,                // power control 2
    0xC5,  CMD_ARG,  0x50, CMD_ARG, 0x5B, // vcom control 1
    0xC7,  CMD_ARG,  0x40,                // vcom offset
//    0x2A,  CMD_ARG,  0x00, CMD_ARG, 0x00, CMD_ARG, 0x00, CMD_ARG, 0x7F,   // set column address, not needed.
//    0x2B,  CMD_ARG,  0x00, CMD_ARG, 0x00, CMD_ARG, 0x00, CMD_ARG, 0x9F,   // set page address, not needed.
    0x36,  CMD_ARG,  0b00000000,          // Horizontal addressing mode
    0x29,
};

////////////////////////////////////////////////////////////////////////////////
//             IL9163 basic 16-bit implementation
////////////////////////////////////////////////////////////////////////////////

template <class I>
void DisplayIL9163_128x128x16<I>::begin()
{
    ssd1306_resetController2( this->m_rstPin, 20 );
    /* Give 120ms display to initialize */
    lcd_delay(120);
    this->m_w = 128;
    this->m_h = 128;
    this->m_intf.start();
    this->m_intf.spiDataMode(0);
    for( uint8_t i=0; i<sizeof(s_IL9163_lcd128x128x16_initData); i++)
    {
        uint8_t data = pgm_read_byte(&s_IL9163_lcd128x128x16_initData[i]);
        if (data == CMD_ARG)
        {
            data = pgm_read_byte(&s_IL9163_lcd128x128x16_initData[++i]);
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
void DisplayIL9163_128x128x16<I>::end()
{
}

static const PROGMEM uint8_t s_IL9163_lcd128x160x16_initData[] =
{
#ifdef SDL_EMULATION
    SDL_LCD_IL9163,
    0b00000011,
#endif
//    0x01,                     // sw reset. not needed, we do hardware reset
    0x11,                       // exit sleep mode
    0x3A, CMD_ARG, 0x05,        // set 16-bit pixel format
    0x26, CMD_ARG, 0x04,        // set gamma curve: valid values 1, 2, 4, 8
//    0xF2, CMD_ARG, 0x01,        // enable gamma adjustment, 0 - to disable
//    0xE0, CMD_ARG, 0x3F, CMD_ARG, 0x25, CMD_ARG, 0x1C,
//          CMD_ARG, 0x1E, CMD_ARG, 0x20, CMD_ARG, 0x12,
//          CMD_ARG, 0x2A, CMD_ARG, 0x90, CMD_ARG, 0x24,
//          CMD_ARG, 0x11, CMD_ARG, 0x00, CMD_ARG, 0x00,
//          CMD_ARG, 0x00, CMD_ARG, 0x00, CMD_ARG, 0x00, // positive gamma correction
//    0xE1, CMD_ARG, 0x20, CMD_ARG, 0x20, CMD_ARG, 0x20,
//          CMD_ARG, 0x20, CMD_ARG, 0x05, CMD_ARG, 0x00,
//          CMD_ARG, 0x15, CMD_ARG, 0xA7, CMD_ARG, 0x3D,
//          CMD_ARG, 0x18, CMD_ARG, 0x25, CMD_ARG, 0x2A,
//          CMD_ARG, 0x2B, CMD_ARG, 0x2B, CMD_ARG, 0x3A, // negative gamma correction
//    0xB1,  CMD_ARG,  0x08, CMD_ARG, 0x08, // frame rate control 1, use by default
//    0xB4,  CMD_ARG, 0x07,                 // display inversion, use by default
    0xC0,  CMD_ARG,  0x0A, CMD_ARG, 0x02, // power control 1
    0xC1,  CMD_ARG,  0x02,                // power control 2
    0xC5,  CMD_ARG,  0x50, CMD_ARG, 0x5B, // vcom control 1
    0xC7,  CMD_ARG,  0x40,                // vcom offset
//    0x2A,  CMD_ARG,  0x00, CMD_ARG, 0x00, CMD_ARG, 0x00, CMD_ARG, 0x7F,   // set column address, not needed.
//    0x2B,  CMD_ARG,  0x00, CMD_ARG, 0x00, CMD_ARG, 0x00, CMD_ARG, 0x9F,   // set page address, not needed.
    0x36,  CMD_ARG,  0b00000000,          // Horizontal addressing mode
    0x29,
};

////////////////////////////////////////////////////////////////////////////////
//             IL9163 basic 16-bit implementation
////////////////////////////////////////////////////////////////////////////////

template <class I>
void DisplayIL9163_128x160x16<I>::begin()
{
    ssd1306_resetController2( this->m_rstPin, 20 );
    /* Give 120ms display to initialize */
    lcd_delay(120);
    this->m_w = 128;
    this->m_h = 160;
    this->m_intf.start();
    this->m_intf.spiDataMode(0);
    for( uint8_t i=0; i<sizeof(s_IL9163_lcd128x160x16_initData); i++)
    {
        uint8_t data = pgm_read_byte(&s_IL9163_lcd128x160x16_initData[i]);
        if (data == CMD_ARG)
        {
            data = pgm_read_byte(&s_IL9163_lcd128x160x16_initData[++i]);
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
void DisplayIL9163_128x160x16<I>::end()
{
}
