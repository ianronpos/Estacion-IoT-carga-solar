#include "NetworkManager.h"
#include "displayManager.h"
#include "StorageManager.h"
#include "MqttManager.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Adafruit_SSD1306.h>
#include <Ticker.h> 


Ticker ticker; 
DisplayManager displayManager; 
NetworkManager wifi;
StorageManager storage; 
MqttManager mqtt; 

float voltageRandom; 
float powerRandom; 
volatile bool flag; 


IRAM_ATTR void getRanNum(){ 
  voltageRandom = random(0,16); 
  powerRandom = random(0,16); 
  flag = true; 
} 

//============================================================================================================
//TODO: Modificar orden de las llamadas a los metodos de mqtt y NetworkManager 
//============================================================================================================
void setup() { 
  Serial.begin(115200); 
  flag = false; 
  
  storage.setITransmisor(&mqtt); 
  storage.setup(); 
  
  mqtt.checkConfig(); 

  displayManager.setup(); 
  
  wifi.setListner(&displayManager); //Tiene que llamarse antes que el wifi.setup() ya que este crea eventos con el listener 
  wifi.setup(storage.getApInfo()); 
  wifi.tryConnect();
  
  mqtt.setup();  


  ticker.attach_ms(2000, getRanNum); 
}


void loop() { 
  wifi.loop(); 
  mqtt.loop(); 
  if(flag){ 
    storage.saveMeasure(voltageRandom, powerRandom); 
    flag = false; 
    char buff [64]; 
    snprintf(buff,sizeof(buff), "Voltage: %.2f \nPotencia: %.2f", voltageRandom, powerRandom);
    Serial.println(buff); 
  }  
  storage.loop(); 
  
  delay(50);
}
