-- Active: 1775234605750@@127.0.0.1@3306@mysql
-- ================================================
-- Sistema de Gestion Academica Universitaria
-- Base de datos: universidad_db
-- ================================================

CREATE DATABASE IF NOT EXISTS universidad_db;
USE universidad_db;

-- ------------------------------------------------
-- Tabla: estudiantes
-- ------------------------------------------------
CREATE TABLE IF NOT EXISTS estudiantes (
    carnet       VARCHAR(10)  PRIMARY KEY,
    nombre       VARCHAR(100) NOT NULL,
    carrera      VARCHAR(100) NOT NULL,
    anio_ingreso INT          NOT NULL,
    promedio     FLOAT        DEFAULT 0.0
);

-- ------------------------------------------------
-- Tabla: cursos
-- ------------------------------------------------
CREATE TABLE IF NOT EXISTS cursos (
    codigo      VARCHAR(10)  PRIMARY KEY,
    nombre      VARCHAR(100) NOT NULL,
    creditos    INT          NOT NULL,
    cupo_maximo INT          NOT NULL,
    cupo_actual INT          DEFAULT 0
);

-- ------------------------------------------------
-- Tabla: matriculas
-- ------------------------------------------------
CREATE TABLE IF NOT EXISTS matriculas (
    id               INT AUTO_INCREMENT PRIMARY KEY,
    carnet_estudiante VARCHAR(10)  NOT NULL,
    codigo_curso      VARCHAR(10)  NOT NULL,
    ciclo             VARCHAR(20)  NOT NULL,
    FOREIGN KEY (carnet_estudiante) REFERENCES estudiantes(carnet),
    FOREIGN KEY (codigo_curso)      REFERENCES cursos(codigo)
);

-- ------------------------------------------------
-- Tabla: historial_academico
-- ------------------------------------------------
CREATE TABLE IF NOT EXISTS historial_academico (
    id               INT AUTO_INCREMENT PRIMARY KEY,
    carnet_estudiante VARCHAR(10)  NOT NULL,
    curso             VARCHAR(100) NOT NULL,
    nota              FLOAT        NOT NULL,
    estado            VARCHAR(10)  NOT NULL,  -- 'aprobado' o 'reprobado'
    FOREIGN KEY (carnet_estudiante) REFERENCES estudiantes(carnet)
);

-- ------------------------------------------------
-- Datos de prueba (opcional pero util para probar)
-- ------------------------------------------------
INSERT INTO estudiantes VALUES ('2024001', 'Ana Garcia',    'Ingenieria en Sistemas', 2024, 0.0);
INSERT INTO estudiantes VALUES ('2024002', 'Luis Perez',    'Ingenieria Civil',       2024, 0.0);
INSERT INTO estudiantes VALUES ('2023001', 'Maria Lopez',   'Ingenieria en Sistemas', 2023, 85.5);
INSERT INTO estudiantes VALUES ('2023002', 'Carlos Ruiz',   'Administracion',         2023, 72.0);

INSERT INTO cursos VALUES ('CC101', 'Programacion I',       4, 30, 0);
INSERT INTO cursos VALUES ('CC102', 'Programacion II',      4, 30, 0);
INSERT INTO cursos VALUES ('CC201', 'Estructuras de Datos', 4, 25, 0);
INSERT INTO cursos VALUES ('MA101', 'Matematica I',         5, 35, 0);
INSERT INTO cursos VALUES ('MA102', 'Matematica II',        5, 35, 0);