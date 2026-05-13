#ifndef AVL_H
#define AVL_H

#include <functional>
#include <algorithm>
using namespace std;

template <typename T, typename K>
class AVL {
private:
    struct Nodo {
        T     dato;
        K     clave;
        int   altura;
        Nodo* izquierdo;
        Nodo* derecho;
        Nodo(T dato, K clave)
            : dato(dato), clave(clave), altura(1),
            izquierdo(nullptr), derecho(nullptr) {}
    };

    Nodo* raiz;

    int altura(Nodo* n) const {
        return n ? n->altura : 0;
    }

    int balance(Nodo* n) const {
        return n ? altura(n->izquierdo) - altura(n->derecho) : 0;
    }

    void actualizarAltura(Nodo* n) {
        n->altura = 1 + max(altura(n->izquierdo), altura(n->derecho));
    }

    // Rotación simple derecha
    Nodo* rotarDerecha(Nodo* y) {
        Nodo* x  = y->izquierdo;
        Nodo* T2 = x->derecho;
        x->derecho   = y;
        y->izquierdo = T2;
        actualizarAltura(y);
        actualizarAltura(x);
        return x;
    }

    // Rotación simple izquierda
    Nodo* rotarIzquierda(Nodo* x) {
        Nodo* y  = x->derecho;
        Nodo* T2 = y->izquierdo;
        y->izquierdo = x;
        x->derecho   = T2;
        actualizarAltura(x);
        actualizarAltura(y);
        return y;
    }

    Nodo* insertar(Nodo* nodo, const T& dato, const K& clave) {
        if (!nodo) return new Nodo(dato, clave);

        if      (clave < nodo->clave) nodo->izquierdo = insertar(nodo->izquierdo, dato, clave);
        else if (clave > nodo->clave) nodo->derecho   = insertar(nodo->derecho,   dato, clave);
        else return nodo; // clave duplicada

        actualizarAltura(nodo);
        int b = balance(nodo);

        // Rotación simple derecha
        if (b > 1 && clave < nodo->izquierdo->clave)
            return rotarDerecha(nodo);
        // Rotación simple izquierda
        if (b < -1 && clave > nodo->derecho->clave)
            return rotarIzquierda(nodo);
        // Rotación doble izquierda-derecha
        if (b > 1 && clave > nodo->izquierdo->clave) {
            nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
            return rotarDerecha(nodo);
        }
        // Rotación doble derecha-izquierda
        if (b < -1 && clave < nodo->derecho->clave) {
            nodo->derecho = rotarDerecha(nodo->derecho);
            return rotarIzquierda(nodo);
        }
        return nodo;
    }

    void inOrden(Nodo* nodo, function<void(const T&)> accion) const {
        if (!nodo) return;
        inOrden(nodo->izquierdo, accion);
        accion(nodo->dato);
        inOrden(nodo->derecho, accion);
    }

    void destruir(Nodo* nodo) {
        if (!nodo) return;
        destruir(nodo->izquierdo);
        destruir(nodo->derecho);
        delete nodo;
    }

public:
    AVL() : raiz(nullptr) {}
    ~AVL() { destruir(raiz); }

    void insertar(const T& dato, const K& clave) {
        raiz = insertar(raiz, dato, clave);
    }

    // Recorre de menor a mayor promedio
    void inOrden(function<void(const T&)> accion) const {
        inOrden(raiz, accion);
    }

    bool estaVacio() const { return raiz == nullptr; }
};

#endif // AVL_H