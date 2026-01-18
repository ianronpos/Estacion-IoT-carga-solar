#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include <LittleFS.h>
#include <Ticker.h>
#include "MqttManager.h"
#include "ITransmisor.h"

struct ApInfo{ 
    String APPassword; 
    String APName; 
}; 

class StorageManager { 

    private: 
        File file; 
        ITransmisor* transmisor; 
        unsigned  long lastPosition; 
        volatile bool sendWatcher; 
        Ticker ticker; 
        ApInfo apInfo; 

        static void IRAM_ATTR interruption(StorageManager* pThis); 

    public: 
        StorageManager() = default; 
        void setITransmisor(ITransmisor* i); 
        bool setup(); 
        bool format(); 
        void saveMeasure(float potency, float voltage); 
        bool hasPendingData(); 
        bool PrepareDataForUpload();
        String getNextLine(); 
        void finishDataUpload(); 
        bool dataToSend(); 
        void loop(); 
        ApInfo getApInfo(); 


        //=============================================================================================================================
        //TODO: Implementar
        //=============================================================================================================================
        bool loadDefaultConfig();  
}; 

#endif