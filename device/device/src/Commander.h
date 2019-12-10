
#ifndef COMMANDER_H
#define COMMANDER_H

#include <Arduino.h>

#include <ArduinoJson.h>

typedef void (*on_command)();

class Commander
{
public:
    void update();

    on_command _show_settings;
    on_command _add_wifi_ap;
    on_command _ping;
    on_command _unknown;
};

#endif
