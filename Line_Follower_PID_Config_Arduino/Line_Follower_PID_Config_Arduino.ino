#include <SoftwareSerial.h>
#include <ArduinoJson.h>

SoftwareSerial ESP8266(4, 2);
DynamicJsonDocument doc(200);


int baseSpeed = 200;
float kp = 0.5;
float kd = 0.14; 

void setup() {
  Serial.begin(9600);
  ESP8266.begin(4800); 
}

void loop() {
  while (ESP8266.available() > 0) {
    String val = ESP8266.readStringUntil('\n');
    manageSerial(val);
  }
}

void manageSerial(String val) {
  if (val.substring(0, 2) == "{\"") {
    DeserializationError error = deserializeJson(doc, val);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    baseSpeed = doc["baseSpeed"].as<String>().toInt();
    kd = doc["kd"].as<String>().toFloat();
    kp = doc["kp"].as<String>().toFloat();
    Serial.print("Adjusting Data: ");
    Serial.println(val);
    
  } else {
    Serial.print("Sending Data :");
    String jsonData = "{\"baseSpeed\" :"+ String(baseSpeed) + ", \"kp\": " + String(kp) + ", \"kd\": " + String(kd) + "}";
    ESP8266.println(jsonData);
    Serial.println(jsonData);
  }
}
