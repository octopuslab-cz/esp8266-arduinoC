#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

#include <OneWire.h> //2016/10 - bylo nutno stáhnout novou verzi:
#include <DallasTemperature.h> // knihovna pro sensor DS18B20
#include <Wire.h> //pro i2c a oled
//#include "OLED.h"

#include "oeWITTY.h"
#include "oeWiFi.h"
//const int pin = 2;

// Declare OLED display - display(SDA, SCL);
//OLED display(2, 14); //2,14

// ------ deklatrace DS18B20
#define ONE_WIRE_BUS 0
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress insideThermometer;
DeviceAddress insideThermometer1;
DeviceAddress insideThermometer2;


//------ konec deklarace

int cnt = 0;
int tmin = 222;
int tmax = -222;
char buffer[7]; //convert int to char*

WiFiClient client;

//===============================================================================
void setup() {
  Serial.begin(9600);
  Serial.setDebugOutput(true);

   // Initialize display
  //display.begin();

  // Test message
  //display.print("IoT - wifi test",1);
  //display.print("IP:192.168.0.207",2);
  //display.print("octopusengine.eu",7);
  //display.print("----------------",6);
 
 //char* toDisplP="pokus";
 //display.print(toDisplP, 3);
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
  if (!sensors.getAddress(insideThermometer1, 1)) Serial.println("Unable to find address for Device 1"); 
  if (!sensors.getAddress(insideThermometer2, 2)) Serial.println("Unable to find address for Device 2"); 

  //sensors.requestTemperatures(); // Send the command to get temperatures
  //float celsius = sensors.getTempCByIndex(0);
  sensors.requestTemperatures(); // Send the command to get temperatures
  // Use a simple function to print out the data
  
  
  float celsius =  sensors.getTempCByIndex(0); //getTemperature(insideThermometer);//sensors.getTempCByIndex(0);
  //dtostrf(celsius, 5, 0, buffer);
  Serial.println("temp1: "+int(celsius*10));

  float celsius2 =  sensors.getTempCByIndex(1);
  //dtostrf(celsius, 5, 0, buffer);
  Serial.println("temp2: "+int(celsius2*10));
  
//  char* toDisplT="temp: "+buffer;
 /// display.print("temp: ", 5,0);
 /// display.print(buffer, 5,7);

 /// sendDataVar("S","W01",int(celsius));  //start log W01=witty IoT01
  //-------------------------------------------------------------
  //void sendDataVar(char* type,char* device, float value )

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
  //respSend("333");
}

//==========================================================================================
void loop() {
  Serial.println("Loop:");
  delay(100);
  //jsonBTC();  
  //jsonLed();

  
  sensors.requestTemperatures(); // Send the command to get temperatures
  float celsius = sensors.getTempCByIndex(0);

  //if (celsius>tmax) tmax= celsius;
  //if (celsius<tmin) tmin= celsius;

  //dtostrf(tmin, 5, 0, buffer);
  ///display.print(buffer, 3,0);
  //dtostrf(tmax, 5, 0, buffer);
  ///display.print(buffer, 3,8);
  //dtostrf(celsius, 5, 0, buffer);
  Serial.println("cnt: "+String(cnt));
  Serial.println("temp: "+String(int(celsius*10)));
  ///display.print("temp: ", 4,0);
  ///display.print(buffer, 4,7);

  //*  //2017/08-ok
  //String toSend="cnt:"+String(cnt)+"%20temp:"+String(celsius);
  String toSend=String(celsius*10);
  Serial.println("toSend String: "+toSend);  
  respSend(toSend);
  
  // if((cnt%100)==0)  sendDataVar("T","W01",int(celsius));  //start log W01=witty IoT01
  //void sendDataVar(char* type,char* device, char *value )
  // 2017/08 - err??
  //sendDataVar("T","W01",int(celsius*10));
  //delay(1000);
  
  Serial.println("closing connection. ");
  //for (int waits=0; waits<30; waits++){ blinkBlu(50,950); }

  cnt++;
  //dtostrf(cnt, 7, 0, buffer);
  ///display.print("cnt: ", 5,0);
  ///display.print(buffer, 5,5);
  //display.print(toDispl, 7); 
  //delay(120000); 
  for (int waits=0; waits<900; waits++){ blinkBlu(30,1970); } //1minuta=30*2s=60s //30minut = 900
}

//----------------------
// function to print the temperature for a device
/*
void getTemperature(DeviceAddress deviceAddress)
{
  // method 1 - slower
  //Serial.print("Temp C: ");
  //Serial.print(sensors.getTempC(deviceAddress));
  //Serial.print(" Temp F: ");
  //Serial.print(sensors.getTempF(deviceAddress)); // Makes a second call to getTempC and then converts to Fahrenheit

  // method 2 - faster
  tempC = sensors.getTempC(deviceAddress)*10;
  Serial.print(" ");
  Serial.print(tempC);
 
  //Serial.print(" Temp F: ");
  //Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
}
*/
