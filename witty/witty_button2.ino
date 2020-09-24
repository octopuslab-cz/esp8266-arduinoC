#include <ESP8266WiFi.h>
#include <WiFiClient.h>

WiFiClient client;

const byte interruptPin = 4;
const int BUTTON = 4;
const int RED = 15;
const int GREEN = 13;
const int BLUE = 12;

int change = 0;
char c;



void setup() {

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
  
  analogWrite(RED,   0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE,  1023);
    
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  WiFi.mode(WIFI_STA);  
  WiFi.begin("SSID", "pass");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
     
    delay(100);

 
}

void loop() {

  int button = digitalRead(BUTTON);   

    if (change == 0)
    {    
      if (client.connect("code21.cz", 80)) {
        Serial.println("Connected to server");
        client.println("GET /led2/showLEDstatus.php HTTP/1.1");
        client.println("Host: code21.cz");
        client.println("Connection: close");
        client.println();
      } else {
        Serial.println("Connection failed to server");
        analogWrite(RED,   1023);
        analogWrite(GREEN, 0);
        analogWrite(BLUE,  0);
      }

      delay(100);

      while (client.available()) {
        c = client.read();
        Serial.print(c);
        if (c == '0')
        {
          analogWrite(RED,   0);
          analogWrite(GREEN, 0);
          analogWrite(BLUE,  1023);
        }
        else
        {
          analogWrite(RED,   0);
          analogWrite(GREEN, 1023);
          analogWrite(BLUE,  0);
        }
        
      }

      delay(100);

      if (!client.connected()) {
        Serial.println();
        Serial.println("Disconnecting");
        client.stop();
      }

    }


    if (change == 1)
    {
    
      if (client.connect("code21.cz", 80)) {
        Serial.println("Connected to server");
        client.println("GET /led2/updateLEDstatus.php?status=1 HTTP/1.1");
        client.println("Host: code21.cz");
        client.println("Connection: close");
        client.println();
      } else {
        Serial.println("Connection failed to server");
        analogWrite(RED,   1023);
        analogWrite(GREEN, 0);
        analogWrite(BLUE,  0);
      }

      delay(200);

      while (client.available()) {
        c = client.read();
        Serial.print(c);
        analogWrite(RED,   0);
        analogWrite(GREEN, 1023);
        analogWrite(BLUE,  0);
        change = 0;
      }

      delay(100);

      if (!client.connected()) {
        Serial.println();
        Serial.println("Disconnecting");
        client.stop();
      }


    }
    
    delay(5000);
    
 
  
}


void blink() {
  
  if (c == '1') 
    {
       Serial.println();
       Serial.println("Akce bezi - cekam na odezvu");
    }

    
    if (c == '0') 
    {
      Serial.println();
      Serial.println("Soustim akci");
      change = 1;   
      analogWrite(RED,   1023);
      analogWrite(GREEN, 1023);
      analogWrite(BLUE,  1023);        
    }

    if (!client.connected()) {
        Serial.println();
        Serial.println("Disconnecting");
        client.stop();
      }
}
