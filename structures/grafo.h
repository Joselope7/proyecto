#ifndef GRAFO_H
#define GRAFO_H

#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <stack>
#include <queue>
#include <functional>
#include <string>
using namespace std;

class Grafo {
private:
    // Lista de adyacencia: curso → lista de prerrequisitos
    unordered_map<string, vector<string>> adyacencia;

    // DFS auxiliar para detección de ciclos
    bool tieneCicloDFS(const string& nodo,
                       unordered_set<string>& visitados,
                       unordered_set<string>& enPila) const {
        visitados.insert(nodo);
        enPila.insert(nodo);

        auto it = adyacencia.find(nodo);
        if (it != adyacencia.end()) {
            for (const string& vecino : it->second) {
                if (!visitados.count(vecino)) {
                    if (tieneCicloDFS(vecino, visitados, enPila))
                        return true;
                } else if (enPila.count(vecino)) {
                    return true;
                }
            }
        }
        enPila.erase(nodo);
        return false;
    }

public:
    // Agregar curso al grafo
    void agregarNodo(const string& curso) {
        if (!adyacencia.count(curso))
            adyacencia[curso] = {};
    }

    // Agregar prerrequisito: para tomar 'curso' se necesita 'prereq'
    void agregarArista(const string& curso, const string& prereq) {
        agregarNodo(curso);
        agregarNodo(prereq);
        adyacencia[curso].push_back(prereq);
    }

    // DFS desde un nodo con acción
    void DFS(const string& inicio, function<void(const string&)> accion) const {
        unordered_set<string> visitados;
        stack<string> pila;
        pila.push(inicio);

        while (!pila.empty()) {
            string actual = pila.top();
            pila.pop();
            if (visitados.count(actual)) continue;
            visitados.insert(actual);
            accion(actual);

            auto it = adyacencia.find(actual);
            if (it != adyacencia.end()) {
                for (const string& vecino : it->second)
                    if (!visitados.count(vecino))
                        pila.push(vecino);
            }
        }
    }

    // BFS desde un nodo con acción
    void BFS(const string& inicio, function<void(const string&)> accion) const {
        unordered_set<string> visitados;
        queue<string> cola;
        cola.push(inicio);
        visitados.insert(inicio);

        while (!cola.empty()) {
            string actual = cola.front();
            cola.pop();
            accion(actual);

            auto it = adyacencia.find(actual);
            if (it != adyacencia.end()) {
                for (const string& vecino : it->second) {
                    if (!visitados.count(vecino)) {
                        visitados.insert(vecino);
                        cola.push(vecino);
                    }
                }
            }
        }
    }

    // Verificar si un estudiante cumple todos los prerrequisitos
    bool cumplePrerrequisitos(const string& curso,
                              const unordered_set<string>& cursosAprobados) const {
        auto it = adyacencia.find(curso);
        if (it == adyacencia.end()) return true; // sin prerrequisitos

        for (const string& prereq : it->second) {
            if (!cursosAprobados.count(prereq)) return false;
        }
        return true;
    }

    // Detección de ciclos en todo el grafo
    bool tieneCiclo() const {
        unordered_set<string> visitados;
        unordered_set<string> enPila;

        for (const auto& par : adyacencia) {
            if (!visitados.count(par.first)) {
                if (tieneCicloDFS(par.first, visitados, enPila))
                    return true;
            }
        }
        return false;
    }

    // Obtener prerrequisitos directos de un curso
    vector<string> getPrerequisitos(const string& curso) const {
        auto it = adyacencia.find(curso);
        if (it == adyacencia.end()) return {};
        return it->second;
    }
};

#endif // GRAFO_H