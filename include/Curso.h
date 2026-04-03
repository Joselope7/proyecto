#ifndef CURSO_H
#define CURSO_H

#include <string>
using namespace std;

class Curso {
private:
    string codigo;
    string nombre;
    int creditos;
    int cupoMaximo;
    int cupoActual;

public:
    Curso();
    Curso(string codigo, string nombre, int creditos, int cupoMaximo);

    // Getters
    string getCodigo() const;
    string getNombre() const;
    int getCreditos() const;
    int getCupoMaximo() const;
    int getCupoActual() const;

    // Setters
    void setNombre(string nombre);
    void setCreditos(int creditos);
    void setCupoMaximo(int cupoMaximo);

    // Metodos
    bool hayEspacio() const;       // true si cupoActual < cupoMaximo
    bool inscribir();              // incrementa cupoActual si hay espacio
    void mostrar() const;
};

#endif