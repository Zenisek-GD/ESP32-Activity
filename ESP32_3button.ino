#include <WiFi.h>

const char* ssid = "Connect_ka?";
const char* password = "qwerty123";

WiFiServer server(80);

const int ledPins[] = {2, 4, 17, 18, 19}; 
bool allOn = false;

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(WiFi.status());
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  Serial.println("New client connected");

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  if (request.indexOf("/LED=ALL") != -1) {
    Serial.println("Toggle All LEDs");
    allOn = !allOn;
    for (int i = 0; i < 5; i++) {
      digitalWrite(ledPins[i], allOn ? HIGH : LOW);
    }
  }
  else if (request.indexOf("/LED=32415") != -1) {
    Serial.println("Sequence 32415");
    const int ledPin[] = {2, 17, 19, 4, 18}; 
    for (int i = 0; i < 5; i++) {
      digitalWrite(ledPin[i], HIGH);
      delay(300);
      digitalWrite(ledPin[i], LOW);
    }
  }
  else if (request.indexOf("/LED=12345") != -1) {
    Serial.println("Sequence 12345");
    one();
    two();
    three();
    two();
    one();
    clr();
  }

  String html = "<!DOCTYPE html><html><head><title>ESP32 LED Control</title>";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  html += "<style>";
  html += "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background: #f0f2f5; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0;}";
  html += ".container { background: white; border-radius: 20px; padding: 30px; box-shadow: 0 15px 25px rgba(0,0,0,0.1); width: 90%; max-width: 400px; text-align: center;}";
  html += "h1 { color: #333; font-size: 28px; margin-bottom: 20px;}";
  html += ".btn { background: #007bff; color: white; border: none; padding: 15px 25px; margin: 10px 0; border-radius: 50px; cursor: pointer; font-size: 18px; width: 80%; transition: all 0.3s ease; box-shadow: 0 8px 15px rgba(0,123,255,0.2);}";
  html += ".btn:hover { background: #0056b3; box-shadow: 0 15px 20px rgba(0,123,255,0.4); transform: translateY(-5px);}";
  html += ".btn:active { transform: translateY(-2px);}";
  html += "@media (max-width: 600px) { h1 { font-size: 24px; } .btn { font-size: 16px; padding: 10px 20px; }}";
  html += "</style>";
  html += "</head><body>";
  html += "<div class=\"container\">";
  html += "<h1>ESP32 LED Control</h1>";
  html += "<a href=\"/LED=ALL\"><button class=\"btn\">Toggle All LEDs</button></a><br>";
  html += "<a href=\"/LED=32415\"><button class=\"btn\">Sequence 32415</button></a><br>";
  html += "<a href=\"/LED=12345\"><button class=\"btn\">Center to Bottom</button></a>";
  html += "</div>";
  html += "</body></html>";

  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  client.println(html);
  client.println();
  delay(10);
  client.stop();
}

void clr() {
  delay(100);
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

void one() {
  clr();
  digitalWrite(17, HIGH);
  delay(300);
}

void two() {
  clr();
  digitalWrite(18, HIGH);
  digitalWrite(4, HIGH);
  delay(300);
}

void three() {
  clr();
  digitalWrite(19, HIGH);
  digitalWrite(2, HIGH);
  delay(300);
}
