#ifndef CURSO_H
#define CURSO_H

#include <string>
#include <vector>
#include "carrera.h"
using namespace std;

class Curso
{
private:
    string          codigo;
    string          nombre;
    int             creditos;
    int             cupoMaximo;
    int             cupoDisponible;
    vector<Carrera*> carreras;

public:
    Curso(string codigo, string nombre, int creditos, int cupoMaximo);

    string getCodigo()          const;
    string getNombre()          const;
    int    getCreditos()        const;
    int    getCupoMaximo()      const;
    int    getCupoDisponible()  const;

    void agregarCarrera(Carrera* c);
    bool reducirCupo();    // retorna false si no hay cupo
    void liberarCupo();

    string getInfo() const;
};

#endif // CURSO_H
