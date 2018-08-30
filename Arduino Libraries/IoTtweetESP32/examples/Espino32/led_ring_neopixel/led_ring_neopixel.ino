#include <Adafruit_NeoPixel.h>

int neoPixelPin = 9;
int numPixels = 16;
int max_brightness = 32;
int i;
int r,g,b;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRB + NEO_KHZ800);

void setup() {
  
  strip.begin();
  strip.setBrightness(8); /* max : 32 */

}

void loop() {
    
    /* - Draw a color - */
    for(i=0; i< numPixels; i++){
      
      r = random(0,255);
      g = random(0,255);
      b = random(0,255);
      
      strip.setPixelColor(i, r, g, b);
      delay(20);
      strip.show(); 
    } 

   delay(1000);
    
}
