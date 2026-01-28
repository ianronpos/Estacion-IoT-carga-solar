#include "inaDriver.h"
#include <Adafruit_INA219.h>
#include <Wire.h>

bool InaDriver::setup(){ 
    InaWatcher = false; 
    ticker.attach(60, interruption, this); 

    Wire.begin(D2, D1); 

    if(!ina219.begin()){
        Serial.println("Failed to find INA219");
        return false; 
    } else return true; 
}

void InaDriver::takeMeasure(){
    measures.current_mA = ina219.getCurrent_mA(); 
    measures.loadVoltage = ina219.getBusVoltage_V() + ina219.getShuntVoltage_mV() /1000; 
    measures.potency_mW = measures.loadVoltage * measures.current_mA; 
    saveMeasure = true; 
}

float InaDriver::getPower(){ 
    return measures.potency_mW; 
}

float InaDriver::getVoltage(){ 
    return measures.loadVoltage; 
}

void InaDriver::loop(){ 
    if(InaWatcher){ 
        InaWatcher = false; 
        takeMeasure();  
    }  
}

void InaDriver::interruption(InaDriver* pThis){ 
    pThis->InaWatcher = true; 
}

bool InaDriver::getSaveMeasure(){ 
    return saveMeasure; 
}

void InaDriver::setMeasure(bool b){ 
    saveMeasure = b; 
}