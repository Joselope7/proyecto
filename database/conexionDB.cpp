#include "conexionDB.h"

ConexionDB* ConexionDB::instancia = nullptr;

ConexionDB::ConexionDB() {
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={MySQL ODBC 9.7 Unicode Driver};"
                       "SERVER=localhost;"
                       "PORT=3306;"
                       "DATABASE=universidad_db;"
                       "USER=root;"
                       "PASSWORD=gazorpazorp;");
}

ConexionDB* ConexionDB::getInstance() {
    if (instancia == nullptr) {
        instancia = new ConexionDB();
    }
    return instancia;
}

bool ConexionDB::conectar() {
    if (db.open()) {
        qDebug() << "Conexión exitosa a universidad_db";
        return true;
    } else {
        qDebug() << "Error al conectar:" << db.lastError().text();
        return false;
    }
}

void ConexionDB::desconectar() {
    if (db.isOpen()) {
        db.close();
        qDebug() << "Conexión cerrada";
    }
}

bool ConexionDB::isConectado() const {
    return db.isOpen();
}

QSqlQuery ConexionDB::ejecutarQuery(const QString& sql) {
    QSqlQuery query(db);
    if (!query.exec(sql)) {
        qDebug() << "Error en query:" << query.lastError().text();
    }
    return query;
}

QSqlDatabase ConexionDB::getDB() const {
    return db;
}