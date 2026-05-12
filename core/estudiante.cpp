#include "estudiante.h"
#include "iostream"

Estudiante::Estudiante(string carnet, string nombre, int edad, Carrera* carrera, string fechaIngreso) : Persona(nombre, edad), carnet(carnet), carrera(carrera), fechaIngreso(fechaIngreso) {}

string   Estudiante::getCarnet()       const { return carnet; }
Carrera* Estudiante::getCarrera()      const { return carrera; }
string   Estudiante::getFechaIngreso() const { return fechaIngreso; }

void Estudiante::agregarRegistro(const RegistroAcademico& r) {
    historial.push_back(r);
}

float Estudiante::getPromedio() const {
    if (historial.empty()) return 0.0f;
    float suma = 0;
    for (const auto& r : historial) suma += r.getNota();
    return suma / historial.size();
}

void Estudiante::mostrarHistorial() const {
    for (int i = historial.size() - 1; i >= 0; i--) {
        cout << historial[i].getNombreCurso()
        << " | " << historial[i].getNota()
        << " | " << historial[i].getEstado() << "\n";
    }
}

string Estudiante::getDatos() const {
    return "Carnet: " + carnet + " | Nombre: " + nombre +
           " | Carrera: " + carrera->getNombre() +
           " | Promedio: " + to_string(getPromedio());
}