//curiosity hanif
#include "DHT.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define DHTPIN 18 
#define DHTTYPE DHT11
const char* ssid = "kicik";      
const char* password = "12345678"; 
const char* serverAddress = "http://192.168.205.88:5000/data"; 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
   dht.begin();

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  delay(2000);
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  DynamicJsonDocument jsonDoc(200);
  jsonDoc["temp"] = temp;
  jsonDoc["hum"] = hum;


  
  String payload;
  serializeJson(jsonDoc, payload);

  
  HTTPClient http;
 Serial.println("Server Address: " + String(serverAddress));
  
  http.begin(serverAddress);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(payload);

  if (httpResponseCode > 0) {
    Serial.printf("HTTP Response code: %d\n", httpResponseCode);
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.printf("HTTP Request failed: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  http.end();

  delay(5000); 
}