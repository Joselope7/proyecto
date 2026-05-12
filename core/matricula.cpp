#include "matricula.h"

Matricula::Matricula(int id, Estudiante* estudiante,
                     Curso* curso, string ciclo)
    : id(id), estudiante(estudiante), curso(curso),
    ciclo(ciclo), estado(EstadoMatricula::ACTIVA) {}

int         Matricula::getId()         const { return id; }
Estudiante* Matricula::getEstudiante() const { return estudiante; }
Curso*      Matricula::getCurso()      const { return curso; }
string      Matricula::getCiclo()      const { return ciclo; }
EstadoMatricula Matricula::getEstado() const { return estado; }

void Matricula::cancelar() {
    estado = EstadoMatricula::CANCELADA;
    curso->liberarCupo();
}

string Matricula::getInfo() const {
    string est = (estado == EstadoMatricula::ACTIVA)    ? "Activa"    :
                     (estado == EstadoMatricula::CANCELADA) ? "Cancelada" : "En espera";
    return "ID: " + to_string(id) +
           " | " + estudiante->getCarnet() +
           " | " + curso->getNombre() +
           " | Ciclo: " + ciclo +
           " | Estado: " + est;
}