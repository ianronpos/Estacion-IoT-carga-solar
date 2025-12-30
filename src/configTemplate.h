#ifndef CONFIGTEMPLATE_H
#define CONFIGTEMPLATE_H

#include <Arduino.h>

//Tiempo que trascurre desde que se crea el AP desde que falla la conexion hasta que se reinicia
static constexpr int APTimeout = 180; 

constexpr const char* APPassword = "tu contraseña, minimo 8 caracter"; 
constexpr const char* APName = "El nombre del AP que se creara cuando se inicie por primera vez";

#endif