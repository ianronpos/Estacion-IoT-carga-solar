#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <stdint.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET     -1   // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C 
#define OLED_SDA 14         // D5
#define OLED_SCL 12         // D6


class DisplayManager{ 

    private: 
        static constexpr uint8_t SCREEN_WIDTH = 128; 
        static constexpr uint8_t SCREEN_HEIGHT = 64; 

        Adafruit_SSD1306 display; 

    public: 
        DisplayManager(); 
        void setup(); 
        void testDisplay(); 

}; 


#endif