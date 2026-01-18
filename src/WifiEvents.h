#ifndef WIFIEVENTS_H
#define WIFIEVENTS_H 

#include <IPAddress.h>
#include <ESP8266WiFi.h>


struct WifiInfo{
    IPAddress ip;
    String ssid; 
    uint8_t status;
};


class WifiEvents{ 
    public: 
        virtual void onWifiConnect(const WifiInfo& info) = 0; 
        virtual void onWifiDisconnect(const WifiInfo& info) = 0; 
        virtual void wifiAp(String ssid, uint8_t error) = 0; 
        
        virtual ~WifiEvents() = default; //Destructor 
};

#endif 