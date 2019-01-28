#include <WiFi.h>
#include <HTTPClient.h>
#include <SoftwareSerial.h>


//LCD Screen settings--------------------------------------------------------
SoftwareSerial OpenLCD(16, 17); //RX, TX
byte contrast = 10;

//Sensor vars----------------------------------------------------------------
int sensePin = 34;  //This is the Arduino Pin that will read the sensor output
int sensorInput;    //The variable we will use to store the sensor input
double temp;        //The variable we will use to store temperature in degrees.
const int BUTTON_PIN = 0;
const int LED_PIN = 5;

//Wifi Setup-----------------------------------------------------------------
//const char * networkName = "******";
//const char * networkPswd = "*******";
const char * networkName = "ptarmigan2";
const char * networkPswd = "ptarm19an5557skiing";
const char * hostDomain = "tempomobile.herokuapp.com";
const int hostPort = 80;

//General Settings----------------------------------------------------------
const int numOfReadings = 2;


void setup() {
  // start baud-----------------------------------------------------
  Serial.begin(9600); //Start the Serial Port at 9600 baud (default)
  OpenLCD.begin(9600); //Start communication with OpenLCD

  //initialize pins-------------------------------------------------
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  //display message------------------------------------------------
  digitalWrite(LED_PIN, LOW); // LED off
  //Serial.print("Press button 0 to take your temperature.");
  //Send the clear command to the display - this returns the cursor to the beginning of the display
  OpenLCD.write('|'); //Setting character
  OpenLCD.write('-'); //Clear display
  OpenLCD.print("SKETCH LOADED                   ");
  delay(5000);
  OpenLCD.write('|'); //Setting character
  OpenLCD.write('-'); //Clear display
  //Send contrast setting
//  OpenLCD.write('|'); //Put LCD into setting mode
//  OpenLCD.write(24); //Send contrast command
//  OpenLCD.write(contrast);

   //Send the clear command to the display - this returns the cursor to the beginning of the display
//  OpenLCD.write('|'); //Setting character
//  OpenLCD.write('-'); //Clear display

 
}

void loop() {
  //Serial.println("entering main loop");
  if (digitalRead(BUTTON_PIN) == LOW){ 
    // Check if button has been pressed
    
    while (digitalRead(BUTTON_PIN) == LOW)
    ; // Wait for button to be released

    //take readings
    //int counter = 0;
    
    double minReading = 96.00;
    double reading = 0.00;

    //while the temps are coming up to body temp, keep reading.
    while (reading < minReading){
      
      double sum = 0;
      int num = 500000;
      
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
          
      reading = degF;
    }

    Serial.println("reached body temp minimum");
//    double readings[numOfReadings - 1];
    double minReadingDif = 0.5;
//    double consecReadingsInRange[3];
//    int indexCounter = 0;
    double countOfConsecReadingsInRange = 0;
    
    //once up to body temp, read until change in temp is less than .5 degrees for 3 consecutive readings.
   
    while(countOfConsecReadingsInRange < 3){
      
      double sum = 0;
      int num = 500000;
      
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

      double diff = abs(degF - reading);

      Serial.print("change: "); Serial.println(diff);
      
      if(diff < minReadingDif){
        countOfConsecReadingsInRange ++;
      }else{
        countOfConsecReadingsInRange = 0;
      }

      reading = degF;
      
    }

    OpenLCD.write('|'); //Setting character
    OpenLCD.write('-'); //Clear display
    int counter = 9;
    while(counter >= 0){
      Serial.print("Most recent reading: "); 
      Serial.println(reading); 
      OpenLCD.print(reading);
      OpenLCD.print("          ");
      OpenLCD.print(counter);
      OpenLCD.print("To send, press O");
      delay(1000);
      counter --;
      if (digitalRead(BUTTON_PIN) == LOW){ 
      // Check if button has been pressed
        while (digitalRead(BUTTON_PIN) == LOW)
        ; // Wait for button to be released

        //Do the post request
        Serial.println("do a post request");
        return;
      }
    }
    
    
    
  }else{
    OpenLCD.print("Press button 0  to take temp    ");
  }
  
  

}
