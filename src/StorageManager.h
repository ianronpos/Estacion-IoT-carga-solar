#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include <LittleFS.h>
#include "config.h"


class StorageManager { 

    public: 
    /**
     * @brief Intenta cargar el sistema de archivos, carga la configuracion base y borra los ultimos datos si se han enviado
     * 
     * @return true si carga el sistema de archivos de forma correcta 
     * @return false si no se ha podido cargar el sistema de archivos
     */
    bool begin(); 

    /**
     * @brief Formatea la memoria y crea el archivo config.json con valores por defecto
     */
    bool format(); 

    void loadConfig(const Configuracion& configuracion); 
    
    /**
     * @brief Crea el archivo config.json con los valores por defecto
     */
    void createDefaultConfig();  

    void saveMeasure(unsigned long timestamp, int value); 
    
    bool hasPendingData(); 

    bool PrepareDataForUpload(); 
    
    File openTempFile(); 
    
    String getNextLine(File &file); 
    
    void finishDataUpload(); 

}; 

#endif