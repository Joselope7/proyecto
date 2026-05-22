-- Active: 1775234605750@@127.0.0.1@3306@universidad_db
-- universidad_db

CREATE TABLE carreras (
    id INT PRIMARY KEY AUTO_INCREMENT,
    nombre VARCHAR(100) NOT NULL
);

CREATE TABLE estudiantes (
    carnet VARCHAR(10) PRIMARY KEY,
    nombre VARCHAR(100) NOT NULL,
    id_carrera INT,
    fecha_ingreso DATE,
    FOREIGN KEY (id_carrera) REFERENCES carreras(id)
);

CREATE TABLE cursos (
    codigo VARCHAR(10) PRIMARY KEY,
    nombre VARCHAR(100) NOT NULL,
    creditos INT,
    cupo_maximo INT
);

CREATE TABLE prerrequisitos (
    codigo_curso VARCHAR(10),
    codigo_prereq VARCHAR(10),
    PRIMARY KEY (codigo_curso, codigo_prereq)
);

CREATE TABLE matriculas (
    id INT PRIMARY KEY AUTO_INCREMENT,
    carnet_estudiante VARCHAR(10),
    codigo_curso VARCHAR(10),
    ciclo VARCHAR(20),
    FOREIGN KEY (carnet_estudiante) REFERENCES estudiantes(carnet),
    FOREIGN KEY (codigo_curso) REFERENCES cursos(codigo)
);

CREATE TABLE aulas (
    id         INT PRIMARY KEY,
    nombre     VARCHAR(50) NOT NULL,
    capacidad  INT NOT NULL,
    ocupado    INT DEFAULT 0,
    disponible TINYINT DEFAULT 1
);

CREATE TABLE historial_academico (
    id INT PRIMARY KEY AUTO_INCREMENT,
    carnet_estudiante VARCHAR(10),
    codigo_curso VARCHAR(10),
    nota DECIMAL(4,2),
    estado ENUM('aprobado','reprobado'),
    fecha DATE
);