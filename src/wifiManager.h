#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "WifiEvents.h"

class WifiManager { 
    private: 
        WifiEvents* listener; 
        WifiInfo info; 
        bool credentialsValid; 
        char ssid [33]; //32 bits por el estandar + 1 bit final que es el bit nulo 
        char password [65];
        wl_status_t status; 
        
        //Event Handlers 
        WiFiEventHandler gotIpEventHandler;
        WiFiEventHandler disconneted; 

    public: 
        WifiManager();  
        
        //Funciones de para WifiEvents 
        void setListner(WifiEvents* listener);         

        //Funciones para cargar en memoria
        bool saveCredentials(); 
        bool loadCredentials();
        bool hasCredentials() const; 

        
        void setup(); 
        void scan(); 

        //loops
        void loop(); 
        
        //Getters & setters
        wl_status_t getStatus() const; 
        void updateState(); 
}; 

#endif
