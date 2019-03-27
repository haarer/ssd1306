#include "ssd1306.h"
#include "ssd1306_console.h"

Ssd1306Console  console;

void setup(void){

 /* Replace the line below with the display initialization function, you want to use */
 ssd1306_128x32_i2c_init();
 ssd1306_clearScreen();
 console.setCursor(0,0);
 /* Set font to use with console */
 ssd1306_setFixedFont(ssd1306xled_font6x8);
 
}

char t[30];
int i=1;

void loop(void){
 sprintf(t, "(%03d) My message\n", i++);
 console.print(t);
 delay(1000);
}

