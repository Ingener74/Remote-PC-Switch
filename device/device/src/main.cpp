#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#include <ArduinoJson.h>

const char* SSID = "iPhone";
const char* PW = "f0755vhxgasa9";

const char* HOME_SSID = "Ingener74";
const char* HOME_PW = "pavel1986";

const int POWER_PIN = 5;
bool power = false;

ESP8266WiFiMulti WiFiMulti;

void setup() {
    Serial.begin(115200);

    Serial.println("\n\n"
    "Start"
    "\n");

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    pinMode(POWER_PIN, OUTPUT);
    digitalWrite(POWER_PIN, LOW);

    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(SSID, PW);
    WiFiMulti.addAP(HOME_SSID, HOME_PW);
}

void powerOn() {

    Serial.printf("[Power] Switch on...%*s\r", 40, " ");
    delay(1000);

    digitalWrite(POWER_PIN, HIGH);
    delay(100);
    digitalWrite(POWER_PIN, LOW);

    Serial.printf("[Power] Switched on...%*s\r", 40, " ");
    delay(1000);
}

void powerOff() {

    Serial.printf("[Power] Switch off...%*s\r", 40, " ");
    delay(1000);

    digitalWrite(POWER_PIN, HIGH);
    delay(5000);
    digitalWrite(POWER_PIN, LOW);

    Serial.printf("[Power] Switched off...%*s\r", 40, " ");
    delay(1000);
}

void query(const char* url, HTTPClient& httpClient, WiFiClient& wifiClient) {
    char buffer[128];
    snprintf(buffer, 128, "%s/esp8266/QUERY", url);

    if (httpClient.begin(wifiClient, buffer)) {
        int httpCode = httpClient.GET();

        if (httpCode > 0) {
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                String payload = httpClient.getString();
                
                // Serial.printf("[HTTP] received: %s\n", payload.c_str());
                
                StaticJsonDocument<256> doc;
                
                DeserializationError error = deserializeJson(doc, payload);

                // Test if parsing succeeds.
                if (error) {
                    Serial.printf("deserializeJson() failed: %s\r", error.c_str());
                    delay(1000);
                    return;
                } else {
                    String act = doc["act"];
                    Serial.printf("Received act: %s%*s\r", act.c_str(), 40, " ");

                    if (act == "POWER_ON") {
                        digitalWrite(LED_BUILTIN, LOW);

                        if (power == false) {
                            powerOn();
                            power = true;
                        }

                    } else if (act == "POWER_ON_RESET") {
                        digitalWrite(LED_BUILTIN, HIGH);
                        power = false;
                    }

                    delay(2000);
                }
            }
        } else {
            Serial.printf("[HTTP] error: %s\r", httpClient.errorToString(httpCode).c_str());
            delay(1000);
        }
    } else {
        Serial.printf("[HTTP] no connection...\r");
        delay(1000);
    }
}

void loop() {
    if ((WiFiMulti.run() == WL_CONNECTED)) {
        WiFiClient client;
        HTTPClient http;
        Serial.printf("[WiFi] local address: %s\r", client.localIP().toString().c_str());
        delay(1000);
        Serial.printf("[HTTP] begin...%*s\r", 40, " ");
        // const char* iphone_query = "http://172.20.10.4:5000/esp8266/QUERY";
        // const char* home_query = "http://192.168.1.102:5000/esp8266/QUERY";
        
        /*if (http.begin(client, home_query)) {
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
                        String act = doc["act"];
                        Serial.printf("Received act: %s%*s\r", act.c_str(), 40, " ");

                        if (act == "POWER_ON") {
                            digitalWrite(LED_BUILTIN, LOW);

                            if (power == false) {
                                powerOn();
                                power = true;
                            }

                        } else if (act == "POWER_ON_RESET") {
                            digitalWrite(LED_BUILTIN, HIGH);
                            power = false;
                        }

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
        }*/

        const char* home = "http://192.168.1.102:5000";
        // const char* vps = "http://192.168.1.102:5000";

        query(home, http, client);

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
