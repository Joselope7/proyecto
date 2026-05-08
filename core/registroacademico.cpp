#include "registroacademico.h"

RegistroAcademico::RegistroAcademico(string codigoCurso, string nombreCurso, float nota, string ciclo, string fecha)
    : codigoCurso(codigoCurso), nombreCurso(nombreCurso), nota(nota), ciclo(ciclo), fecha(fecha)
{
    estado = (nota >= 61.0f) ? "aprobado" : "reprobado";
}

string RegistroAcademico::getCodigoCurso() const { return codigoCurso; }
string RegistroAcademico::getNombreCurso() const { return nombreCurso; }
float  RegistroAcademico::getNota()        const { return nota; }
string RegistroAcademico::getEstado()      const { return estado; }
string RegistroAcademico::getCiclo()       const { return ciclo; }
string RegistroAcademico::getFecha()       const { return fecha; }