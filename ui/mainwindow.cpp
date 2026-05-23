#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QTableWidget>
#include <QHeaderView>
#include <QLineEdit>
#include <QFormLayout>
#include <QComboBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    btnNavActivo(nullptr)
{
    ui->setupUi(this);
    qApp->setStyleSheet(R"(
        QTableWidget {
            color: black;
            background-color: white;
            gridline-color: #e0e0e0;
        }
        QTableWidget::item {
            color: black;
            padding: 4px;
        }
        QHeaderView::section {
            color: black;
            background-color: #f5f5f5;
            font-weight: 500;
            padding: 6px;
            border: none;
            border-bottom: 1px solid #ddd;
        }
        QLabel { color: black; }
        QPushButton {
            border-style: outset;
            border-width: 1px;
            border-radius: 6px;
            border-color: black;
            background: transparent;
            color: black;
        }
        QPushButton:checked {
            background: #EBF3FF;
            color: #1a6bbf;
            font-weight: 500;
        }
        QPushButton:hover:!checked {
            background: #f5f5f5;
        }
        QLineEdit {
            border: 2px solid #8f8f91;
            padding: 2px 8px;
            background-color: #ffffff;
            color: #333333;
        }
        QComboBox {
            border: 2px solid #8f8f91;
            border-radius: 3px;
            padding: 2px 8px;
            color: black;
        }
        QComboBox QAbstractItemView {
            border: 2px solid darkgray;
            selection-background-color: lightgray;
            color: black;
        }
        QMessageBox {
            background: white;
            color: black;
        }
    )");
    setWindowTitle("Sistema de Gestión Académica Universitaria");
    setMinimumSize(900, 600);

    // Conectar e inicializar sistema
    sistema = sistemaAcademico::getInstance();
    ConexionDB::getInstance()->conectar();
    sistema->cargarTodo();

    setupSidebar();
    setupTopbar();
    mostrarDashboard();
}

MainWindow::~MainWindow() {
    ConexionDB::getInstance()->desconectar();
    delete ui;
}

// -- Sidebar -----------------------------------------------

void MainWindow::setupSidebar() {
    QWidget*     sidebar = ui->sidebar;
    QVBoxLayout* layout  = new QVBoxLayout(sidebar);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(2);

    // Logo
    QLabel* logo = new QLabel("Universidad");
    logo->setStyleSheet("font-size:15px; font-weight:500; padding:8px 6px 4px;");
    QLabel* sub  = new QLabel("Sistema académico");
    sub->setStyleSheet("font-size:11px; color:gray; padding:0 6px 10px;");

    layout->addWidget(logo);
    layout->addWidget(sub);

    QFrame* sep = new QFrame();
    sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet("color: #ddd;");
    layout->addWidget(sep);
    layout->addSpacing(6);

    // Botones de navegación
    auto crearBtn = [&](const QString& texto, const QString& icono) {
        QPushButton* btn = new QPushButton(icono + "  " + texto);
        btn->setCheckable(true);
        btn->setStyleSheet(R"(
            QPushButton {
                text-align: left;
                padding: 9px 12px;
                border: none;
                border-radius: 6px;
                font-size: 13px;
                background: transparent;
                color: black;
            }
            QPushButton:checked {
                background: #EBF3FF;
                color: #1a6bbf;
                font-weight: 500;
            }
            QPushButton:hover:!checked {
                background: #f5f5f5;
            }
        )");
        return btn;
    };

    btnDashboard   = crearBtn("Dashboard",   "⊞");
    btnEstudiantes = crearBtn("Estudiantes", "👥");
    btnCursos      = crearBtn("Cursos",      "📚");
    btnMatricula   = crearBtn("Matrícula",   "📋");
    btnHistorial   = crearBtn("Historial",   "🕐");
    btnReportes    = crearBtn("Reportes",    "📊");
    btnCarreras    = crearBtn("Carreras",    "🎓");

    layout->addWidget(btnDashboard);
    layout->addWidget(btnEstudiantes);
    layout->addWidget(btnCarreras);
    layout->addWidget(btnCursos);
    layout->addWidget(btnMatricula);
    layout->addWidget(btnHistorial);
    layout->addWidget(btnReportes);
    layout->addStretch();

    QLabel* user = new QLabel("Administrador");
    user->setStyleSheet("font-size:11px; color:gray; padding:6px;");
    layout->addWidget(user);

    connect(btnDashboard,   &QPushButton::clicked, this, &MainWindow::mostrarDashboard);
    connect(btnEstudiantes, &QPushButton::clicked, this, &MainWindow::mostrarEstudiantes);
    connect(btnCarreras,    &QPushButton::clicked, this, &MainWindow::mostrarCarreras);
    connect(btnCursos,      &QPushButton::clicked, this, &MainWindow::mostrarCursos);
    connect(btnMatricula,   &QPushButton::clicked, this, &MainWindow::mostrarMatricula);
    connect(btnHistorial,   &QPushButton::clicked, this, &MainWindow::mostrarHistorial);
    connect(btnReportes,    &QPushButton::clicked, this, &MainWindow::mostrarReportes);
}

void MainWindow::setNavActivo(QPushButton* boton) {
    if (btnNavActivo) btnNavActivo->setChecked(false);
    boton->setChecked(true);
    btnNavActivo = boton;
}

void MainWindow::setupTopbar() {}

// -- Dashboard ---------------------------------------------

void MainWindow::mostrarDashboard() {
    setNavActivo(btnDashboard);

    QWidget*     page   = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(16);

    QLabel* titulo = new QLabel("Dashboard");
    titulo->setStyleSheet("font-size:18px; font-weight:500;");
    layout->addWidget(titulo);

    // Contar desde BD
    int totalEst = 0, totalCursos = 0, totalMatriculas = 0, totalEspera = 0;
    sistema->listarEstudiantes([&](Estudiante*){ totalEst++; });
    sistema->listarCursos([&](Curso*){ totalCursos++; });

    QSqlQuery qMat = ConexionDB::getInstance()->ejecutarQuery(
        "SELECT COUNT(*) FROM matriculas WHERE estado = 'activa'");
    if (qMat.next()) totalMatriculas = qMat.value(0).toInt();

    QSqlQuery qEsp = ConexionDB::getInstance()->ejecutarQuery(
        "SELECT COUNT(*) FROM matriculas WHERE estado = 'en_espera'");
    if (qEsp.next()) totalEspera = qEsp.value(0).toInt();

    QHBoxLayout* cards = new QHBoxLayout();
    cards->setSpacing(12);

    auto crearCard = [&](const QString& label, const QString& valor, const QString& color) {
        QFrame* card = new QFrame();
        card->setStyleSheet("background:#f7f7f7; border-radius:8px; padding:4px;");
        QVBoxLayout* cl = new QVBoxLayout(card);
        QLabel* lbl = new QLabel(label);
        lbl->setStyleSheet("font-size:12px; color:gray;");
        QLabel* val = new QLabel(valor);
        val->setStyleSheet("font-size:24px; font-weight:500; color:" + color + ";");
        cl->addWidget(lbl);
        cl->addWidget(val);
        return card;
    };

    cards->addWidget(crearCard("Estudiantes",     QString::number(totalEst),       "black"));
    cards->addWidget(crearCard("Cursos",          QString::number(totalCursos),    "black"));
    cards->addWidget(crearCard("Matrículas",      QString::number(totalMatriculas),"#1a6bbf"));
    cards->addWidget(crearCard("Lista de espera", QString::number(totalEspera),    "#c0392b"));
    layout->addLayout(cards);

    // Acciones rápidas
    QLabel* lblAcciones = new QLabel("Acciones rápidas");
    lblAcciones->setStyleSheet("font-size:13px; font-weight:500;");
    layout->addWidget(lblAcciones);

    QHBoxLayout* acciones = new QHBoxLayout();
    auto crearAccion = [&](const QString& texto) {
        QPushButton* btn = new QPushButton(texto);
        btn->setStyleSheet("padding:7px 14px; font-size:13px; border-radius:6px;");
        return btn;
    };

    QPushButton* aNuevoEst  = crearAccion("+ Nuevo estudiante");
    QPushButton* aNuevoCur  = crearAccion("+ Nuevo curso");
    QPushButton* aMatricula = crearAccion("+ Procesar matrícula");

    connect(aNuevoEst,  &QPushButton::clicked, this, &MainWindow::mostrarEstudiantes);
    connect(aNuevoCur,  &QPushButton::clicked, this, &MainWindow::mostrarCursos);
    connect(aMatricula, &QPushButton::clicked, this, &MainWindow::mostrarMatricula);

    acciones->addWidget(aNuevoEst);
    acciones->addWidget(aNuevoCur);
    acciones->addWidget(aMatricula);
    acciones->addStretch();
    layout->addLayout(acciones);
    layout->addStretch();

    ui->stackedWidget->addWidget(page);
    ui->stackedWidget->setCurrentWidget(page);
}

// -- Estudiantes -------------------------------------------

void MainWindow::mostrarEstudiantes() {
    setNavActivo(btnEstudiantes);

    QWidget*     page   = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(12);

    // Header
    QHBoxLayout* header = new QHBoxLayout();
    QLabel* titulo = new QLabel("Estudiantes");
    titulo->setStyleSheet("font-size:18px; font-weight:500;");
    QPushButton* btnNuevo = new QPushButton("+ Nuevo estudiante");
    btnNuevo->setStyleSheet("padding:7px 14px; font-size:13px; border-radius:6px;");
    header->addWidget(titulo);
    header->addStretch();
    header->addWidget(btnNuevo);
    layout->addLayout(header);

    // Barra de búsqueda
    QHBoxLayout* busqLayout = new QHBoxLayout();
    QLineEdit* inBuscar = new QLineEdit();
    inBuscar->setPlaceholderText("Buscar por carnet...");
    inBuscar->setMaximumWidth(240);
    QPushButton* btnBuscar  = new QPushButton("Buscar");
    QPushButton* btnMostrar = new QPushButton("Mostrar todos");
    btnBuscar->setStyleSheet("padding:6px 14px; font-size:13px;");
    btnMostrar->setStyleSheet("padding:6px 14px; font-size:13px;");
    busqLayout->addWidget(inBuscar);
    busqLayout->addWidget(btnBuscar);
    busqLayout->addWidget(btnMostrar);
    busqLayout->addStretch();
    layout->addLayout(busqLayout);

    // Tabla
    QTableWidget* tabla = new QTableWidget();
    tabla->setColumnCount(5);
    tabla->setHorizontalHeaderLabels({"Carnet","Nombre","Carrera","Ingreso","Promedio"});
    tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabla->setAlternatingRowColors(true);
    tabla->setStyleSheet("font-size:13px; color:black; background-color:white;");

    auto cargarTabla = [=](Estudiante* filtro = nullptr) {
        tabla->setRowCount(0);
        int fila = 0;

        auto llenarFila = [&](Estudiante* e) {
            // Calcular promedio directo desde BD
            QSqlQuery q(ConexionDB::getInstance()->getDB());
            q.prepare("SELECT AVG(nota) FROM historial_academico "
                      "WHERE carnet_estudiante = ?");
            q.addBindValue(QString::fromStdString(e->getCarnet()));
            q.exec();
            QString promedio = "—";
            if (q.next() && !q.value(0).isNull())
                promedio = QString::number(q.value(0).toDouble(), 'f', 2);

            tabla->insertRow(fila);
            tabla->setItem(fila, 0, new QTableWidgetItem(
                                        QString::fromStdString(e->getCarnet())));
            tabla->setItem(fila, 1, new QTableWidgetItem(
                                        QString::fromStdString(e->getNombre())));
            tabla->setItem(fila, 2, new QTableWidgetItem(
                                        QString::fromStdString(e->getCarrera()->getNombre())));
            tabla->setItem(fila, 3, new QTableWidgetItem(
                                        QString::fromStdString(e->getFechaIngreso())));
            tabla->setItem(fila, 4, new QTableWidgetItem(promedio));
            fila++;
        };

        if (filtro) {
            llenarFila(filtro);
        } else {
            sistema->listarEstudiantes([&](Estudiante* e){ llenarFila(e); });
        }
    };

    cargarTabla();
    layout->addWidget(tabla);

    // Búsqueda por carnet usando TablaHash
    connect(btnBuscar, &QPushButton::clicked, [=](){
        string carnet = inBuscar->text().toStdString();
        if (carnet.empty()) return;
        Estudiante* e = sistema->buscarEstudiante(carnet);
        if (e) {
            cargarTabla(e);
        } else {
            QMessageBox::warning(this, "No encontrado",
                                 "No se encontró ningún estudiante con ese carnet.");
        }
    });

    connect(btnMostrar, &QPushButton::clicked, [=](){ cargarTabla(); });

    // Formulario nuevo estudiante
    QFrame* formFrame = new QFrame();
    formFrame->setStyleSheet("background:#f7f7f7; border-radius:8px;");
    formFrame->setVisible(false);
    QFormLayout* form = new QFormLayout(formFrame);
    form->setContentsMargins(16, 12, 16, 12);
    form->setSpacing(8);

    QLineEdit* inCarnet = new QLineEdit(); inCarnet->setPlaceholderText("ej. 2026001");
    QLineEdit* inNombre = new QLineEdit(); inNombre->setPlaceholderText("Nombre completo");
    QLineEdit* inEdad   = new QLineEdit(); inEdad->setPlaceholderText("ej. 20");
    QLineEdit* inFecha  = new QLineEdit(); inFecha->setPlaceholderText("YYYY-MM-DD");
    QComboBox* cbCarrera = new QComboBox();

    // Cargar carreras desde BD
    QSqlQuery qCar(ConexionDB::getInstance()->getDB());
    qCar.exec("SELECT id, nombre FROM carreras ORDER BY nombre");
    while (qCar.next())
        cbCarrera->addItem(qCar.value(1).toString(), qCar.value(0).toInt());

    form->addRow("Carnet:",  inCarnet);
    form->addRow("Nombre:",  inNombre);
    form->addRow("Edad:",    inEdad);
    form->addRow("Carrera:", cbCarrera);
    form->addRow("Ingreso:", inFecha);

    QHBoxLayout* formBtns = new QHBoxLayout();
    QPushButton* btnGuardar  = new QPushButton("Guardar");
    QPushButton* btnCancelar = new QPushButton("Cancelar");
    btnGuardar->setStyleSheet("padding:6px 16px; font-size:13px;");
    btnCancelar->setStyleSheet("padding:6px 16px; font-size:13px;");
    formBtns->addStretch();
    formBtns->addWidget(btnCancelar);
    formBtns->addWidget(btnGuardar);

    QWidget* btnRow = new QWidget();
    btnRow->setLayout(formBtns);
    form->addRow(btnRow);
    layout->addWidget(formFrame);

    connect(btnNuevo, &QPushButton::clicked, [=](){
        formFrame->setVisible(true);
        btnNuevo->setEnabled(false);
    });
    connect(btnCancelar, &QPushButton::clicked, [=](){
        formFrame->setVisible(false);
        btnNuevo->setEnabled(true);
    });
    connect(btnGuardar, &QPushButton::clicked, [=](){
        bool ok = sistema->registrarEstudiante(
            inCarnet->text().toStdString(),
            inNombre->text().toStdString(),
            inEdad->text().toInt(),
            cbCarrera->currentData().toInt(),
            inFecha->text().toStdString()
            );
        if (ok) {
            QMessageBox::information(this, "Éxito", "Estudiante registrado.");
            mostrarEstudiantes();
        } else {
            QMessageBox::warning(this, "Error",
                                 "No se pudo registrar. Verifica que el carnet no exista.");
        }
    });

    ui->stackedWidget->addWidget(page);
    ui->stackedWidget->setCurrentWidget(page);
}
// -- Carreras ----------------------------------------------

void MainWindow::mostrarCarreras() {
    setNavActivo(btnCarreras);

    QWidget*     page   = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(12);

    QHBoxLayout* header = new QHBoxLayout();
    QLabel* titulo = new QLabel("Carreras");
    titulo->setStyleSheet("font-size:18px; font-weight:500;");
    QPushButton* btnNuevo = new QPushButton("+ Nueva carrera");
    btnNuevo->setStyleSheet("padding:7px 14px; font-size:13px; border-radius:6px;");
    header->addWidget(titulo);
    header->addStretch();
    header->addWidget(btnNuevo);
    layout->addLayout(header);

    QTableWidget* tabla = new QTableWidget();
    tabla->setColumnCount(2);
    tabla->setHorizontalHeaderLabels({"ID", "Nombre"});
    tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabla->setAlternatingRowColors(true);
    tabla->setStyleSheet("font-size:13px; color:black; background-color:white;");

    auto cargarTabla = [=]() {
        tabla->setRowCount(0);
        QSqlQuery q = ConexionDB::getInstance()->ejecutarQuery(
            "SELECT id, nombre FROM carreras ORDER BY nombre");
        int fila = 0;
        while (q.next()) {
            tabla->insertRow(fila);
            tabla->setItem(fila, 0, new QTableWidgetItem(q.value(0).toString()));
            tabla->setItem(fila, 1, new QTableWidgetItem(q.value(1).toString()));
            fila++;
        }
    };
    cargarTabla();
    layout->addWidget(tabla);

    // Formulario
    QFrame* formFrame = new QFrame();
    formFrame->setStyleSheet("background:#f7f7f7; border-radius:8px;");
    formFrame->setVisible(false);
    QFormLayout* form = new QFormLayout(formFrame);
    form->setContentsMargins(16, 12, 16, 12);

    QLineEdit* inNombre = new QLineEdit();
    inNombre->setPlaceholderText("ej. Ingeniería Industrial");
    form->addRow("Nombre:", inNombre);

    QHBoxLayout* formBtns = new QHBoxLayout();
    QPushButton* btnGuardar  = new QPushButton("Guardar");
    QPushButton* btnCancelar = new QPushButton("Cancelar");
    btnGuardar->setStyleSheet("padding:6px 16px; font-size:13px;");
    btnCancelar->setStyleSheet("padding:6px 16px; font-size:13px;");
    formBtns->addStretch();
    formBtns->addWidget(btnCancelar);
    formBtns->addWidget(btnGuardar);
    QWidget* btnRow = new QWidget();
    btnRow->setLayout(formBtns);
    form->addRow(btnRow);
    layout->addWidget(formFrame);

    connect(btnNuevo, &QPushButton::clicked, [=](){
        formFrame->setVisible(true);
        btnNuevo->setEnabled(false);
    });
    connect(btnCancelar, &QPushButton::clicked, [=](){
        formFrame->setVisible(false);
        btnNuevo->setEnabled(true);
    });
    connect(btnGuardar, &QPushButton::clicked, [=](){
        QString nombre = inNombre->text().trimmed();
        if (nombre.isEmpty()) {
            QMessageBox::warning(this, "Error", "Ingresa el nombre de la carrera.");
            return;
        }

        QSqlQuery q(ConexionDB::getInstance()->getDB());
        q.prepare("INSERT INTO carreras (nombre) VALUES (?)");
        q.addBindValue(nombre);
        if (q.exec()) {
            int nuevoId = q.lastInsertId().toInt();
            // Agregar a la memoria del sistema
            sistema->agregarCarrera(nuevoId, nombre.toStdString());
            QMessageBox::information(this, "Éxito", "Carrera registrada.");
            inNombre->clear();
            formFrame->setVisible(false);
            btnNuevo->setEnabled(true);
            cargarTabla();
        } else {
            QMessageBox::warning(this, "Error", "No se pudo registrar la carrera.");
        }
    });

    ui->stackedWidget->addWidget(page);
    ui->stackedWidget->setCurrentWidget(page);
}

// -- Cursos ------------------------------------------------

void MainWindow::mostrarCursos() {
    setNavActivo(btnCursos);

    QWidget*     page   = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(12);

    QHBoxLayout* header = new QHBoxLayout();
    QLabel* titulo = new QLabel("Cursos");
    titulo->setStyleSheet("font-size:18px; font-weight:500;");
    QPushButton* btnNuevo = new QPushButton("+ Nuevo curso");
    btnNuevo->setStyleSheet("padding:7px 14px; font-size:13px; border-radius:6px;");
    header->addWidget(titulo);
    header->addStretch();
    header->addWidget(btnNuevo);
    layout->addLayout(header);

    QTableWidget* tabla = new QTableWidget();
    tabla->setColumnCount(4);
    tabla->setHorizontalHeaderLabels({"Código","Nombre","Créditos","Cupo disponible"});
    tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabla->setAlternatingRowColors(true);
    tabla->setStyleSheet("font-size:13px; color:black; background-color:white;");

    auto cargarTabla = [=]() {
        tabla->setRowCount(0);
        int fila = 0;
        sistema->listarCursos([&](Curso* c) {
            tabla->insertRow(fila);
            tabla->setItem(fila, 0, new QTableWidgetItem(
                                        QString::fromStdString(c->getCodigo())));
            tabla->setItem(fila, 1, new QTableWidgetItem(
                                        QString::fromStdString(c->getNombre())));
            tabla->setItem(fila, 2, new QTableWidgetItem(
                                        QString::number(c->getCreditos())));
            tabla->setItem(fila, 3, new QTableWidgetItem(
                                        QString::number(c->getCupoDisponible()) + "/" +
                                        QString::number(c->getCupoMaximo())));
            const_cast<int&>(fila)++;
        });
    };
    cargarTabla();
    layout->addWidget(tabla);

    // Formulario nuevo curso
    QFrame* formFrame = new QFrame();
    formFrame->setStyleSheet("background:#f7f7f7; border-radius:8px;");
    formFrame->setVisible(false);
    QFormLayout* form = new QFormLayout(formFrame);
    form->setContentsMargins(16, 12, 16, 12);
    form->setSpacing(8);

    QLineEdit* inCodigo  = new QLineEdit(); inCodigo->setPlaceholderText("ej. PRG103");
    QLineEdit* inNombre  = new QLineEdit(); inNombre->setPlaceholderText("Nombre del curso");
    QLineEdit* inCreditos = new QLineEdit(); inCreditos->setPlaceholderText("ej. 4");
    QLineEdit* inCupo    = new QLineEdit(); inCupo->setPlaceholderText("ej. 25");
    QLineEdit* inPrereq = new QLineEdit(); inPrereq->setPlaceholderText("ej. PRG101");

    form->addRow("Código:",   inCodigo);
    form->addRow("Nombre:",   inNombre);
    form->addRow("Créditos:", inCreditos);
    form->addRow("Cupo:",     inCupo);
    form->addRow("Prerrequisitos:", inPrereq);

    QHBoxLayout* formBtns = new QHBoxLayout();
    QPushButton* btnGuardar  = new QPushButton("Guardar");
    QPushButton* btnCancelar = new QPushButton("Cancelar");
    btnGuardar->setStyleSheet("padding:6px 16px; font-size:13px;");
    btnCancelar->setStyleSheet("padding:6px 16px; font-size:13px;");
    formBtns->addStretch();
    formBtns->addWidget(btnCancelar);
    formBtns->addWidget(btnGuardar);
    QWidget* btnRow = new QWidget();
    btnRow->setLayout(formBtns);
    form->addRow(btnRow);
    layout->addWidget(formFrame);

    connect(btnNuevo, &QPushButton::clicked, [=](){
        formFrame->setVisible(true);
        btnNuevo->setEnabled(false);
    });
    connect(btnCancelar, &QPushButton::clicked, [=](){
        formFrame->setVisible(false);
        btnNuevo->setEnabled(true);
    });
    connect(btnGuardar, &QPushButton::clicked, [=](){
        string codigo   = inCodigo->text().toStdString();
        string nombre   = inNombre->text().toStdString();
        int    creditos = inCreditos->text().toInt();
        int    cupo     = inCupo->text().toInt();

        if (codigo.empty() || nombre.empty() || creditos == 0 || cupo == 0) {
            QMessageBox::warning(this, "Error", "Completa todos los campos obligatorios.");
            return;
        }

        bool ok = sistema->registrarCurso(codigo, nombre, creditos, cupo);
        if (!ok) {
            QMessageBox::warning(this, "Error",
                                 "No se pudo registrar. Verifica que el código no exista.");
            return;
        }

        // Registrar prerrequisitos si se ingresaron
        QString prereqs = inPrereq->text().trimmed();
        if (!prereqs.isEmpty()) {
            QStringList lista = prereqs.split(",", Qt::SkipEmptyParts);
            for (const QString& p : lista) {
                string prereq = p.trimmed().toStdString();
                if (prereq.empty()) continue;

                // Verificar que el prerrequisito existe
                if (!sistema->buscarCurso(prereq)) {
                    QMessageBox::warning(this, "Advertencia",
                                         "El prerrequisito '" + QString::fromStdString(prereq) +
                                             "' no existe y fue omitido.");
                    continue;
                }

                // Guardar en BD
                QSqlQuery q(ConexionDB::getInstance()->getDB());
                q.prepare("INSERT INTO prerrequisitos VALUES (?, ?)");
                q.addBindValue(QString::fromStdString(codigo));
                q.addBindValue(QString::fromStdString(prereq));
                q.exec();

                // Actualizar grafo en memoria
                sistema->agregarPrerrequisito(codigo, prereq);
            }
        }

        QMessageBox::information(this, "Éxito", "Curso registrado correctamente.");
        mostrarCursos();
    });

    ui->stackedWidget->addWidget(page);
    ui->stackedWidget->setCurrentWidget(page);
}

// -- Matrícula ---------------------------------------------

void MainWindow::mostrarMatricula() {
    setNavActivo(btnMatricula);

    QWidget*     page   = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(16);

    QLabel* titulo = new QLabel("Procesar matrícula");
    titulo->setStyleSheet("font-size:18px; font-weight:500;");
    layout->addWidget(titulo);

    QFrame* formFrame = new QFrame();
    formFrame->setStyleSheet("background:#f7f7f7; border-radius:8px;");
    formFrame->setMaximumWidth(480);
    QFormLayout* form = new QFormLayout(formFrame);
    form->setContentsMargins(20, 16, 20, 16);
    form->setSpacing(10);

    QLineEdit* inCarnet = new QLineEdit(); inCarnet->setPlaceholderText("ej. 2026001");
    QLineEdit* inCurso  = new QLineEdit(); inCurso->setPlaceholderText("ej. PRG101");
    QLineEdit* inCiclo  = new QLineEdit(); inCiclo->setPlaceholderText("ej. 2026-1");

    form->addRow("Carnet estudiante:", inCarnet);
    form->addRow("Código curso:",      inCurso);
    form->addRow("Ciclo:",             inCiclo);

    QPushButton* btnProcesar = new QPushButton("Procesar matrícula");
    btnProcesar->setStyleSheet("padding:8px 20px; font-size:13px; margin-top:8px;");
    form->addRow(btnProcesar);
    layout->addWidget(formFrame);

    QLabel* lblResultado = new QLabel("");
    lblResultado->setStyleSheet("font-size:13px; padding:8px;");
    layout->addWidget(lblResultado);
    layout->addStretch();

    connect(btnProcesar, &QPushButton::clicked, [=](){
        string carnet = inCarnet->text().toStdString();
        string curso  = inCurso->text().toStdString();
        string ciclo  = inCiclo->text().toStdString();

        if (carnet.empty() || curso.empty() || ciclo.empty()) {
            lblResultado->setStyleSheet("color:red; font-size:13px;");
            lblResultado->setText("⚠ Completa todos los campos.");
            return;
        }

        bool ok = sistema->procesarMatricula(carnet, curso, ciclo);
        if (ok) {
            lblResultado->setStyleSheet("color:green; font-size:13px;");
            lblResultado->setText("✓ Matrícula procesada correctamente.");
            inCarnet->clear(); inCurso->clear(); inCiclo->clear();
        } else {
            lblResultado->setStyleSheet("color:red; font-size:13px;");
            lblResultado->setText("✗ No se pudo procesar. Verifica prerrequisitos o datos.");
        }
    });

    ui->stackedWidget->addWidget(page);
    ui->stackedWidget->setCurrentWidget(page);
}

// -- Historial ---------------------------------------------

void MainWindow::mostrarHistorial() {
    mostrarNotas();
}

// -- Reportes ----------------------------------------------

void MainWindow::mostrarReportes() {
    setNavActivo(btnReportes);

    QWidget*     page   = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(16);

    QLabel* titulo = new QLabel("Reportes");
    titulo->setStyleSheet("font-size:18px; font-weight:500;");
    layout->addWidget(titulo);

    auto crearCard = [&](const QString& nombre, const QString& desc,
                         std::function<void()> exportar) {
        QFrame* card = new QFrame();
        card->setStyleSheet("background:#f7f7f7; border-radius:8px;");
        QVBoxLayout* cl = new QVBoxLayout(card);
        cl->setContentsMargins(16,14,16,14);
        QLabel* lNombre = new QLabel(nombre);
        lNombre->setStyleSheet("font-size:14px; font-weight:500;");
        QLabel* lDesc = new QLabel(desc);
        lDesc->setStyleSheet("font-size:12px; color:gray;");
        QPushButton* btn = new QPushButton("Exportar CSV");
        btn->setStyleSheet("padding:6px 14px; font-size:12px; margin-top:6px;");
        btn->setMaximumWidth(140);
        connect(btn, &QPushButton::clicked, exportar);
        cl->addWidget(lNombre);
        cl->addWidget(lDesc);
        cl->addWidget(btn);
        return card;
    };

    // Reporte 1: estudiantes por curso
    layout->addWidget(crearCard(
        "Estudiantes por curso",
        "Lista de estudiantes inscritos en cada curso",
        [=](){
            QString path = QFileDialog::getSaveFileName(
                this, "Guardar reporte", "estudiantes_por_curso.csv", "CSV (*.csv)");
            if (path.isEmpty()) return;
            QFile file(path);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
            QTextStream out(&file);
            out.setEncoding(QStringConverter::System);
            out << "Curso,Código,Carnet,Nombre\n";
            sistema->listarCursos([&](Curso* c){
                sistema->estudiantesPorCurso(c->getCodigo(), [&](Estudiante* e){
                    out << QString::fromStdString(c->getNombre()) << ","
                        << QString::fromStdString(c->getCodigo()) << ","
                        << QString::fromStdString(e->getCarnet()) << ","
                        << QString::fromStdString(e->getNombre()) << "\n";
                });
            });
            file.close();
            QMessageBox::information(this, "Éxito", "Reporte exportado correctamente.");
        }
        ));

    // Reporte 2: cursos por demanda
    layout->addWidget(crearCard(
        "Cursos por demanda",
        "Ranking de cursos con más inscripciones",
        [=](){
            QString path = QFileDialog::getSaveFileName(
                this, "Guardar reporte", "cursos_por_demanda.csv", "CSV (*.csv)");
            if (path.isEmpty()) return;
            QFile file(path);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
            QTextStream out(&file);
            out.setEncoding(QStringConverter::System);
            out << "Código,Nombre,Créditos,Total inscritos\n";
            sistema->cursosPorDemanda([&](Curso* c, int total){
                out << QString::fromStdString(c->getCodigo()) << ","
                    << QString::fromStdString(c->getNombre()) << ","
                    << c->getCreditos() << ","
                    << total << "\n";
            });
            file.close();
            QMessageBox::information(this, "Éxito", "Reporte exportado correctamente.");
        }
        ));

    layout->addStretch();

    ui->stackedWidget->addWidget(page);
    ui->stackedWidget->setCurrentWidget(page);
}

//-- Notas ----------------------------------------------

void MainWindow::mostrarNotas() {
    setNavActivo(btnHistorial);

    QWidget*     page   = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(16);

    QLabel* titulo = new QLabel("Historial académico y notas");
    titulo->setStyleSheet("font-size:18px; font-weight:500;");
    layout->addWidget(titulo);

    // Búsqueda
    QHBoxLayout* busqLayout = new QHBoxLayout();
    QLineEdit* inCarnet = new QLineEdit();
    inCarnet->setPlaceholderText("Carnet del estudiante");
    inCarnet->setMaximumWidth(240);
    QPushButton* btnBuscar = new QPushButton("Consultar");
    btnBuscar->setStyleSheet("padding:7px 16px; font-size:13px;");
    busqLayout->addWidget(inCarnet);
    busqLayout->addWidget(btnBuscar);
    busqLayout->addStretch();
    layout->addLayout(busqLayout);

    // Info estudiante
    QLabel* lblInfo = new QLabel("");
    lblInfo->setStyleSheet("font-size:13px; color:gray;");
    layout->addWidget(lblInfo);

    // Tabla historial
    QTableWidget* tabla = new QTableWidget();
    tabla->setColumnCount(5);
    tabla->setHorizontalHeaderLabels({"Código","Curso","Nota","Estado","Ciclo"});
    tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabla->setAlternatingRowColors(true);
    tabla->setStyleSheet("font-size:13px; color:black; background-color:white;");
    layout->addWidget(tabla);

    // Sección cursos matriculados
    QLabel* lblMatriculas = new QLabel("Cursos matriculados");
    lblMatriculas->setStyleSheet("font-size:14px; font-weight:500; margin-top:8px;");
    layout->addWidget(lblMatriculas);

    QTableWidget* tablaMatriculas = new QTableWidget();
    tablaMatriculas->setColumnCount(4);
    tablaMatriculas->setHorizontalHeaderLabels({"Código","Curso","Ciclo","Estado"});
    tablaMatriculas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tablaMatriculas->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablaMatriculas->setAlternatingRowColors(true);
    tablaMatriculas->setStyleSheet("font-size:13px; color:black; background-color:white;");
    tablaMatriculas->setMaximumHeight(180);
    layout->addWidget(tablaMatriculas);

    // Formulario agregar nota
    QFrame* formFrame = new QFrame();
    formFrame->setStyleSheet("background:#f7f7f7; border-radius:8px;");
    formFrame->setVisible(false);
    QFormLayout* form = new QFormLayout(formFrame);
    form->setContentsMargins(16, 12, 16, 12);
    form->setSpacing(8);

    QLineEdit* inCurso = new QLineEdit(); inCurso->setPlaceholderText("ej. PRG101");
    QLineEdit* inNota  = new QLineEdit(); inNota->setPlaceholderText("0 - 100");
    QLineEdit* inCiclo = new QLineEdit(); inCiclo->setPlaceholderText("ej. 2026-1");
    QLineEdit* inFecha = new QLineEdit(); inFecha->setPlaceholderText("YYYY-MM-DD");

    form->addRow("Código curso:", inCurso);
    form->addRow("Nota:",         inNota);
    form->addRow("Ciclo:",        inCiclo);
    form->addRow("Fecha:",        inFecha);

    QHBoxLayout* formBtns = new QHBoxLayout();
    QPushButton* btnGuardar  = new QPushButton("Guardar nota");
    QPushButton* btnCancelar = new QPushButton("Cancelar");
    btnGuardar->setStyleSheet("padding:6px 16px; font-size:13px;");
    btnCancelar->setStyleSheet("padding:6px 16px; font-size:13px;");
    formBtns->addStretch();
    formBtns->addWidget(btnCancelar);
    formBtns->addWidget(btnGuardar);
    QWidget* btnRow = new QWidget();
    btnRow->setLayout(formBtns);
    form->addRow(btnRow);
    layout->addWidget(formFrame);

    QPushButton* btnAgregarNota = new QPushButton("+ Agregar nota");
    btnAgregarNota->setStyleSheet("padding:7px 14px; font-size:13px;");
    btnAgregarNota->setVisible(false);
    layout->addWidget(btnAgregarNota);

    // Función para recargar tabla
    auto cargarHistorial = [=]() {
        tabla->setRowCount(0);
        string carnet = inCarnet->text().toStdString();

        QSqlQuery q(ConexionDB::getInstance()->getDB());
        q.prepare("SELECT h.codigo_curso, c.nombre, h.nota, h.estado, h.ciclo "
                  "FROM historial_academico h "
                  "LEFT JOIN cursos c ON h.codigo_curso = c.codigo "
                  "WHERE h.carnet_estudiante = ? ORDER BY h.fecha DESC");
        q.addBindValue(QString::fromStdString(carnet));
        q.exec();

        int fila = 0;
        while (q.next()) {
            tabla->insertRow(fila);
            tabla->setItem(fila, 0, new QTableWidgetItem(q.value(0).toString()));
            tabla->setItem(fila, 1, new QTableWidgetItem(q.value(1).toString()));
            tabla->setItem(fila, 2, new QTableWidgetItem(
                                        QString::number(q.value(2).toDouble(), 'f', 2)));
            QTableWidgetItem* estado = new QTableWidgetItem(q.value(3).toString());
            estado->setForeground(q.value(3).toString() == "aprobado"
                                      ? QColor("#1a7a3a") : QColor("#c0392b"));
            tabla->setItem(fila, 3, estado);
            tabla->setItem(fila, 4, new QTableWidgetItem(q.value(4).toString()));
            fila++;
        }

        // Actualizar promedio en lblInfo
        Estudiante* e = sistema->buscarEstudiante(carnet);
        if (e) {
            float prom = e->getPromedio();
            lblInfo->setText("Estudiante: " +
                             QString::fromStdString(e->getNombre()) +
                             "   |   Promedio: " +
                             (prom > 0 ? QString::number(prom, 'f', 2) : "Sin notas"));
        }

        // Cargar matrículas activas
        tablaMatriculas->setRowCount(0);
        QSqlQuery qMat(ConexionDB::getInstance()->getDB());
        qMat.prepare("SELECT m.codigo_curso, c.nombre, m.ciclo, m.estado "
                     "FROM matriculas m "
                     "LEFT JOIN cursos c ON m.codigo_curso = c.codigo "
                     "WHERE m.carnet_estudiante = ? "
                     "ORDER BY m.ciclo DESC");
        qMat.addBindValue(QString::fromStdString(carnet));
        qMat.exec();

        int filaMat = 0;
        while (qMat.next()) {
            tablaMatriculas->insertRow(filaMat);
            tablaMatriculas->setItem(filaMat, 0, new QTableWidgetItem(qMat.value(0).toString()));
            tablaMatriculas->setItem(filaMat, 1, new QTableWidgetItem(qMat.value(1).toString()));
            tablaMatriculas->setItem(filaMat, 2, new QTableWidgetItem(qMat.value(2).toString()));
            QTableWidgetItem* est = new QTableWidgetItem(qMat.value(3).toString());
            est->setForeground(qMat.value(3).toString() == "activa"
                                   ? QColor("#1a7a3a") : QColor("#c0392b"));
            tablaMatriculas->setItem(filaMat, 3, est);
            filaMat++;
        }
    };

    connect(btnBuscar, &QPushButton::clicked, [=](){
        string carnet = inCarnet->text().toStdString();
        if (carnet.empty()) return;
        Estudiante* e = sistema->buscarEstudiante(carnet);
        if (!e) {
            QMessageBox::warning(this, "No encontrado", "Estudiante no encontrado.");
            return;
        }
        cargarHistorial();
        btnAgregarNota->setVisible(true);
    });

    connect(btnAgregarNota, &QPushButton::clicked, [=](){
        formFrame->setVisible(true);
        btnAgregarNota->setEnabled(false);
    });
    connect(btnCancelar, &QPushButton::clicked, [=](){
        formFrame->setVisible(false);
        btnAgregarNota->setEnabled(true);
    });
    connect(btnGuardar, &QPushButton::clicked, [=](){
        string carnet = inCarnet->text().toStdString();
        bool ok = sistema->registrarNota(
            carnet,
            inCurso->text().toStdString(),
            inNota->text().toFloat(),
            inCiclo->text().toStdString(),
            inFecha->text().toStdString()
            );
        if (ok) {
            QMessageBox::information(this, "Éxito", "Nota registrada.");
            inCurso->clear(); inNota->clear();
            inCiclo->clear(); inFecha->clear();
            formFrame->setVisible(false);
            btnAgregarNota->setEnabled(true);
            cargarHistorial();
        } else {
            QMessageBox::warning(this, "Error",
                                 "No se pudo registrar. Verifica el carnet y código de curso.");
        }
    });

    ui->stackedWidget->addWidget(page);
    ui->stackedWidget->setCurrentWidget(page);
}