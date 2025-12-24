#include "config.h"
#include "wifiManager.h"
#include "displayManager.h"
#include "inaDriver.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Adafruit_SSD1306.h>
 
DisplayManager displayManager; 
WifiManager wifi;

void setup() { 
  Serial.begin(115200); 


  wifi.setListner(&displayManager); //Tiene que llamarse antes que el wifi.setup() ya que este crea eventos con el listener 
  displayManager.setup(); 
  wifi.setup(); 

}


void loop() { 
  Serial.println(wifi.getStatus()); 
  wifi.loop(); 
  delay(500);

}