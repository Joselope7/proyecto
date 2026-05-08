#include "persona.h"

Persona::Persona(string nombre, int edad)
    : nombre(nombre), edad(edad) {}

string Persona::getNombre() const { return nombre; }
int    Persona::getEdad()   const { return edad; }