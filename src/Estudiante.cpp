#include "../include/Estudiante.h"
#include <iostream>
using namespace std;

// Constructor por defecto
Estudiante::Estudiante() {
    carnet = "";
    nombre = "";
    carrera = "";
    anioIngreso = 0;
    promedio = 0.0;
}

// Constructor con parametros
Estudiante::Estudiante(string carnet, string nombre, string carrera, int anioIngreso) {
    this->carnet = carnet;
    this->nombre = nombre;
    this->carrera = carrera;
    this->anioIngreso = anioIngreso;
    this->promedio = 0.0;
}

// Getters
string Estudiante::getCarnet() const { return carnet; }
string Estudiante::getNombre() const { return nombre; }
string Estudiante::getCarrera() const { return carrera; }
int Estudiante::getAnioIngreso() const { return anioIngreso; }
float Estudiante::getPromedio() const { return promedio; }

// Setters
void Estudiante::setNombre(string nombre) { this->nombre = nombre; }
void Estudiante::setCarrera(string carrera) { this->carrera = carrera; }
void Estudiante::setPromedio(float promedio) { this->promedio = promedio; }

// Mostrar datos del estudiante en consola
void Estudiante::mostrar() const {
    cout << "Carnet  : " << carnet << endl;
    cout << "Nombre  : " << nombre << endl;
    cout << "Carrera : " << carrera << endl;
    cout << "Ingreso : " << anioIngreso << endl;
    cout << "Promedio: " << promedio << endl;
}