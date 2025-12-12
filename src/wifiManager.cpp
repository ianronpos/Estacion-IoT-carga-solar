#include "wifiManager.h"
#include <ESP8266WiFi.h>


WifiManager::WifiManager(const char* ssid, const char* password){ 
    this->ssid = ssid; 
    this->password = password; 
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
    //Implementacion de espera ocupada + timeout para comprobar su funcionalida
    unsigned long timeout = 30000;  
    WiFi.waitForConnectResult(timeout); 

    setStatus(); 
}

wl_status_t WifiManager::getStatus(){ 
    return status;  
}

void WifiManager::setStatus(){ 
    status = WiFi.status(); 
}