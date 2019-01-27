#include <SoftwareSerial.h>
SoftwareSerial OpenLCD(16, 17); //RX, TX

//LCD Screen settings:
byte contrast = 10;

int sensePin = 34;  //This is the Arduino Pin that will read the sensor output
int sensorInput;    //The variable we will use to store the sensor input
double temp;        //The variable we will use to store temperature in degrees.

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Start the Serial Port at 9600 baud (default)

  OpenLCD.begin(9600); //Start communication with OpenLCD

  //Send contrast setting
  OpenLCD.write('|'); //Put LCD into setting mode
  OpenLCD.write(24); //Send contrast command
  OpenLCD.write(contrast);

   //Send the clear command to the display - this returns the cursor to the beginning of the display
  OpenLCD.write('|'); //Setting character
  OpenLCD.write('-'); //Clear display
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  
  double sum = 0;
  int num = 1000000;
  for (int i = 1; i <= num; i++) {
   sum = sum + analogRead(sensePin);
  }
  double average = sum / num;
  
  Serial.print(average); Serial.print(" average in mV,  ");

  double degF = (average - 652)/5.59 + 60;

  Serial.print(degF); Serial.println(" average in F");

  
  OpenLCD.print(degF); //For 16x2 LCD
  OpenLCD.print(" ");
  OpenLCD.write(0xDF);
  OpenLCD.print("F                        ");
//  OpenLCD.print("F MiliVolts=");
//  OpenLCD.print(average);
//  OpenLCD.print(" ");

}
