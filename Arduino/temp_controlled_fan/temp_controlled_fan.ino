
#include "Adafruit_MAX31855.h"

//setup thermocouple
int thermoDO = 3;
int thermoCS = 4;
int thermoCLK = 5;
Adafruit_MAX31855 thermocouple(thermoCLK, thermoCS, thermoDO);

//setup fan control
int TIP120pin = 11; //for this project, I pick Arduino's PMW pin 11
int highspeed = 255;
int medspeed = 200;
int lowspeed = 150;

//temp tracking vars
int outlier_difference = 20;
static const int readings = 15;
double lastReadings[readings];
int i = 0;
double currentAverage;
static const int idealTemp = 230;
int emailTimer = 0;
  
void setup() {
  Serial.begin(9600);
  
  Serial.println("MAX31855 test");
  // wait for MAX chip to stabilize
  delay(500);
  pinMode(TIP120pin, OUTPUT); // Set pin for output to control TIP120 Base pin
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
     
       if(idealTemp - currentAverage > 15){
         analogWrite(TIP120pin, highspeed);
         Serial.println("Set fan to High speed");
       }else if(idealTemp - currentAverage > 5){
         analogWrite(TIP120pin, medspeed);
         Serial.println("Set fan to Med speed");
       }else if(currentAverage < idealTemp){
         analogWrite(TIP120pin, lowspeed);
         Serial.println("Set fan to Low speed");
       }else{
         analogWrite(TIP120pin, 0);
        Serial.println("Fan off");  
       }
       
       if(idealTemp - currentAverage > 20 && (emailTimer++ % 4 == 0)){
        Serial.println("temp too low! send email!");
       }
     
     }else{
       i++;
     }
     
     Serial.print("F = "); 
     Serial.println(currentTemp);
     delay(1000);
   }
}
