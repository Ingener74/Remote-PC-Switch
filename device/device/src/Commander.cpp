#include "Commander.h"

void Commander::update()
{
    int bytes = Serial.available();
    if (bytes > 0) {
        String s = Serial.readString();
        
        StaticJsonDocument<128> command;
        deserializeJson(command, s);

        if (command["c"].as<int>() == 0 && _show_settings) { // show settings
            _show_settings();
        } else if (command["c"].as<int>() == 1 && _add_wifi_ap) { // add wifi
            _add_wifi_ap();
    //         if (settings.containsKey("wifi")) {
    //             settings["wifi"]["ssid"] = command["p"]["ssid"];
    //             settings["wifi"]["pswd"] = command["p"]["pswd"];
    //         } else {
    //             JsonObject w = settings.createNestedObject("wifi");
    //             w["ssid"] = command["p"]["ssid"];
    //             w["pswd"] = command["p"]["pswd"];
    //         }
    //         SaveSettings(settings);
    //         String set_str;
    //         serializeJson(settings, set_str);
    //         Serial.println(set_str);
    //         delay(1000);

    //         SendStatus(String(set_str.length()).c_str());
    //         delay(1000);

    //         SendStatus("Settings saved");
    //         delay(1000);
        } else if (command["c"].as<int>() == 2) { // load wifi
    //         LoadSettings(settings);
    //         // String set_str;
    //         // serializeJson(settings, set_str);
    //         // Serial.println(set_str);
    //         // delay(1000);            
    //         // SendStatus("Settings loaded");
    //         // delay(1000);
        } else if (command["c"].as<int>() == 3 && _ping) { // ping
            _ping();
        } else if (_unknown) {
            _unknown();            
        }
    }
}
