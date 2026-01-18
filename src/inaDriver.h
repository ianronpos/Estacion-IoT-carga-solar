//REVISAR, PISA AL LCD
//DEBE CONECTARSE EL SDA AL PIN 14, GPIO2 
//DEBE CONECTARSE EL SCL AL PIN 9, GPIO14 
#ifndef INADRIVER_H
#define INADRIVER_H

#include <Adafruit_INA219.h>
#include <Ticker.h>

struct Measures{ 
    float current_mA; 
    float loadVoltage; 
    float potency_mW; 
}; 

class  InaDriver{ 
    private:
        Measures measures; 
        Adafruit_INA219 ina219; 
        Ticker ticker; 
        volatile bool InaWatcher; 

        static void IRAM_ATTR  interruption(InaDriver* pThis); 

    public: 
        InaDriver() = default; 
        bool setup(); 
        void takeMeasure(); 
        float getPower();
        float getVoltage();  
        void loop(); 
}; 

#endif