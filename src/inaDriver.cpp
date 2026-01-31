#include "inaDriver.h"
#include <Adafruit_INA219.h>
#include <Wire.h>

bool InaDriver::setup(){ 
    InaWatcher = false; 
    ticker.attach(1, interruption, this); 

    if(!ina219.begin()){
        Serial.println("Failed to find INA219");
        return false; 
    } else return true; 
}

void InaDriver::takeMeasure(){
     shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    power_mW = ina219.getPower_mW();
    loadvoltage = busvoltage + (shuntvoltage / 1000);
    saveMeasure = true; 
    Serial.print("tension: "); 
    Serial.print(loadvoltage);
    Serial.print("  Intensidad: "); 
    Serial.println(current_mA);
}

float InaDriver::getPower(){ 
    return power_mW; 
}

float InaDriver::getVoltage(){ 
    return loadvoltage; 
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