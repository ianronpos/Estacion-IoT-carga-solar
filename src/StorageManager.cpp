#include "StorageManager.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

void StorageManager::setITransmisor(ITransmisor* i){ 
    this->transmisor = i; 
}

bool StorageManager::setup(){
    ticker.attach(1, interruption, this); 
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
                loadDefaultConfig(); 
                res = true; 
            }
        }
    } else { 
        Serial.println("Sistema de archivos cargado correctamente"); 

        if(!LittleFS.exists("/config.json")){ //Si no existe el archivo de configuracion 
            Serial.println("No se encontro el archivo de configuracion"); 
            loadDefaultConfig(); 
        }

        loadDefaultConfig(); 
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
    loadDefaultConfig(); 
    return true; 
}

void StorageManager::saveMeasure(float potency, float voltage){
    //Abrimos el archivo en modo append, si no existe, se crea 
    File file = LittleFS.open("/log.csv", "a");

    if(!file) { //Si el archivo no se habre
        Serial.println("Error al abrir el archivo log.csv"); 
        return; 
    }

    //Obtencion del timeStamp
    time_t ts = time(nullptr); 

    //Guarda medicion
    file.printf("%.2f,%.2f,%ld\n", potency, voltage, (long)ts); 

    file.close(); 
}

bool StorageManager::hasPendingData(){ 
    if(!LittleFS.exists("/log.csv")){ //Comprueba existencia  
        Serial.println("El archivo no existe"); 
        return false; 
    } 
    
    File temp = LittleFS.open("/log.csv", "r"); 
    if(!temp){ //Comprueba si se abrio 
        Serial.println("El archivo no se pudo abrir"); 
        return false; 
    }

    size_t size = temp.size();  //Guarda el valor del tamaño
    temp.close();  //lo cierra

    if(size == 0){  //esta vacio
        Serial.println("El archivo esta vacio"); 
        return false; 
    } else { 
        Serial.println("El archivo tiene contenido"); 
        return true; 
    }

}

bool StorageManager::PrepareDataForUpload(){ 
    if(!hasPendingData()){ 
        return false; 
    } else { 
        Serial.println("Info preparada para enviar");
        LittleFS.rename("/log.csv", "/temp.csv"); 
        lastPosition = 0L; 
        return true; 
    }
}

void StorageManager::finishDataUpload(){ 
    LittleFS.remove("/temp.csv"); 
}

bool StorageManager::dataToSend(){ 
    file = LittleFS.open("/temp.csv", "r"); 
    if(!file) return false; //No se pudo abrir el archivo 

    file.seek(lastPosition); // Abre desde la posicion incial


    while(file.available()){
        JsonDocument doc; //Crea el documento
        JsonArray array = doc.to<JsonArray>(); //Se  define como array

        int i = 0; //indice para enviar datos cada 5 lecturas y no saturar la RAM 

        while(i < 10 && file.available()){ 
            String linea = file.readStringUntil('\n'); 
            linea.trim(); 

            if(linea.length() > 0){ 
                int comaIndex1 = linea.indexOf(','); 
                int comaIndex2 = linea.indexOf(',', comaIndex1 + 1); 
                JsonObject measure = array.add<JsonObject>(); //crea un nuevo objeto dentro del array
                
                measure["p"] = linea.substring(comaIndex1 + 1, comaIndex2); //valor potencia 
                measure["v"] = linea.substring(0, comaIndex1); //valor voltage
                measure["timeStamp"] = linea.substring(comaIndex2 + 1); 
                i++; 
            }
        }

        if(i > 0){ 
            Serial.println("Enviando bloque de datos"); 
            transmisor->publish(doc);
            yield(); 
        }
    }

    file.close(); 
    finishDataUpload(); 
    return  true; 
}

void StorageManager::loop(){ 
    if(sendWatcher){ 
        if(PrepareDataForUpload()){
            Serial.println("hay datos cargados");  
            dataToSend() ? Serial.println("Datos enviados de forma correcta") : 
                Serial.println("No se pudo enviar los datos"); 
        }
        sendWatcher = false; 
    }
}

ApInfo StorageManager::getApInfo(){ 
    return apInfo; 
}


void StorageManager::interruption(StorageManager* pThis){ 
    pThis->sendWatcher = true; 
}

bool StorageManager::loadDefaultConfig(){ 
    File configFile = LittleFS.open("config.json", "r");  
    
    if(!configFile){ 
        Serial.println("No se ha podido abrir el archivo"); 
        return false; 
    }

    //Asignacion temporal documento Json
    JsonDocument doc; 

    //Deserilizar archivo json
    DeserializationError error = deserializeJson(doc, configFile); 
    
    //Despues de leerlo se cierra, ahorramos recursos
    configFile.close(); 

    if(error){ 
        Serial.println("Error al leer el archivo"); 
        Serial.println(error.c_str()); 
        return false; 
    }

    String server = doc["MQTT_SERVER"].as<String>(); 
    int port = doc["MQTT_PORT"] | 1883; 
    String user = doc["MQTT_USER"].as<String>(); 
    String password = doc["MQTT_PASSWORD"].as<String>(); 
    String topic = doc["MQTT_TOPIC"].as<String>(); 
    apInfo.APPassword = doc["AP_PASSWORD"].as<String>(); 
    apInfo.APName = doc["ESPAP"].as<String>(); 

    if(transmisor != nullptr){ 
        transmisor->loadMqttConfig(server, port, user,  password, topic); 
        Serial.println("Se ha llamado a mqtt"); 
    }

    Serial.println("Se ha cargado la configuracion de forma correcta"); 
    return true; 
}
