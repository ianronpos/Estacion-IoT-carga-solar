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
  display.clearDisplay(); 
  display.println("Inciando Conexion"); 
  display.display(); 
}

void DisplayManager::testDisplay(){ 
  display.clearDisplay(); 
  display.setCursor(0,0);
  display.println("Test"); 
  display.display(); 
}

void DisplayManager::onWifiConnect(const WifiInfo& info){ 
  display.clearDisplay();
  display.setCursor(0,0); 
  display.println("Conexion exitosa"); 
  char bufferIp [32];
  snprintf(bufferIp, sizeof(bufferIp), "Direccion Ip: %s", info.ip.toString().c_str());
  display.println(bufferIp);  
  char bufferSSID [32]; 
  snprintf(bufferSSID, sizeof(bufferSSID), "SSID: %s", info.ssid); 
  display.println(bufferSSID); 
  char bufferRSSI [32]; 
  snprintf(bufferRSSI, sizeof(bufferRSSI), "RSSI: %d", info.rssi); 
  display.display(); 
}

void DisplayManager::onWifiDisconnect(const WifiInfo& info){ 
  display.clearDisplay();
  display.setCursor(0,0); 
  display.println(info.disconnectReason); 
  display.display(); 
}