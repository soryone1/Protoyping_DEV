#include <CapPin.h>
#define LED1 9
#define LED2 10
#define LED3 11

CapPin cPin_1 = CapPin(A5);   // read pin A1
CapPin cPin_2 = CapPin(A4);   // read pin A4
CapPin cPin_3 = CapPin(A0);   // read pin A0

float smoothed_1;
float smoothed_2;
float smoothed_3;

int pressed_1;
int pressed_2;
int pressed_3;

int buttonState1 = LOW;
int buttonState2 = LOW;
int buttonState3 = LOW;

int lastButtonState1 = LOW;
int lastButtonState2 = LOW;
int lastButtonState3 = LOW;

int lastPressedTime1 = 0;
int lastPressedTime2 = 0;
int lastPressedTime3 = 0;

bool turnState1 = false;
bool turnState2 = false;
bool turnState3 = false;

int sensorValue1;
int sensorValue2;
int sensorValue3;

const int debounceDelay = 10;


int brightness;

void setup()
{

  Serial.begin(115200);
  Serial.println("start");
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);


}

void loop()
{


  long start = millis();
  long total1 =  cPin_1.readPin(2000);
  long total2 =  cPin_2.readPin(2000);
  long total3 =  cPin_3.readPin(2000);

  // simple lowpass filter to take out some of the jitter
  // change parameter (0 is min, .99 is max) or eliminate to suit
  smoothed_1 = smooth(total1, .8, smoothed_1);
  smoothed_2 = smooth(total2, .8, smoothed_2);
  smoothed_3 = smooth(total3, .8, smoothed_3);

  if (millis() > 2000) {                                                // wait 2 seconds for the initialization
    if ( millis() - start > 5) {                                        // a slightly delay

      Serial.println((int) smoothed_1);
      Serial.println((int) smoothed_2);
      Serial.println((int) smoothed_3);

      // -------------------------------------------------------

      if ((int) smoothed_1 > 100) {
        pressed_1 = HIGH;
      } else {
        pressed_1 = LOW;
      }

      sensorValue1 = pressed_1;                                             // get the result of the input 0/1

      // -------------------------------------------------------

      if ((int) smoothed_2 > 100) {
        pressed_2 = HIGH;
      } else {
        pressed_2 = LOW;
      }

      sensorValue2 = pressed_2;

      // -------------------------------------------------------

      if ((int) smoothed_3 > 100) {
        pressed_3 = HIGH;
      } else {
        pressed_3 = LOW;
      }

      sensorValue3 = pressed_3;

      // -------------------------------------------------------

      if (sensorValue1 != lastButtonState1) {                              // if input changes
        lastPressedTime1 = millis();                                      // grab the time
      }

      // -------------------------------------------------------

      if (sensorValue2 != lastButtonState2) {                              // if input changes
        lastPressedTime2 = millis();                                      // grab the time
      }

      // -------------------------------------------------------

      if (sensorValue3 != lastButtonState3) {                              // if input changes
        lastPressedTime3 = millis();                                      // grab the time
      }

      // -------------------------------------------------------


      if (millis() - lastPressedTime1 > debounceDelay ) {                  // wait a certain time
        if (sensorValue1 != buttonState1) {                                 //
          buttonState1 = sensorValue1;
          if (buttonState1 == HIGH) {
            //    Serial.println("touched");
            turnState1 = ! turnState1;
          }
        }

      }

      // -------------------------------------------------------


      if (millis() - lastPressedTime2 > debounceDelay ) {                  // wait a certain time
        if (sensorValue2 != buttonState2) {                                 //
          buttonState2 = sensorValue2;
          if (buttonState2 == HIGH) {
            //    Serial.println("touched");

            turnState2 = ! turnState2;
          }
        }

      }

      // -------------------------------------------------------

      if (millis() - lastPressedTime3 > debounceDelay ) {                  // wait a certain time
        if (sensorValue3 != buttonState3) {                                 //
          buttonState3 = sensorValue3;
          if (buttonState3 == HIGH) {
            //    Serial.println("touched");
            turnState3 = ! turnState3;
          }
        }

      }

      // -------------------------------------------------------

      lastButtonState1 = sensorValue1;
      lastButtonState2 = sensorValue2;
      lastButtonState3 = sensorValue3;
    }
  }

  // -------------------------------------------------------

  if (turnState1) {
    analogWrite(LED1, 255);
  } else {
    analogWrite(LED1, 0);
  }

  // -------------------------------------------------------

  if (turnState2) {
    analogWrite(LED2, 255);
  } else {
    analogWrite(LED2, 0);
  }

  // -------------------------------------------------------

  if (turnState3) {
    analogWrite(LED3, 255);
  } else {
    analogWrite(LED3, 0);
  }

  // -------------------------------------------------------
}



int smooth(int data, float filterVal, float smoothedVal) {                    // simple lowpass filter                                                                              // requires recycling the output in the "smoothedVal" param

  if (filterVal > 1) {                                                        // check to make sure param's are within range
    filterVal = .999999;
  }
  else if (filterVal <= 0) {
    filterVal = 0;
  }

  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);

  return (int)smoothedVal;
}
