#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "ITransmisor.h"

struct MqttConfig{ 
    String mqttServer;
    int mqttPort;
    String mqttUser;
    String mqttPassword;
    String mqttTopic;
}; 

class MqttManager : public ITransmisor{
    private: 
        WiFiClient wlanClient; 
        PubSubClient client; 
        MqttConfig config; 
        String espId; 
        unsigned long lastReconnectAttemp; 
        Ticker ticker; 
        volatile bool mqttWatcher; 

        static void IRAM_ATTR interruption(MqttManager* pThis);

        //Implementar metodo que devuelva el stado de la conexion mqtt
    public: 
        MqttManager();  
        void setup(); 
        void reconnect(); 
        bool isConnected(); 
        int8_t getConnectionStatus() override; 
        void loadMqttConfig(String server, int port, String user,  String password, String topic) override; 
        bool publish(JsonDocument& doc) override; 
        void loop();


        void checkConfig(); 
}; 


#endif