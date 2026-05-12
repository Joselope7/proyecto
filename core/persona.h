#ifndef PERSONA_H
#define PERSONA_H

#include <string>
using namespace std;

class Persona {
protected:
    string nombre;
    int    edad;

public:
    Persona(string nombre, int edad);
    virtual ~Persona() = default;

    string getNombre() const;
    int    getEdad()   const;

    // Método virtual puro → clase abstracta
    virtual string getDatos() const = 0;
};

#endif // PERSONA_H