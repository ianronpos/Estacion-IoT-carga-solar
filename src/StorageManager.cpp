#include "StorageManager.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

void StorageManager::setITransmisor(ITransmisor* i){ 
    this->transmisor = i; 
}

bool StorageManager::setup(){
    ticker.attach(60, interruption, this); 
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

    //Guarda medicion
    file.printf("%.2f,%.2f\n", potency, voltage); 

    file.close(); 
}

bool StorageManager::hasPendingData(){ 
    file = LittleFS.open("/log.csv", "r"); 

    if(!file){ 
        Serial.println("El archivo no existe"); 
        file.close();
        return false; 
    } else if(file.size() == 0){ 
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
        LittleFS.rename("/getApInfolog.csv", "/temp.csv"); 
        lastPosition = 0L; 
        return true; 
    }
}

String StorageManager::getNextLine(){ 
    if(!file.available()){
        Serial.println("El archivo no esta disponible"); 
        finishDataUpload(); 
        return String((char*)NULL);  ; 
    } else { 
        String linea = file.readStringUntil('\n'); 
        linea.trim(); 
        return linea; 
    }
}

void StorageManager::finishDataUpload(){ 
    LittleFS.remove("/temp.csv"); 
}

bool StorageManager::dataToSend(){
    file = LittleFS.open("/temp.csv", "r"); 
    file.seek(lastPosition); // Abre desde la posicion incial
    JsonDocument doc; //Crea el documento

    JsonArray array = doc.to<JsonArray>(); //Se  define como array

    for(int i = 0; i < 5; i++){ 
        JsonObject measure = array.add<JsonObject>(); //crea un nuevo objeto dentro del array

        String linea = getNextLine(); //obtiene la siguiente linea
        if(linea.equals("")){ 
            finishDataUpload(); 
            break;
        }else{ 
            int comaIndex = linea.indexOf(','); //obtiene la coma que separa los valores del csv
            measure["v"] = linea.substring(0, comaIndex); //valor voltage
            measure["p"] = linea.substring(comaIndex + 1); //valor potencia 
        }
    }

    lastPosition  = file.position(); //obtiene el numero del ultimo byte leido
    file.close(); 
    return transmisor->publish(doc); //manda  las medidas por mqtt 
}

void StorageManager::loop(){ 
    if(sendWatcher){ 
        if(PrepareDataForUpload()){ 
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
    pThis->sendWatcher; 
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
    String user = doc["yanIoT"].as<String>(); 
    String password = doc["MQTT_PASSWORD"].as<String>(); 
    String topic = doc["Iot/test"].as<String>(); 
    apInfo.APPassword = doc["AP_PASSWORD"].as<String>(); 
    apInfo.APName = doc["ESPAP"].as<String>(); 

    if(transmisor != nullptr){ 
        transmisor->loadMqttConfig(server, port, user,  password, topic); 
    }

    return true; 
}