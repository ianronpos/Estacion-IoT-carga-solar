#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <stdint.h>
#include <Adafruit_SSD1306.h>
#include "WifiEvents.h"
#include <Ticker.h>

#define OLED_RESET     -1   // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C 
#define OLED_SDA 14         // D5
#define OLED_SCL 12         // D6

class DisplayManager : public WifiEvents{ 

    private: 
        static constexpr uint8_t SCREEN_WIDTH = 128; 
        static constexpr uint8_t SCREEN_HEIGHT = 64; 

        Adafruit_SSD1306 display;


    public:
        /**
         * @brief Construct a new Display Manager object
         * 
         */
        DisplayManager(); 
        ~DisplayManager() override = default; 
        
        /**
         * @brief Inicia configura la pantalla para su uso
         * 
         */
        void setup();  

        /**
         * @brief conexion correcta, muestra la ssid y la ip 
         * 
         * @param info estructura con informacion sobre la ssid y la ip
         */
        void onWifiConnect(const WifiInfo& info) override; 

        /**
         * @brief 
         * 
         * @param info Structura con informacion del error 
         */
        void onWifiDisconnect(const WifiInfo& info) override;   
        
        /**
         * @brief Muestra por que no se ha podido conectar a la red wifi guardada e indica su ssid
         * 
         * @param ssid nombre de la red que crea  
         * @param error codigo del estado del dispositivo
         */
        void wifiAp(String ssid, uint8_t error) override; 

}; 


#endif