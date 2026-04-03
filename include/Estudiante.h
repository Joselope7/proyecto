#ifndef ESTUDIANTE_H
#define ESTUDIANTE_H

#include <string>
using namespace std;

class Estudiante {
private:
    string carnet;
    string nombre;
    string carrera;
    int anioIngreso;
    float promedio;

public:
    // Constructor por defecto
    Estudiante();

    // Constructor con parametros
    Estudiante(string carnet, string nombre, string carrera, int anioIngreso);

    // Getters
    string getCarnet() const;
    string getNombre() const;
    string getCarrera() const;
    int getAnioIngreso() const;
    float getPromedio() const;

    // Setters
    void setNombre(string nombre);
    void setCarrera(string carrera);
    void setPromedio(float promedio);

    // Metodos
    void mostrar() const;
};

#endif