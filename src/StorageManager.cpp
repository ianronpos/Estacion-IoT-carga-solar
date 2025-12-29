#include "StorageManager.h"
#include <LittleFS.h>

bool StorageManager::begin(){ 
    //Comprobar que arranca el sistema de archivos
    if(!LittleFS.begin()){ 
        Serial.println("Error al arrancar el Sistema de archivos littleFS"); 
        return false; 
    }

    Serial.println("Sistema de archivos cargado correctamente"); 
    
    if(!LittleFS.exists("/config.json")){ //Si no existe el archivo de configuracion 
        Serial.println("No se encontro el archivo de configuracion"); 
        createDefaultConfig(); 
    }

    //TODO: COMPROBAR SI LOS ULTIMOS DATOS SE HAN ENVIADO Y BORRARLOS
}

bool StorageManager::format(){ 
    if(!LittleFS.format()){ 
        Serial.println("No se pudo formatear la memoria"); 
        return false; 
    }

    //Se crea crea el archivo con la configuracion por defecto
    createDefaultConfig(); 
    return true; 
}

//=================================================================================================================================================
//TODO: Implementar cuando tenga idea de que se necesita para MQTT
//=================================================================================================================================================
void StorageManager::createDefaultConfig(){ 
    
}

