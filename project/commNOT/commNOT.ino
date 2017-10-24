#include <ESP8266WiFi.h>
const char WiFiAPPSK[] = "ese350";
/////////////////////
// Pin Definitions //
/////////////////////
const int LED_PIN = 5; // Thing's onboard, green LED
const int ANALOG_PIN = A0; // The only analog pin on 
const int DIGITAL_PIN = 12; // Digital pin to be read
String bot;
#include <SPI.h>
#include <SD.h>
File file;
WiFiServer server(80);
char line[25];
int n;
void setup(){
  setupWiFi();
  server.begin();
}

void loop(){
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   file = SD.open("website.html", O_READ);
  while (file.available() > 0) {
     Serial.println(file.println());
    }
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);  
  // Match the request
  int val = 0; // We'll use 'val' to keep track of both the
                // request type (read/set) and value if set.
  if (req.indexOf("/control.html") != -1){
    val = 1; // Will write LED low
    }
  if(val == 1){
     file = SD.open("website.html", O_READ);
  while (file.available() > 0) {
     Serial.println(file.println());
    }
  file.close();
  delay(1);
  Serial.println("Client disonnected");
 }
 else{
   bot = 1; 
    String ins = "1" ;
    client.print(ins);
 }
}

void setupWiFi(){
  WiFi.mode(WIFI_AP);
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "Wizard Chess" + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);
  WiFi.softAP(AP_NameChar, WiFiAPPSK);
}
