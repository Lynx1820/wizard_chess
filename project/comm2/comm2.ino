#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;

// Replace with your network credentials
const char* ssid = "hp-setup-1";
const char* password = "WelcomeHal";

ESP8266WebServer server(80);

String webPage = "";

int gpio0_pin = 0;
int gpio2_pin = 2;

void setup(void){
  webPage += "2 2 3";
  
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", [](){    
  String s;
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  s += "Welcome to Wizard Chess!!!";
  s += "<br>";
  s += "Choose the piece to move";
  s += "<form action='/controls'> \n";
  s += "Piece:<br> \n";
  s +="<input type='text' name='piece'>\n";
  s +="Move x:<br> \n";
  s +="<br>\n";
  s +="<input type='text' name='x'>\n";
  s +="<br>\n";
  s +="Move y:<br>\n";
  s +="<input type='text' name='y'>\n";
  s +="<br><br>\n";
  s +="<input type='submit' value='Submit'>\n";
  s += "</form> \n";
  s += "<br>";
  s += "</html>\n";
    server.send(200, "text/html", s);
  });
  server.on("/controls", [](){
    String s2 = "<div id='test'>\n";
    s2 += "1- 2- 3";
    s2 += "</div>";
    server.send(200, "text/html", s2);
    
  });  

  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
} 
