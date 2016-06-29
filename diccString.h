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
    tupString(string cla, const Significado& sign){
        clave = cla;
        significado = sign;
    }
    string clave;
    const Significado& significado;
};

template<class Significado>
class diccString {
public:

    diccString();
    ~diccString();

    void definir(string clave, const Significado& significado);
    bool def(string clave) const;
    const Significado& obtener(string clave) const;
    const Conj<string> claves() const;
    void borrar(string clave);
    class Lista<struct tupString<Significado> >::Iterador vistaDicc() const;
    string min() const;
    string max() const;

    diccString(const diccString<Significado>& other);
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
ostream& operator<<(ostream& os, const diccString<Significado>& d){
  class Lista<struct tupString<Significado> >::Iterador it = d.vistaDicc();
  os << "{ ";
  while(it.HaySiguiente()){
    os << "(" << it.Siguiente().clave << ", " << it.Siguiente().significado << ")";
    it.Avanzar();
    if (it.HaySiguiente()) os << ", ";
  }
  os << " }" << endl;
  return os;
}

template<class Significado>
diccString<Significado>::diccString(){
    nodoTrie =  Trie();
    valores = Lista<tupString<Significado> >();
}

template<class Significado>
diccString<Significado>::~diccString(){
  string maximo = max();
  string minimo = min();
  while (def(maximo)){
    minimo = min();
    borrar(minimo);
  }
}

template<class Significado>
void diccString<Significado>::definir(string clave, const Significado& significado) {

    tupString<Significado> entrada;
    entrada.clave=clave;
    entrada.significado=significado;
    class Lista<tupString<Significado> >::Iterador iter = valores.AgregarAdelante(entrada);
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
bool diccString<Significado>::def(string clave) const {
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
const Significado& diccString<Significado>::obtener(string clave) const {
    const Significado& res;
    Trie* actual = &(this->nodoTrie);
    for (int i = 0; i < clave.length(); i++) {
        actual = (actual->hijos)[clave[i]];
    }
    res = (*(actual->valor)).Siguiente().significado;
    return res;
}

template<class Significado>
const Conj<string> diccString<Significado>::claves() const {
    const Conj<string> res;
    class Lista<class tupString<Significado> >::Iterador it = valores.CrearIt();
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
class Lista<tupString<Significado> >::Iterador diccString<Significado>::vistaDicc() const {
    return valores.CrearIt();
}

template<class Significado>
string diccString<Significado>::min() const {
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
string diccString<Significado>::max() const {
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

template<class Significado>
diccString<Significado>::diccString(const diccString<Significado>& other){
    *this = other;
}

#endif
