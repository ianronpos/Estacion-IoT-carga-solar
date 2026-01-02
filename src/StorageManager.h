#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include <LittleFS.h>
#include "MqttManager.h"


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

    //=============================================================================================================================
    //Implementar
    //=============================================================================================================================
    void loadConfig(const MqttConfig& configuracion); 
    
    /**
     * @brief Crea el archivo config.json con los valores por defecto
     */
    void createDefaultConfig();  

    /**
     * @brief Guarda la medida en /log.csv
     * 
     * @param timestamp identifica cuando se tomo la medida 
     * @param potency valor de la potencia 
     * @param voltage valor del voltaje
     */
    void saveMeasure(unsigned long timestamp, double potency, double voltage); 
    
    /**
     * @brief Revisa si /log.csv contiene informacion, si contiene informacion es que esta pendiente
     * 
     * @return true tiene datos pendientes 
     * @return false no tiene datos pendientes
     */
    bool hasPendingData(); 

    /**
     * @brief Si hay infomracion pendiente en  "/log.csv" lo renombre a "/temp.csv"
     * 
     * @return true si hay informacion y se renombbre a /temp.csv
     * @return false si no hay informacion
     */
    bool PrepareDataForUpload(); 
    
    /**
     * @brief Abre el archivo /temp,csv
     * 
     * @return File 
     */
    File openTempFile(); 

    /**
     * @brief Get the Next Line 
     * 
     * @param file Archivo que se esta leyendo
     * @return String cadena que devuelve
     */
    String getNextLine(File &file); 
    
    /**
     * @brief Borra el archivo /temp.csv
     * 
     */
    void finishDataUpload(); 

}; 

#endif