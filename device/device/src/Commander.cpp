#include "Commander.h"

void Commander::update()
{
    int bytes = Serial.available();
    if (bytes > 0) {
        String s = Serial.readString();
        
        StaticJsonDocument<128> command;
        deserializeJson(command, s);

        int c = command["c"].as<int>();

        if (c >= 0 && c < COMMANDS_SIZE) {
            commands[c](command);
        } else if (_unknown) {
            _unknown(command);
        }

    //     if (command["c"].as<int>() == 0 && _show_settings) { // show settings
    //         _show_settings(command);
    //     } else if (command["c"].as<int>() == 1 && _add_wifi_ap) { // add wifi
    //         _add_wifi_ap(command);
    // //         if (settings.containsKey("wifi")) {
    // //             settings["wifi"]["ssid"] = command["p"]["ssid"];
    // //             settings["wifi"]["pswd"] = command["p"]["pswd"];
    // //         } else {
    // //             JsonObject w = settings.createNestedObject("wifi");
    // //             w["ssid"] = command["p"]["ssid"];
    // //             w["pswd"] = command["p"]["pswd"];
    // //         }
    // //         SaveSettings(settings);
    //     } else if (command["c"].as<int>() == 2) { // load wifi
    //     } else if (command["c"].as<int>() == 3 && _ping) { // ping
    //         _ping(command);
    //     } else if (command["c"].as<int>() == 4 && _test_query) { // test_query
    //         _test_query(command);
    //     } else if (command["c"].as<int>() == 5 && _query_timer_ctrl) { // test_query
    //         _query_timer_ctrl(command);
    //     } else if (command["c"].as<int>() == 6 && _build_in_led) { // test_query
    //         _build_in_led(command);
    //     } else if (_unknown) {
    //         _unknown(command);
    //     }
    }
}
