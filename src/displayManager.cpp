#include "displayManager.h"
#include <Adafruit_SSD1306.h>
#include <Ticker.h>
#include "config.h"
  
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


void DisplayManager::onWifiConnect(const WifiInfo& info){ 
  display.clearDisplay();
  display.setCursor(0,0); 
  display.println("Conexion exitosa"); 
  char bufferIp [32];
  snprintf(bufferIp, sizeof(bufferIp), "Ip: %s", info.ip.toString().c_str());
  display.println(bufferIp);  
  char bufferSSID [32]; 
  snprintf(bufferSSID, sizeof(bufferSSID), "SSID: %s", info.ssid.c_str()); 
  display.println(bufferSSID); 

  display.display(); 
}

void DisplayManager::onWifiDisconnect(const WifiInfo& info){ 
  display.clearDisplay();
  display.setCursor(0,0); 
  display.println(info.status); 
  display.display(); 
}

void DisplayManager::wifiAp(String ssid, uint8_t error){ 
  display.clearDisplay(); 
  display.setCursor(0,0); 
  if(error == 1) {
    display.println("No se encontro ssid guardada"); 
  } else if( error == 6){
    display.println("La contraseña  guarda no es corrrecta");
  }
  char buff [32]; 
  snprintf(buff, sizeof(buff), "AP creado: %s", ssid.c_str());
  display.println(buff); 
  display.display(); 
}