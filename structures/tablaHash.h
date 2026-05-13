#ifndef TABLAHASH_H
#define TABLAHASH_H

#include <vector>
#include <list>
#include <functional>
#include <stdexcept>
using namespace std;

template <typename T, typename K>
class TablaHash {
private:
    struct Par {
        K clave;
        T dato;
        Par(K clave, T dato) : clave(clave), dato(dato) {}
    };

    vector<list<Par>> tabla;
    int               capacidad;
    int               tamanio;

    int hashFunc(const K& clave) const {
        return hash<K>{}(clave) % capacidad;
    }

public:
    TablaHash(int capacidad = 101)
        : capacidad(capacidad), tamanio(0) {
        tabla.resize(capacidad);
    }

    void insertar(const K& clave, const T& dato) {
        int indice = hashFunc(clave);
        // Si ya existe, actualizar
        for (auto& par : tabla[indice]) {
            if (par.clave == clave) {
                par.dato = dato;
                return;
            }
        }
        tabla[indice].emplace_back(clave, dato);
        tamanio++;
    }

    T* buscar(const K& clave) {
        int indice = hashFunc(clave);
        for (auto& par : tabla[indice]) {
            if (par.clave == clave) return &par.dato;
        }
        return nullptr;
    }

    bool eliminar(const K& clave) {
        int indice = hashFunc(clave);
        auto& lista = tabla[indice];
        for (auto it = lista.begin(); it != lista.end(); ++it) {
            if (it->clave == clave) {
                lista.erase(it);
                tamanio--;
                return true;
            }
        }
        return false;
    }

    int  getTamanio()  const { return tamanio; }
    bool estaVacia()   const { return tamanio == 0; }
};

#endif // TABLAHASH_H