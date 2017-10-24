#include <ESP8266WiFi.h>

//const char* ssid = "HP-setup-south";
//const char* password = "detkinlabsouth101999";

const char* ssid = "hp-setup-1";
const char* password = "WelcomeHal";
 
const char* host = "192.168.86.248";

int stby = 16; //used to be 13
int inA1 = 4;
int inA2 = 5;
int inB1 = 13;
int inB2 = 0; // used to be 14
int left = 12;
int right =14;

int my_id= 1;
int id = 0;
int y = 0 ;
int x = 0;
int inst = 0;

int fwd = 12;
int fwd_del = 1;
int turn = 5;
int turn_del = 10;
int inst_set = 0;

String extra_controls="";

int my_move = 0;

long previousTime = 0;
long interval = 2000;

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
    print_var(); 
  }

  
  while (my_move){
    // y direction
    if (y<0){
      for (int i = 0; i < abs(y); i++){
        back();
      }
    }
    else if(y>0){
      move_piece(y);
    }
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
 unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    client.readStringUntil('>');
    String line2 = client.readStringUntil('/');
    id = line2.toInt();
    String line3 = client.readStringUntil('/');
    y = line3.toInt();
    String line4 = client.readStringUntil('/');
    x = line4.toInt();
    String line5 = client.readStringUntil('/');
    int temp = line5.toInt();
    if (temp == inst){
      return;
    }
    inst = temp;
    
    if(my_id == id){
      my_move = 1;
      //blink_blue();
    }
    
    String line6 = client.readStringUntil('/');
    temp = line6.toInt();
    if (temp == inst_set){
      return;
    }
    inst_set = temp;
    String line7 = client.readStringUntil('/');
    fwd = line7.toInt();
    String line8 = client.readStringUntil('/');
    fwd_del = line8.toInt();
    String line9 = client.readStringUntil('/');
    turn = line9.toInt();
    String line10 = client.readStringUntil('<');
    turn_del = line10.toInt();
    client.flush();
    break;
  }
}

void move_piece(int a){
  for (int i = 0; i < a; i++){
    forward();
  }
}

void turn_right(){
  digitalWrite(inA1, HIGH);
  digitalWrite(inA2, LOW);
  digitalWrite(inB1, LOW);
  digitalWrite(inB2, HIGH);
  count(turn,turn_del); 
}


void turn_left(){
  digitalWrite(inA2, HIGH);
  digitalWrite(inA1, LOW);
  digitalWrite(inB2, LOW);
  digitalWrite(inB1, HIGH);
  count(turn,turn_del); 
}

void forward(){
  //forward
  digitalWrite(inA1, HIGH);
  digitalWrite(inA2, LOW);
  digitalWrite(inB1, HIGH);
  digitalWrite(inB2, LOW);
  count(fwd,fwd_del);
}

void back(){
  //back
  digitalWrite(inA2, HIGH);
  digitalWrite(inA1, LOW);
  digitalWrite(inB2, HIGH);
  digitalWrite(inB1, LOW);
  count(fwd,fwd_del);
}

void count(int n, int d){
  int old_l =  digitalRead(left);
  int old_r = digitalRead(right);
  int k = 0;
  int p = 0;
  int new_l;
  int new_r;
  digitalWrite(stby, HIGH);
  while (k<n ||  p<n){
    new_l = digitalRead(left);
    new_r = digitalRead(right);
    if (new_l!=old_l){
      k+=1;
      old_l = new_l;
      if (k==n){
          digitalWrite(inA1, LOW);
          digitalWrite(inA2, LOW);
      }
    }
    else if (new_r!=old_r){
      p+=1;
      old_r = new_r;
      if (p==n){
          digitalWrite(inB1, LOW);
          digitalWrite(inB2, LOW);
      }
    }
    delay (10);
  }
  delay(d);
  digitalWrite(stby, LOW);
}

void print_var(){
  Serial.println("x: "+ String(x));
  Serial.println("y: "+ String(y));
  Serial.println("id: "+ String(id));
  Serial.println("fwd: "+ String(fwd));
  Serial.println("fwd_del: "+ String(fwd_del));
  Serial.println("turn: "+ String(turn));
  Serial.println("turn_del: "+ String(turn_del));
}

void blink_blue(){
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  delay(500);
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  delay(500);
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  delay(500);
}
