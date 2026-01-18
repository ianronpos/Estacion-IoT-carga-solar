#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include "WifiEvents.h"

//Tiempo que trascurre desde que se crea el AP desde que falla la conexion hasta que se reinicia
static constexpr int APTimeout = 180; 


class NetworkManager { 
    private: 
        WifiEvents* listener; 
        WifiInfo info; 
        bool credentialsValid; 
        char ssid [33];
        char password [65];
        wl_status_t status; 
        ApInfo apInfo; 

        //Event Handlers 
        WiFiEventHandler gotIpEventHandler;
        WiFiEventHandler disconnetedEventHandler; 
        WiFiEventHandler connectedEventHandler; 

    public: 
        NetworkManager();  
        void setListner(WifiEvents* listener);        
        void setup(const ApInfo& apInfo);
        void tryConnect(); 
        void scan(); 
        void loop(); 
        
        /**
         * @brief Devuelve el estado actual de la conexión WiFi.
         * * @details Este método devuelve el código numérico del estado interno del ESP8266.
         * Si el enlace a la imagen local falla en VS Code, usa esta referencia rápida:
         * * | Código | Estado (Enum)        | Significado           |
         * | :----: | :------------------- | :-------------------- |
         * |   0    | WL_IDLE_STATUS       | Inactivo              |
         * |   1    | WL_NO_SSID_AVAIL     | SSID no encontrado    |
         * |   3    | WL_CONNECTED         | Conectado con IP      |
         * |   4    | WL_CONNECT_FAILED    | Fallo general         |
         * |   6    | WL_WRONG_PASSWORD    | Contraseña incorrecta |
         * |   7    | WL_DISCONNECTED      | Desconectado          |
         * * Para ver el diagrama completo :
         * https://github.com/ianronpos/Estacion-IoT-carga-solar/blob/main/docs/Codigos_wl_status.png
         * * @return wl_status_t Estado actual.
         */
        uint8_t getStatus() const; 
        void updateState(); 

        /**
         * @brief Obtiene la info de especifica de WifiInfo 
         * 
         * @tparam T 
         * @param field 
         * @return T 
         */
        template <typename T>
        T getInfoField(T WifiInfo::*field) {
            return info.*field;
        }
}; 

#endif
