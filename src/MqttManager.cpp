#include "MqttManager.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

MqttManager::MqttManager(){ 
    client.setClient(wlanClient); 
    

}

void MqttManager::setup(MqttConfig& config){ 

}