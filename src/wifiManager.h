#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

class WifiManager { 
    private: 

        bool credentialsValid; 
        char ssid [33]; //32 bits por el estandar + 1 bit final que es el bit nulo 
        char password [65];
        wl_status_t status; 
        
        //Event Handlers 
        WiFiEventHandler gotIpEventHandler;
        WiFiEventHandler disconneted; 

        

    public: 
        //Builder
        WifiManager(); 

        bool saveCredentials(); 
        bool loadCredentials();
        bool hasCredentials() const; 

        void setup(); 
        void scan(); 
        wl_status_t getStatus() const; 
        void setStatus(); 
}; 

#endif
