#ifndef DICCNAT_H_INCLUDED
#define DICCNAT_H_INCLUDED

#include "aed2.h"
#include "Dato.h"
#include <cassert>
#include <string>
#include <iostream>

using namespace std;

//TODO, ESTO LLEVA TEMPLATE CASES? (<>)
struct k_S{
  unsigned int& clave;
  const Significado& significado;
} tuplaDicc;

template<class Significado>
class diccNat{
    public:
      diccNat();

      void definir(unsigned int clave, const Significado& significado);
      bool def(unsigned int clave) const;
      const Significado& obtener(unsigned int clave) const;
      void borrar(unsigned int clave);
      tuplaDicc min() const;
      tuplaDicc max() const;

      class Iterador{
        public:
          Iterador crearIt();
          Lista<tuplaDicc> siguientes() const;
          Iterador avanzar();
          bool hayMas() const;
          tuplaDicc actual() const;

        private:
          pila iter;
      };

    private:
      struct nodoDiccNat{
        unsigned int clave;
        Significado* significado;
        nodoDiccNat* izq;
        nodoDiccNat* der;
      };
      nodoDiccNat(unsigned int k, Significado* s) 
      	: clave(k), signficado(s), izq(NULL), der(NULL) {};
      nodoDiccNat* estr;
};

template<class Significado>
diccNat<Significado>::diccNat()
 : estr(NULL);

template<class Significado>
void diccNat<Significado>::definir(unsigned int clave, const Significado& significado){
	nodoDiccNat<Significado>* diccAux = this->estr;
	bool termine = false;
	while (!termine) {
		if (diccAux == NULL) {
			diccAux = new nodoDiccNat(n, &s);
		} else {
			if (diccAux->clave > n) {
				if (diccAux->izq != NULL) {
					diccAux = (diccAux->izq);
				} else {
					(diccAux->izq) = new nodoDiccNat(n, &s); 
					termine = true;
				}
			} else {
				if (diccAux->der != NULL) {
					diccAux = (diccAux->der);
				} else {
					(diccAux->der) = new nodoDiccNat(n, &s);
					termine = true;
				}
			}
		}
	}
}

template<class Significado>
bool diccNat<Significado>::def(unsigned int clave) const{
	nodoDiccNat<Significado>* diccAux = this->estr;
	bool termine = false;
	while (!termine){
		if (diccAux == NULL) {
			bool res = false;
			termine = true;
		} else {
			if (diccAux->clave == n) {
				termine = true;
				res = true;
			} else {
				if (diccAux->clave > n) {
					diccAux = (diccAux->izq);
				} else {
					diccAux = (diccAux->der);
				}
			}
		}
	}
	return res;
}

template<class Significado>
const Significado& diccNat<Significado>::obtener(unsigned int clave) const{
	nodoDiccNat<Significado>* diccAux = this->estr;
	bool termine = false;
	while (!termine) {
		if (diccAux->clave > n) {
			diccAux = diccAux->izq;
		} else {
			if (diccAux->clave == n) {
				res = diccAux->significado;
				termine = true;
			} else {
				diccAux = (diccAux->der);
			}
		}
	}
	return res;
}

template<class Significado>
void diccNat<Significado>::borrar(unsigned int clave){
	nodoDiccNat<Significado>* diccAux = this->estr;
	bool termine = false;
	nodoDiccNat<Significado>* padre = NULL;
	while (!termine) {
		if (diccAux->clave < n) {
			padre = diccAux;
			diccAux = diccAux->izq;
		} else {
			if (diccAux->clave == n) {
				termine = true;
			} else {
				padre = diccAux;
				diccAux = diccAux->der;
			}
		}
	}
	if (diccAux->izq == NULL && diccAux->der == NULL) {
		if (padre->izq == diccAux) {
			delete diccAux;
			padre->izq = NULL;
		} else {
			//TODO CHEQUEAR LOS DELETES
			delete diccAux;
			padre->der = NULL;
		}
	} else if (diccAux->izq == NULL && diccAux->der != NULL) {
		if (padre->izq == diccAux) {
			delete diccAux;
			padre->izq = diccAux->der;
		} else {
			delete diccAux;
			padre->der = diccAux->der;
		}
	} else if (diccAux->izq != NULL && diccAux->der == NULL) {
		if (padre->izq == diccAux) {
			delete diccAux;
			padre->izq = diccAux->izq;
		} else {
			delete diccAux;
			padre->der = diccAux->izq;
		}
	} else if (diccAux->izq != NULL && diccAux->der != NULL) {
		//TODO CHEQUEAR SI ESTE CASTEO VILLERO SE PUEDE PASAR A CONSTRUCTOR
		tuplaDicc temp = diccAux->der->min();
		this->borrar(temp.clave); 
		diccAux->clave = temp.clave;
		diccAux->significado = &(temp.significado);
	}
}

template<class Significado>
tuplaDicc diccNat<Significado>::min() const{
	nodoDiccNat<Significado>* diccAux = this->estr;
	while (diccAux->izq != NULL) {
		diccAux = (diccAux->izq);
	}
	tuplaDicc res = {diccAux->clave, *(diccAux->significado)}; 
}

template<class Significado>
tuplaDicc diccNat<Significado>::max() const{
	nodoDiccNat<Significado>* diccAux = this->estr;
	while (diccAux->der != NULL) {
		diccAux = (diccAux->der);
	}
	tuplaDicc res = {diccAux->clave, *(diccAux->significado)}; 
}