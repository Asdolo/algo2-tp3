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
    tupString(string cla, const Significado& sign) : clave(cla), significado(sign) {};
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
    class Lista<struct tupString<Significado> >::const_Iterador vistaDicc() const;
    string min() const;
    string max() const;

    diccString(const diccString<Significado>& other);
    diccString<Significado>& operator=(const diccString<Significado>& other);

private:

    struct Trie {
        Trie() : valor(NULL), cantHijos(0) {
          for (int i = 0; i < 256; i++) {
            hijos[i] = NULL;
          }
        }

        class Lista<struct tupString<Significado> >::Iterador* valor;
        Trie* hijos[256];
        unsigned int cantHijos;
    };
    Trie nodoTrie;
    Lista<class tupString<Significado> > valores;

};

template<class Significado>
ostream& operator<<(ostream& os, const diccString<Significado>& d){
  class Lista<struct tupString<Significado> >::const_Iterador it = d.vistaDicc();
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
ostream& operator<<(ostream& os, const tupString<Significado>& t){
  os << "(" << t.clave << ", " << t.significado << ")";
  return os;
}

template<class Significado>
diccString<Significado>::diccString(){
    nodoTrie =  Trie();
    valores = Lista<tupString<Significado> >();
}

template<class Significado>
diccString<Significado>::~diccString(){
  string minimo;
  int i = 0;
  while ( nodoTrie.cantHijos > 0 ){
    minimo = min();
    borrar(minimo);
  }
}

template<class Significado>
void diccString<Significado>::definir(string clave, const Significado& significado) {
    assert(!def(clave) && clave.length() > 0);

    tupString<Significado> entrada = tupString<Significado>(clave, significado);
    class Lista<tupString<Significado> >::Iterador it = valores.AgregarAdelante(entrada);
    Trie* actual = &(this->nodoTrie);

    for (int i = 0; i < clave.length(); i++) {
        if ((actual->hijos)[clave[i]] == NULL) {
          (actual->hijos)[clave[i]] = new Trie();
        }
        actual->cantHijos++;
        actual = (actual->hijos)[clave[i]];
    }
    class Lista<tupString<Significado> >::Iterador* it_ptr = new class Lista<tupString<Significado> >::Iterador(it);
    actual->valor = it_ptr;
}

template<class Significado>
bool diccString<Significado>::def(string clave) const {
    const Trie* actual = &(this->nodoTrie);
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
    assert(def(clave) && clave.length() > 0);

    const Trie* actual = &(this->nodoTrie);
    for (int i = 0; i < clave.length(); i++) {
        actual = (actual->hijos)[clave[i]];
    }
    const Significado& res = actual->valor->Siguiente().significado;
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
    assert(def(clave) && clave.length() > 0);

    Trie* actual = &(this->nodoTrie);
    Trie* temp = actual->hijos[clave[0]];
    actual->cantHijos--;

    bool borrar = false;
    for (int i = 0; i < clave.length(); i++) {
        if (i + 1 != clave.length() ){
         temp->cantHijos--;
        }
        if ( i+1 == clave.length() ) {
          temp->valor->EliminarSiguiente();
          delete temp->valor;
          temp->valor = NULL;
        }
        if ( !borrar && temp->cantHijos == 0 && temp->valor == NULL) {
          actual->hijos[clave[i]] = NULL;
          borrar = true;
        }
        actual = temp;
        temp = actual->hijos[clave[i+1]];
        if ( borrar ){
          delete actual;
        }
    }
}



template<class Significado>
class Lista<tupString<Significado> >::const_Iterador diccString<Significado>::vistaDicc() const {
    return valores.CrearIt();
}

template<class Significado>
string diccString<Significado>::min() const {
    assert(this->nodoTrie.cantHijos > 0);
    const Trie* actual = &(this->nodoTrie);
    bool termine = false;
    bool seguir;
    string res;
    while (!termine) {
        if (actual->valor == NULL) {
            seguir = true;
            for (int i = 0; i < 256 && seguir; i++) {
                if (actual->hijos[i] != NULL){
                    actual = actual->hijos[i];
                    seguir = false;
                }
            }
        } else {
            termine = true;
            res = actual->valor->Siguiente().clave;
        }
    }
    return res;
}

template<class Significado>
string diccString<Significado>::max() const {
    assert(this->nodoTrie.cantHijos > 0);

    const Trie* actual = &(this->nodoTrie);
    bool termine = false;
    string res;
    while (!termine) {
        if (actual->cantHijos == 0) {
            res = actual->valor->Siguiente().clave;
            termine = true;
        } else {
            int i = 255;
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
    class Lista< tupString<Significado> >::const_Iterador it = other.valores.CrearIt();
    while (it.HaySiguiente()) {
        Significado copia = Significado(it.Siguiente().significado);
        this->definir(it.Siguiente().clave, copia);
        it.Avanzar();
    }
}

template<class Significado>
diccString<Significado>::diccString(const diccString<Significado>& other){
    *this = other;
}

#endif
