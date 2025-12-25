#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "WifiEvents.h"

class WifiManager { 
    private: 
        WifiEvents* listener; 
        WifiInfo info; 
        bool credentialsValid; 
        char ssid [33];
        char password [65];
        wl_status_t status; 
        
        //Event Handlers 
        WiFiEventHandler gotIpEventHandler;
        WiFiEventHandler disconneted; 

    public: 
        
        /**
         * @brief Construye un objeto WifiManager, con valores predeterminado
        */
        WifiManager();  
        
                
        /**
         * @brief Asigna el valor ptr al puntero listener
         * @param ptr 
         */
        void setListner(WifiEvents* listener);         

        //Funciones para cargar en memoria
        //TODO: Implementar las funciones
        bool saveCredentials(); 
        bool loadCredentials();
        bool hasCredentials() const; 

        /**
         * @brief Inicializa los events handler
         */
        void setup();
        
        /**
         * @brief Intenta conectarse segun las credenciales que tiene guardadas
         */
        void tryConnect();

        void scan(); 

                
        /**
         * @brief loop principal de Manager, comprueba estados... 
        */
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
         * * Para ver el diagrama completo (si VS Code lo permite):
         * [Ver Diagrama en docs](../docs/Codigos_wl_status.png)
         * * @return wl_status_t Estado actual.
         */
        wl_status_t getStatus() const; 
      
        /**
         * @brief Actualiza el estado de la conexion
         */
        void updateState(); 
}; 

#endif
