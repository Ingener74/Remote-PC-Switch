
#ifndef COMMANDER_H
#define COMMANDER_H

#include <Arduino.h>

#include <ArduinoJson.h>

enum Commands {
    SHOW_SETTINGS,
    ADD_WIFI,
    LOAD_WIFI, // Wtf??
    PING,
    TEST_QUERY,
    QUERY_TIMER_CTRL,
    BUILD_IN_LED,
    COMMANDS_SIZE,
};

typedef void (*on_command)(StaticJsonDocument<128> const&);

class Commander
{
public:
    void update();

    on_command commands[COMMANDS_SIZE];
    on_command _unknown;

    // on_command _show_settings;
    // on_command _add_wifi_ap;
    // on_command _ping;    
    // on_command _test_query;
    // on_command _query_timer_ctrl;
    // on_command _build_in_led;
};

#endif
