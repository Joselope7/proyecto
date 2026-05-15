#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include "../core/sistemaAcademico.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void mostrarDashboard();
    void mostrarEstudiantes();
    void mostrarCursos();
    void mostrarMatricula();
    void mostrarHistorial();
    void mostrarReportes();

private:
    Ui::MainWindow* ui;
    sistemaAcademico* sistema;

    void setupSidebar();
    void setupTopbar();
    void actualizarDashboard();
    void setNavActivo(QPushButton* boton);

    QPushButton* btnDashboard;
    QPushButton* btnEstudiantes;
    QPushButton* btnCursos;
    QPushButton* btnMatricula;
    QPushButton* btnHistorial;
    QPushButton* btnReportes;
    QPushButton* btnNavActivo;
};

#endif