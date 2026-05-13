#ifndef LISTASIMPLE_H
#define LISTASIMPLE_H

#include <stdexcept>
#include <functional>
using namespace std;

template <typename T>
class ListaSimple {
private:
    struct Nodo {
        T     dato;
        Nodo* siguiente;
        Nodo(T dato) : dato(dato), siguiente(nullptr) {}
    };

    Nodo* cabeza;
    int   tamanio;

public:
    ListaSimple() : cabeza(nullptr), tamanio(0) {}

    ~ListaSimple() {
        while (cabeza != nullptr) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }

    void insertar(const T& dato) {
        Nodo* nuevo = new Nodo(dato);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        tamanio++;
    }

    bool eliminar(function<bool(const T&)> criterio) {
        Nodo* actual   = cabeza;
        Nodo* anterior = nullptr;

        while (actual != nullptr) {
            if (criterio(actual->dato)) {
                if (anterior == nullptr)
                    cabeza = actual->siguiente;
                else
                    anterior->siguiente = actual->siguiente;
                delete actual;
                tamanio--;
                return true;
            }
            anterior = actual;
            actual   = actual->siguiente;
        }
        return false;
    }

    // Buscar con criterio lambda
    T* buscar(function<bool(const T&)> criterio) {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            if (criterio(actual->dato)) return &actual->dato;
            actual = actual->siguiente;
        }
        return nullptr;
    }

    // Recorrer con función lambda
    void recorrer(function<void(const T&)> accion) const {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            accion(actual->dato);
            actual = actual->siguiente;
        }
    }

    bool estaVacia() const { return cabeza == nullptr; }
    int  getTamanio() const { return tamanio; }
};

#endif // LISTASIMPLE_H
