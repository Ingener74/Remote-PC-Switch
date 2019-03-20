#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#include <ArduinoJson.h>

const char* SSID = "iPhone";
const char* PW = "f0755vhxgasa9";

ESP8266WiFiMulti WiFiMulti;

void setup() {
    Serial.begin(115200);

    Serial.println("\n\n"
    "Start"
    "\n");

    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(SSID, PW);
}

void loop() {
    if ((WiFiMulti.run() == WL_CONNECTED)) {
        WiFiClient client;
        HTTPClient http;
        Serial.printf("[WiFi] local address: %s\r", client.localIP().toString().c_str());
        delay(1000);
        Serial.printf("[HTTP] begin...%*s\r", 40, " ");
        if (http.begin(client, "http://172.20.10.4:5000/esp8266/QUERY")) {
            int httpCode = http.GET();

            if (httpCode > 0) {
                if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                    String payload = http.getString();
                    
                    // Serial.printf("[HTTP] received: %s\n", payload.c_str());
                    
                    StaticJsonDocument<256> doc;
                    
                    DeserializationError error = deserializeJson(doc, payload);

                    // Test if parsing succeeds.
                    if (error) {
                        Serial.printf("deserializeJson() failed: %s\r", error.c_str());
                        delay(1000);
                        return;
                    } else {
                        const char* act = doc["act"];
                        Serial.printf("Received act: %s%*s\r", act, 40, " ");

                        delay(2000);
                    }
                }
            } else {
                Serial.printf("[HTTP] error: %s\r", http.errorToString(httpCode).c_str());
                delay(1000);
            }
        } else {
            Serial.printf("[HTTP] no connection...\r");
            delay(1000);
        }
    } else {
        Serial.printf("[WiFi] no connection...%*s\r", 40, " ");
        delay(1000);
    }
    Serial.printf("Wait.%*s\r", 40, " ");
    delay(1000);
    Serial.printf("Wait..\r");
    delay(1000);
    Serial.printf("Wait...\r");
    delay(1000);
}
