#include <CapPin.h>
#define LED 11


CapPin cPin_1 = CapPin(A5);   // read pin A1
CapPin cPin_2 = CapPin(A4);   // read pin A4
CapPin cPin_3 = CapPin(A0);   // read pin A0

float smoothed_1;
float smoothed_2;
float smoothed_3;

int pastBrightness;

const int debounceDelay = 80;


int brightness;

void setup()
{

  Serial.begin(115200);
  Serial.println("start");
  pinMode(LED, OUTPUT);


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
        brightness = 0;
      } else if((int) smoothed_2 > 100){
        brightness = 80;
      }else if((int) smoothed_3 > 100){
         brightness = 255;
      }
        
    }
  }

  if (brightness != pastBrightness){
     analogWrite(LED,brightness);
     pastBrightness = brightness;
  }

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
