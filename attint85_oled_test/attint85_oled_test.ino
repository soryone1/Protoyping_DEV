
#include "TinyWireM.h"
#include "TinyOzOLED.h"

void setup() {
  analogReference(INTERNAL);
  OzOled.init();
  OzOled.clearDisplay();
  OzOled.setNormalDisplay();
  OzOled.setBrightness(50);
}
void loop() {
  OzOled.setCursorXY(7, 3);  // draw "H" when heating mode enabled
  OzOled.printString("H");
  OzOled.setCursorXY(15, 1);  // dram "o" 
  OzOled.printChar(67);
  OzOled.setCursorXY(14, 0);    // draw "c"
  OzOled.printChar(46);
  OzOled.printBigNumber("13", 8, 0, 3);    // draw number
  //  OzOled.clearDisplay();
} 
