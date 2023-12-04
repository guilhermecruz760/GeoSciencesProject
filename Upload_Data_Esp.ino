#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP_Mail_Client.h>

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

#define AUTHOR_EMAIL "pgeociencias@gmail.com"
#define AUTHOR_PASSWORD "jmymwafnvjgouhqa"

#define RECIPIENT_EMAIL "guilhermecruz760@gmail.com"
#define RECIPIENT_NAME "Guilherme"

// Declare the global variable used SMTPSession object for SMTP transport //
SMTPSession smtp;

// Callback function to check the Author Email status //
void smtpCallback(SMTP_Status status);


const int senPin1 = 34;
const int senPin2 = 32;
const int transistorPin = 16;
unsigned long period = 547;  // 900 seconds = 15 minutes


String URL = "http://geoscienceprojects.space/test_data.php";

const char *ssid = "Vodafone-72BD23";
const char *password = "k97E7mU6uQ";

int value1 = 50;
int value2 = 50;

int sensorValue(int senPin) {
  int aux = 4096 - analogRead(senPin);
  int aux1 = map(aux, 0, 4096, 0, 100);
  Serial.println("-------");
  Serial.println(aux1);
  return (aux1);
}

void connectWiFi() {
  WiFi.mode(WIFI_OFF);
  delay(1000);
  // This line hides the viewing of ESP as a Wi-Fi hotspot
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

void setup() {
  Serial.begin(115200);

  // Add delay to ensure serial monitor is opened before uploading
  delay(1000);

  // Initialize transistor pin
  pinMode(transistorPin, OUTPUT);
  // Ensure sensors are initially powered off
  digitalWrite(transistorPin, LOW);

  // Connect to WiFi
  connectWiFi();
  // Delay to ensure connection before starting sensor readings
  delay(2000);
  smtp.debug(1);
}


void loop() {
  // Power on the sensors
  digitalWrite(transistorPin, HIGH);
  // Allow sensors to stabilize
  delay(1000);

  // Measure sensor values
  value1 = sensorValue(senPin1);
  value2 = sensorValue(senPin2);
  Serial.println("-------");

  // Power off the sensors
  digitalWrite(transistorPin, LOW);

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

  if (((value1 + value2) / 2.0) > 85) {
    Session_Config session;
    session.server.host_name = SMTP_HOST;
    session.server.port = SMTP_PORT;
    session.login.email = AUTHOR_EMAIL;
    session.login.password = AUTHOR_PASSWORD;
    session.login.user_domain = "";

    SMTP_Message message;
    message.sender.name = "ESP 32";
    message.sender.email = AUTHOR_EMAIL;
    message.subject = "Soil Humidity is above 85 percent";
    message.addRecipient(RECIPIENT_NAME, RECIPIENT_EMAIL);

    //Send message
    String textMsg = "You must pay attention to possible floods";
    message.text.content = textMsg.c_str();
    message.text.charSet = "us-ascii";
    message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

    if (!smtp.connect(&session))
      printf("Message sent");

    if (!MailClient.sendMail(&smtp, &message))
      Serial.print("Error sending Email" + smtp.errorReason());
  }

  // Disconnect from WiFi before entering deep sleep
  WiFi.disconnect(true);

  //Turn off board led
  Serial.println("Entering deep sleep");

  // Enter deep sleep for the specified period (in microseconds)
  esp_sleep_enable_timer_wakeup(period * 1000000);
  esp_deep_sleep_start();
  // Check is ESP is really in deep sleep mode
  Serial.println("Deep sleep failed");
}
