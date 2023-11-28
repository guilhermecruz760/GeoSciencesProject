const int senPin1 = 34;
const int senPin2 = 32;
unsigned long period = 1000;

int sensorValue(int senPin) {
  int aux = 4096 - analogRead(senPin);
  Serial.println(aux);
  Serial.println("-------");
  int aux1 = map(aux, 0, 4096, 0, 100);
  Serial.print("Humidity: ");
  Serial.println(aux1);
  Serial.println("-------");
  return analogRead(senPin);
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  delay(period);
  sensorValue(senPin1);
  sensorValue(senPin2);
}