#ifndef DICCSTRING_H_INCLUDED
#define DICCSTRING_H_INCLUDED

#include "aed2.h"
#include "Dato.h"
#include "Pila.h"
#include "diccNat.h"
#include <cassert>
#include <string>
#include <iostream>

using namespace std;

template<class Significado>
struct tupString {
    string clave;
    Significado significado;
};

template<class Significado>
class diccString {
public:

    class Iterador;
    /**
     * Crea un dicc vacio.
     */
    diccString();
    /**
     * define un string,significado en el dicc.
     */
    definir(string clave, Significado significado);
    bool def(string clave);
    Significado obtener(string clave);
    Conj<string> claves();
    borrar(string clave);
    Iterador vistaDicc();
    string min();
    string max();
    //FIJARSE SI NECESITAMOS COPIAR!
    diccString(const diccString& otro);

    class Iterador {
    public:
        Lista<tuplaDicc<Significado> > siguientes() const;
        void avanzar();
        bool hayMas() const;
        tuplaDicc<Significado> actual() const;

    private:
    };

private:

    trie nodoTrie;
    Lista<tupString> valores;

    struct trie {
        Iterador* valor;
        Lista<trie*> hijos;
        unsigned int cantHijos;
    };
};

bool operator==(const diccString& d1, const diccString& d2);

template<class Significado>
std::ostream& operator<<(std::ostream& os, const diccString& d);

diccString::diccString() :
nodoTrie(new trie), valores(Lista<tupString>) {
}

template<class Significado>
void diccString<Significado>::definir(string clave, Significado significado) {

    tupString entrada;
    entrada.clave = clave;
    entrada.significado = significado;
    Lista::Iterador iter = (this->valores).AgregarAdelante(entrada);
    trie* actual = &(this->nodoTrie);

    for (int i = 0; i < clave.length(); i++) {
        if ((actual->hijos)[clave[i]] == NULL) {
            (actual->hijos)[clave[i]] = new trie;
            (actual->cantHijos)++;
        }
        actual = (actual->hijos)[clave[i]];
    }
    actual->valor = &iter;

}

template<class Significado>
bool diccString<Significado>::def(string clave) {
    trie* actual = &(this->nodoTrie);
    bool res = true;
    int i = 0;

    while (i < clave.length() && res) {
        if (actual->cantHijos > 0) {
            if (actual->hijos[clave[i]] == NULL) {
                res = false;
            } else {
                actual = (actual->hijos[clave[i]]);
            }
            i++;
        } else {
            res = false;
        }
    }

    if (res) res = !(actual->valor == NULL);
    return res;
}

template<class Significado>
Significado diccString<Significado>::obtener(string clave) {
    Significado res;
    trie* actual = &(this->nodoTrie);
    for (int i = 0; i < clave.length(); i++) {
        actual = (actual->hijos)[clave[i]];
    }
    res = (*(actual->valor)).actual().significado;
    return res;
}

template<class Significado>
Conj<string> claves() {
    Lista::Iterador it = this->valores.CrearIt();
    Conj <string> res;
    while (it.HaySiguiente()) {
        res.AgregarRapido(it.Siguiente().clave);
        it.Avanzar();
    }
    return res;
}

template<class Significado>
void diccString<Significado>::borrar(string clave) {
    trie* actual = &(this->nodoTrie);
    bool listo = false;
    for (int i = 0; i < clave.length(); i++) {
        trie* temp = (actual->hijos)[clave[i]];
        if (temp->cantHijos == 0) {
            delete temp;
            (actual->hijos)[clave[i]] = NULL;
        }
        actual = temp;
    }
    *(actual->valor).eliminarSiguiente();

    actual->valor = NULL;
    actual = &(this->nodoTrie);
    int i = 0;
    while (i < clave.length() - 1 && !listo) {
        if (actual->hijos[clave[i]]->cantHijos > 0) {
            actual = actual->hijos[clave[i]];
        } else {
            actual->hijos[clave[i]] = NULL;
            listo = true;
        }
        i++;
    }
}

template<class Significado>
Lista::Iterador diccString<Significado>::vistaDicc() {
    return this->valores.CrearIt();
}

template<class Significado>
string diccString<Significado>::min() {
    trie* actual = &(this->nodoTrie);
    bool termine = false;
    string res;
    while (!termine) {
        if (actual->valor == NULL) {
            for (int i = 0; i < 256; i++) {
                if (actual->hijos[i] != NULL)
                    actual = actual->hijos[i];
            }
        } else {
            termine = true;
            res = *(actual->valor)->actual().clave;
        }
    }
    return res;
}

template<class Significado>
string diccString<Significado>::max() {
    trie* actual = &(this->nodoTrie);
    bool termine = false;
    string res;
    while (!termine) {
        if (actual->cantHijos == 0) {
            res = *(actual->valor)->actual().clave;
            termine = true;
        } else {
            int i;
            bool seguir = true;

            while (i >= 0 && seguir) {
                if (actual->hijos[i] != NULL) {
                    actual = (actual->hijos[i]);
                    seguir = false;
                }
                i--;
            }

        }
    }
    return res;
}

/* vale para constructor por copia tambien*/
template<class Significado>
diccString<Significado>& diccString<Significado>::operator=(const diccString<significado>& other) {
    Lista::Iterador it = other.valores.crearIt();
    diccString <Significado> res;
    while it.haySiguiente() {
        res.definir(it.Siguiente().clave,it.Siguiente().significado);
        it.Avanzar();
    }
    return res;
}



#endif
