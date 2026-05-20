#ifndef ARBOLEXPRESIONES_H
#define ARBOLEXPRESIONES_H

#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

class ArbolExpresiones {
private:
    struct Nodo {
        string valor;       // operador (+, /) o número
        Nodo*  izquierdo;
        Nodo*  derecho;
        Nodo(string v) : valor(v), izquierdo(nullptr), derecho(nullptr) {}
    };

    Nodo* raiz;

    float evaluar(Nodo* nodo) const {
        if (!nodo) throw runtime_error("Árbol vacío");

        // Si es hoja → es un número
        if (!nodo->izquierdo && !nodo->derecho)
            return stof(nodo->valor);

        float izq = evaluar(nodo->izquierdo);
        float der = evaluar(nodo->derecho);

        if (nodo->valor == "+") return izq + der;
        if (nodo->valor == "-") return izq - der;
        if (nodo->valor == "*") return izq * der;
        if (nodo->valor == "/") {
            if (der == 0) throw runtime_error("División por cero");
            return izq / der;
        }
        throw runtime_error("Operador desconocido: " + nodo->valor);
    }

    void destruir(Nodo* nodo) {
        if (!nodo) return;
        destruir(nodo->izquierdo);
        destruir(nodo->derecho);
        delete nodo;
    }

public:
    ArbolExpresiones() : raiz(nullptr) {}
    ~ArbolExpresiones() { destruir(raiz); }

    // Construye el árbol para calcular promedio de notas
    // Ejemplo con 3 notas: (n1 + n2 + n3) / 3
    void construirPromedio(const vector<float>& notas) {
        destruir(raiz);
        raiz = nullptr;

        if (notas.empty()) return;
        if (notas.size() == 1) {
            raiz = new Nodo(to_string(notas[0]));
            return;
        }

        // Construir árbol de sumas encadenadas
        Nodo* suma = new Nodo("+");
        suma->izquierdo = new Nodo(to_string(notas[0]));
        suma->derecho   = new Nodo(to_string(notas[1]));

        for (size_t i = 2; i < notas.size(); i++) {
            Nodo* nuevo = new Nodo("+");
            nuevo->izquierdo = suma;
            nuevo->derecho   = new Nodo(to_string(notas[i]));
            suma = nuevo;
        }

        // Raíz = suma / cantidad
        raiz = new Nodo("/");
        raiz->izquierdo = suma;
        raiz->derecho   = new Nodo(to_string((float)notas.size()));
    }

    float calcularPromedio(const vector<float>& notas) {
        construirPromedio(notas);
        if (!raiz) return 0.0f;
        return evaluar(raiz);
    }

    bool estaVacio() const { return raiz == nullptr; }
};

#endif // ARBOLEXPRESIONES_H
