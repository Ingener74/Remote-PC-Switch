#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

// #include <Keyboard.h>

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

    // while(WiFi.status() != WL_CONNECTED) {
    //     delay(500);
    //     Serial.print(".");
    // }
    // Serial.println("");

    // Serial.println("WiFi connected");
    // Serial.print("IP address: "); Serial.println(WiFi.localIP());

    // Keyboard.begin();
}

// const char* wheel = "|/-\\";
void loop() {
    // for (int i = 0; i < 4; ++i) {
    //     Serial.print(wheel[i]); Serial.print("\r");
    //     delay(100);
    // }

    if ((WiFiMulti.run() == WL_CONNECTED)) {
        WiFiClient client;
        HTTPClient http;
        Serial.printf("[HTTP] begin...%*s\r", 40, " ");
        if (http.begin(client, "http://172.20.10.4:8000/")) {
            int httpCode = http.GET();

            if (httpCode > 0) {
                if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                    String payload = http.getString();
                    Serial.printf("[HTTP] received: %s\n", payload.c_str());
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
