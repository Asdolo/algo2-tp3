#ifndef DICCNAT_H_INCLUDED
#define DICCNAT_H_INCLUDED

#include "Conj.h"
#include "Lista.h"
#include "Pila.h"
#include <cassert>
#include <string>
#include <iostream>

using namespace std;
using namespace aed2;

namespace tp3{

template<class Significado>
struct tuplaDicc{
  unsigned int clave;
  Significado significado;
};

//Diccionario(Nat, Significado) implementado en ABB, iterador DFS

template<class Significado>
class diccNat{

    public:

      class Iterador;

      diccNat();

      ~diccNat();

      template<class S>

      friend ostream& operator<<(ostream& os, const diccNat<S>& d);


      void definir(const unsigned int clave, const Significado& significado);

      bool def(const unsigned int clave) const;

      Significado& obtener(const unsigned int clave) const;

      void borrar(const unsigned int clave);

      tuplaDicc<Significado> min() const;

      tuplaDicc<Significado> max() const;

      Iterador crearIt() const;

      class Iterador{

        public:

          Lista<tuplaDicc<Significado> > siguientes() const;

          void avanzar();

          bool hayMas() const;

          tuplaDicc<Significado> actual() const;

        private:

          Pila<class diccNat<Significado>::nodoDiccNat*> iter_;

          Iterador();

          Iterador(const diccNat<Significado>* dicc);

          friend class diccNat<Significado>::Iterador diccNat<Significado>::crearIt() const;
      };

    private:

      struct nodoDiccNat{

        nodoDiccNat(unsigned int k, Significado* s)
            : clave(k), significado(s), izq(NULL), der(NULL) {};

        unsigned int clave;

        Significado* significado;

        nodoDiccNat* izq;

        nodoDiccNat* der;

      };

      nodoDiccNat* estr;
};

/*  ***********************************
    DiccNat
    ***********************************
*/

template<class Significado>
diccNat<Significado>::diccNat()
 : estr(NULL) {};

template<class Significado>
diccNat<Significado>::~diccNat(){
  int minimo;
  while ( estr != NULL ){
    minimo = min().clave;
    borrar(minimo);
  }
}

template<class Significado>
void diccNat<Significado>::definir(const unsigned int clave, const Significado& significado){
  assert(!def(clave));


	nodoDiccNat* diccAux = this->estr;
	bool termine = false;
  Significado* signi = new Significado(significado);
	while (!termine) {
		if (this->estr == NULL) {
			this->estr = new nodoDiccNat(clave, signi);
      termine = true;
		} else {
			if (diccAux->clave >clave) {
				if (diccAux->izq != NULL) {
					diccAux = (diccAux->izq);
				} else {
					(diccAux->izq) = new nodoDiccNat(clave, signi);
					termine = true;
				}
			} else {
				if (diccAux->der != NULL) {
					diccAux = (diccAux->der);
				} else {
					(diccAux->der) = new nodoDiccNat(clave, signi);
					termine = true;
				}
			}
		}
	}
}

template<class Significado>
bool diccNat<Significado>::def(const unsigned int clave) const{
	nodoDiccNat* diccAux = this->estr;
	bool termine = false;
  bool res;
	while (!termine){
		if (diccAux == NULL) {
			res = false;
			termine = true;
		} else {
			if (diccAux->clave ==clave) {
				termine = true;
				res = true;
			} else {
				if (diccAux->clave >clave) {
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
Significado& diccNat<Significado>::obtener(const unsigned int clave) const{
  assert(def(clave));

	nodoDiccNat* diccAux = this->estr;
	bool termine = false;
	while (!termine) {
		if (diccAux->clave >clave) {
			diccAux = diccAux->izq;
		} else {
			if (diccAux->clave == clave) {
				termine = true;
			} else {
				diccAux = (diccAux->der);
			}
		}
	}
  Significado& res = *(diccAux->significado);
	return res;
}

template<class Significado>
void diccNat<Significado>::borrar(const unsigned int clave){
  assert(def(clave));


	nodoDiccNat* diccAux = estr;
	bool termine = false;
	nodoDiccNat* padre = NULL;
	while (!termine) {
		if (clave < diccAux->clave) {
			padre = diccAux;
			diccAux = diccAux->izq;
		} else {
			if (diccAux->clave == clave) {
				termine = true;
			} else {
				padre = diccAux;
				diccAux = diccAux->der;
			}
		}
	}

  //SE AGREGARON LOS CASOS HEAD
	if (diccAux->izq == NULL && diccAux->der == NULL) {                           // HOJA/HEAD
    if (estr == diccAux){
      estr = NULL;
    } else if (padre->izq == diccAux) {
			padre->izq = NULL;
		} else {
			padre->der = NULL;
		}
    delete diccAux->significado;
    diccAux->significado = NULL;
    delete diccAux;
    diccAux = NULL;
	} else if (diccAux->izq == NULL && diccAux->der != NULL) {                    // HIJO UNICO IZQ
    if (estr == diccAux){
      estr = diccAux->der;
    } else if (padre->izq == diccAux) {
			padre->izq = diccAux->der;
		} else {
			padre->der = diccAux->der;
		}
    delete diccAux->significado;
    diccAux->significado = NULL;
    delete diccAux;
    diccAux = NULL;
	} else if (diccAux->izq != NULL && diccAux->der == NULL) {                    // HIJO UNICO DER
    if (estr == diccAux){
      estr = diccAux->izq;
    } else if (padre->izq == diccAux) {
			padre->izq = diccAux->izq;
		} else {
			padre->der = diccAux->izq;
		}
    delete diccAux->significado;
    diccAux->significado = NULL;
    delete diccAux;
    diccAux = NULL;
	} else if (diccAux->izq != NULL && diccAux->der != NULL) {                    // DOS HIJOS
    diccNat<Significado> dicc;         // Por discrepancias diseño/c++...
    dicc.estr = diccAux->der;
		tuplaDicc<Significado> temp = dicc.min();
		borrar(temp.clave);               // Por ser el mínimo, cae en caso hoja o hijo único derecho (no hace recursión)
		diccAux->clave = temp.clave;
		diccAux->significado = &(temp.significado);
	}
}

template<class Significado>
tuplaDicc<Significado> diccNat<Significado>::min() const{
  assert(this->estr != NULL);
	nodoDiccNat* diccAux = this->estr;
	while (diccAux->izq != NULL) {
		diccAux = (diccAux->izq);
	}
	tuplaDicc<Significado> res = {diccAux->clave, *(diccAux->significado)};
  return res;
}

template<class Significado>
tuplaDicc<Significado> diccNat<Significado>::max() const{
  assert(this->estr != NULL);
	nodoDiccNat* diccAux = this->estr;
	while (diccAux->der != NULL) {
		diccAux = (diccAux->der);
	}
	tuplaDicc<Significado> res = {diccAux->clave, *(diccAux->significado)};
  return res;
}


/*  ***********************************
    Iterador
    ***********************************
*/

template<class Significado>
class diccNat<Significado>::Iterador diccNat<Significado>::crearIt() const{
  return Iterador(this);
}

//  Apilamos por DFS punteros a nodos del diccionario (ver invRep del diseño)

template<class Significado>
Lista<tuplaDicc<Significado> > diccNat<Significado>::Iterador::siguientes() const{
  Lista<tuplaDicc<Significado> > res;
  Iterador it;
  it.iter_ = this->iter_;
  while (! it.iter_.esVacia()){
    nodoDiccNat* prox = it.iter_.desapilar();
    Significado copia = prox->significado;
    tuplaDicc<Significado> tuplaAg = {prox->clave, *copia};
    res.AgregarAtras(tuplaAg);
    if (prox->der != NULL) {
      it.iter_.apilar(prox->der);
    }
    if (prox->izq != NULL) {
      it.iter_.apilar(prox->izq);
    }
  }
  return res;
}

template<class Significado>
void diccNat<Significado>::Iterador::avanzar(){
  assert(! iter_.esVacia());
  nodoDiccNat* prox = iter_.desapilar();
  if (prox->der != NULL){
    iter_.apilar(prox->der);
  }
  if (prox->izq != NULL){
    iter_.apilar(prox->izq);
  }
}

template<class Significado>
bool diccNat<Significado>::Iterador::hayMas() const{
  return !iter_.esVacia();
}

template<class Significado>
tuplaDicc<Significado> diccNat<Significado>::Iterador::actual() const{
  assert(hayMas());
  tuplaDicc<Significado> res = { iter_.tope()->clave, *(iter_.tope()->significado) };
  return res;
}

template<class Significado>
diccNat<Significado>::Iterador::Iterador() : iter_(Pila<nodoDiccNat*>()) {}

template<class Significado>
diccNat<Significado>::Iterador::Iterador(const diccNat<Significado>* dicc){
  iter_ = Pila<nodoDiccNat*>();
  if (dicc->estr != NULL) {
    iter_.apilar(dicc->estr);
  }
}

template<class Significado>
ostream& operator<<(ostream& os, const diccNat<Significado>& d){
  typename diccNat<Significado>::Iterador it = d.crearIt();
  os << "{ ";
  while(it.hayMas()) {
      os << "(" << it.actual().clave << ", " << it.actual().significado << ")";
      it.avanzar();
      if(it.hayMas()) os << ", ";
  }
  os << " }" << endl;
  return os;
}

}
#endif
