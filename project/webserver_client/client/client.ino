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

int rt_turn =150;
int lf_turn = 150;
int square_delay=130;

int my_id= 1;
int id = 0;
int x = 0;
int y = 0 ;
int turn = 0;

int my_move = 0;

long previousTime = 0;
long interval = 5000;

void setup() {
  pinMode(stby,OUTPUT);
  digitalWrite(stby, LOW);
  
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
}

void loop() {
  unsigned long currentTime = millis();

  // Check and set ID
  if(currentTime - previousTime > interval) {
    previousTime = currentTime;
    read_server();
  }
  
  while (my_move){
    // y direction
    move_piece(y);
    delay(500);
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
      delay(500);
      move_piece(abs(x));
      delay(500);
      turn_right();
      delay(500);
    }
    
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
    String line2 = client.readStringUntil('/');
    id = line2.toInt();
    String line3 = client.readStringUntil('/');
    y = line3.toInt();
    String line4 = client.readStringUntil('/');
    x = line4.toInt();
    String line5 = client.readStringUntil('<');
    int temp = line5.toInt();
    if (temp == turn){
      return;
    }
    turn = temp; 
    if(my_id == id){
      my_move = 1;
      Serial.println("My turn");
    }
    return;
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
  delay(rt_turn);
  digitalWrite(stby, LOW);
}


void turn_left(){
  digitalWrite(stby, HIGH);
  digitalWrite(inA2, HIGH);
  digitalWrite(inA1, LOW);
  digitalWrite(inB2, LOW);
  digitalWrite(inB1, HIGH);
  delay(lf_turn);
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
