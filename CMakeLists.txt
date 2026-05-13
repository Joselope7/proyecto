cmake_minimum_required(VERSION 3.19)
project(universidad LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql LinguistTools)

qt_standard_project_setup()

qt_add_executable(universidad
    WIN32 MACOSX_BUNDLE
    main.cpp
    ui/mainwindow.cpp
    ui/mainwindow.h
    ui/mainwindow.ui
    core/persona.h core/persona.cpp
    core/carrera.h core/carrera.cpp
    core/registroacademico.h core/registroacademico.cpp
    core/estudiante.h core/estudiante.cpp
    core/curso.h core/curso.cpp
    core/matricula.h core/matricula.cpp
    database/conexionDB.h database/conexionDB.cpp
    structures/pila.h
    structures/cola.h
    structures/listaSimple.h
    structures/listaDoble.h
    structures/bst.h
    structures/avl.h
    structures/tablaHash.h
    structures/grafo.h
    core/sistemaAcademico.h core/sistemaAcademico.cpp



)

target_link_libraries(universidad
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Sql
)

qt_add_translations(
    TARGETS universidad
    TS_FILES universidad_es_419.ts
)

install(TARGETS universidad
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET universidad
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})