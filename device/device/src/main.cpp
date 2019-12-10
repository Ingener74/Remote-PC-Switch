#include <Arduino.h>
#include <ArduinoJson.h>

#include <FS.h>

#include <WiFier.h>
#include <Commander.h>

const int POWER_PIN = 5;
bool power = false;

int SaveSettings(StaticJsonDocument<256>& settings);
int LoadSettings(StaticJsonDocument<256>& settings);

WiFier wiFier;
Commander commander;

void on_connected();
void on_disconnected();

void on_pong_command();
void on_unknown_command();
void on_show_settings_command();
void on_add_ap_command();

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    pinMode(POWER_PIN, OUTPUT);
    digitalWrite(POWER_PIN, LOW);

    wiFier._on_connected = on_connected;
    wiFier._on_disconnected = on_disconnected;
    commander._ping = on_pong_command;
    commander._unknown = on_unknown_command;
    commander._show_settings = on_show_settings_command;
    commander._add_wifi_ap = on_add_ap_command;

    Serial.begin(115200);

    StaticJsonDocument<64> response;
    response["start"] = "start";
    String s;
    serializeJsonPretty(response, s);
    Serial.println(s);
    
    StaticJsonDocument<256> settings;
    int r = LoadSettings(settings);
    if (r == 0) {
        const char* ssid = settings["ssid"];
        const char* pswd = settings["pswd"];
        Serial.println(ssid);
        Serial.println(pswd);
        wiFier._wifi.addAP(ssid, pswd);
    } else {
        StaticJsonDocument<64> response;
        response["cant load settings"] = r;
        String s;
        serializeJsonPretty(response, s);
        Serial.println(s);
    }
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

// void query(const char* url, HTTPClient& httpClient, WiFiClient& wifiClient) {
//     char buffer[128];
//     snprintf(buffer, 128, "%s/esp8266/QUERY", url);

//     if (httpClient.begin(wifiClient, buffer)) {
//         int httpCode = httpClient.GET();

//         if (httpCode > 0) {
//             if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
//                 String payload = httpClient.getString();
                
//                 // Serial.printf("[HTTP] received: %s\n", payload.c_str());
                
//                 StaticJsonDocument<256> doc;
                
//                 DeserializationError error = deserializeJson(doc, payload);

//                 // Test if parsing succeeds.
//                 if (error) {
//                     SendStatus("deserializeJson() failed: ");
//                     delay(1000);
//                     return;
//                 } else {
//                     String act = doc["act"];
//                     SendStatus("Received act: ");

//                     if (act == "POWER_ON") {
//                         digitalWrite(LED_BUILTIN, LOW);

//                         if (power == false) {
//                             powerOn();
//                             power = true;
//                         }

//                     } else if (act == "POWER_ON_RESET") {
//                         digitalWrite(LED_BUILTIN, HIGH);
//                         power = false;
//                     }

//                     delay(2000);
//                 }
//             }
//         } else {
//             SendStatus("[HTTP] error: ");
//             delay(1000);
//         }
//     } else {
//         SendStatus("[HTTP] no connection...");
//         delay(1000);
//     }
// }

int SaveSettings(StaticJsonDocument<256>& settings) {
    SPIFFS.begin();

    File f = SPIFFS.open("/s.json", "w");
    if (!f) {
        return 1;
    }
    String s;
    serializeJson(settings, s);
    if (f.println(s) == s.length()) {
        // SPIFFS.end();
        return 0;
    }
    // SPIFFS.end();
    return 2;
}

int LoadSettings(StaticJsonDocument<256>& settings) {
    SPIFFS.begin();

    if (!SPIFFS.exists("/s.json")) {
        return 1;
    }

    File f = SPIFFS.open("/s.json", "r");
    if (!f) {
        return 2;
    }

    String s = f.readString();
    deserializeJson(settings, s);
    return 0;
}

void loop() {
    wiFier.update();
    commander.update();
}

void on_connected()
{
    StaticJsonDocument<64> response;
    response["wifier"] = "connnected";
    String s;
    serializeJson(response, s);
    Serial.println(s);
}

void on_disconnected()
{
    StaticJsonDocument<64> response;
    response["wifier"] = "disconnnected";
    String s;
    serializeJson(response, s);
    Serial.println(s);
}

void on_pong_command()
{
    StaticJsonDocument<64> response;
    response["commander"] = "pong";
    String s;
    serializeJson(response, s);
    Serial.println(s);
}

void on_unknown_command()
{
    StaticJsonDocument<64> response;
    response["commander"] = "i dont know this command";
    String s;
    serializeJson(response, s);
    Serial.println(s);
}

void on_show_settings_command()
{
    StaticJsonDocument<64 + 256> response;
    response["commander"] = "settings";
    StaticJsonDocument<256> settings;
    int r = LoadSettings(settings);
    if (r == 0) {
        response["settings"] = settings.to<JsonObject>();
    } else {
        response["cant load settings"] = r;
    }
    String s;
    serializeJson(response, s);
    Serial.println(s);
}

void on_add_ap_command()
{    
    StaticJsonDocument<64> response;
    response["commander"] = "i dont want add ap";
    String s;
    serializeJson(response, s);
    Serial.println(s);
}
