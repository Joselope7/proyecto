#include "include/ConexionDB.h"

int main() {
    ConexionDB db;

    if (db.conectar()) {
        auto res = db.consultar("SELECT * FROM estudiantes");
        for (auto fila : res) {
            std::cout << fila[0] << " | " << fila[1] << " | " << fila[2] << std::endl;
        }
    }

    return 0;
}