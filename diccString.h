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
    tupString(string cla, Significado sign){
        clave = cla;
        significado = sign;
    }
    string clave;
    Significado significado;
};

template<class Significado>
class diccString {
public:

    /**
     * Crea un dicc vacio.
     */
    diccString();
    /**
     * define un string,significado en el dicc.
     */
    void definir(string clave, Significado significado);
    bool def(string clave);
    Significado obtener(string clave);
    Conj<string> claves();
    void borrar(string clave);
    class Lista<struct tupString<Significado> >::Iterador vistaDicc();
    string min();
    string max();
    //FIJARSE SI NECESITAMOS COPIAR!
    diccString(const diccString& otro);
   diccString<Significado>& operator=(const diccString<Significado>& other);

private:

    struct Trie {
        class Lista<struct tupString<Significado> >::Iterador* valor;
        Lista<Trie*> hijos;
        unsigned int cantHijos;
    };
    Trie nodoTrie;
    Lista<class tupString<Significado> > valores;


};

template<class Significado>
std::ostream& operator<<(std::ostream& os, const diccString<Significado>& d);

template<class Significado>
diccString<Significado>::diccString(){
    nodoTrie =  Trie();
    valores = Lista<tupString<Significado> >();
}

template<class Significado>
void diccString<Significado>::definir(string clave, Significado significado) {

     tupString<Significado> entrada;
     entrada.clave=clave;
     entrada.significado=significado;
   class Lista<tupString<Significado> >::Iterador iter;
   iter = valores.AgregarAdelante(entrada);
    Trie* actual = &(this->nodoTrie);

    for (int i = 0; i < clave.length(); i++) {
        if ((actual->hijos)[clave[i]] == NULL) {
            (actual->hijos)[clave[i]] = new Trie();
            (actual->cantHijos)++;
        }
        actual = (actual->hijos)[clave[i]];
    }
    actual->valor = &iter;

}

template<class Significado>
bool diccString<Significado>::def(string clave) {
    Trie* actual = &(this->nodoTrie);
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
    Trie* actual = &(this->nodoTrie);
    for (int i = 0; i < clave.length(); i++) {
        actual = (actual->hijos)[clave[i]];
    }
    res = (*(actual->valor)).Siguiente().significado;
    return res;
}

template<class Significado>
Conj<string> diccString<Significado>::claves() {
    Conj<string> res;
    class Lista<class tupString<Significado> >::Iterador it;
    it = valores.CrearIt();
    while (it.HaySiguiente()) {
        res.AgregarRapido(it.Siguiente().clave);
        it.Avanzar();
    }
    return res;
}

template<class Significado>
void diccString<Significado>::borrar(string clave) {
    Trie* actual = &(this->nodoTrie);
    bool listo = false;
    for (int i = 0; i < clave.length(); i++) {
        Trie* temp = (actual->hijos)[clave[i]];
        if (temp->cantHijos == 0) {
            delete temp;
            (actual->hijos)[clave[i]] = NULL;
        }
        actual = temp;
    }
    *(actual->valor).EliminarSiguiente();

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
class Lista<tupString<Significado> >::Iterador diccString<Significado>::vistaDicc() {
    return valores.CrearIt();
}

template<class Significado>
string diccString<Significado>::min() {
    Trie* actual = &(this->nodoTrie);
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
            res = *(actual->valor)->Siguiente().clave;
        }
    }
    return res;
}

template<class Significado>
string diccString<Significado>::max() {
    Trie* actual = &(this->nodoTrie);
    bool termine = false;
    string res;
    while (!termine) {
        if (actual->cantHijos == 0) {
            res = *(actual->valor)->Siguiente().clave;
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
diccString<Significado>& diccString<Significado>::operator=(const diccString<Significado>& other) {
    class Lista< tupString<Significado> >::Iterador it = other.valores.CrearIt();
    diccString <Significado> res;
    while (it.HaySiguiente()) {
        res.definir(it.Siguiente().clave, it.Siguiente().significado);
        it.Avanzar();
    }
    return res;
}



#endif
