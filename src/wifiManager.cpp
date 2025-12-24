#include "wifiManager.h"
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include "WifiEvents.h"
#include "config.h"

// Builder 
WifiManager::WifiManager()
    : listener(nullptr), credentialsValid(false) {
    ssid[0] = '\0';
    password[0] = '\0';
}

void WifiManager::setup(){
    //Inicializar handlers
    gotIpEventHandler = WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP& event){ 
        //INICIAR MQTT
        Serial.println("Conexion exitosa"); 
        updateState(); 
        info.ip = event.ip; 
        info.ssid = WIFI_SSID; 
        info.rssi = WiFi.RSSI(); 
    }); 


    disconneted = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected& event){
        updateState(); 
        info.disconnectReason = event.reason; //esto devuelve  un numero, buscar su significado
        //Si quito el punto de acceso -> 201
        //Si reinicio y la red no existe 201
    }); 
 
    Serial.println("Intento de conexion"); 
    WiFi.mode(WIFI_STA); //Modo por defecto STA 
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); //Se intenta conectar
}


void WifiManager::setListner(WifiEvents* ptr){ 
    this->listener = ptr; 
}

void WifiManager::loop() { 
    if(listener){
        switch(getStatus()){ 
            case WL_DISCONNECTED: //7
                listener->onWifiDisconnect(info); 
                break;
            
            case WL_CONNECTED: //3
                listener->onWifiConnect(info); 
                break; 
            default: 
                break;
        }
    }
}

//Getters & Setters
wl_status_t WifiManager::getStatus() const { 
    return status;  
}

void WifiManager::updateState(){ 
    status = WiFi.status(); 

}
