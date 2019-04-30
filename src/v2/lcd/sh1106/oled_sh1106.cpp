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

#include "ssd1306_hal/io.h"
#include "oled_sh1106.h"

////////////////////////////////////////////////////////////////////
//                        SH1106 128x64
////////////////////////////////////////////////////////////////////

void DisplaySH1106_128x64_I2C::begin()
{
    m_i2c.begin();
    DisplaySH1106_128x64::begin();
}

void DisplaySH1106_128x64_I2C::end()
{
    DisplaySH1106_128x64::end();
    m_i2c.end();
}


void DisplaySH1106_128x64_SPI::begin()
{
    m_spi.begin();
    DisplaySH1106_128x64::begin();
}

void DisplaySH1106_128x64_SPI::end()
{
    DisplaySH1106_128x64::end();
    m_spi.end();
}

