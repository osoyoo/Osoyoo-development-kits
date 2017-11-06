#include "U8glib.h"

//**************************************************
// Change this constructor to match your display!!!
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);
//**************************************************

int yPos = 0;

void setup() {  
  
  u8g.setFont(u8g_font_unifont);
  u8g.setColorIndex(1); // Instructs the display to draw with a pixel on. 
}

void loop() {  
  u8g.firstPage();
  do {  
    draw();
  } while( u8g.nextPage() );
  
  // If its too fast, you could add a delay
  if(yPos < 83){
    // if it's too slow, you could increment y by a greater number
    yPos++;  }
  else{
    // When the yPos is off the screen, reset to 0.
    yPos = 0;
  }  
}  
void draw(){
  u8g.drawStr( 0, yPos, "Hello World");    
}
