//octopusengine - basic test: simple dallas temperature sensor
//-----------------------------------------------------------
#include <OneWire.h> //2016/10 - new edition: https://github.com/PaulStoffregen/OneWire
#include <DallasTemperature.h> // lib. for DS18B20
#include <Wire.h> //for i2c / oled
#include "oeESP.h"
// ------ declaration DS18B20
#define ONE_WIRE_BUS 0
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//------ /declar
int interval=2; // every 2 sec.
float celsius;

unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Dallas test"); 
  sensors.begin();  // ----- DS18B20
  delay(1000); 
  ledRGB(); 
}

void loop() {
  blinkBlu();
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
     previousMillis = currentMillis;
     sensors.requestTemperatures(); // Send the command to get temperatures
     celsius = sensors.getTempCByIndex(0);

     Serial.println(celsius);
     if (int(celsius)>25){
       ledRed(); 
       delay(500);  
     } 
   }
}
