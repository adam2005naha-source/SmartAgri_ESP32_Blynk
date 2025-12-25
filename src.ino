#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// Paramètres Blynk
char auth[] = "a1b2c3d4e5f6g7h8i9j0k1l2m3n4o5p6";

// Paramètres WiFi
char ssid[] = "WIFI";
char pass[] = "123ab123";

// Capteur DHT11
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Capteur humidité sol
#define SOIL_PIN 34

// Pompe (relais)
#define PUMP_PIN 26

// Seuil humidité sol
int soilThreshold = 500;

BlynkTimer timer;

void sendSensorData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int soilMoisture = analogRead(SOIL_PIN);

  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, soilMoisture);

  if (soilMoisture < soilThreshold) {
    digitalWrite(PUMP_PIN, HIGH);
    Blynk.virtualWrite(V3, 1);
  } else {
    digitalWrite(PUMP_PIN, LOW);
    Blynk.virtualWrite(V3, 0);
  }
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  pinMode(PUMP_PIN, OUTPUT);

  timer.setInterval(5000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}
