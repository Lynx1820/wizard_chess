//int stby = 13;
//int inA1 = 4;
//int inA2 = 5;
//int inB1 = 12;
//int inB2 = 14;
#include <ESP8266WiFi.h>
 
const char* ssid = "hp-setup-1";
const char* password = "WelcomeHal";
 
const char* host = "192.168.86.169";

int stby = 16; //used to be 13
int inA1 = 4;
int inA2 = 5;
int inB1 = 13;
int inB2 = 0; // used to be 14
int sda = 14;
int scl = 12;

//int turn_delay=200;
int square_delay=250;

int my_id= 1;
int id = 0;
int x = 0;
int y = 0 ;

int my_move = 0;

long previousTime = 0;
long interval = 5000;

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  pinMode(inA1,OUTPUT);
  pinMode(inA2,OUTPUT);
  pinMode(inB1,OUTPUT);
  pinMode(inB2,OUTPUT);
  pinMode(stby,OUTPUT);
}

void loop() {
  read_server();
  unsigned long currentTime = millis();

  // Check and set ID
  if(currentTime - previousTime > interval) {
    previousTime = currentTime;
    // ------ check server ---- //
    // read_server();
    //--------set ID ---------//
    if(my_id = id){
      my_move = 1;
      Serial.println("My turn");
    }
  }
  while (my_move){
    // x direction
    if (x > 0){
      turn_right();
      delay(500);
      move_piece(x);
      delay(500);
      turn_left();
      delay(500);
    }
    if (x < 0){
      turn_left();
      move_piece(abs(x));
      turn_right();
    }
    // y direction
    move_piece(y);
    
    my_move = 0;
    id = 0;
  }
}

void read_server(){
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/controls";
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('>');
    String line2 = client.readStringUntil('-');
    id = line2.toInt();
    Serial.println("var1" + line2);
    String line3 = client.readStringUntil('-');
    x = line3.toInt();
    Serial.println("var2" + line3);
    String line4 = client.readStringUntil('<');
    y = line4.toInt();
    Serial.println("var2" + line4);
    
  }
}

void move_piece(int a){
  for (int i = 0; i < a; i++){
    forward();
  }
}

void turn_right(){
  digitalWrite(stby, HIGH);
  digitalWrite(inA1, HIGH);
  digitalWrite(inA2, LOW);
  digitalWrite(inB1, LOW);
  digitalWrite(inB2, HIGH);
  delay(200);
  digitalWrite(stby, LOW);
}


void turn_left(){
  digitalWrite(stby, HIGH);
  digitalWrite(inA2, HIGH);
  digitalWrite(inA1, LOW);
  digitalWrite(inB2, LOW);
  digitalWrite(inB1, HIGH);
  delay(150);
  digitalWrite(stby, LOW);
}


void forward(){
  //forward
  digitalWrite(stby, HIGH);
  digitalWrite(inA1, HIGH);
  digitalWrite(inA2, LOW);
  digitalWrite(inB1, HIGH);
  digitalWrite(inB2, LOW);
  delay(square_delay);
  digitalWrite(stby, LOW);
}
