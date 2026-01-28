#ifndef ITRANSMISOR_H 
#define ITRANSMISOR_H

#include <Arduino.h>
#include <ArduinoJson.h>

class ITransmisor{
    private: 

    public:
        virtual void loadMqttConfig(String server, int port, String user,  String password, String topic) = 0;  
        virtual bool publish(JsonDocument& doc) = 0; 
        virtual ~ITransmisor() = default;
        virtual int8_t getConnectionStatus() = 0;
};

#endif