#include "NetworkManager.h"
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include "WifiEvents.h"
#include "config.h"
#include "StorageManager.h"

NetworkManager::NetworkManager()
    : listener(nullptr), credentialsValid(false) {
    ssid[0] = '\0';
    password[0] = '\0';
}

void NetworkManager::setup(const ApInfo& apInfo){
    //Inicializar handlers
    gotIpEventHandler = WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP& event){ 
        //INICIAR MQTT <- IMPORTATNTE, aqui se tiene que avisar de que ya existe conexion para iniciar MQTT, si no -> kernell panic
        Serial.println("Conexion exitosa"); 
        updateState(); 
        info.ip = event.ip; 
        info.ssid = WiFi.SSID(); 
    }); 

    disconnetedEventHandler = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected& event){
        updateState(); 
    }); 

    connectedEventHandler = WiFi.onStationModeConnected([this] (const WiFiEventStationModeConnected& event){
        updateState(); 
    });

    this->apInfo = apInfo; 
}  

void NetworkManager::tryConnect(){ 
    Serial.println("Intento de conexion"); 
   
    WiFiManager wm; 
    wm.setConfigPortalTimeout(APTimeout); 
    wm.setAPCallback([this] (WiFiManager *mywm){
        Serial.println("Entrando en modo AP"); 
        
        String ssid = mywm->getConfigPortalSSID(); 

        if(listener){ 
            listener->wifiAp(ssid, info.status);
        }
    }); 
    if(!wm.autoConnect(APName, APPassword)){ 
        Serial.println("error de conexion"); 
        ESP.restart(); 
    } 
}

void NetworkManager::setListner(WifiEvents* ptr){ 
    this->listener = ptr; 
}

void NetworkManager::loop() { 
    if(listener){
        switch(getStatus()){ 
            case WL_CONNECTED: //Codigo 3
            listener->onWifiConnect(info); 
            break; 

            case WL_DISCONNECTED: //Codigo 7
            listener->onWifiDisconnect(info); 
            tryConnect();
            break;

            case WL_WRONG_PASSWORD: //Codigo 6
            listener->onWifiDisconnect(info); 
            tryConnect(); 
            break;    
            
            case WL_NO_SSID_AVAIL: //Codigo 1
            listener->onWifiDisconnect(info); 
            tryConnect(); 
            break; 

            default: 
            break; 
        }
    }
}

uint8_t NetworkManager::getStatus() const { 
    return info.status;  
}

void NetworkManager::updateState(){ 
    info.status = WiFi.status(); 
    Serial.println(WiFi.status());
}