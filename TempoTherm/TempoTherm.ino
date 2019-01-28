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


const char * hostDomain = "tempomobile.herokuapp.com";
const int hostPort = 80;

void setup() {
  // start baud-----------------------------------------------------
  Serial.begin(9600); //Start the Serial Port at 9600 baud (default)
  OpenLCD.begin(9600); //Start communication with OpenLCD

  //set backlight---------------------------------------------------
  OpenLCD.write('|'); //Put LCD into setting mode
  OpenLCD.write(158 + 0); //Set green backlight amount to 0%
  OpenLCD.write('|'); //Put LCD into setting mode
  OpenLCD.write(188 + 0); //Set blue backlight amount to 0%
  OpenLCD.write('|'); //Put LCD into setting mode
  OpenLCD.write(128); //Set white/red backlight amount to 0%
  
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
  OpenLCD.write(24); //Send contrast command
  OpenLCD.write(contrast);

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
        OpenLCD.print("Sending...                      ");
        delay(2000);
        OpenLCD.write('|'); //Setting character
        OpenLCD.write('-'); //Clear display
        Serial.println("do a post request");
        connectToWiFi(networkName, networkPswd);
        requestURL(hostDomain, hostPort, reading); // Connect to server
        
        
        return;
      }
    }
    
    
    
  }else{
    OpenLCD.print("Press button 0  to take temp    ");
  }
  
  

}





void connectToWiFi(const char * ssid, const char * pwd)
{
  int ledState = 0;

 // printLine();
  Serial.println("Connecting to WiFi network: " + String(ssid));

  WiFi.begin(ssid, pwd);

  while (WiFi.status() != WL_CONNECTED) 
  {
    // Blink LED while we're connecting:
    digitalWrite(LED_PIN, ledState);
    ledState = (ledState + 1) % 2; // Flip ledState
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void requestURL(const char * host, uint8_t port, double temp)
{

  
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
   
     HTTPClient http;   
   
     http.begin("http://tempomobile.herokuapp.com/entries/1");  //Specify destination for HTTP request
     http.addHeader("Content-Type", "application/json");             //Specify content-type header
   
     int httpResponseCode = http.POST("{\"date\": \"2019-02-08\", \"temp\": " + String(temp) + "}");   //Send the actual POST request
   
     if(httpResponseCode>0){
   
      String response = http.getString();                       //Get the response to the request
   
      Serial.println(httpResponseCode);   //Print return code
      Serial.println(response);           //Print request answer
      OpenLCD.write('|'); //Setting character
      OpenLCD.write('-'); //Clear display 
      if(httpResponseCode == 200){
        Serial.println("SUCCESSFUL RESPONSE");
        OpenLCD.print("Success!                        ");
        delay(5000);
        OpenLCD.write('|'); //Setting character
        OpenLCD.write('-'); //Clear display 
        
      }else{
        OpenLCD.print("Error. Please  Try again.        ");
        delay(5000);
        OpenLCD.write('|'); //Setting character
        OpenLCD.write('-'); //Clear display 
      }
      
   
     }else{
   
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
      delay(5000);
      OpenLCD.write('|'); //Setting character
      OpenLCD.write('-'); //Clear display   
      OpenLCD.print("Error. Please  Try again.        ");
      delay(5000);
      OpenLCD.write('|'); //Setting character
      OpenLCD.write('-'); //Clear display 
   
     }
   
     http.end();  //Free resources
   
   }else{
   
      Serial.println("Error in WiFi connection");   
   
   }
}
