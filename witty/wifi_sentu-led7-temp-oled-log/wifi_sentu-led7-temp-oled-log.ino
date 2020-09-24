#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

#include <OneWire.h> //2016/10 - bylo nutno stáhnout novou verzi:
#include <DallasTemperature.h> // knihovna pro sensor DS18B20
#include <Wire.h> //pro i2c a oled
#include "OLED.h"

#include "oeESP.h"
#include "oeWiFi.h"
//const int pin = 2;

// Declare OLED display - display(SDA, SCL);
OLED display(2, 14); //2,14

// ------ deklatrace DS18B20
#define ONE_WIRE_BUS 0
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//------ konec deklarace

int cnt = 0;
int tmin = 222;
int tmax = -222;
char buffer[7]; //convert int to char*

WiFiClient client;

//===============================================================================
void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

   // Initialize display
  display.begin();

  // Test message
  display.print("IoT - wifi test",1);
  display.print("IP:192.168.0.207",2);
  display.print("octopusengine.eu",7);
  display.print("----------------",6);
 // Test message postioning
 // display.print("TOP-LEFT");
 // display.print("4th row", 4);
 // display.print("RIGHT-BOTTOM", 7, 4);
 
 //char* toDisplP="pokus";
 //display.print(toDisplP, 3);

  sensors.requestTemperatures(); // Send the command to get temperatures
  float celsius = sensors.getTempCByIndex(0);
  dtostrf(celsius, 5, 0, buffer);

  Serial.println("temp: "+int(celsius*10));
//  char* toDisplT="temp: "+buffer;
  display.print("temp: ", 5,0);
  display.print(buffer, 5,7);

  sendDataVar("S","W01",int(celsius));  //start log W01=witty IoT01

/*
 // Test display OFF
  display.off();
  display.print("3rd row", 3, 8);
  delay(3*1000);

  // Test display ON
  display.on();
  delay(3*1000);
}
*/  

  // ----- pro DS18B20
  sensors.begin();

  ledRGB(300);
  oeWiFiStart();
 
  delay(1000);
  respTest();

  //delay(1000);
  //respSend("XYZ");
}

//==========================================================================================
void loop() {
  //jsonBTC();
  
  jsonLed();

  delay(1000);
  // ------ pro DS18B20
  
  sensors.requestTemperatures(); // Send the command to get temperatures
  int celsius = sensors.getTempCByIndex(0);

  if (celsius>tmax) tmax= celsius;
  if (celsius<tmin) tmin= celsius;

  dtostrf(tmin, 5, 0, buffer);
  display.print(buffer, 3,0);
  
  dtostrf(tmax, 5, 0, buffer);
  display.print(buffer, 3,8);
  
  dtostrf(celsius, 5, 0, buffer);
  Serial.println("temp: "+int(celsius*10));
  display.print("temp: ", 4,0);
  display.print(buffer, 4,7);

  if((cnt%100)==0)  sendDataVar("T","W01",int(celsius));  //start log W01=witty IoT01
  
  String toSend="cnt:"+String(cnt)+"%20temp:"+String(celsius);
  Serial.print("toSend String: "+toSend);  
  respSend(toSend);
  
  Serial.print("closing connection. ");
  for (int waits=0; waits<30; waits++){ blinkBlu(50,950); }

  cnt++;
  dtostrf(cnt, 7, 0, buffer);
  display.print("cnt: ", 5,0);
  display.print(buffer, 5,5);
  //display.print(toDispl, 7); 
}
