#include "carrera.h"

Carrera::Carrera(int id, string nombre)
    : id(id), nombre(nombre) {}

int Carrera::getId() const { return id; }
string Carrera::getNombre() const {return nombre; }