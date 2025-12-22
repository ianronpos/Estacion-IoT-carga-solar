#include "config.h"
#include "wifiManager.h"
#include "displayManager.h"
#include "inaDriver.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Adafruit_SSD1306.h>


/*
//Probar a mover al displayManager
//Configuracion del Display 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1   // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // If not work please scan the bus
#define OLED_SDA 14         // D5
#define OLED_SCL 12         // D6
*/
WifiManager wifi; 
DisplayManager displayManager; 

/*
void displaySetUp(){ //Mover al displayManager 
  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(0,0);
}
*/

void setup() { 
  displayManager.setup(); 
  wifi.setup(); 

  displayManager.testDisplay(); 
}


void loop() { 

}