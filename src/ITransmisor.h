#ifndef ITRANSMISOR_H 
#define ITRANSMISOR_H

#include <Arduino.h>
#include <ArduinoJson.h>

class ITransmisor{
    private:
        
    public:
        virtual bool publish(JsonDocument& doc) = 0; 
        virtual ~ITransmisor() = default;
};

#endif