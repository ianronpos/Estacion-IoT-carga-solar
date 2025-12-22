#include "wifiManager.h"
#include <ESP8266WiFi.h>
#include <EEPROM.h>


WifiManager::WifiManager()
    : credentialsValid(false) {
    ssid[0] = '\0';
    password[0] = '\0';
}



void WifiManager::setup(){ 
    //Inicializar handlers
    gotIpEventHandler = WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP& event){ 
        //INICIAR MQTT
    }); 

    disconneted = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected& event){
        setStatus(); 
        
    }); 

    WiFi.mode(WIFI_STA); //Modo por defecto STA 
    WiFi.begin(ssid, password); //Se intenta conectar
    
    unsigned long timeout = 30000; //30" para establecer conexione  
    WiFi.waitForConnectResult(timeout); 

    setStatus(); 
}


//Getters & Setters
wl_status_t WifiManager::getStatus() const { 
    return status;  
}

void WifiManager::setStatus(){ 
    status = WiFi.status(); 
}