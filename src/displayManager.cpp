#include "displayManager.h"
#include <Adafruit_SSD1306.h>


DisplayManager::DisplayManager()
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {
}

void DisplayManager::setup(){ 
  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(0,0);
}

void DisplayManager::testDisplay(){ 
    display.println("Test"); 
    display.display(); 
}