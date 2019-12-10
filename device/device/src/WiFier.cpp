
#include "WiFier.h"

WiFier::WiFier()
{
    WiFi.mode(WIFI_STA);
    // WiFiMulti.addAP(ssid, pswd);
    _on_connected = 0;
    _on_disconnected = 0;
    _status = WL_DISCONNECTED;
}

void WiFier::update()
{
    // WiFiClient client;
    // HTTPClient http;
    
    wl_status_t status = _wifi.run();
    if (status == WL_CONNECTED && _status != WL_CONNECTED && _on_connected) {
        _on_connected();
    }
    if (status == WL_DISCONNECTED && _status != WL_DISCONNECTED && _on_disconnected) {
        _on_disconnected();
    }

    _status = status;
}
