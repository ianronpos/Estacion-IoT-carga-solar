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

void setup() { 
  Serial.begin(115200); 


  wifi.setListner(&displayManager); //Tiene que llamarse antes que el wifi.setup() ya que este crea eventos con el listener 
  displayManager.setup(); 
  wifi.setup(); 
  wifi.tryConnect(); 
}


void loop() { 
  wifi.loop(); 
  
  
  delay(500);
}
