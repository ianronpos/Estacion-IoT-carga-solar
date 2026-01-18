#include "config.h"
#include "NetworkManager.h"
#include "displayManager.h"
#include "inaDriver.h"
#include "StorageManager.h"
#include "MqttManager.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Adafruit_SSD1306.h>
 
DisplayManager displayManager; 
NetworkManager wifi;
StorageManager storage; 
MqttManager mqtt; 
InaDriver ina; 

void setup() { 
  Serial.begin(115200); 

  if(!ina.setup()){ 
    displayManager.displayText("Error al encontrar el sensor INA219"); 
    while (true) {}
  }

  wifi.setListner(&displayManager); //Tiene que llamarse antes que el wifi.setup() ya que este crea eventos con el listener 
  displayManager.setup(); 
  wifi.setup(storage.getApInfo()); 
  storage.setup(); 
  wifi.tryConnect();
  mqtt.setup();  
}


void loop() { 
  wifi.loop(); 
  mqtt.loop(); 
  ina.loop(); 
  storage.saveMeasure(ina.getPower(), ina.getVoltage()); 
  delay(50);
}