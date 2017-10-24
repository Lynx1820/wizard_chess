Serial.begin(115200);
while(!Serial){}
WiFiClient client;
const char* host="http://jsonplaceholder.typicode.com/";
String PostData = "title=foo&body=bar&userId=1";

if (client.connect(host, 80)) {

client.println("POST /posts HTTP/1.1");
client.println("Host: jsonplaceholder.typicode.com");
client.println("Cache-Control: no-cache");
client.println("Content-Type: application/x-www-form-urlencoded");
client.print("Content-Length: ");
client.println(PostData.length());
client.println();
client.println(PostData);

long interval = 2000;
unsigned long currentMillis = millis(), previousMillis = millis();

while(!client.available()){

  if( (currentMillis - previousMillis) > interval ){

    Serial.println("Timeout");
    blinkLed.detach();
    digitalWrite(2, LOW);
    client.stop();     
    return;
  }
  currentMillis = millis();
}

while (client.connected())
{
  if ( client.available() )
  {
   char str=client.read();
   Serial.println(str);
  }      
}

