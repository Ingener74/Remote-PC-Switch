
#ifndef SETTINGS_H
#define SETTINGS_H

#include <ArduinoJson.h>

class Settings
{
public:
    static int Save(StaticJsonDocument<256>& settings);
    static int Load(StaticJsonDocument<256>& settings);
};

#endif
