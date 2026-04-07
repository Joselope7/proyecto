#include "../include/ConexionDB.h"

ConexionDB::ConexionDB(std::string host, std::string usuario, std::string password,
                       std::string bd, int puerto) {
    this->host      = host;
    this->usuario   = usuario;
    this->password  = password;
    this->baseDatos = bd;
    this->puerto    = puerto;
    this->sesion    = nullptr;
    this->schema    = nullptr;
}

ConexionDB::~ConexionDB() {
    desconectar();
}

bool ConexionDB::conectar() {
    try {
        sesion = new mysqlx::Session(host, puerto, usuario, password);
        sesion->sql("USE " + baseDatos).execute();   // <- agrega esta linea
        schema = new mysqlx::Schema(sesion->getSchema(baseDatos));
        std::cout << "Conexion exitosa a " << baseDatos << std::endl;
        return true;
    }
    catch (const mysqlx::Error& e) {
        std::cerr << "Error al conectar: " << e.what() << std::endl;
        sesion = nullptr;
        schema = nullptr;
        return false;
    }
}

void ConexionDB::desconectar() {
    if (schema != nullptr) {
        delete schema;
        schema = nullptr;
    }
    if (sesion != nullptr) {
        sesion->close();
        delete sesion;
        sesion = nullptr;
    }
}

bool ConexionDB::isConectado() const {
    return sesion != nullptr;
}

bool ConexionDB::ejecutar(const std::string& query) {
    if (!isConectado()) {
        std::cerr << "Error: No hay conexion activa" << std::endl;
        return false;
    }
    try {
        sesion->sql(query).execute();
        return true;
    }
    catch (const mysqlx::Error& e) {
        std::cerr << "Error en query: " << e.what() << std::endl;
        return false;
    }
}

mysqlx::RowResult ConexionDB::consultar(const std::string& query) {
    if (!isConectado()) {
        throw std::runtime_error("No hay conexion activa");
    }
    try {
        return sesion->sql(query).execute();
    }
    catch (const mysqlx::Error& e) {
        std::cerr << "Error en consulta: " << e.what() << std::endl;
        throw;
    }
}

mysqlx::Schema* ConexionDB::getSchema() {
    return schema;
}