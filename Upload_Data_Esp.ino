#include <WiFi.h>
#include <HTTPClient.h>

const int senPin1 = 34;
const int senPin2 = 32;
unsigned long period = 900000;

String URL = "http://geoscienceprojects.space/test_data.php";

const char *ssid = "Vodafone-72BD23";
const char *password = "k97E7mU6uQ";

int value1 = 50;
int value2 = 50;

int sensorValue(int senPin)
{
  int aux = 4096 - analogRead(senPin);
  int aux1 = map(aux, 0, 4096, 0, 100);
  Serial.println("-------");
  Serial.println(aux1);
  return (aux1);
}

void setup()
{
  Serial.begin(115200);
  connectWiFi();
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    connectWiFi();
  }

  delay(period);
  value1 = sensorValue(senPin1);
  value2 = sensorValue(senPin2);

  String postData = "value1=" + String(value1) + "&value2=" + String(value2);

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

void connectWiFi()
{
  WiFi.mode(WIFI_OFF);
  delay(1000);
  // This line hides the viewing of ESP as wifi hotspot
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.print("connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
