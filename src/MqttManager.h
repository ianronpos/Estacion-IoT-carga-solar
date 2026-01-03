#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

struct MqttConfig{ 
    String mqttServer;
    int mqttPort;
    String mqttUser;
    String mqttPassword;
    String mqttTopic;
}; 

class MqttManager{
    private: 
        WiFiClient wlanClient; 
        PubSubClient client; 
        MqttConfig config; 
        String espId; 
        unsigned long lastReconnectAttemp; 

    public: 
        /**
         * @brief Contructor, crea la configuracion en funcion del config.h
         */
        MqttManager(); 

        /**
         * @brief Configura la conexion e intenta iniciarla
         */
        void setup(); 
 

        /**
         * @brief Intenta recuperar la conexion mqtt
         */
        void reconnect();
        
        /**
         * @brief compruba si hay conexion
         * 
         * @return true 
         * @return false 
         */
        bool isConnected(); 

        /**
         * @brief devuelve el estado de la conexion mqtt
         * 
         * @return int8_t 
         */
        int8_t getConnectionStatus(); 

           /**
         * @brief envia los datos en formato json 
         * 
         * @param timestap timestap de cuando se hace la medida
         * @param potency potencia medida
         * @param voltage voltaje medido 
         * @return true si se envio de forma correcta 
         * @return false si no se envio de forma correcta
         */
        bool publish(unsigned long timestap, double potency, double voltage); 

        /**
         * @brief 
         * 
         */
        void loop();

}; 


#endif