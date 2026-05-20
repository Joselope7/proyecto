#ifndef ARBOLB_H
#define ARBOLB_H

#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class ArbolB {
private:
    static const int ORDEN = 3; // árbol B de orden 3 (máx 2 claves por nodo)

    struct Nodo {
        vector<string> claves;
        vector<Nodo*>  hijos;
        bool           esHoja;

        Nodo(bool esHoja = true) : esHoja(esHoja) {}

        bool estaLleno() const {
            return (int)claves.size() == 2 * ORDEN - 1;
        }
    };

    Nodo* raiz;

    // Buscar clave en subárbol
    bool buscar(Nodo* nodo, const string& clave) const {
        int i = 0;
        while (i < (int)nodo->claves.size() && clave > nodo->claves[i]) i++;

        if (i < (int)nodo->claves.size() && clave == nodo->claves[i])
            return true;

        if (nodo->esHoja) return false;

        return buscar(nodo->hijos[i], clave);
    }

    // Dividir hijo lleno
    void dividirHijo(Nodo* padre, int i, Nodo* hijo) {
        Nodo* nuevo = new Nodo(hijo->esHoja);
        int medio = ORDEN - 1;

        // Copiar mitad derecha al nuevo nodo
        for (int j = 0; j < ORDEN - 1; j++)
            nuevo->claves.push_back(hijo->claves[medio + 1 + j]);

        if (!hijo->esHoja)
            for (int j = 0; j < ORDEN; j++)
                nuevo->hijos.push_back(hijo->hijos[medio + 1 + j]);

        string claveMedia = hijo->claves[medio];

        // Recortar hijo original
        hijo->claves.resize(medio);
        if (!hijo->esHoja) hijo->hijos.resize(medio + 1);

        // Insertar en padre
        padre->hijos.insert(padre->hijos.begin() + i + 1, nuevo);
        padre->claves.insert(padre->claves.begin() + i, claveMedia);
    }

    // Insertar en nodo no lleno
    void insertarNoLleno(Nodo* nodo, const string& clave) {
        int i = (int)nodo->claves.size() - 1;

        if (nodo->esHoja) {
            nodo->claves.push_back("");
            while (i >= 0 && clave < nodo->claves[i]) {
                nodo->claves[i + 1] = nodo->claves[i];
                i--;
            }
            nodo->claves[i + 1] = clave;
        } else {
            while (i >= 0 && clave < nodo->claves[i]) i--;
            i++;
            if (nodo->hijos[i]->estaLleno()) {
                dividirHijo(nodo, i, nodo->hijos[i]);
                if (clave > nodo->claves[i]) i++;
            }
            insertarNoLleno(nodo->hijos[i], clave);
        }
    }

    void destruir(Nodo* nodo) {
        if (!nodo) return;
        for (auto h : nodo->hijos) destruir(h);
        delete nodo;
    }

    // Recorrido inorden para listar todas las claves
    void inOrden(Nodo* nodo, vector<string>& resultado) const {
        if (!nodo) return;
        for (int i = 0; i < (int)nodo->claves.size(); i++) {
            if (!nodo->esHoja) inOrden(nodo->hijos[i], resultado);
            resultado.push_back(nodo->claves[i]);
        }
        if (!nodo->esHoja) inOrden(nodo->hijos[nodo->claves.size()], resultado);
    }

public:
    ArbolB() : raiz(new Nodo(true)) {}
    ~ArbolB() { destruir(raiz); }

    void insertar(const string& clave) {
        if (raiz->estaLleno()) {
            Nodo* nueva_raiz = new Nodo(false);
            nueva_raiz->hijos.push_back(raiz);
            dividirHijo(nueva_raiz, 0, raiz);
            raiz = nueva_raiz;
        }
        insertarNoLleno(raiz, clave);
    }

    bool buscar(const string& clave) const {
        return buscar(raiz, clave);
    }

    vector<string> listarTodo() const {
        vector<string> resultado;
        inOrden(raiz, resultado);
        return resultado;
    }

    bool estaVacio() const {
        return raiz->claves.empty();
    }
};

#endif // ARBOLB_H
