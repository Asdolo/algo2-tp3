#ifndef DICCSTRING_H_INCLUDED
#define DICCSTRING_H_INCLUDED

#include "Conj.h"
#include "Lista.h"
#include <cassert>
#include <string>
#include <iostream>

using namespace std;
using namespace aed2;

namespace tp3{

template<class Significado>
struct tupString {
    tupString(string cla, const Significado& sign) : clave(cla), significado(sign) {};
    bool operator==(const tupString& other) const {                             //No sé por qué presiento que esto no debería ser necesario, pero me lo pide en tabla...
      return (clave == other.clave && significado == other.significado);
    }

    string clave;
    Significado significado;
};

template<class Significado>
class diccString {
public:

    diccString();
    ~diccString();

    void definir(const string& clave, const Significado& significado);
    bool def(const string& clave) const;
    Significado& obtener(const string& clave) const;
    const Conj<string> claves() const;
    void borrar(string clave);
    class Lista<struct tupString<Significado> >::const_Iterador vistaDicc() const;
    string min() const;
    string max() const;

    diccString(const diccString<Significado>& other);
    diccString<Significado>& operator=(const diccString<Significado>& other);
    bool operator==(const diccString<Significado>& other) const;
    bool operator!=(const diccString<Significado>& other) const;


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
  while ( nodoTrie.cantHijos > 0 ){
    //std::cout << "Hijos: " << nodoTrie.cantHijos << std::endl;
    minimo = min();
    //std::cout << "Mínimo: " << minimo << std::endl;
    borrar(minimo);
  }
}

template<class Significado>
void diccString<Significado>::definir(const string& clave, const Significado& significado) {
    assert(!def(clave) && clave.length() > 0);

    tupString<Significado> entrada = tupString<Significado>(clave, significado);
    class Lista<tupString<Significado> >::Iterador it = valores.AgregarAdelante(entrada);
    Trie* actual = &(this->nodoTrie);
    for (unsigned int i = 0; (unsigned int) i < clave.length(); i++) {
       if ((actual->hijos)[(unsigned char) clave[i]] == NULL) {
          (actual->hijos)[(unsigned char) clave[i]] = new Trie();
        }
        actual->cantHijos++;
        actual = (actual->hijos)[(unsigned char) clave[i]];
    }
    class Lista<tupString<Significado> >::Iterador* it_ptr = new class Lista<tupString<Significado> >::Iterador(it);
    actual->valor = it_ptr;
}

template<class Significado>
bool diccString<Significado>::def(const string& clave) const {
    const Trie* actual = &(this->nodoTrie);
    bool res = true;
    unsigned int i = 0;

    while (i < clave.length() && res) {
        if (actual->cantHijos > 0) {
            if (actual->hijos[(unsigned char) clave[i]] == NULL) {
                res = false;
            } else {
                actual = (actual->hijos[(unsigned char) clave[i]]);
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
Significado& diccString<Significado>::obtener(const string& clave) const {
    assert(def(clave) && clave.length() > 0);

    const Trie* actual = &(this->nodoTrie);
    for (unsigned int i = 0; i < clave.length(); i++) {
        actual = (actual->hijos)[(unsigned char) clave[i]];
    }
    Significado& res = actual->valor->Siguiente().significado;
    return res;
}

template<class Significado>
const Conj<string> diccString<Significado>::claves() const {
    Conj<string> res;
    class Lista<class tupString<Significado> >::const_Iterador it = valores.CrearIt();
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
    Trie* temp = actual->hijos[(unsigned char) clave[0]];
    actual->cantHijos--;

    bool borrar = false;
    for (unsigned int i = 0; i < clave.length(); i++) {
        if (i + 1 != clave.length() ){
         temp->cantHijos--;
        }
        if ( i+1 == clave.length() ) {
          temp->valor->EliminarSiguiente();
          delete temp->valor;
          temp->valor = NULL;
        }
        if ( !borrar && temp->cantHijos == 0 && temp->valor == NULL) {
          actual->hijos[(unsigned char) clave[i]] = NULL;
          borrar = true;
        }
        actual = temp;
        temp = actual->hijos[(unsigned char) clave[i+1]];
        if ( borrar ){
          delete actual;
          actual = NULL;
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

template<class Significado>
diccString<Significado>& diccString<Significado>::operator=(const diccString<Significado>& other) {
    class Lista< tupString<Significado> >::const_Iterador it = other.valores.CrearIt();
    while (it.HaySiguiente()) {
        definir(it.Siguiente().clave, it.Siguiente().significado);
        it.Avanzar();
    }
    return *this;
}

template<class Significado>
bool diccString<Significado>::operator==(const diccString<Significado>& other) const {
    bool res = true;
    class Lista< tupString<Significado> >::const_Iterador it = other.valores.CrearIt();
    while ( it.HaySiguiente() && res ) {
      string k = it.Siguiente().clave;
      Significado s = it.Siguiente().significado;
      if ( def(k) ) {
        res = ( obtener(k) == s);
      } else {
        res = false;
      }
      it.Avanzar();
    }
    it = valores.CrearIt();
    while ( it.HaySiguiente() && res ) {
      string k = it.Siguiente().clave;
      Significado s = it.Siguiente().significado;
      if ( other.def(k) ) {
        res = ( other.obtener(k) == s );
      } else {
        res = false;
      }
      it.Avanzar();
    }
    return res;
}

template<class Significado>
bool diccString<Significado>::operator!=(const diccString<Significado>& other) const {
    return !(*this == other);
}

template<class Significado>
diccString<Significado>::diccString(const diccString<Significado>& other){
    *this = other;
}

}
#endif
