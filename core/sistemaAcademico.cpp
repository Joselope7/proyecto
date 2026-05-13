#include "sistemaAcademico.h"
#include <QDebug>

sistemaAcademico* sistemaAcademico::instancia = nullptr;

sistemaAcademico::sistemaAcademico()
    : db(ConexionDB::getInstance()), contadorMatriculas(0) {}

sistemaAcademico* sistemaAcademico::getInstance() {
    if (!instancia) instancia = new sistemaAcademico();
    return instancia;
}

sistemaAcademico::~sistemaAcademico() {
    for (auto c : carreras)   delete c;
    for (auto m : matriculas) delete m;
}

// ── Carreras ──────────────────────────────────────────────

void sistemaAcademico::cargarCarrerasDB() {
    QSqlQuery q = db->ejecutarQuery("SELECT id, nombre FROM carreras");
    while (q.next()) {
        carreras.push_back(new Carrera(
            q.value(0).toInt(),
            q.value(1).toString().toStdString()
            ));
    }
}

Carrera* sistemaAcademico::buscarCarrera(int id) {
    for (auto c : carreras)
        if (c->getId() == id) return c;
    return nullptr;
}

// ── Estudiantes ───────────────────────────────────────────

bool sistemaAcademico::registrarEstudiante(const string& carnet,
                                           const string& nombre,
                                           int edad, int idCarrera,
                                           const string& fechaIngreso) {
    Carrera* carrera = buscarCarrera(idCarrera);
    if (!carrera) return false;

    // Verificar duplicado
    if (buscarEstudiante(carnet)) return false;

    // Guardar en BD
    QSqlQuery q(db->getDB());
    q.prepare("INSERT INTO estudiantes VALUES (?, ?, ?, ?, ?)");
    q.addBindValue(QString::fromStdString(carnet));
    q.addBindValue(QString::fromStdString(nombre));
    q.addBindValue(edad);
    q.addBindValue(idCarrera);
    q.addBindValue(QString::fromStdString(fechaIngreso));
    if (!q.exec()) return false;

    // Guardar en estructuras
    Estudiante* e = new Estudiante(carnet, nombre, edad, carrera, fechaIngreso);
    listaEstudiantes.insertar(e);
    bstEstudiantes.insertar(e, carnet);
    hashEstudiantes.insertar(carnet, e);

    return true;
}

bool sistemaAcademico::eliminarEstudiante(const string& carnet) {
    Estudiante* e = buscarEstudiante(carnet);
    if (!e) return false;

    QSqlQuery q(db->getDB());
    q.prepare("DELETE FROM estudiantes WHERE carnet = ?");
    q.addBindValue(QString::fromStdString(carnet));
    if (!q.exec()) return false;

    listaEstudiantes.eliminar([&](Estudiante* est){ return est->getCarnet() == carnet; });
    bstEstudiantes.eliminar(carnet);
    hashEstudiantes.eliminar(carnet);
    delete e;
    return true;
}

Estudiante* sistemaAcademico::buscarEstudiante(const string& carnet) {
    Estudiante** ptr = hashEstudiantes.buscar(carnet);
    return ptr ? *ptr : nullptr;
}

void sistemaAcademico::cargarEstudiantesDB() {
    QSqlQuery q = db->ejecutarQuery(
        "SELECT carnet, nombre, edad, id_carrera, fecha_ingreso FROM estudiantes");
    while (q.next()) {
        string  carnet  = q.value(0).toString().toStdString();
        string  nombre  = q.value(1).toString().toStdString();
        int     edad    = q.value(2).toInt();
        int     idCar   = q.value(3).toInt();
        string  fecha   = q.value(4).toString().toStdString();
        Carrera* carrera = buscarCarrera(idCar);
        if (!carrera) continue;

        Estudiante* e = new Estudiante(carnet, nombre, edad, carrera, fecha);
        listaEstudiantes.insertar(e);
        bstEstudiantes.insertar(e, carnet);
        hashEstudiantes.insertar(carnet, e);
    }
}

void sistemaAcademico::listarEstudiantes(function<void(Estudiante*)> accion) {
    listaEstudiantes.recorrer([&](Estudiante* e){ accion(e); });
}

// ── Cursos ────────────────────────────────────────────────

bool sistemaAcademico::registrarCurso(const string& codigo,
                                      const string& nombre,
                                      int creditos, int cupo) {
    if (buscarCurso(codigo)) return false;

    QSqlQuery q(db->getDB());
    q.prepare("INSERT INTO cursos VALUES (?, ?, ?, ?)");
    q.addBindValue(QString::fromStdString(codigo));
    q.addBindValue(QString::fromStdString(nombre));
    q.addBindValue(creditos);
    q.addBindValue(cupo);
    if (!q.exec()) return false;

    Curso* c = new Curso(codigo, nombre, creditos, cupo);
    listaCursos.insertarAlFinal(c);
    grafoPrerrequisitos.agregarNodo(codigo);
    return true;
}

bool sistemaAcademico::eliminarCurso(const string& codigo) {
    Curso* c = buscarCurso(codigo);
    if (!c) return false;

    QSqlQuery q(db->getDB());
    q.prepare("DELETE FROM cursos WHERE codigo = ?");
    q.addBindValue(QString::fromStdString(codigo));
    if (!q.exec()) return false;

    listaCursos.eliminar([&](Curso* cur){ return cur->getCodigo() == codigo; });
    delete c;
    return true;
}

Curso* sistemaAcademico::buscarCurso(const string& codigo) {
    Curso** ptr = nullptr;
    Curso*  resultado = nullptr;
    listaCursos.recorrerAdelante([&](Curso* c){
        if (c->getCodigo() == codigo) resultado = c;
    });
    return resultado;
}

void sistemaAcademico::cargarCursosDB() {
    QSqlQuery q = db->ejecutarQuery(
        "SELECT codigo, nombre, creditos, cupo_maximo FROM cursos");
    while (q.next()) {
        Curso* c = new Curso(
            q.value(0).toString().toStdString(),
            q.value(1).toString().toStdString(),
            q.value(2).toInt(),
            q.value(3).toInt()
            );
        listaCursos.insertarAlFinal(c);
        grafoPrerrequisitos.agregarNodo(c->getCodigo());
    }
}

void sistemaAcademico::listarCursos(function<void(Curso*)> accion) {
    listaCursos.recorrerAdelante([&](Curso* c){ accion(c); });
}

// ── Prerrequisitos ────────────────────────────────────────

void sistemaAcademico::cargarPrerrequisitosDB() {
    QSqlQuery q = db->ejecutarQuery(
        "SELECT codigo_curso, codigo_prereq FROM prerrequisitos");
    while (q.next()) {
        grafoPrerrequisitos.agregarArista(
            q.value(0).toString().toStdString(),
            q.value(1).toString().toStdString()
            );
    }
}

bool sistemaAcademico::cumplePrerrequisitos(const string& carnet,
                                            const string& codigoCurso) {
    Estudiante* e = buscarEstudiante(carnet);
    if (!e) return false;

    // Obtener cursos aprobados del historial
    unordered_set<string> aprobados;
    QSqlQuery q(db->getDB());
    q.prepare("SELECT codigo_curso FROM historial_academico "
              "WHERE carnet_estudiante = ? AND estado = 'aprobado'");
    q.addBindValue(QString::fromStdString(carnet));
    q.exec();
    while (q.next())
        aprobados.insert(q.value(0).toString().toStdString());

    return grafoPrerrequisitos.cumplePrerrequisitos(codigoCurso, aprobados);
}

// ── Matrícula ─────────────────────────────────────────────

bool sistemaAcademico::procesarMatricula(const string& carnet,
                                         const string& codigoCurso,
                                         const string& ciclo) {
    Estudiante* e = buscarEstudiante(carnet);
    Curso*      c = buscarCurso(codigoCurso);
    if (!e || !c) return false;

    // Verificar prerrequisitos
    if (!cumplePrerrequisitos(carnet, codigoCurso)) return false;

    // Encolar estudiante
    colaMatricula.encolar(e);
    Estudiante* atendido = colaMatricula.verFrente();
    colaMatricula.desencolar();

    // Verificar cupo
    string estado = c->reducirCupo() ? "activa" : "en_espera";

    // Guardar en BD
    QSqlQuery q(db->getDB());
    q.prepare("INSERT INTO matriculas (carnet_estudiante, codigo_curso, ciclo, estado) "
              "VALUES (?, ?, ?, ?)");
    q.addBindValue(QString::fromStdString(carnet));
    q.addBindValue(QString::fromStdString(codigoCurso));
    q.addBindValue(QString::fromStdString(ciclo));
    q.addBindValue(QString::fromStdString(estado));
    if (!q.exec()) return false;

    int id = q.lastInsertId().toInt();
    Matricula* m = new Matricula(id, e, c, ciclo);
    if (estado == "en_espera") m->cancelar(); // marca en espera
    matriculas.push_back(m);

    return true;
}

bool sistemaAcademico::cancelarMatricula(int idMatricula) {
    for (auto m : matriculas) {
        if (m->getId() == idMatricula) {
            m->cancelar();
            QSqlQuery q(db->getDB());
            q.prepare("UPDATE matriculas SET estado = 'cancelada' WHERE id = ?");
            q.addBindValue(idMatricula);
            return q.exec();
        }
    }
    return false;
}

void sistemaAcademico::cargarMatriculasDB() {
    QSqlQuery q = db->ejecutarQuery(
        "SELECT id, carnet_estudiante, codigo_curso, ciclo FROM matriculas");
    while (q.next()) {
        Estudiante* e = buscarEstudiante(q.value(1).toString().toStdString());
        Curso*      c = buscarCurso(q.value(2).toString().toStdString());
        if (!e || !c) continue;
        Matricula* m = new Matricula(q.value(0).toInt(), e, c,
                                     q.value(3).toString().toStdString());
        matriculas.push_back(m);
        contadorMatriculas++;
    }
}

// ── Historial ─────────────────────────────────────────────

bool sistemaAcademico::registrarNota(const string& carnet,
                                     const string& codigoCurso,
                                     float nota, const string& ciclo,
                                     const string& fecha) {
    Estudiante* e = buscarEstudiante(carnet);
    Curso*      c = buscarCurso(codigoCurso);
    if (!e || !c) return false;

    string estado = (nota >= 61.0f) ? "aprobado" : "reprobado";

    QSqlQuery q(db->getDB());
    q.prepare("INSERT INTO historial_academico "
              "(carnet_estudiante, codigo_curso, nota, estado, ciclo, fecha) "
              "VALUES (?, ?, ?, ?, ?, ?)");
    q.addBindValue(QString::fromStdString(carnet));
    q.addBindValue(QString::fromStdString(codigoCurso));
    q.addBindValue(nota);
    q.addBindValue(QString::fromStdString(estado));
    q.addBindValue(QString::fromStdString(ciclo));
    q.addBindValue(QString::fromStdString(fecha));
    if (!q.exec()) return false;

    RegistroAcademico r(codigoCurso, c->getNombre(), nota, ciclo, fecha);
    e->agregarRegistro(r);

    // Actualizar AVL de promedios
    avlPromedios.insertar(e, e->getPromedio());
    return true;
}

// ── Reportes ──────────────────────────────────────────────

void sistemaAcademico::estudiantesPorCurso(const string& codigoCurso,
                                           function<void(Estudiante*)> accion) {
    QSqlQuery q(db->getDB());
    q.prepare("SELECT carnet_estudiante FROM matriculas "
              "WHERE codigo_curso = ? AND estado = 'activa'");
    q.addBindValue(QString::fromStdString(codigoCurso));
    q.exec();
    while (q.next()) {
        Estudiante* e = buscarEstudiante(q.value(0).toString().toStdString());
        if (e) accion(e);
    }
}

void sistemaAcademico::cursosPorDemanda(function<void(Curso*, int)> accion) {
    listaCursos.recorrerAdelante([&](Curso* c) {
        QSqlQuery q(db->getDB());
        q.prepare("SELECT COUNT(*) FROM matriculas WHERE codigo_curso = ?");
        q.addBindValue(QString::fromStdString(c->getCodigo()));
        q.exec();
        if (q.next()) accion(c, q.value(0).toInt());
    });
}

// ── Inicialización ────────────────────────────────────────

void sistemaAcademico::cargarTodo() {
    cargarCarrerasDB();
    cargarCursosDB();
    cargarPrerrequisitosDB();
    cargarEstudiantesDB();
    cargarMatriculasDB();
}
