
#define ledPin 0
#define sensorPin 2

int brightness = 0;
int fadeAmount = 5;
int fadeInterval = 20;
unsigned long preTime = 0;

unsigned long preWarnTime = 0;
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);

}

void loop() {
  int sensorValue = digitalRead(sensorPin);

  if (sensorValue == 1) {
    unsigned long currentMillis = millis();
    warning(currentMillis);
  } else {
    turnOff();
  }

}

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

void turnOff(){
   digitalWrite(ledPin, LOW);
  }
