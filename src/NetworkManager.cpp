#include "WifiEvents.h"
#include "StorageManager.h"
#include "NetworkManager.h"
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <Ticker.h>

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

bool NetworkManager::tryConnect(int timeout){ 
    Serial.println("Intento de conexion"); 
   
    WiFiManager wm; 
    wm.setConfigPortalTimeout(timeout); 
    wm.setAPCallback([this] (WiFiManager *mywm){
        Serial.println("Entrando en modo AP"); 
        
        String ssid = mywm->getConfigPortalSSID(); 

        if(listener){ 
            listener->wifiAp(ssid, info.status);
        }
    }); 
    if(!wm.autoConnect(apInfo.APName.c_str(), apInfo.APPassword.c_str())){ 
        Serial.println("error de conexion"); 
        return false;         
    } else {
        Serial.println("Wifi conectado"); 
        return true; 
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
            if(reconnectTicker) tryConnect();
            break;

            case WL_WRONG_PASSWORD: //Codigo 6
            listener->onWifiDisconnect(info); 
            if(reconnectTicker) tryConnect();
            break;    
            
            case WL_NO_SSID_AVAIL: //Codigo 1
            listener->onWifiDisconnect(info); 
            if(reconnectTicker) tryConnect();
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

void NetworkManager::reconnect(NetworkManager* pThis){ 
    pThis->reconnectTicker = true; 
}