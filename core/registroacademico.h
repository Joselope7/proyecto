#ifndef REGISTROACADEMICO_H
#define REGISTROACADEMICO_H

class RegistroAcademico
{
private:
    string codigoCurso;
    string nombreCurso;
    float  nota;
    string estado;
    string ciclo;
    string fecha;
public:
    RegistroAcademico(string codigoCurso, string nombreCurso, float nota, string ciclo, string fecha);

    string getCodigoCurso() const;
    string getNombreCurso() const;
    float  getNota()        const;
    string getEstado()      const;
    string getCiclo()       const;
    string getFecha()       const;
};

#endif // REGISTROACADEMICO_H
