#include <Arduino.h>
#include <ESP8266WiFi.h>

const char* SSID = "iPhone";
const char* PW = "f0755vhxgasa9";

void setup() {
    Serial.begin(115200);

    Serial.println();Serial.println();Serial.println();
    Serial.println("Start");

    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PW);

    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");

    Serial.println("WiFi connected");
    Serial.print("IP address: "); Serial.println(WiFi.localIP());
}

const char* wheel = "|/-\\";
void loop() {
    for (int i = 0; i < 4; ++i) {
        Serial.print(wheel[i]); Serial.print("\r");
        delay(1000);
    }
}
