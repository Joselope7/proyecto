#ifndef PILA_H
#define PILA_H

#include <stdexcept>
using namespace std;

template <typename T>
class Pila {
private:
    struct Nodo {
        T     dato;
        Nodo* siguiente;
        Nodo(T dato) : dato(dato), siguiente(nullptr) {}
    };

    Nodo* tope;
    int   tamanio;

public:
    Pila() : tope(nullptr), tamanio(0) {}

    ~Pila() {
        while (!estaVacia()) pop();
    }

    void push(const T& dato) {
        Nodo* nuevo = new Nodo(dato);
        nuevo->siguiente = tope;
        tope = nuevo;
        tamanio++;
    }

    void pop() {
        if (estaVacia()) throw runtime_error("Pila vacia");
        Nodo* temp = tope;
        tope = tope->siguiente;
        delete temp;
        tamanio--;
    }

    T& top() {
        if (estaVacia()) throw runtime_error("Pila vacia");
        return tope->dato;
    }

    bool estaVacia() const { return tope == nullptr; }
    int  getTamanio() const { return tamanio; }
};

#endif // PILA_H
