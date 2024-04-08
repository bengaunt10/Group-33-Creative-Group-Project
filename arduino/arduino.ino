#include <OneWire.h> //include libraries and define the pin for these
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS); //create one wire object
DallasTemperature sensors(&oneWire);
const int soilSensorPin = A0; // define the pin for the soil moisture sensor(A0 is used as is analog) 

void setup() {
  sensors.begin();  // initialisation
  Serial.begin(9600);
  delay(100);
}

void loop() {
  ReadTemp(); // calls function for temp then soil with a slight delay inbetween for secureness
  ReadSoil();
  delay(50);
}

void ReadSoil() {
  int soilPercentage =0;  //create variable to store the percentage of the soil moisture
  float sensorSoilValue = analogRead(soilSensorPin); //reads analog value from the soil moisture sensor
  //Determine the soil moisture percentage based on sensor value, we used values tailored to our sensor as it gave
  //a certain range of values based on the moisture amount. 
  if(sensorSoilValue >= 550){ // makes sure the return value is always between 0 and 9
    soilPercentage = 0;
  }
  else if(sensorSoilValue <= 300){
    soilPercentage = 9;
  }
  else{
    soilPercentage = map(sensorSoilValue, 300, 550, 10, 0);
  }#
  //prints a value between 0 and 9 to act as our percentage as we only want to send a single integer over to out maxpat file
  Serial.println(soilPercentage); //prints to serial monitor, our maxpat file then picks up whatever is outputted on the serial.
}


void ReadTemp() {
  int tempPercentage = 0;
  sensors.requestTemperatures(); //request temp readings from sensors
  float tempValue = sensors.getTempCByIndex(0) -5; //had to do -5 to calibrate our temp sensor to the most accurate temp reading.
  if(tempValue <= 10){ // determines the temp value based on the readings 
    tempPercentage = 0; //same mapping technique as readsoil()
  }
  else if(tempValue >=34){
    tempPercentage = 9;
  }
  else{
    tempPercentage = map(tempValue, 10, 34, 0, 9);
  }
  Serial.print(tempPercentage);
}
