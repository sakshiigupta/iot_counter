#include <WiFi.h>
#include <HTTPClient.h>

#define in 32
#define out 33
#define led 2

int count = 0;

const char* ssid = "xyz"; //ssid
const char* password = "******"; // password for connection

String serverName = "https://api.thingspeak.com/update?api_key=FCQTWGDAMSPCPTX1";
//String serverName = "http://ptsv2.com/t/b9ppu-1593539172/post?";
unsigned long lastTime = 0;
unsigned long timerDelay = 2000;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Visitor Counter");
  delay(2000);
  pinMode(in, INPUT);
  pinMode(out, INPUT);
  //  pinMode(led, OUTPUT);
  Serial.print("Person In Room: ");
  Serial.println(count);
}

void loop() {

  int in_value = digitalRead(in);
  int out_value = digitalRead(out);
  if (in_value == LOW)
  {
    count++;
    Serial.print("Person In Room: ");
    Serial.println(count);
    delay(100);
  }

  if (out_value == LOW)
  {
    count--;
    Serial.print("Person In Room: ");
    Serial.println(count);
    delay(100);
  }

  if ((millis() - lastTime) > timerDelay) {

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      String serverPath = serverName + "&field1=" + count;

      http.begin(serverPath.c_str());

      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
