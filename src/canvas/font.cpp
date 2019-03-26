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

#include "font.h"

enum
{
    SSD1306_OLD_FIXED_FORMAT = 0x00,
    SSD1306_NEW_FIXED_FORMAT = 0x01,
    SSD1306_NEW_FORMAT       = 0x02,
    SSD1306_SQUIX_FORMAT     = 0x03,
};

void NanoFont::loadFixedFont(const uint8_t * progmemFont)
{
}

void NanoFont::loadFreeFont(const uint8_t * progmemFont)
{
}

void NanoFont::loadSecondaryFont(const uint8_t * progmemUnicode)
{
}

void NanoFont::loadFixedFont_oldStyle(const uint8_t * progmemFont)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/// SQUIX FORMAT: 1.7.8 and later
/// SQUIX FORMAT is not fully supported. Use it at your own risk

static void _ssd1306_squixFormatGetBitmap(SFixedFontInfo &font, uint16_t unicode, SCharInfo *info)
{
    if (info)
    {
        const uint8_t *data = font.primary_table;
        /* Check that unicode in the section being processed */
        if ( !data || ( unicode < font.h.ascii_offset) || ( unicode >= (font.h.ascii_offset + font.count) ) )
        {
            info->width = 0;
            info->height = 0;
            info->spacing = font.h.width >> 1;
            info->glyph = font.primary_table;
            return;
        }
        /* At this point data points to jump table (offset|offset|bytes|width) */
        const uint8_t * bitmap_data = data + (uint16_t)font.count * 4;
        unicode -= font.h.ascii_offset;
        data += (unicode * 4);
        uint16_t offset = (pgm_read_byte(&data[0]) << 8) | pgm_read_byte(&data[1]);
        uint8_t glyph_bytes = pgm_read_byte(&data[2]);
//        uint8_t width = pgm_read_byte(&data[3]);
        info->width = glyph_bytes; //(glyph_bytes + font.pages - 1)  / font.pages;
        info->height = font.h.height / 2;
        info->spacing = 1;
//        uint8_t index=0;
        info->glyph = bitmap_data;
        if ( offset != 0xFFFF )
        {
             info->glyph += offset;
        }
    }
}

void NanoFont::loadSquixFont(const uint8_t * progmemFont)
{
    m_fixedFont.h.type = SSD1306_SQUIX_FORMAT;
    m_fixedFont.h.width  = pgm_read_byte(&progmemFont[0]);
    m_fixedFont.h.height = pgm_read_byte(&progmemFont[1]);
    m_fixedFont.h.ascii_offset = pgm_read_byte(&progmemFont[2]);
    m_fixedFont.count = pgm_read_byte(&progmemFont[3]);
    m_fixedFont.primary_table = progmemFont + 4;
    m_getCharBitmap = _ssd1306_squixFormatGetBitmap;
    m_fixedFont.pages = (m_fixedFont.h.height + 7) >> 3;
    m_fixedFont.glyph_size = m_fixedFont.pages * m_fixedFont.h.width;
#ifdef CONFIG_SSD1306_UNICODE_ENABLE
    m_fixedFont.secondary_table = NULL;
#endif
}

void NanoFont::getCharBitmap(uint16_t ch, SCharInfo *info)
{
}

lcduint_t NanoFont::getTextSize(const char *text, lcduint_t *height)
{
    return 0;
}

uint16_t NanoFont::unicode16FromUtf8(uint8_t ch)
{
#ifdef CONFIG_SSD1306_UNICODE_ENABLE
    static uint16_t unicode = 0;
    ch &= 0x000000FF;
    if (!unicode)
    {
        if ( ch >= 0xc0 )
        {
            unicode = ch;
            return SSD1306_MORE_CHARS_REQUIRED;
        }
        return ch;
    }
    uint16_t code = ((unicode & 0x1f) << 6) | (ch & 0x3f);
    unicode = 0;
    return code;
#else
    return ch;
#endif
}
