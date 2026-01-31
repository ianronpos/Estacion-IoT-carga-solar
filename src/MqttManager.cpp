#include "MqttManager.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

MqttManager::MqttManager(){ 
    espId = "Esp8266_" + WiFi.macAddress(); 
}

void MqttManager::setup(){ 
    mqttWatcher = false; 
    client.setClient(wlanClient); 
    client.setBufferSize(512); //Cambia el tamaño del buffer para poder enviar json
    client.setServer(config.mqttServer.c_str(), config.mqttPort); 
    if(client.connect(espId.c_str() ,config.mqttUser.c_str(), config.mqttPassword.c_str())){ 
        Serial.println("Mqtt conectado"); 
    }  else { 
        Serial.println("No se pudo conectar mqtt"); 
    }
    ticker.attach_ms(2200, interruption, this); 
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

bool MqttManager::publish(JsonDocument& doc){
    
    char buff[512]; 
    serializeJson(doc, buff); //pone el archivo como cadena de caracteres en buff
    
    //envia los datos al topico indicado
    return client.publish(config.mqttTopic.c_str(), buff); 
}

void MqttManager::loop(){ 
    if(mqttWatcher){
        if(!client.connected()){ 
            unsigned long now = millis(); 

            if(now - lastReconnectAttemp >= 5000){ 
                lastReconnectAttemp = now; 
                reconnect(); 
            }
        } else{ 
            client.loop(); //Mantiene activa la conexion
            Serial.println("Conexion actualizada");
        }
        mqttWatcher = false;
    }
}

void MqttManager::loadMqttConfig(String server, int port, String user,  String password, String topic){
    config.mqttServer = server; 
    config.mqttPort = port; 
    config.mqttUser = user;
    config.mqttPassword = password; 
    config.mqttTopic = topic; 
}

void MqttManager::checkConfig(){ 
    Serial.println(config.mqttServer);
    Serial.println(config.mqttPort);
    Serial.println(config.mqttUser);
    Serial.println(config.mqttPassword);
    Serial.println(config.mqttTopic);
}

void MqttManager::interruption(MqttManager* pThis){ 
    pThis->mqttWatcher = true; 
}