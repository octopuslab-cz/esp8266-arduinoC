#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

#include "oeESP.h"

const char* ssid     = "UPCDEA3B7C";  
const char* password = "xxxxxx";

const int httpPort = 80;

const char* hostLed  = "sentu.cz"; // Your domain  
String pathLed       = "/api/led2.json";  
//{"light": "off"}

const int pin        = 2;


WiFiClient client;

void jsonLed()
{
  Serial.println(" ");
  Serial.println("------------ connecting to PHP.JSON ");
  Serial.println(hostLed);
  WiFiClient client;
  //const int httpPort = 80;
 if (!client.connect(hostLed, httpPort)) { Serial.println("connection failed");  return; }

  client.print(String("GET ") + pathLed + " HTTP/1.1\r\n" +
               "Host: " + hostLed + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");

  delay(500); // wait for server to respond

  // read response
  String section="header";
  while(client.available()){
    String line = client.readStringUntil('\r');
    // Serial.print(line);
    // we’ll parse the HTML body here
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
      const char* datetime    = json_parsed["datetime"];
      Serial.println(datetime);

      const char* sms    = json_parsed["sms"]; //short message
      Serial.println(sms);
      
      // Make the decision to turn off or on the LED
      if (strcmp(json_parsed["light"], "on") == 0) {
        Serial.println("LED ON");
        //digitalWrite(pin, HIGH);
        blinkGre(1000,1000);       
      }
      else {
        //digitalWrite(pin, LOW);
        Serial.println("led off");
        blinkRed(3000,1000);
       }
    }
  }
}


void resp()
{
    Serial.println(" ");
    Serial.println("------------ resp() -------------");
    //blinkBlu();
    String URL = "sentu.cz";
    //Serial.print("Requesting URL: ");
    //Serial.println(URL);
  
   //if (client.connect("api.openweathermap.org", 80)) {
   if (client.connect("sentu.cz", 80)) {
    //Serial.println("Connected to server");
    /////client.println("GET /data/2.5/weather?q=Prague&appid=746f2d2df9ba7023c76e501efec02520 HTTP/1.1");
    client.println("GET /api/datetime.php HTTP/1.1");    //http://www.octopusengine.eu/api/datetime.php   
    client.println("Host: " + URL);
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("Connection failed to server");
    blinkRed(100,1000);
  }
  delay(500);

 // while (client.available()) {
 //   char c = client.read();
 //   Serial.print(c);
 // }
  // Read all the lines of the reply from server and print them to Serial
  Serial.println("Respond >");
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
   }   
  
  }

//===============================================================================
void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  
  WiFi.mode(WIFI_STA);
  //WiFi.begin("ParalelniPolis", "vejdiven");
  ledRGB(300);
  
  WiFi.begin("UPCDEA3B7C", "fudbexMj8xas");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  delay(1000);
  resp();

  if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnecting");
    client.stop();
  }  
}

//==========================================================================================
void loop() {
  //jsonBTC();
  
  jsonLed();
  
  Serial.print("closing connection. ");
  for (int waits=0; waits<30; waits++){ blinkBlu(50,950); }

   
}
