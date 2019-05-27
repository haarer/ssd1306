#ifndef _ARDUINO_H_
#define _ARDUINO_H_

#define ARDUINO_FAKE
#if defined(__MINGW32__)
#include "lcd_hal/mingw/io.h"
#include "lcd_hal/Print_internal.h"
#else
#include "lcd_hal/linux/io.h"
#include "lcd_hal/Print_internal.h"
#endif

#ifndef boolean
    typedef uint8_t boolean;
#endif

#endif

