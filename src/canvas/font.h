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
 * @file font.h Font class
 */

#ifndef _NANO_FONT_H_
#define _NANO_FONT_H_

#include "ssd1306_hal/io.h"
#include "nano_gfx_types.h"

/**
 * @ingroup NANO_ENGINE_API
 * @{
 */

/** Flag means that more chars are required to decode utf-8 */
#define SSD1306_MORE_CHARS_REQUIRED  0xffff

class NanoFont
{
public:
    NanoFont() {}

    void loadFixedFont(const uint8_t * progmemFont);

    void loadFreeFont(const uint8_t * progmemFont);

    void loadSecondaryFont(const uint8_t * progmemUnicode);

    void loadFixedFont_oldStyle(const uint8_t * progmemFont);

    void loadSquixFont(const uint8_t * progmemFont);

    void getCharBitmap(uint16_t ch, SCharInfo *info);

    lcduint_t getTextSize(const char *text, lcduint_t *height);

    /**
     * Returns 16-bit unicode char, encoded in utf8
     *         SSD1306_MORE_CHARS_REQUIRED if more characters is expected
     * @param ch character byte to decode
     * @return 16-bit unicode char, encoded in utf8
     *         SSD1306_MORE_CHARS_REQUIRED if more characters is expected
     */
    static uint16_t unicode16FromUtf8(uint8_t ch);

private:
    SFixedFontInfo m_fixedFont{};
    void (*m_getCharBitmap)(SFixedFontInfo &font,uint16_t unicode, SCharInfo *info) = nullptr;
};

/**
 * @}
 */

#endif

