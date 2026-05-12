#include <QApplication>
#include "database/ConexionDB.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Probar conexión
    ConexionDB* db = ConexionDB::getInstance();

    if (db->conectar()) {
        qDebug() << "Sistema listo";

        // Query de prueba
        QSqlQuery q = db->ejecutarQuery("SELECT nombre FROM carreras");
        while (q.next()) {
            qDebug() << "Carrera:" << q.value(0).toString();
        }

        db->desconectar();
    }

    return 0;
}