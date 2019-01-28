#include <HTTPClient.h>

#include <WiFi.h>


// WiFi network name and password:
//const char * networkName = "******";
//const char * networkPswd = "*******";


// Internet domain to request from:
const char * hostDomain = "tempomobile.herokuapp.com";
const int hostPort = 80;

const int BUTTON_PIN = 0;
const int LED_PIN = 5;

void setup()
{
  // Initilize hardware:
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  // Connect to the WiFi network (see function below loop)
  connectToWiFi(networkName, networkPswd);

  digitalWrite(LED_PIN, LOW); // LED off
  Serial.print("Press button 0 to connect to ");
  Serial.println(hostDomain);
  
}

void loop()
{
  if (digitalRead(BUTTON_PIN) == LOW){ 
    // Check if button has been pressed
    while (digitalRead(BUTTON_PIN) == LOW)
      ; // Wait for button to be released

    digitalWrite(LED_PIN, HIGH); // Turn on LED
    requestURL(hostDomain, hostPort); // Connect to server
    digitalWrite(LED_PIN, LOW); // Turn off LED
  }
}

void connectToWiFi(const char * ssid, const char * pwd)
{
  int ledState = 0;

  printLine();
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

void requestURL(const char * host, uint8_t port)
{

  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
   
     HTTPClient http;   
   
     http.begin("http://tempomobile.herokuapp.com/entries/1");  //Specify destination for HTTP request
     http.addHeader("Content-Type", "application/json");             //Specify content-type header
   
     int httpResponseCode = http.POST("{\"date\": \"2019-02-06\", \"temp\": \"98.60\"}");   //Send the actual POST request
   
     if(httpResponseCode>0){
   
      String response = http.getString();                       //Get the response to the request
   
      Serial.println(httpResponseCode);   //Print return code
      Serial.println(response);           //Print request answer
   
     }else{
   
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
   
     }
   
     http.end();  //Free resources
   
   }else{
   
      Serial.println("Error in WiFi connection");   
   
   }
}

void printLine()
{
  Serial.println();
  for (int i=0; i<30; i++)
    Serial.print("-");
  Serial.println();
}
