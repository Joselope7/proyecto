#ifndef CONEXIONDB_H
#define CONEXIONDB_H

#include <mysqlx/xdevapi.h>
#include <string>
#include <iostream>

// NO se usan "using namespace" aqui para evitar conflicto entre
// std::string y mysqlx::string

class ConexionDB {
private:
    mysqlx::Session* sesion;
    mysqlx::Schema*  schema;
    std::string      host;
    std::string      usuario;
    std::string      password;
    std::string      baseDatos;
    int              puerto;

public:
    ConexionDB(std::string host     = "localhost",
               std::string usuario  = "root",
               std::string password = "gazorpazorp",
               std::string bd       = "universidad_db",
               int         puerto   = 33060);

    ~ConexionDB();

    bool conectar();
    void desconectar();
    bool isConectado() const;

    // Ejecuta INSERT, UPDATE o DELETE
    bool ejecutar(const std::string& query);

    // Ejecuta SELECT y retorna el resultado
    mysqlx::RowResult consultar(const std::string& query);

    // Retorna el schema activo
    mysqlx::Schema* getSchema();
};

#endif