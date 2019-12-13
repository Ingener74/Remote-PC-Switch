#include <Arduino.h>
#include <ArduinoJson.h>

#include <FS.h>
#include <ESP8266HTTPClient.h>

#include "WiFier.h"
#include "Commander.h"
#include "Settings.h"
#include "PeriodTimer.h"

const int POWER_PIN = 5;
bool power = false;

bool query_timer_ctrl = false;
bool build_in_led_ctrl = false;

void on_connected(String const&);
void on_disconnected();

void on_show_settings_command(StaticJsonDocument<128> const&);
void on_add_ap_command(StaticJsonDocument<128> const&);
void on_load_wifi_command(StaticJsonDocument<128> const&);
void on_pong_command(StaticJsonDocument<128> const&);
void on_unknown_command(StaticJsonDocument<128> const&);
void on_test_query_command(StaticJsonDocument<128> const&);
void on_query_timer_ctrl_command(StaticJsonDocument<128> const&);
void on_build_in_led_command(StaticJsonDocument<128> const&);

void on_query_timer_period();

WiFier wiFier;
Commander commander;
PeriodTimer queryTimer(5000, on_query_timer_period);

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    pinMode(POWER_PIN, OUTPUT);
    digitalWrite(POWER_PIN, LOW);

    wiFier._on_connected = on_connected;
    wiFier._on_disconnected = on_disconnected;
    commander._unknown = on_unknown_command;
    commander.commands[SHOW_SETTINGS] = on_show_settings_command;
    commander.commands[ADD_WIFI] = on_add_ap_command;
    commander.commands[LOAD_WIFI] = on_load_wifi_command;
    commander.commands[PING] = on_pong_command;
    commander.commands[TEST_QUERY] = on_test_query_command;
    commander.commands[QUERY_TIMER_CTRL] = on_query_timer_ctrl_command;
    commander.commands[BUILD_IN_LED] = on_build_in_led_command;

    Serial.begin(115200);

    StaticJsonDocument<64> response;
    response["start"] = "start";
    String s;
    serializeJson(response, s);
    Serial.println(s);
    
    StaticJsonDocument<256> settings;
    int r = Settings::Load(settings);
    if (r == 0) {
        const char* ssid = settings["ssid"];
        const char* pswd = settings["pswd"];
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

void loop() {
    wiFier.update();
    commander.update();
    queryTimer.update();
}

////////////////////////////////////////////////////////////////
//////////////////////////// WiFier ////////////////////////////
////////////////////////////////////////////////////////////////
void on_connected(String const& ip)
{
    StaticJsonDocument<64> response;
    response["wifier"] = "connnected";
    response["ip"] = ip;
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

///////////////////////////////////////////////////////////////////
//////////////////////////// Commander ////////////////////////////
///////////////////////////////////////////////////////////////////
void on_pong_command(StaticJsonDocument<128> const& command)
{
    StaticJsonDocument<64> response;
    response["commander"] = "pong";
    String s;
    serializeJson(response, s);
    Serial.println(s);
}

void on_unknown_command(StaticJsonDocument<128> const& command)
{
    StaticJsonDocument<64> response;
    response["commander"] = "i dont know this command";
    String s;
    serializeJson(response, s);
    Serial.println(s);
}

void on_show_settings_command(StaticJsonDocument<128> const& command)
{
    StaticJsonDocument<64 + 256> response;
    response["commander"] = "settings";
    StaticJsonDocument<256> settings;
    int r = Settings::Load(settings);
    if (r == 0) {
        String s;
        serializeJson(settings, s);
        response["settings"] = s; //settings.to<JsonObject>();
    } else {
        response["cant load settings"] = r;
    }
    String s;
    serializeJson(response, s);
    Serial.println(s);
}

void on_add_ap_command(StaticJsonDocument<128> const& command)
{    
    StaticJsonDocument<64> response;
    response["commander"] = "i dont want add ap";
    String s;
    serializeJson(response, s);
    Serial.println(s);
}

void on_load_wifi_command(StaticJsonDocument<128> const&)
{
    StaticJsonDocument<64> response;
    response["commander"] = "i dont know that to do";
    String s;
    serializeJson(response, s);
    Serial.println(s);
}

void on_test_query_command(StaticJsonDocument<128> const& command)
{
    StaticJsonDocument<256> response;
    response["commander"] = "test_query";
    JsonObject ans = response.createNestedObject("ans");
    if (wiFier.isConnected()) {
        WiFiClient client;
        HTTPClient http;

        if (http.begin(client, command["q"])) {
            ans["get"] = "ok";
            
            int httpCode = http.GET();
            
            if (httpCode > 0) {
                ans["get_code"] = httpCode;
                
                if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                    String payload = http.getString();
                    ans["payload"] = payload;
                }
            } else {
                ans["get"] = "fail";
                ans["get_error"] = http.errorToString(httpCode);
            }
            http.end();
        } else {
            ans["http"] = "fail";
        }
    } else {
        ans["wifi"] = "not connected";
    }
    String s;
    serializeJson(response, s);
    Serial.println(s);
}

void on_query_timer_ctrl_command(StaticJsonDocument<128> const& command)
{
    StaticJsonDocument<64> response;
    response["commander"] = "query_timer_ctrl";
    query_timer_ctrl = command["e"].as<bool>();
    response["enabled"] = query_timer_ctrl;
    String s;
    serializeJson(response, s);
    Serial.println(s);
}

void on_build_in_led_command(StaticJsonDocument<128> const& command)
{
    StaticJsonDocument<64> response;
    response["commander"] = "build_in_led_ctrl";
    build_in_led_ctrl = command["e"].as<bool>();
    digitalWrite(LED_BUILTIN, build_in_led_ctrl ? LOW : HIGH);
    response["enabled"] = build_in_led_ctrl;
    String s;
    serializeJson(response, s);
    Serial.println(s);
}
/////////////////////////////////////////////////////////////////////
//////////////////////////// Query timer ////////////////////////////
/////////////////////////////////////////////////////////////////////
void on_query_timer_period()
{    
    if (query_timer_ctrl) {
        StaticJsonDocument<64> response;
        response["query_timer"] = "Hello, I'm query timer";
        String s;
        serializeJson(response, s);
        Serial.println(s);
    }
}

