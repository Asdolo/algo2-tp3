#ifndef DICCNAT_H_INCLUDED
#define DICCNAT_H_INCLUDED

#include "aed2.h"
#include "Dato.h"
#include <cassert>
#include <string>
#include <iostream>

using namespace std;

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
        unsigned int nat;
        Significado* significado;
        nodoDiccNat* izq;
        nodoDiccNat* der;
      };
      nodoDiccNat(unsigned int k, Significado* s) : nat(k), signficado(s), izq(NULL), der(NULL) {};
      nodoDiccNat* estr;
};

diccNat() : estr(NULL);
