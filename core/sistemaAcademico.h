#ifndef sistemaAcademico_H
#define sistemaAcademico_H

#include "estudiante.h"
#include "curso.h"
#include "carrera.h"
#include "matricula.h"
#include "registroacademico.h"
#include "../structures/listaSimple.h"
#include "../structures/listaDoble.h"
#include "../structures/bst.h"
#include "../structures/avl.h"
#include "../structures/tablaHash.h"
#include "../structures/cola.h"
#include "../structures/grafo.h"
#include "../structures/ArbolExpresiones.h"
#include "../structures/ArbolB.h"
#include "../structures/SimuladorMemoria.h"
#include "../database/conexionDB.h"
#include <unordered_set>
#include <vector>
#include <QString>
#include <QSqlQuery>
#include <QVariant>
using namespace std;

class sistemaAcademico
{
private:
    ListaSimple<Estudiante*>  listaEstudiantes;
    ListaDoble<Curso*>        listaCursos;
    BST<Estudiante*, string>  bstEstudiantes;
    AVL<Estudiante*, float>   avlPromedios;
    TablaHash<Estudiante*, string> hashEstudiantes;
    Cola<Estudiante*>         colaMatricula;
    Grafo                     grafoPrerrequisitos;
    ArbolExpresiones arbolPromedio;
    ArbolB           arbolIndice;
    SimuladorMemoria simuladorAulas;

    vector<Carrera*>   carreras;
    vector<Matricula*> matriculas;

    ConexionDB* db;
    int         contadorMatriculas;

    // Singleton
    static sistemaAcademico* instancia;
    sistemaAcademico();

public:
    static sistemaAcademico* getInstance();
    ~sistemaAcademico();

    // -- Carreras ------------------------------
    void        cargarCarrerasDB();
    Carrera*    buscarCarrera(int id);

    // -- Estudiantes ---------------------------
    bool        registrarEstudiante(const string& carnet, const string& nombre,
                             int edad, int idCarrera,
                             const string& fechaIngreso);
    bool        eliminarEstudiante(const string& carnet);
    Estudiante* buscarEstudiante(const string& carnet);
    void        cargarEstudiantesDB();
    void        listarEstudiantes(function<void(Estudiante*)> accion);

    // -- Carreras ------------------------------
    void agregarCarrera(int id, const string& nombre);

    // -- Cursos --------------------------------
    bool   registrarCurso(const string& codigo, const string& nombre,
                        int creditos, int cupo);
    bool   eliminarCurso(const string& codigo);
    Curso* buscarCurso(const string& codigo);
    void   cargarCursosDB();
    void   listarCursos(function<void(Curso*)> accion);

    // -- Prerrequisitos ------------------------
    void agregarPrerrequisito(const string& curso, const string& prereq);
    void cargarPrerrequisitosDB();
    bool cumplePrerrequisitos(const string& carnet, const string& codigoCurso);

    // -- Matrícula -----------------------------
    bool procesarMatricula(const string& carnet, const string& codigoCurso,
                           const string& ciclo);
    bool cancelarMatricula(int idMatricula);
    void cargarMatriculasDB();

    // -- Historial -----------------------------
    bool registrarNota(const string& carnet, const string& codigoCurso,
                       float nota, const string& ciclo, const string& fecha);
    void cargarHistorialDB(Estudiante* estudiante);

    // -- Reportes ------------------------------
    void estudiantesPorCurso(const string& codigoCurso,
                             function<void(Estudiante*)> accion);
    void cursosPorDemanda(function<void(Curso*, int)> accion);

    // -- Aulas -----------------------
    void inicializarAulas();
    SimuladorMemoria& getSimuladorAulas() { return simuladorAulas; }
    ArbolB&           getArbolIndice()    { return arbolIndice; }

    // -- Inicialización ------------------------
    void cargarTodo();

};

#endif // sistemaAcademico_H
