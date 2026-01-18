#ifndef CONFIGTEMPLATE_H
#define CONFIGTEMPLATE_H

#include <Arduino.h>


//Nombre y contraseña para  el AP cuando no se puede conectar al wifi
constexpr const char* APPassword = "tu contraseña, minimo 8 caracter"; 
constexpr const char* APName = "El nombre del AP que se creara cuando se inicie por primera vez";

//Datos para la conexion MQTT 
const String MQTT_SERVER = ""; 
const int MQTT_PORT = 1883; 
const String MQTT_USER = ""; 
const String MQTT_PASSWORD = ""; 
const String MQTT_TOPIC = ""; 

#endif