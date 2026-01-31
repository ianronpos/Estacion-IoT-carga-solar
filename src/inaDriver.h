//REVISAR, PISA AL LCD
//DEBE CONECTARSE EL SDA AL PIN 14, GPIO2 
//DEBE CONECTARSE EL SCL AL PIN 9, GPIO14 
#ifndef INADRIVER_H
#define INADRIVER_H

#include <Adafruit_INA219.h>
#include <Ticker.h>


//TODO: revisar si es necesaria al struct, mirar si comprar otro sensor, y hacer pruebas con la bateria en vez de con el panel
//Para comprobar el funcionamiento correcto del sensor y evitar el error 
//supuestamente al hacer "ina.takeMeasure()", sin encontrar el sensor ina
struct Measures{ 
    float current_mA; 
    float loadVoltage; 
    float potency_mW; 
}; 

class  InaDriver{ 
    private:
        float shuntvoltage;
        float busvoltage;
        float current_mA;
        float loadvoltage;
        float power_mW;
        Adafruit_INA219 ina219;
        Ticker ticker; 
        volatile bool InaWatcher; 
        bool saveMeasure;  

        static void IRAM_ATTR  interruption(InaDriver* pThis); 

    public: 
        InaDriver() = default;
        bool setup(); 
        void takeMeasure(); 
        float getPower();
        float getVoltage();  
        void loop(); 
        bool getSaveMeasure(); 
        void setMeasure(bool b);
}; 

#endif