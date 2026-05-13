#ifndef LISTADOBLE_H
#define LISTADOBLE_H

#include <stdexcept>
#include <functional>
using namespace std;

template <typename T>
class ListaDoble {
private:
    struct Nodo {
        T     dato;
        Nodo* siguiente;
        Nodo* anterior;
        Nodo(T dato) : dato(dato), siguiente(nullptr), anterior(nullptr) {}
    };

    Nodo* cabeza;
    Nodo* cola;
    int   tamanio;

public:
    ListaDoble() : cabeza(nullptr), cola(nullptr), tamanio(0) {}

    ~ListaDoble() {
        while (cabeza != nullptr) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }

    void insertarAlFinal(const T& dato) {
        Nodo* nuevo = new Nodo(dato);
        if (estaVacia()) {
            cabeza = cola = nuevo;
        } else {
            nuevo->anterior = cola;
            cola->siguiente = nuevo;
            cola = nuevo;
        }
        tamanio++;
    }

    bool eliminar(function<bool(const T&)> criterio) {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            if (criterio(actual->dato)) {
                if (actual->anterior) actual->anterior->siguiente = actual->siguiente;
                else                  cabeza = actual->siguiente;
                if (actual->siguiente) actual->siguiente->anterior = actual->anterior;
                else                   cola = actual->anterior;
                delete actual;
                tamanio--;
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }

    T* buscar(function<bool(const T&)> criterio) {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            if (criterio(actual->dato)) return &actual->dato;
            actual = actual->siguiente;
        }
        return nullptr;
    }

    // Recorrer hacia adelante
    void recorrerAdelante(function<void(const T&)> accion) const {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            accion(actual->dato);
            actual = actual->siguiente;
        }
    }

    // Recorrer hacia atrás
    void recorrerAtras(function<void(const T&)> accion) const {
        Nodo* actual = cola;
        while (actual != nullptr) {
            accion(actual->dato);
            actual = actual->anterior;
        }
    }

    bool estaVacia() const { return cabeza == nullptr; }
    int  getTamanio() const { return tamanio; }
};

#endif // LISTADOBLE_H
