int sensePin = 34;  //This is the Arduino Pin that will read the sensor output
int sensorInput;    //The variable we will use to store the sensor input
double temp;        //The variable we will use to store temperature in degrees.

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Start the Serial Port at 9600 baud (default)
}

void loop() {
  // put your main code here, to run repeatedly:
// int sensorInput1 = analogRead(sensePin); //read the analog sensor and store it
// int sensorInput2 = sensorInput1 + analogRead(sensePin); //read the analog sensor and store it
// int sensorInput3 = sensorInput2 + analogRead(sensePin); //read the analog sensor and store it
// int sensorInput4 = senorInput3 + analogRead(sensePin); //read the analog sensor and store it
// int sensorInput5 = senorInput4 +analogRead(sensePin); //read the analog sensor and store it
// int sensorInput6 = senorInput5 + analogRead(sensePin); //read the analog sensor and store it
// int sensorInput7 = senorInput3analogRead(sensePin); //read the analog sensor and store it
// int sensorInput8 = analogRead(sensePin); //read the analog sensor and store it
// int sensorInput9 = analogRead(sensePin); //read the analog sensor and store it
// int sensorInput10 = analogRead(sensePin); //read the analog sensor and store it
  
  
  double sum = 0;
  int num = 1000000;
  for (int i = 1; i <= num; i++) {
   sum = sum + analogRead(sensePin);
  }
  double average = sum / num;
  
  Serial.print(average); Serial.print(" average in mV,  ");

  double degF = (average - 652)/5.59 + 60;

  Serial.print(degF); Serial.println(" average in F");
  
  //Serial.print("milivots "); Serial.println(sensorInput);
  
  // converting that reading to voltage, for 3.3v arduino use 3.3  
  //float voltage = average * 4.095;
  //voltage /= 1024.0; 
 
 // print out the voltage
// Serial.print(voltage); Serial.println(" volts");
 
 // now print out the temperature
 //float temperatureC = (voltage - 2.7) * 100;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)
// Serial.print(temperatureC); Serial.println(" degrees C");
 
 // now convert to Fahrenheit
// float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
// Serial.print(temperatureF); Serial.println(" degrees F");
 
// delay(1000);                                     //waiting a second
}
