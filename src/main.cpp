#include "NetworkManager.h"
#include "displayManager.h"
#include "inaDriver.h"
#include "StorageManager.h"
#include "MqttManager.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Adafruit_SSD1306.h>
#include <time.h>
 
//DisplayManager displayManager;
NetworkManager wifi;
StorageManager storage; 
MqttManager mqtt; 
InaDriver ina; 

//Cadena sacada de https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv Europe/Madrid 
const char* TZ_INFO = "CET-1CEST,M3.5.0,M10.5.0/3"; 

void setup() { 
  Serial.begin(115200); 

  if(!ina.setup()){ 
    //displayManager.displayText("Error al encontrar el sensor INA219"); 
    while (true) {}
  }
  Serial.println("ina conectado");

//  wifi.setListner(&displayManager); //Tiene que llamarse antes que el wifi.setup() ya que este crea eventos con el listener 
  storage.setITransmisor(&mqtt); 
  //displayManager.setup(); 
  wifi.setup(storage.getApInfo()); 
  storage.setup(); 
  if(wifi.tryConnect()) configTime(TZ_INFO, "pool.ntp.org", "time.google.com"); 
  Serial.print("Sincronizando la hora."); 
  while(time(nullptr) < 1000000){ 
    delay(500); 
    Serial.print("."); 
  }
  Serial.println("\nHora sincronizada");
  mqtt.setup();  
}

void loop() { 
  wifi.loop(); 
  mqtt.loop(); 
  ina.loop(); 
  if(ina.getSaveMeasure()){ 
    storage.saveMeasure(ina.getPower(), ina.getVoltage()); 
    ina.setMeasure(false); 
  }
  storage.loop();
  delay(50);
}