# This is Makefile for ESP32 IDF

COMPONENT_ADD_INCLUDEDIRS := ./src \

COMPONENT_SRCDIRS := ./src \
                     ./src/ssd1306_hal \
                     ./src/ssd1306_hal/esp \
                     ./src/fonts \
                     ./src/v1 \
                     ./src/v1/intf/uart \
                     ./src/v1/intf/vga \
                     ./src/v1/lcd \
                     ./src/v2 \
                     ./src/v2/lcd \
                     ./src/v2/lcd/pcd8544 \
                     ./src/v2/lcd/ssd1306 \
                     ./src/v2/lcd/sh1106 \
                     ./src/v2/lcd/ssd1331 \
                     ./src/v2/lcd/ssd1351 \
                     ./src/v2/lcd/st7735 \
                     ./src/v2/lcd/il9163 \
                     ./src/v2/canvas \
                     ./src/v2/nano_engine \

