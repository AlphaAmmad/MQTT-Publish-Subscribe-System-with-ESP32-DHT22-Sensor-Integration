#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// ⚠️ YAHAN APNI DETAILS BHARO
const char* ssid = "Aamirkhan";
const char* password = "Aashirkhan.777";
const char* mqtt_server = "192.168.0.33"; // apna IP yahan
const char* mqtt_user = "user1";
const char* mqtt_pass = "ammad"; // apna password

WiFiClient espClient;
PubSubClient client(espClient);

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup_wifi() {
  Serial.println("WiFi se connect ho raha hun...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("MQTT connect ho raha hun...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {
      Serial.println("MQTT Connected!");
    } else {
      Serial.print("Failed! Error: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  dht.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (!isnan(h) && !isnan(t)) {
    client.publish("home/room1/temperature", String(t).c_str());
    client.publish("home/room1/humidity", String(h).c_str());
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print("°C  |  Humidity: ");
    Serial.print(h);
    Serial.println("%");
  } else {
    Serial.println("DHT22 sensor error!");
  }

  delay(2000);
}