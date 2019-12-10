
#ifndef WIFIER_H
#define WIFIER_H

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
// #include <ESP8266HTTPClient.h>

typedef void (*on_status_change_t)();
class WiFier {
public:
    WiFier();
    void update();

    ESP8266WiFiMulti _wifi;
    on_status_change_t _on_connected;
    on_status_change_t _on_disconnected;
    wl_status_t _status;
};

#endif
