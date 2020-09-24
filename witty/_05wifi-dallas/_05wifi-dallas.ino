#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
//#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`  https://github.com/squix78/esp8266-oled-ssd1306
#include <DallasTemperature.h> // knihovna pro sensor DS18B20
#include <Wire.h> //pro i2c a oled

#include "images.h"
#include "oeESP.h"

WiFiClient client;

const char* ssid     = "UPCDC21985";  
const char* password = "fuu2rFefzw6t";
const int httpPort = 80;
const char* host  = "octopusengine.org"; // Your domain  
String pathLed       = "/api/bitstamp-witty.php";  

String wittyGET;
String dolni;
String horni;
String btcLast;

float Dolni;
float Horni;
float BtcLast;

int alarm = 0;
String alarmActive = "0";

// ------ deklatrace DS18B20
#define ONE_WIRE_BUS 0
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//------ konec deklarace


// Initialize the OLED display using brzo_i2c
// D3 -> SDA
// D5 -> SCL

//SSD1306  display(0x3c, D3, D5);

const int buzzer = 5; //buzzer to arduino pin 5


String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void json()
{

  Serial.println(" ");
  Serial.println("------------ connecting to PHP.JSON ");
  Serial.println(host);
  WiFiClient client;
  
  if (!client.connect(host, httpPort)) { Serial.println("connection failed");  return; }

  client.print(String("GET ") + pathLed + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");

  delay(500); // wait for server to respond

  // read response
  String section="header";
  while(client.available()){
    String line = client.readStringUntil('\r');
    // Serial.print(line);
    // we�ll parse the HTML body here
    if (section=="header") { // headers..
      Serial.print(".");
      if (line=="\n") { section="json"; } // skips the empty space at the beginning
    }
    else if (section=="json") {  // print the good stuff
      section="ignore";
      String result = line.substring(1);

      // Parse JSON
      Serial.println("parseObject: ");
      int size = result.length() + 1;
      char json[size];
      result.toCharArray(json, size);
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& json_parsed = jsonBuffer.parseObject(json);
      if (!json_parsed.success()) { Serial.println("parseObject() failed");   return; }
      
      //Serial.println(json_parsed["datetime"]);
      //https://github.com/bblanchon/ArduinoJson/wiki/Decoding-JSON
      const char* last    = json_parsed["last"];
      Serial.println(last);

      //const char* timestamp    = json_parsed["timestamp"]; //short message
      //Serial.println(timestamp);
      
      // Make the decision to turn off or on the LED
      if (strcmp(json_parsed["light"], "on") == 0) {
        Serial.println("LED ON");
        //digitalWrite(pin, HIGH);
        //blinkGre(1000,1000);       
      }
      else {
        //digitalWrite(pin, LOW);
        Serial.println("led off");
        //blinkRed(3000,1000);
       }
    }
  }  
}


void btclast() {

    Serial.println(" ");
    Serial.println("------------ resp() -------------");
    //blinkBlu();
    String URL = "code21.cz";
    //Serial.print("Requesting URL: ");
    //Serial.println(URL);
    
   if (client.connect(host, 80)) {
    client.println("GET /api/bitstamp-witty.php HTTP/1.1"); 
    client.println("Host: " + URL);
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("Connection failed to server");
    blinkRed(500,1000);
    return;
  }
  delay(500);

  while(client.available()){
    wittyGET = client.readStringUntil('\r');    
   }   

    btcLast = getValue(wittyGET, '#', 0);
    dolni = getValue(wittyGET, '#', 1);
    horni = getValue(wittyGET, '#', 2);
    alarmActive = getValue(wittyGET, '#', 3);
    BtcLast = btcLast.toFloat();
    Dolni = dolni.toFloat();
    Horni = horni.toFloat();
/*      
   display.clear();
   display.setFont(ArialMT_Plain_16);
   display.setTextAlignment(TEXT_ALIGN_LEFT);
   display.drawString(0, 0, "BTC/USD");   
   display.drawString(0, 25, btcLast);   
   display.setFont(ArialMT_Plain_10);
   display.drawString(0, 50, dolni);   
   display.drawString(64, 50, horni); 
   if (alarmActive == "1")   display.drawString(80, 0, "alarm ON");   
   else display.drawString(80, 0, "alarm OFF");
   display.display();
   blinkBlu(200,1000);
  */ 
   Serial.println(btcLast);

  
}




void setup() {
  
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 5 as an output
  ledRGB(300);
  delay(500); 

 
  Serial.begin(9600);
  //Serial.setDebugOutput(true);
  Serial.println("------------ setup() -------------");

// ------ deklatrace DS18B20
#define ONE_WIRE_BUS 0
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//------ konec deklarace



  // ----- pro DS18B20
  sensors.begin();
 sensors.requestTemperatures(); // Send the command to get temperatures
  float celsius = sensors.getTempCByIndex(0);
  //dtostrf(celsius, 5, 0, buffer);

  Serial.println("temp: "+int(celsius*10));

  
  //oeWiFiStart();

  //respTest();

  //delay(1000);
  //respSend("XYZ");

//  display.init();
//  display.flipScreenVertically();  
  
  //WiFi.mode(WIFI_STA);
  WiFi.mode(WIFI_OFF);
  //WiFi.begin("ParalelniPolis", "vejdiven");
  //ledRGB(300);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    blinkBlu(50, 100);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  //String ip = WiFi.localIP();

//  display.setFont(ArialMT_Plain_16);
//  display.setTextAlignment(TEXT_ALIGN_LEFT);
//  display.drawString(20, 30, "BitCoin Alarm");
//  display.display();

  delay(5000);
  
//  display.clear();
//  display.drawXbm(34, 0, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);  
//  display.setFont(ArialMT_Plain_10);
//  display.drawString(30, 40, WiFi.localIP().toString());
//  display.display();

  delay(4000);

  
}

void loop() {
  Serial.println("Loop->");
  btclast();
  delay(100);
  //if (alarmActive == "1") controlAlarm();
  Serial.println("temp->");
  sensors.requestTemperatures(); // Send the command to get temperatures
  float celsius = sensors.getTempCByIndex(0);
  //dtostrf(celsius, 5, 0, buffer);
  Serial.println(celsius);
  Serial.println("temp: "+int(celsius*10));
  delay(100);
//  char* toDisplT="temp: "+buffer;
 // display.print("temp: ", 5,0);
 // display.print(buffer, 5,7);

///  sendDataVar("S","W01",int(celsius));  //start log W01=witty IoT01

  delay(5000);
  ledRGB(300);

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


  
  
}
