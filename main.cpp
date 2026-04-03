#include "include/Estudiante.h"
#include "include/Curso.h"
#include <iostream>
using namespace std;

int main() {
    Estudiante e("2024001", "Ana Garcia", "Ingenieria", 2024);
    e.mostrar();

    cout << "---" << endl;

    Curso c("CC101", "Programacion I", 4, 30);
    c.mostrar();

    return 0;
}