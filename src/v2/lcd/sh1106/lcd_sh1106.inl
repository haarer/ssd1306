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

#include "lcd_hal/io.h"
#include "../ssd1306/ssd1306_commands.h"
#ifdef SDL_EMULATION
#include "sdl_core.h"
#endif

static const uint8_t PROGMEM s_sh1106_oled128x64_initData[] =
{
#ifdef SDL_EMULATION
    SDL_LCD_SH1106,
    0x00,
#endif
    SSD1306_DISPLAYOFF, // display off
    SSD1306_COMSCANDEC,             // Scan from 127 to 0 (Reverse scan)
    SSD1306_SETSTARTLINE | 0x00,    // First line to start scanning from
    SSD1306_SETCONTRAST, 0x7F,      // contast value to 0x7F according to datasheet
    SSD1306_SEGREMAP | 0x01,        // Use reverse mapping. 0x00 - is normal mapping 
    SSD1306_NORMALDISPLAY,
    SSD1306_SETMULTIPLEX, 63,       // Reset to default MUX. See datasheet
    SSD1306_SETDISPLAYOFFSET, 0x00, // no offset
    SSD1306_SETDISPLAYCLOCKDIV, 0x80,// set to default ratio/osc frequency
    SSD1306_SETPRECHARGE, 0x22,     // switch precharge to 0x22 // 0xF1
    SSD1306_SETCOMPINS, 0x12,       // set divide ratio
    SSD1306_SETVCOMDETECT, 0x20,    // vcom deselect to 0x20 // 0x40
    SSD1306_CHARGEPUMP, 0x14,       // Enable charge pump
    SSD1306_DISPLAYALLON_RESUME,
    SSD1306_DISPLAYON
};

template <class I>
void InterfaceSH1106<I>::spiDataMode(uint8_t mode)
{
    if (m_dc >= 0)
    {
        lcd_gpioWrite( m_dc, mode ? LCD_HIGH : LCD_LOW);
    }
}

template <class I>
void InterfaceSH1106<I>::commandStart()
{
    this->start();
    if (m_dc >= 0)
        spiDataMode(0);
    else
        this->send(0x00);
}

template <class I>
void InterfaceSH1106<I>::startBlock(lcduint_t x, lcduint_t y, lcduint_t w)
{
    m_column = x;
    m_page = y;
    commandStart();
    this->send(SSD1306_SETPAGE | y);
    this->send(((x+2)>>4) | SSD1306_SETHIGHCOLUMN);
    this->send(((x+2) & 0x0f) | SSD1306_SETLOWCOLUMN);
    if (m_dc >= 0)
    {
        spiDataMode(1);
    }
    else
    {
        this->stop();
        this->start();
        this->send(0x40);
    }
}

template <class I>
void InterfaceSH1106<I>::nextBlock()
{
    this->stop();
    startBlock(m_column,m_page+1,0);
}

template <class I>
void InterfaceSH1106<I>::endBlock()
{
    this->stop();
}

template <class I>
void InterfaceSH1106<I>::setStartLine(uint8_t line)
{
    m_startLine = line;
    commandStart();
    this->send( SSD1306_SETSTARTLINE | (line & 0x3F) );
    this->stop();
}

template <class I>
uint8_t InterfaceSH1106<I>::getStartLine()
{
    return m_startLine;
}

template <class I>
void InterfaceSH1106<I>::normalMode()
{
    commandStart();
    this->send(SSD1306_NORMALDISPLAY);
    this->stop();
}

template <class I>
void InterfaceSH1106<I>::invertMode()
{
    commandStart();
    this->send(SSD1306_INVERTDISPLAY);
    this->stop();
}

template <class I>
void InterfaceSH1106<I>::setContrast(uint8_t contrast)
{
    commandStart();
    this->send(SSD1306_SETCONTRAST);
    this->send(contrast);
    this->stop();
}

template <class I>
void InterfaceSH1106<I>::displayOff()
{
    commandStart();
    this->send(SSD1306_DISPLAYOFF);
    this->stop();
}

template <class I>
void InterfaceSH1106<I>::displayOn()
{
    commandStart();
    this->send(SSD1306_DISPLAYON);
    this->stop();
}

template <class I>
void InterfaceSH1106<I>::flipHorizontal(uint8_t mode)
{
    commandStart();
    this->send( SSD1306_SEGREMAP | (mode ? 0x00: 0x01 ) );
    this->stop();
}

template <class I>
void InterfaceSH1106<I>::flipVertical(uint8_t mode)
{
    commandStart();
    this->send( mode ? SSD1306_COMSCANINC : SSD1306_COMSCANDEC );
    this->stop();
}

template <class I>
void DisplaySH1106_128x64<I>::begin()
{
    ssd1306_resetController2( this->m_rstPin, 10 );
    this->m_w = 128;
    this->m_h = 64;
    for( uint8_t i=0; i < sizeof(s_sh1106_oled128x64_initData); i++)
    {
        this->m_intf.commandStart();
        this->m_intf.send(pgm_read_byte(&s_sh1106_oled128x64_initData[i]));
        this->m_intf.stop();
    }
}

template <class I>
void DisplaySH1106_128x64<I>::end()
{
}
