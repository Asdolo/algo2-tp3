#ifndef DICCNAT_H_INCLUDED
#define DICCNAT_H_INCLUDED

#include "aed2.h"
#include "Dato.h"
#include "Pila.h"
#include <cassert>
#include <string>
#include <iostream>

using namespace std;

template<class Significado>
struct tuplaDicc{
  unsigned int& clave;
  const Significado& significado;
};

template<class Significado>
class diccNat{
    public:
      diccNat();

      void definir(unsigned int clave, const Significado& significado);
      bool def(unsigned int clave) const;
      const Significado& obtener(unsigned int clave) const;
      void borrar(unsigned int clave);
      tuplaDicc<Significado> min() const;
      tuplaDicc<Significado> max() const;
      Iterador crearIt();

      class Iterador{
        public:
          Lista<tuplaDicc<Significado>> siguientes() const;
          Iterador avanzar();
          bool hayMas() const;
          tuplaDicc<Significado> actual() const;

        private:
          Pila<nodoDiccNat*> iter;

          Iterador();
          Iterador(diccNat<Significado>* dicc);
          friend class diccNat<Significado>::Iterador diccNat<Significado>::crearIt();
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

/*  ***********************************
    DiccNat
    ***********************************
*/

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
			//TODO CHEQUEAR CASO HEAD
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
		tuplaDicc<Significado> temp = diccAux->der->min();
		this->borrar(temp.clave);
		diccAux->clave = temp.clave;
		diccAux->significado = &(temp.significado);
	}
}

template<class Significado>
tuplaDicc<Significado> diccNat<Significado>::min() const{
	nodoDiccNat<Significado>* diccAux = this->estr;
	while (diccAux->izq != NULL) {
		diccAux = (diccAux->izq);
	}
	tuplaDicc<Significado> res = {diccAux->clave, *(diccAux->significado)};
}

template<class Significado>
tuplaDicc<Significado> diccNat<Significado>::max() const{
	nodoDiccNat<Significado>* diccAux = this->estr;
	while (diccAux->der != NULL) {
		diccAux = (diccAux->der);
	}
	tuplaDicc<Significado> res = {diccAux->clave, *(diccAux->significado)};
}


/*  ***********************************
    Iterador
    ***********************************
*/

template<class Significado>
class diccNat<Significado>::Iterador diccNat<Significado>::crearIt(){
  return Iterador(this);
}

template<class Significado>
Lista<tuplaDicc<Significado>> diccNat<Significado>::Iterador::siguientes(){
  Lista<tuplaDicc<Significado>> res;
  Iterador it;
  it.iter = this->iter;
  while (! it.iter.esVacia(){
    nodoDiccNat* prox = it.iter.desapilar();
    Significado copia = prox->significado;
    res.AgregarAtras({prox->clave, *copia});
    if (prox->der != NULL) {
      it.iter.apilar(prox->der);
    }
    if (prox->izq != NULL) {
      it.iter.apilar(prox->der);
    }
  }
  return res;
}

template<class Significado>
void diccNat<Significado>::Iterador::avanzar(){
  nodoDiccNat* prox = iter.desapilar();
  if (prox->der != NULL){
    iter.apilar(prox->der);
  }
  if (prox->izq != NULL){
    iter.apilar(prox->izq);
  }
}

template<class Significado>
bool diccNat<Significado>::Iterador::hayMas(){
  return !iter.esVacia();
}

template<class Significado>
tuplaDicc<Significado> diccNat<Significado>::Iterador::actual(){
  return { iter.tope()->clave, *(iter.tope()->significado) };
}

template<class Significado>
diccNat<Significado>::Iterador::Iterador() : iter(Pila()) {}

template<class Significado>
diccNat<Significado>::Iterador::Iterador(diccNat<Significado>* dicc){
  this->iter = Pila();
  if (dicc != NULL) {
    (this->iter).Apilar()
  }
}

#endif
