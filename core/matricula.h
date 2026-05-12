#ifndef MATRICULA_H
#define MATRICULA_H

#include "estudiante.h"
#include "curso.h"
#include <string>
using namespace std;

enum class EstadoMatricula { ACTIVA, CANCELADA, EN_ESPERA };

class Matricula {
private:
    int              id;
    Estudiante*      estudiante;
    Curso*           curso;
    string           ciclo;
    EstadoMatricula  estado;

public:
    Matricula(int id, Estudiante* estudiante, Curso* curso, string ciclo);

    int         getId()          const;
    Estudiante* getEstudiante()  const;
    Curso*      getCurso()       const;
    string      getCiclo()       const;
    EstadoMatricula getEstado()  const;

    void cancelar();
    string getInfo() const;
};

#endif // MATRICULA_H
