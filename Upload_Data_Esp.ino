#include <WiFi.h>
#include <HTTPClient.h>
const int senPin1 = 34;
const int senPin2 = 32;
unsigned long period = 1000;

String URL = "http://192.168.1.68/dht11_project/test_data.php";

const char* ssid = "Vodafone-72BD23";
const char* password = "k97E7mU6uQ";

int temperature = 50;
int humidity = 50;

int sensorValue(int senPin) {
  int aux = 4096 - analogRead(senPin);
  int aux1 = map(aux, 0, 4096, 0, 100);
  Serial.println("-------");
  Serial.println(aux1);
  return (aux1);
}

void setup() {
  Serial.begin(115200);
  connectWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  delay(5000);
  temperature = sensorValue(senPin1);
  humidity = sensorValue(senPin2);

  String postData = "temperature=" + String(temperature) + "&humidity=" + String(humidity);

  HTTPClient http;
  http.begin(URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST(postData);
  String payload = http.getString();

  Serial.print("URL : ");
  Serial.println(URL);
  Serial.print("Data: ");
  Serial.println(postData);
  Serial.print("httpCode: ");
  Serial.println(httpCode);
  Serial.print("payload : ");
  Serial.println(payload);
  Serial.println("--------------------------------------------------");
}



void connectWiFi() {
  WiFi.mode(WIFI_OFF);
  delay(1000);
  //This line hides the viewing of ESP as wifi hotspot
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
