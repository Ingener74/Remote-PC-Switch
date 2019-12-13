#include <FS.h>
#include "Settings.h"

int Settings::Save(StaticJsonDocument<256>& settings) {
    SPIFFS.begin();

    File f = SPIFFS.open("/s.json", "w");
    if (!f) {
        SPIFFS.end();
        return 1;
    }
    String s;
    serializeJson(settings, s);
    if (f.println(s) == s.length()) {
        SPIFFS.end();
        return 0;
    }
    SPIFFS.end();
    return 2;
}

int Settings::Load(StaticJsonDocument<256>& settings) {
    SPIFFS.begin();

    if (!SPIFFS.exists("/s.json")) {
        SPIFFS.end();
        return 1;
    }

    File f = SPIFFS.open("/s.json", "r");
    if (!f) {
        SPIFFS.end();
        return 2;
    }

    String s = f.readString();
    deserializeJson(settings, s);
    SPIFFS.end();
    return 0;
}

