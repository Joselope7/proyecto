#ifndef COLA_H
#define COLA_H

#include <stdexcept>
using namespace std;

template <typename T>
class Cola {
private:
    struct Nodo {
        T     dato;
        Nodo* siguiente;
        Nodo(T dato) : dato(dato), siguiente(nullptr) {}
    };

    Nodo* frente;
    Nodo* final;
    int   tamanio;

public:
    Cola() : frente(nullptr), final(nullptr), tamanio(0) {}

    ~Cola() {
        while (!estaVacia()) desencolar();
    }

    void encolar(const T& dato) {
        Nodo* nuevo = new Nodo(dato);
        if (estaVacia()) {
            frente = final = nuevo;
        } else {
            final->siguiente = nuevo;
            final = nuevo;
        }
        tamanio++;
    }

    void desencolar() {
        if (estaVacia()) throw runtime_error("Cola vacia");
        Nodo* temp = frente;
        frente = frente->siguiente;
        if (frente == nullptr) final = nullptr;
        delete temp;
        tamanio--;
    }

    T& verFrente() {
        if (estaVacia()) throw runtime_error("Cola vacia");
        return frente->dato;
    }

    bool estaVacia() const { return frente == nullptr; }
    int  getTamanio() const { return tamanio; }
};

#endif // COLA_H