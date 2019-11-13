/* Smart Containers v1.1
 *  
 *  Read Me :
 *  
 *  Detect the moisture from the soil, if the soil is wet, shut down the led lights.
 *  If the soil is dry, start warning mood, breath every offTime milliseconds, each time lasts onTime millisecond.
 *  
 *  TO DO :
 *  
 *  Put to sleep mode to save power, only wake up when the soil needs water
 *  
 *  BY: Jasper Wang
 *  TIME : 12/11/2019
 *  
 */
 
#define ledPin 0
#define sensorPin A3                  // set the soil sensor pin to A3 (PB3/2).
#define potPin A2                     // Set the potentiometer pin to A2 (PB3 / 3).

int brightness = 0;                             // variables for the Led beath function.
int fadeAmount = 5;
int fadeInterval = 20;                         
unsigned long preTime = 0;

bool ledStatus = false;                         // variables for the warn interval.
unsigned long lastTimeOn = 0;
unsigned long lastTimeOff = 0;
unsigned long onTime = 20000;                   // change here to adjust the time for warning.
unsigned long offTime = 7000;                   // change here to adjust the time not warning.

/*************************** setup here ***************************/

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  pinMode(potPin, INPUT);
  pinMode(1, OUTPUT);                             // debug led pin !! delete it lastly.

}

/*************************** main loop here ***************************/

void loop() {
  int sensorValue = analogRead(sensorPin);        // read the value from the soil sensor.
  int threshold = analogRead(potPin);             // read the value from the potentiometer.
  float brightness = threshold / 4;              // debug led pin !! delete it lastly.
  analogWrite(1,brightness);

  
  if (sensorValue >= threshold) {               // if the soil is dry under the threshold, enter warning mode.

    if (ledStatus == false) {
      if (millis() - lastTimeOff >= offTime) {
                
        lastTimeOn = millis();                     // mark the time when led was on, reverse the led status.
        ledStatus = true;
      }
    } else {
       
       unsigned long timeOn = millis();             // grab the current time and start breathing the led.
       warning(timeOn);
       
      if (millis() - lastTimeOn >= onTime ) {
      
        turnOff();                                  // turn the led off.

        lastTimeOff = millis();                     // grab the time when led was off and reverse the status.
        ledStatus = false;
      }
    }
  } else {
    turnOff();                                       // if the soil suck enough water, turn off the led.
  }

}

/*************************** a function to breath the led  ***************************/

void warning(unsigned long thisMillis) {

  if (thisMillis - preTime >= fadeInterval) {

    analogWrite(ledPin, brightness);
    brightness += fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = - fadeAmount;
    }
    preTime = thisMillis;
  }

}

/*************************** a function to turn off the led  ***************************/

void turnOff() {
  digitalWrite(ledPin, LOW);
}
