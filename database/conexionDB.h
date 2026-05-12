#ifndef CONEXIONDB_H
#define CONEXIONDB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>

class ConexionDB {
private:
    QSqlDatabase db;
    static ConexionDB* instancia;  // Singleton

    ConexionDB();  // constructor privado

public:
    // Singleton: una sola conexión en todo el sistema
    static ConexionDB* getInstance();

    bool conectar();
    void desconectar();
    bool isConectado() const;

    QSqlQuery ejecutarQuery(const QString& sql);
    QSqlDatabase getDB() const;
};

#endif // CONEXIONDB_H
