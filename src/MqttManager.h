#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

struct MqttConfig{ 
    const char* mqttServer;
    int mqttPort;
    const char* mqttUser;
    const char* mqttPassword;
    const char* mqttTopic;
}; 

class MqttManager{
    private: 
        WiFiClient wlanClient; 
        PubSubClient client; 
        MqttConfig config; 

    public: 
        MqttManager(); 
        void setup(MqttConfig& config); 
        void loop(); 
        bool publish(unsigned long timestap, double potency, double voltage); 
        int8_t isConnected(); 
        //getLastError

}; 


#endif