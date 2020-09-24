const char* ssid     = "UPCDC21985";  
const char* password = "fuu2rFefzw6t";
const int httpPort = 80;
const char* clientURL  = "octopusengine.org"; // Your domain  
String clientURLs = "octopusengine.org";
String pathLed       = "/api/led2.json";  
//{"light": "off"}

void oeWiFiStart(){  
  //WiFi.mode(WIFI_STA);
  WiFi.mode(WIFI_OFF);    
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected. IP address: ");
  Serial.println(WiFi.localIP());
  //display.print((WiFi.localIP());
}

void respSend(String toSend)
{
    WiFiClient client;
    Serial.println(" ");
    Serial.println("------------ respSend() -------------");
    Serial.println(" > "+toSend);

   if (client.connect(clientURL, 80)) {
    //Serial.println("Connected to server");
    /////client.println("GET /data/2.5/weather?q=Prague&appid=746f2d2df9ba7023c76e501efec02520 HTTP/1.1");
    ///****************************************----------------------------****************************************************************************
    //place=klub
    //device=w02 (witty)  
    //type=tempk2 - klub 02
    
    client.println("GET /iot17/add.php?place=klub&device=wp0&type=tempk9&value="+toSend+" HTTP/1.1");    //http://www.octopusengine.eu/api/datetime.php  
   //http://www.octopusengine.org/iot17/add.php?value=123

     
    client.println("Host: " + clientURLs);
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("Connection failed to server");
    blinkRed(100,1000);
  }
  delay(500);
  Serial.println("Respond >");
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
   } 
    if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnecting");
    client.stop();
  } 
}

void respTest()
{
    WiFiClient client;
    Serial.println(" ");
    Serial.println("------------ respTest() -------------");
    //blinkBlu();   
    //Serial.print("Requesting URL: ");
    //Serial.println(URL);
  
   //if (client.connect("api.openweathermap.org", 80)) {
   if (client.connect(clientURL, 80)) {
    client.println("GET /api/datetime.php HTTP/1.1");    //http://www.octopusengine.eu/api/datetime.php   
    client.println("Host: " + clientURLs);
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

    if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnecting");
    client.stop();
  } 
}
  
//-------------------------------------------------------------
void sendDataVar(char* type,char* device, int value ) {  
  // Connect to the server (your computer or web page)
  Serial.println(" ");
  Serial.println("------------ sendDataVar > ");
  const char* hostLog  = "octopusengine.org"; // Your domain  
  String pathLog       = "/iot17/add.php";
 
  Serial.println(hostLog);
  WiFiClient client;
  //const int httpPort = 80;
  if (!client.connect(clientURL, httpPort)) { Serial.println("connection failed");  return; }
  //if (client.connect(server, 80)) {
    client.print("GET /iot17/add.php?value="); // This 
    //client.print(int(value*10));
    client.print(value);
    client.print("&type=\"");
    client.print(type);
    client.print("\"&device=\"");
    client.print(device);
    client.println("\" HTTP/1.1"); // Part of the GET request
   
    client.println("HOST: sentu.cz"); // IMPORTANT: If you are using XAMPP you will have to find out the IP address of your computer and put it here (it is explained in previous article). If you have a web page, enter its address (ie.Host: “www.yourwebpage.com”)
    client.println(""); //mandatory blank line
    client.println("Connection: close"); // Part of the GET request telling the server that we are over transmitting the message
    client.println(); // Empty line
    client.stop();
}

//--------------------------------------------------------------
void jsonLed()
{
  Serial.println(" ");
  Serial.println("------------ connecting to PHP.JSON ");
  Serial.println(clientURLs);
  WiFiClient client;
  //const int httpPort = 80;
 if (!client.connect(clientURL, httpPort)) { Serial.println("connection failed");  return; }

  client.print(String("GET ") + pathLed + " HTTP/1.1\r\n" +
               "Host: " + clientURLs + "\r\n" + 
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
  
  

