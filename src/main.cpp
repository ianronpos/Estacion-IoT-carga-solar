#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Adafruit_SSD1306.h>
#include "config.h"
#include "wifiManager.h"

//Configuracion del Display 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1   // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // If not work please scan the bus
#define OLED_SDA 14         // D5
#define OLED_SCL 12         // D6
Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire);

bool wifiConectado = false; 

void onWifiConnect(const WiFiEventStationModeGotIP& event){
  Serial.println("Conexion Exitosa"); 
  wifiConectado = true; 
}

void wifiSetUp(){ 
  /*
  1º Intentar conexion
    si fallo en conexion -> preguntar scan
  */

  /*WiFi.mode(WIFI_STA); 
  WiFi.scanNetworksAsync(); //Cuando termina ejecuta el metodo
  */
 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 
  WiFi.onStationModeGotIP(onWifiConnect);

}

void displaySetUp(){
  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(0,0);
}

void setup() { 
  displaySetUp(); 
  wifiSetUp(); 
}


void loop() {
  display.clearDisplay(); 
  if(wifiConectado){ 
    display.println("Conexion realizada"); 
    display.display();
  } else { 
    display.println("Intentando conectar");
    display.display(); 
  }
  delay(500); 


}