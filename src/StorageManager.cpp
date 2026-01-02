#include "StorageManager.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

bool StorageManager::begin(){
    bool res = false; 
    if(!LittleFS.begin()){ //Si no se puede arrancar el sistema de archivos
        Serial.println("Error al arrancar el Sistema de archivos littleFS. Probando formateo..."); 
       
        if(!format()){ 
            Serial.println("No se ha podido formatear el sistema de archivos"); 
            res = false; 
        } else { 
            if(!LittleFS.begin()){ 
                Serial.println("No se ha podido montar incluso despues de formateo"); 
                res = false; 
            } else{
                //No hace falta comprobar que existe por que se ha formateado previamente el sistema de archivos 
                createDefaultConfig(); 
                res = true; 
            }
        }
    } else { 
        Serial.println("Sistema de archivos cargado correctamente"); 

        if(!LittleFS.exists("/config.json")){ //Si no existe el archivo de configuracion 
            Serial.println("No se encontro el archivo de configuracion"); 
            createDefaultConfig(); 
        }
        res = true; 
    }
    return res; 
}

bool StorageManager::format(){ 
    if(!LittleFS.format()){ 
        Serial.println("No se pudo formatear la memoria"); 
        return false; 
    }

    if(!LittleFS.begin()){ 
        Serial.println("No se puede pontar despues de formateo"); 
        return false; 
    }

    Serial.println("Formateo y montaje correcto, cargando la configuracion por defecto"); 
    //Se crea crea el archivo con la configuracion por defecto
    createDefaultConfig(); 
    return true; 
}

void StorageManager::saveMeasure(unsigned long timestamp, double potency, double voltage){
    //Abrimos el archivo en modo append, si no existe, se crea 
    File file = LittleFS.open("/log.csv", "a"); 

    if(!file) { //Si el archivo no se habre
        Serial.println("Error al abrir el archivo log.csv"); 
        return; 
    }

    //Añadimos cabecera
    file.printf("%lu,%.2f,%.2f\n", timestamp, potency, voltage); 

    file.close(); 
}

bool StorageManager::hasPendingData(){ 
    File file = LittleFS.open("/log.csv", "r"); 

    if(!file){ 
        Serial.println("El archivo no existe"); 
        file.close();
        return false; 
    } else if(file.size() == 0 || file.size() == 1){ 
        Serial.println("El archivo esta vacio"); 
        file.close();
        return false; 
    } else {
        Serial.println("El archivo tiene contenido"); 
        file.close();
        return true; 
    }
    
}

bool StorageManager::PrepareDataForUpload(){ 
    if(!hasPendingData()){ 
        return false; 
    } else { 
        LittleFS.rename("/log.csv", "/temp.csv"); 
        return true; 
    }
}

File StorageManager::openTempFile(){ 
    //Se abre en modo lecture "read"
    File file = LittleFS.open("/temp.csv", "r"); 

    return file; 
}

String StorageManager::getNextLine(File& file){ 
    if(!file.available()){
        Serial.println("El archivo no esta disponible"); 
        return String((char*)NULL);  
    } else { 
        String res = file.readStringUntil('\n'); 
        return res; 
    }
}

void StorageManager::finishDataUpload(){ 
    LittleFS.remove("/temp.csv"); 
}

//=================================================================================================================================================
//TODO: Implementar cuando tenga idea de que se necesita para MQTT
//=================================================================================================================================================
void StorageManager::createDefaultConfig(){ 
}