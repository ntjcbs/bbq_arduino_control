/*************************************************** 
  This is an example for the Adafruit Thermocouple Sensor w/MAX31855K

  Designed specifically to work with the Adafruit Thermocouple Sensor
  ----> https://www.adafruit.com/products/269

  These displays use SPI to communicate, 3 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "Adafruit_MAX31855.h"

int thermoDO = 3;
int thermoCS = 4;
int thermoCLK = 5;


 int outlier_difference = 20;
 static const int readings = 15;
 double lastReadings[readings];
 int i = 0;
 double currentAverage;
Adafruit_MAX31855 thermocouple(thermoCLK, thermoCS, thermoDO);
  
void setup() {
  Serial.begin(9600);
  
  Serial.println("MAX31855 test");
  // wait for MAX chip to stabilize
  delay(500);
}

void loop() {
  // basic readout test, just print the current temp
   //Serial.print("Internal Temp = ");
   //Serial.println(thermocouple.readInternal());
   
   double currentTemp = thermocouple.readFarenheit();
   if (isnan(currentTemp)) {
     Serial.println("Something wrong with thermocouple!");
   } else {
     
     if(i != 0 ){
      double tempDiff = lastReadings[i-1] - currentTemp;
       if(tempDiff > 20 || tempDiff < -20){
         Serial.print("Bad Temp reading!");
         currentTemp = lastReadings[i-1];
       }
     }
     
     lastReadings[i] = currentTemp;
     
     if(i == readings - 1){
       //get average of last session
       double sum = 0;
       for(int j = 0; j < readings; j++){
         sum += lastReadings[j];
       }
       currentAverage = sum/readings;
       Serial.print("Current average temp is = "); 
       Serial.println(currentAverage);
     i = 0;
     }else{
       i++;
     }
     
     Serial.print("F = "); 
     Serial.println(currentTemp);
     delay(1000);
   }
}
