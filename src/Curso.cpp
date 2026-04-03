#include "../include/Curso.h"
#include <iostream>
using namespace std;

Curso::Curso() {
    codigo = "";
    nombre = "";
    creditos = 0;
    cupoMaximo = 0;
    cupoActual = 0;
}

Curso::Curso(string codigo, string nombre, int creditos, int cupoMaximo) {
    this->codigo = codigo;
    this->nombre = nombre;
    this->creditos = creditos;
    this->cupoMaximo = cupoMaximo;
    this->cupoActual = 0;
}

// Getters
string Curso::getCodigo() const { return codigo; }
string Curso::getNombre() const { return nombre; }
int Curso::getCreditos() const { return creditos; }
int Curso::getCupoMaximo() const { return cupoMaximo; }
int Curso::getCupoActual() const { return cupoActual; }

// Setters
void Curso::setNombre(string nombre) { this->nombre = nombre; }
void Curso::setCreditos(int creditos) { this->creditos = creditos; }
void Curso::setCupoMaximo(int cupoMaximo) { this->cupoMaximo = cupoMaximo; }

// Verifica si hay espacio disponible en el curso
bool Curso::hayEspacio() const {
    return cupoActual < cupoMaximo;
}

// Inscribe un estudiante si hay espacio, retorna true si tuvo exito
bool Curso::inscribir() {
    if (hayEspacio()) {
        cupoActual++;
        return true;
    }
    return false;
}

void Curso::mostrar() const {
    cout << "Codigo : " << codigo << endl;
    cout << "Nombre : " << nombre << endl;
    cout << "Creditos: " << creditos << endl;
    cout << "Cupo   : " << cupoActual << "/" << cupoMaximo << endl;
}