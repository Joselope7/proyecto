#ifndef ESTUDIANTE_H
#define ESTUDIANTE_H

#include "persona.h"
#include "carrera.h"
#include "registroacademico.h"
#include <stack>
using namespace std;

class Estudiante
{
private:
    string carnet;
    Carrera* carrera;
    string fechaIngreso;
    stack<RegistroAcademico> historial;
public:
    Estudiante(string carnet, string nombre, int edad, Carrera* carrera, string fechaIngreso);

    string getNombre() const;
    int getEdad() const;
    Carrera* getCarrera() const;
    string getFechaIngreso() const;

    void agregarRegistro(const RegistroAcademico& r);
    float getPromedio() const;
    void mostrarHistorial() const;

    string getDatos() const override;
};

#endif // ESTUDIANTE_H