#ifndef SIMULADORMEMORIA_H
#define SIMULADORMEMORIA_H

#include <vector>
#include <string>
using namespace std;

struct Aula {
    int    id;
    string nombre;
    int    capacidad;
    int    ocupado;    // estudiantes asignados
    bool   disponible;

    Aula(int id, string nombre, int capacidad)
        : id(id), nombre(nombre), capacidad(capacidad),
        ocupado(0), disponible(true) {}

    int espacioLibre() const { return capacidad - ocupado; }
};

class SimuladorMemoria {
private:
    vector<Aula> aulas;

public:
    void agregarAula(int id, const string& nombre, int capacidad) {
        aulas.emplace_back(id, nombre, capacidad);
    }

    void liberarAulas() {
        for (auto& a : aulas) {
            a.ocupado    = 0;
            a.disponible = true;
        }
    }

    // Primer ajuste — asigna la primera aula con espacio suficiente
    int primerAjuste(int estudiantesInscritos) {
        for (auto& a : aulas) {
            if (a.disponible && a.espacioLibre() >= estudiantesInscritos) {
                a.ocupado    += estudiantesInscritos;
                a.disponible  = (a.espacioLibre() > 0);
                return a.id;
            }
        }
        return -1; // sin aula disponible
    }

    // Mejor ajuste — asigna el aula con el espacio más justo
    int mejorAjuste(int estudiantesInscritos) {
        int mejorIdx   = -1;
        int menorSobra = INT_MAX;

        for (int i = 0; i < (int)aulas.size(); i++) {
            Aula& a = aulas[i];
            if (a.disponible && a.espacioLibre() >= estudiantesInscritos) {
                int sobra = a.espacioLibre() - estudiantesInscritos;
                if (sobra < menorSobra) {
                    menorSobra = sobra;
                    mejorIdx   = i;
                }
            }
        }

        if (mejorIdx == -1) return -1;
        aulas[mejorIdx].ocupado    += estudiantesInscritos;
        aulas[mejorIdx].disponible  = (aulas[mejorIdx].espacioLibre() > 0);
        return aulas[mejorIdx].id;
    }

    // Peor ajuste — asigna el aula con más espacio disponible
    int peorAjuste(int estudiantesInscritos) {
        int peorIdx    = -1;
        int mayorSobra = -1;

        for (int i = 0; i < (int)aulas.size(); i++) {
            Aula& a = aulas[i];
            if (a.disponible && a.espacioLibre() >= estudiantesInscritos) {
                int sobra = a.espacioLibre() - estudiantesInscritos;
                if (sobra > mayorSobra) {
                    mayorSobra = sobra;
                    peorIdx    = i;
                }
            }
        }

        if (peorIdx == -1) return -1;
        aulas[peorIdx].ocupado    += estudiantesInscritos;
        aulas[peorIdx].disponible  = (aulas[peorIdx].espacioLibre() > 0);
        return aulas[peorIdx].id;
    }

    vector<Aula> getAulas() const { return aulas; }

    Aula* getAula(int id) {
        for (auto& a : aulas)
            if (a.id == id) return &a;
        return nullptr;
    }
};

#endif // SIMULADORMEMORIA_H
