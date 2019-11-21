/*
  Smart Container

  A serial of containers let you raise you plants or insects in a smart environment.
  This one is for small insects, it moniter the temperature inside the room, you can also set
  the temperature you want, then the heater will heat the room to the certain temperature.

  Circuit:

  - Attiny85
  - 0.91 inch OLED screen
  - Heating pad
  - tem36
  - 3386F

  Last modified time : 20/11/2019
  by Jasper Wang

*/

#include "TinyWireM.h"
#include "TinyOzOLED.h"

#define outputPin 1
#define temPin A3
#define tarPin A2

float tem;
float lastTem = 0;
float stableTem;
long tarTem;
float lastTarTem = 0;

float addjustTime;
int textDisTime = 5000;

float sensorValue;
float lastSensorValue = 0;
unsigned long lastCheckSensorTime = 0;
bool sensorChanged = false;
unsigned long lastCheckTime = 0;
unsigned long lastCheckTemTime = 0;
int counter = 0;

float aveTem;

void setup() {
  pinMode(outputPin, OUTPUT);
  pinMode(temPin, INPUT);
  pinMode(tarPin, INPUT);

  OzOled.init();
  OzOled.clearDisplay();
  OzOled.setNormalDisplay();
  OzOled.setBrightness(50);
}

void loop() {
  getTem();
  drawDegree();
  MainNum();

  sensorValue = analogRead(tarPin);              // read the value from the popotentiometer.
  tarTem = map(sensorValue, 0, 1023, 15, 35);    // convert it between 15 and 30.

  //OzOled.printNumber(tem, 1, 0, 0);      // for debug only.

  if (millis() - lastCheckTime >= 100) {
    float flucPo = abs(sensorValue - lastSensorValue);
    if (flucPo > 10) {
      sensorChanged = true;
    } else {
      sensorChanged = false;
    }

    lastSensorValue = sensorValue;
    lastCheckSensorTime = millis();
  }

  if (millis() - lastCheckTime >= 200) {
    if (sensorChanged) {
      counter = 25;
    }
    if (counter > 0) {
      counter --;
    }
    lastCheckTime = millis();
  }

  if (counter == 0) {
    OzOled.setCursorXY(0, 3);
    OzOled.printString("    ");
  } else {
    OzOled.setCursorXY(0, 3);
    OzOled.printString("T");
    OzOled.printNumber(tarTem, 2, 3);  // should be tarTem
  }

  if (tem < tarTem + 1) {
    startHeat();
  } else  if (tem > tarTem - 1) {
    stopHeat();
  }
}

void getTem() {
  
  /* read 30 temperatures into an array, and get the average value of them */
  float rawTem [30];
  for (int i = 0; i < 30; i++) {
    float voltage = analogRead(temPin) * 5.0 / 1024.0;
    tem = (voltage - 0.5) * 100;
    rawTem[i] = tem;
  }

  float sumTem = 0;
  for (int i = 0; i < 30; i++) {
    sumTem += rawTem[i];
  }
  
  aveTem = sumTem / 30;
  //OzOled.printNumber(aveTem, 1, 0, 0);      // for debug only.
}

void printText() {
  OzOled.setCursorXY(0, 0);
  OzOled.printString("Hi");
  OzOled.setCursorXY(0, 1);
  OzOled.printString("Jasper");
}

void MainNum() {
  char tmp[10];
  dtostrf(aveTem, 2, 0, tmp);                // convert the number to string
  OzOled.printBigNumber(tmp, 8, 0, 2);       // draw number
}

void startHeat() {
  OzOled.setCursorXY(7, 3);            // draw "H" when heating mode enabled
  OzOled.printString("H");
  digitalWrite(outputPin, HIGH);
}

void stopHeat() {
  OzOled.setCursorXY(7, 3);            // wipe "H" when heating mode disabled
  OzOled.printString(" ");
  digitalWrite(outputPin, LOW);
}

void drawDegree() {
  OzOled.setCursorXY(15, 1);           // dram "o"
  OzOled.printChar(67);
  OzOled.setCursorXY(14, 0);           // draw "c"
  OzOled.printChar(46);
}
