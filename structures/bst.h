#ifndef BST_H
#define BST_H

#include <functional>
#include <stdexcept>
using namespace std;

template <typename T, typename K>
class BST {
private:
    struct Nodo {
        T     dato;
        K     clave;
        Nodo* izquierdo;
        Nodo* derecho;
        Nodo(T dato, K clave)
            : dato(dato), clave(clave),
            izquierdo(nullptr), derecho(nullptr) {}
    };

    Nodo* raiz;

    Nodo* insertar(Nodo* nodo, const T& dato, const K& clave) {
        if (nodo == nullptr) return new Nodo(dato, clave);
        if (clave < nodo->clave)
            nodo->izquierdo = insertar(nodo->izquierdo, dato, clave);
        else if (clave > nodo->clave)
            nodo->derecho = insertar(nodo->derecho, dato, clave);
        return nodo;
    }

    Nodo* buscar(Nodo* nodo, const K& clave) const {
        if (nodo == nullptr || nodo->clave == clave) return nodo;
        if (clave < nodo->clave) return buscar(nodo->izquierdo, clave);
        return buscar(nodo->derecho, clave);
    }

    Nodo* minimoNodo(Nodo* nodo) {
        while (nodo->izquierdo != nullptr)
            nodo = nodo->izquierdo;
        return nodo;
    }

    Nodo* eliminar(Nodo* nodo, const K& clave) {
        if (nodo == nullptr) return nullptr;
        if (clave < nodo->clave)
            nodo->izquierdo = eliminar(nodo->izquierdo, clave);
        else if (clave > nodo->clave)
            nodo->derecho = eliminar(nodo->derecho, clave);
        else {
            if (nodo->izquierdo == nullptr) {
                Nodo* temp = nodo->derecho;
                delete nodo;
                return temp;
            } else if (nodo->derecho == nullptr) {
                Nodo* temp = nodo->izquierdo;
                delete nodo;
                return temp;
            }
            Nodo* temp = minimoNodo(nodo->derecho);
            nodo->dato  = temp->dato;
            nodo->clave = temp->clave;
            nodo->derecho = eliminar(nodo->derecho, temp->clave);
        }
        return nodo;
    }

    void inOrden(Nodo* nodo, function<void(const T&)> accion) const {
        if (nodo == nullptr) return;
        inOrden(nodo->izquierdo, accion);
        accion(nodo->dato);
        inOrden(nodo->derecho, accion);
    }

    void preOrden(Nodo* nodo, function<void(const T&)> accion) const {
        if (nodo == nullptr) return;
        accion(nodo->dato);
        preOrden(nodo->izquierdo, accion);
        preOrden(nodo->derecho, accion);
    }

    void postOrden(Nodo* nodo, function<void(const T&)> accion) const {
        if (nodo == nullptr) return;
        postOrden(nodo->izquierdo, accion);
        postOrden(nodo->derecho, accion);
        accion(nodo->dato);
    }

    void destruir(Nodo* nodo) {
        if (nodo == nullptr) return;
        destruir(nodo->izquierdo);
        destruir(nodo->derecho);
        delete nodo;
    }

public:
    BST() : raiz(nullptr) {}
    ~BST() { destruir(raiz); }

    void insertar(const T& dato, const K& clave) {
        raiz = insertar(raiz, dato, clave);
    }

    T* buscar(const K& clave) const {
        Nodo* resultado = buscar(raiz, clave);
        return resultado ? &resultado->dato : nullptr;
    }

    void eliminar(const K& clave) {
        raiz = eliminar(raiz, clave);
    }

    void inOrden  (function<void(const T&)> accion) const { inOrden  (raiz, accion); }
    void preOrden (function<void(const T&)> accion) const { preOrden (raiz, accion); }
    void postOrden(function<void(const T&)> accion) const { postOrden(raiz, accion); }

    bool estaVacio() const { return raiz == nullptr; }
};

#endif // BST_H