#include "MqttManager.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <config.h>
#include <ArduinoJson.h>

MqttManager::MqttManager(){ 
    config.mqttServer = MQTT_SERVER; 
    config.mqttPort = MQTT_PORT; 
    config.mqttUser = MQTT_USER;
    config.mqttPassword = MQTT_PASSWORD; 
    config.mqttTopic = MQTT_TOPIC; 
    espId = "Esp8266_" + WiFi.macAddress(); 
}

void MqttManager::setup(){ 
    client.setClient(wlanClient); 
    client.setBufferSize(512); //Cambia el tamaño del buffer para poder enviar json
    client.setServer(config.mqttServer.c_str(), config.mqttPort); 
    if(client.connect(espId.c_str() ,config.mqttUser.c_str(), config.mqttPassword.c_str())){ 
        Serial.println("Mqtt conectado"); 
    }  else { 
        Serial.println("No se pudo conectar mqtt"); 
    }
}

void MqttManager::reconnect(){ 
    if(client.connect(espId.c_str() ,config.mqttUser.c_str(), config.mqttPassword.c_str())){ 
        Serial.println("Mqtt conectado"); 
    } else { 
        Serial.println("No se ha podrido reconectar mqtt"); 
    }
}

bool MqttManager::isConnected(){ 
    return client.state() == 0 ? true : false; 
}

int8_t MqttManager::getConnectionStatus(){ 
    return client.state(); 
}

bool MqttManager::publish(unsigned long timestamp, double potency, double voltage){ 
    JsonDocument doc; 

    //Añadiendo los valores 
    doc["timestamp"] = timestamp;
    doc["potencia"] = potency;  
    doc["voltaje"] = voltage; 

    char buff[256]; 
    serializeJson(doc, buff); //pone el archivo como cadena de caracteres en buff
    
    //envia los datos al topico indicado
    return client.publish(config.mqttTopic.c_str(), buff); 
}

void MqttManager::loop(){ 
    if(!client.connected()){ 
        unsigned long now = millis(); 

        if(now - lastReconnectAttemp >= 5000){ 
            lastReconnectAttemp = now; 
            reconnect(); 
        }
    } else{ 
        client.loop(); //Mantiene activa la conexion
    }
}