#include "ssd1306.h"

NanoEngine1 engine;
NanoRect rect = { {20,30}, {50,60} };


bool drawAll(void)
{
    engine.canvas.clear();
    engine.canvas.drawRect(rect);
    return true;
}

void setup()
{
    ssd1306_128x64_spi_init(6, 12, 4);
//    ssd1306_128x64_i2c_init();
//    ssd1306_128x64_i2c_initEx(PB2, PB0, 0);
    engine.begin();
    engine.setFrameRate(30);
    engine.canvas.setMode(CANVAS_MODE_TRANSPARENT);
    engine.drawCallback(drawAll);
    engine.refresh();
}

void loop()
{
    if(!engine.nextFrame()) return;
    engine.refresh();
    engine.display();
}
