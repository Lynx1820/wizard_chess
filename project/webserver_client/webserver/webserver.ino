#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <stdlib.h>
//const char* ssid = "HP-setup-south";
//const char* password = "detkinlabsouth101999";
//TO DO:convert pieces into integers
const char* ssid = "hp-setup-1";
const char* password = "WelcomeHal";
int posted = 1; 
String botId;
int x; 
int y; 
int botx; 
int boty;
WiFiServer server(80);

String webString = "";   // String to display
String s = "";
String str = "0/0/0/0/0/0";
// Generally, you should use "unsigned long" for variables that hold time
unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 2000;              // interval at which to read sensor

void handle_root() {
  s = "";
  s += "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  s += "<form action='/controls'> \n";
  s += "Piece:<br> \n";
  s +="<input type='text' name='piece'>\n";
  s += "<br>\n";
  s +="to<br> \n";
  s +="<br>\n";
  s +="<input type='text' name='loc'>\n";
  s +="<br><br>\n";
  s +="<input type='submit' value='Submit'>\n";
  s += "</form> \n";
  s += "<br>";
  s += "</html>\n";
  
 // server.send(200,"text/html", s);
  delay(1);
}
class Piece{
  public:
  int id;
  int x; 
  int y; 
};
Piece* knight; 
Piece* pawn;
Piece* rook; 
static int i = 0;
static int k = 0;
void setup(void)
{ 
  //How to distinguish between pieces?
  pawn = new Piece(); 
  rook = new Piece(); 
  knight = new Piece();
  knight->x = 2; 
  knight->y = 1;
  rook->x = 1;
  rook->y = 1; 
  pawn->x = 1; 
  pawn->y = 2; 
  // You can open the Arduino IDE Serial Monitor window to see what the code is doing
  Serial.begin(115200);  // Serial connection from ESP-01 via 3.3v console cable

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rWorking to connect");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  

  server.begin();
  Serial.println("HTTP server started");
}
  
void loop(void)
{
   String req;
   read_serial();
   WiFiClient client = server.available();
   if(!client){
      return;
    }
    // Read the first line of the request
    req = client.readStringUntil('\r');
    client.flush();

   int val = -1; 
   if(req.indexOf("/") != -1){
    val = 0;
    }
  if(req.indexOf("/controls") != -1){
    val = 1;
    }
   if(req.indexOf("/settings") != -1){
    val = 2;
    }
   //server.println(String("Refresh: 5\r\n"));
   //read_serial();
  if(val == 0){
    handle_root();
    client.print(s);
    }
  if(val == 1){
    req.trim();
    Serial.println(req);
    parse_string(req);
   if(posted == 0){
      update_pos();
   }
   client.print(webString);
  }
   if(val == 2){
     Serial.println(req);
     update_settings();
     client.print(webString);
    }
  client.flush();
}
void read_serial(){
  if(Serial.available() > 0)
  {
      str = Serial.readStringUntil('\n');
 }
}
void parse_string(String str){
  posted = 0;
  int start = str.indexOf('=');
  str.remove(0,start+1);
  int end1 = str.indexOf('&');
  int start2 = str.indexOf('=');
  String temp = str; 
  temp.remove(end1);
  String piece = temp;
  str.remove(0,start2 +1);
  int end2 = str.indexOf('H');
  str.remove(end2);
  String dest = str;
  piece.toLowerCase();
  dest.toLowerCase();
  Serial.println("piece:" + piece);
  Serial.println("pos:" + dest);
  char letter = dest.charAt(0); 
  x = int(letter) - 96; 
  char num = dest.charAt(1);
  y = int(num) - 48;
  if(piece.equals("knight")){
    botId= 1;
    botx = x - knight->x;
    boty = y - knight->y;
    knight->x = x; 
    knight->y = y;   
  }
  else if(piece.equals("rook")){
    botId = 2;
    rook->id = 1;
    botx = x - rook->x;
    boty = y - rook->y;
    rook->x = x; 
    rook->y = y;    
  }
  else if(piece.equals("pawn")){
    botId = 1;
    botx = x - pawn->x;
    boty = y - pawn->y;  
    pawn->x = x; 
    pawn->y = y;  
  }
  else{
    Serial.println("invalid piece"); 
  }
  
}
void update_pos(){
  i += 1;
  webString = "";
  webString +="HTTP/1.1 200 OK\r\n";
  webString += "Content-Type: text/plain\r\n\r\n";
  webString += "<h>"+ botId +"/"+ boty + "/" + botx + "/" + String(i)+ "</h>";  
  posted=1;
  delay(1000);
}
void update_settings(){
  k +=1; 
  webString = "";
  webString +="HTTP/1.1 200 OK\r\n";
  webString += "Content-Type: text/plain\r\n\r\n";
  webString += "<h>"+ String(k) + "/" +  str + "</h>";  
  delay(1000);
}
