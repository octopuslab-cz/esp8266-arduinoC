#include <ESP8266WiFi.h>        // Include the Wi-Fi library

const char* ssid     = "UPCDC21985";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "fuu2rFefzw6t";     // The password of the Wi-Fi network

void setup() {
  WiFi.mode(WIFI_OFF); //////------------------------!!!!!!!!!!!!!!!-------------------------------
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(3000);
  Serial.println('\n');
  
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  Serial.printf("Connection status: %d\n", WiFi.status());
  //Serial.print(WL_NO_SSID_AVAIL);
  //if (WiFi.status() == WL_NO_SSID_AVAIL) {Serial.print("ssid-ok");}
  //else {Serial.print("ssid-NO");}

  
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}

void loop() { }
