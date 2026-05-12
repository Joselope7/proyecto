#include "curso.h"

Curso::Curso(string codigo, string nombre, int creditos, int cupoMaximo)
    : codigo(codigo), nombre(nombre), creditos(creditos),
    cupoMaximo(cupoMaximo), cupoDisponible(cupoMaximo) {}

string Curso::getCodigo()         const { return codigo; }
string Curso::getNombre()         const { return nombre; }
int    Curso::getCreditos()       const { return creditos; }
int    Curso::getCupoMaximo()     const { return cupoMaximo; }
int    Curso::getCupoDisponible() const { return cupoDisponible; }

void Curso::agregarCarrera(Carrera* c) { carreras.push_back(c); }

bool Curso::reducirCupo() {
    if (cupoDisponible <= 0) return false;
    cupoDisponible--;
    return true;
}

void Curso::liberarCupo() {
    if (cupoDisponible < cupoMaximo) cupoDisponible++;
}

string Curso::getInfo() const {
    return "Codigo: " + codigo + " | " + nombre +
           " | Creditos: " + to_string(creditos) +
           " | Cupo: " + to_string(cupoDisponible) + "/" + to_string(cupoMaximo);
}