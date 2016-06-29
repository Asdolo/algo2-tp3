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
  unsigned int clave;
  const Significado& significado;
};

template<class Significado>
class diccNat{
    public:
      class Iterador;
      diccNat();
      template<class S>
      friend ostream& operator<<(ostream& os, const diccNat<S>& d);

      void definir(unsigned int clave, const Significado& significado);
      bool def(unsigned int clave) const;
      const Significado& obtener(unsigned int clave) const;
      void borrar(unsigned int clave);
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
        nodoDiccNat(unsigned int k, const Significado* s)
        : clave(k), significado(s), izq(NULL), der(NULL) {};

        unsigned int clave;
        const Significado* significado;
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
void diccNat<Significado>::definir(unsigned int clave, const Significado& significado){
  assert(!def(clave));
	nodoDiccNat* diccAux = this->estr;
	bool termine = false;
	while (!termine) {
		if (this->estr == NULL) {
			this->estr = new nodoDiccNat(clave, &significado);
      termine = true;
		} else {
			if (diccAux->clave >clave) {
				if (diccAux->izq != NULL) {
					diccAux = (diccAux->izq);
				} else {
          std::cout << "padre: " << diccAux->clave << std::endl;
          std::cout << "hijo izq: " << clave << std::endl;
					(diccAux->izq) = new nodoDiccNat(clave, &significado);
					termine = true;
				}
			} else {
				if (diccAux->der != NULL) {
					diccAux = (diccAux->der);
				} else {
          std::cout << "padre: " << diccAux->clave << std::endl;
          std::cout << "hijo der: " << clave << std::endl;
					(diccAux->der) = new nodoDiccNat(clave, &significado);
					termine = true;
				}
			}
		}
	}
}

template<class Significado>
bool diccNat<Significado>::def(unsigned int clave) const{
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
const Significado& diccNat<Significado>::obtener(unsigned int clave) const{
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
  const Significado& res = *(diccAux->significado);
	return res;
}

template<class Significado>
void diccNat<Significado>::borrar(unsigned int clave){
  assert(def(clave));
  std::cout << endl << "Inicio de borrar... clave: " << clave << std::endl;
	nodoDiccNat* diccAux = this->estr;
	bool termine = false;
	nodoDiccNat* padre = NULL;
	while (!termine) {
    std::cout << "Inside while" << std::endl;
		if (clave < diccAux->clave) { //ACÁ FLASHEAMOS EN EL DISEÑO
			padre = diccAux;
			diccAux = diccAux->izq;
		} else {
			if (diccAux->clave == clave) {
				termine = true;
        std::cout << "Encontró el padre" << std::endl;
			} else {
				padre = diccAux;
				diccAux = diccAux->der;
			}
		}
	}

  //SE AGREGARON LOS CASOS HEAD
	if (diccAux->izq == NULL && diccAux->der == NULL) {
    std::cout << "Caso hoja" << std::endl;
    if (estr == diccAux){
      estr = NULL;
    } else if (padre->izq == diccAux) {
			padre->izq = NULL;
		} else {
			padre->der = NULL;
		}
    delete diccAux;
	} else if (diccAux->izq == NULL && diccAux->der != NULL) {
    std::cout << "Caso hijo único der" << std::endl;
    if (estr == diccAux){
      estr = diccAux->der;
    } else if (padre->izq == diccAux) {
			padre->izq = diccAux->der;
		} else {
			padre->der = diccAux->der;
		}
    delete diccAux;
	} else if (diccAux->izq != NULL && diccAux->der == NULL) {
    std::cout << "Caso hijo único izq" << std::endl;
    if (estr == diccAux){
      estr = diccAux->izq;
    } else if (padre->izq == diccAux) {
			padre->izq = diccAux->izq;
		} else {
			padre->der = diccAux->izq;
		}
    delete diccAux;
	} else if (diccAux->izq != NULL && diccAux->der != NULL) {
    std::cout << "Caso dos hijos" << std::endl;
    diccNat<Significado> droga;
    droga.estr = diccAux->der;
		tuplaDicc<Significado> temp = droga.min();
		this->borrar(temp.clave);
		diccAux->clave = temp.clave;
		diccAux->significado = &(temp.significado);
	}
  std::cout << "" << std::endl;
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
      it.iter_.apilar(prox->der);
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
      os << it.actual().clave;
      it.avanzar();
      if(it.hayMas()) os << ", ";
  }
  os << "}";
  return os;
}

#endif
