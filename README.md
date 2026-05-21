# Sistema de Gestión Académica Universitaria

Proyecto Final — Programación II  
Facultad de Ingeniería — Universidad Mariano Gálvez  
Ing. David Marroquín

---

## Descripción

Sistema de escritorio desarrollado en C++ con Qt 6 que simula el funcionamiento
de un registro académico universitario. Gestiona estudiantes, cursos, matrículas,
historial académico y prerrequisitos, integrando estructuras de datos avanzadas
implementadas desde cero y persistencia en MySQL.

---

## Tecnologías

- Lenguaje: C++ 17
- Interfaz gráfica: Qt 6
- Base de datos: MySQL
- Control de versiones: Git
- IDE: Qt Creator

---

## Arquitectura

El sistema está dividido en 4 capas:

- UI
- Logica
- Estructuras de datos
- Base de Datos

---

## Estructuras de Datos Implementadas

| Estructura | Archivo | Uso en el sistema |
|---|---|---|
| Pila | structures/pila.h | Historial académico por estudiante |
| Cola | structures/cola.h | Fila de espera de matrícula |
| Lista Simple | structures/listaSimple.h | Registro general de estudiantes |
| Lista Doble | structures/listaDoble.h | Catálogo de cursos |
| BST | structures/bst.h | Búsqueda de estudiantes por carnet |
| AVL | structures/avl.h | Clasificación por promedio |
| Árbol de Expresiones | structures/ArbolExpresiones.h | Cálculo de promedio académico |
| Árbol B | structures/ArbolB.h | Índice académico |
| Tabla Hash | structures/tablaHash.h | Acceso rápido por carnet |
| Grafo Dirigido | structures/grafo.h | Prerrequisitos con DFS/BFS |
| Simulador de Memoria | structures/SimuladorMemoria.h | Asignación de aulas |

---

## Base de Datos

**Nombre:** universidad_db  
**Motor:** MySQL 8.0  
**Conexión:** ODBC Driver

### Tablas

- `carreras` — carreras universitarias
- `estudiantes` — datos de estudiantes
- `cursos` — catálogo de cursos
- `curso_carrera` — relación curso-carrera (muchos a muchos)
- `prerrequisitos` — dependencias entre cursos
- `matriculas` — inscripciones de estudiantes
- `historial_academico` — notas y estados por curso

---

## Funcionalidades

- Registro, edición y eliminación de estudiantes
- Catálogo de cursos con control de cupo
- Proceso de matrícula con validación automática de prerrequisitos
- Lista de espera cuando el cupo está agotado
- Historial académico por estudiante
- Cálculo de promedio con árbol de expresiones
- Búsqueda rápida por carnet (tabla hash + BST)
- Exportación de reportes a CSV
- Simulación de asignación de aulas (primer, mejor y peor ajuste)

---
## Autor

Proyecto desarrollado por José Alfredo López Argueta