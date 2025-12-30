#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <Arduino.h>

struct MqttConfig{ 
    String mqttServer;
    int mqttPort;
    String mqttUser;
    String mqttPassword;
    String mqttTopic;
}; 



#endif