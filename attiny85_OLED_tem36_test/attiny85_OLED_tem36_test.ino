#include "TinyWireM.h"
#include "TinyOzOLED.h"

#define ledPin 1
#define temPin A3
#define tarPin A2

float tem;
float temTheshold = 25.00;
long tarTem;
float lastTarTem = 0;

float addjustTime;
int textDisTime = 5000;
bool disappeared = true;    // means the text has already disappeared
bool startCount = false;

long sensorValue;

unsigned long clockTime = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(temPin, INPUT);
  pinMode(tarPin, INPUT);

  //  analogReference(INTERNAL);
  OzOled.init();
  OzOled.clearDisplay();
  OzOled.setNormalDisplay();
  OzOled.setBrightness(50);
}

void loop() {
  getTem();
  drawDegree();
  MainNum();

  OzOled.printNumber(tem, 1, 0, 0);      // for debug only.

  sensorValue = analogRead(tarPin);              // read the value from the popotentiometer.
  tarTem = map(sensorValue, 0, 1023, 15, 30);    // convert it between 15 and 30.

   


  if ( disappeared == true) {
    addjustTime = millis();              // show adjust time here
    OzOled.setCursorXY(0, 3);
    OzOled.printString("T");
    OzOled.printNumber(tarTem, 2, 3);  // should be tarTem
    startCount = true;
  } else {

    if (startCount == true) {
      if (millis() - addjustTime >= textDisTime) {

        OzOled.setCursorXY(0, 3);
        OzOled.printString(" ");

        lastTarTem = tarTem;
        disappeared = true;
        startCount = false;

      }
    }
  }

  if (tem < tarTem + 1) {
    startHeat();
  } else  if (tem > tarTem - 1) {
    stopHeat();
  }
}

void getTem() {
  float voltage = analogRead(temPin) * 5.0 / 1024.0;
  tem = (voltage - 0.5) * 100  ;
}

void MainNum() {
  char tmp[10];
  dtostrf(tem, 2, 0, tmp);                // convert the number to string
  OzOled.printBigNumber(tmp, 8, 0, 2);    // draw number

}
void startHeat() {
  OzOled.setCursorXY(7, 3);     // draw "H" when heating mode enabled
  OzOled.printString("H");
  digitalWrite(ledPin, HIGH);
}

void stopHeat() {
  OzOled.setCursorXY(7, 3);     // wipe "H" when heating mode disabled
  OzOled.printString(" ");
  digitalWrite(ledPin, LOW);
}

void drawDegree() {
  OzOled.setCursorXY(15, 1);    // dram "o"
  OzOled.printChar(67);
  OzOled.setCursorXY(14, 0);    // draw "c"
  OzOled.printChar(46);
}
