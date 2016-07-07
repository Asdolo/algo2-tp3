#ifndef TABLA_H_INCLUDED
#define TABLA_H_INCLUDED

#include "Dato.h"
#include "diccString.h"
#include "diccNat.h"

typedef tp3::diccString<tp3::Dato> Registro_tp3;

#include <cassert>
#include <string>
#include <iostream>


using namespace std;
using namespace aed2;

namespace tp3{

class Tabla {
    public:

      Tabla(const string& nombre, Conj<string> claves,Registro_tp3 columnas);
      ~Tabla();

      void agregarRegistro(Registro_tp3 r);

      void borrarRegistro(Registro_tp3 cr);

      void indexar(const string& c);

      const string& nombre() const;

      Conj<string> claves() const;

      Lista<Registro_tp3> buscar(const string& campo, const Dato& d) const;

      Conj<string> indices() const;

      const Conj<string> campos() const;

      bool tipoCampo(const string& campo) const;

      const Conj<Registro_tp3>& registros() const;

      unsigned int cantidadDeAccesos() const;

      const Dato& minimo(string campo) const;

      const Dato& maximo(string campo) const;

      Tabla& operator=(const Tabla& t);

      Tabla(const Tabla& other);

    private:

      struct InfoIndice {
        string campo;
        Dato max;
        Dato min;
        bool vacio;
        InfoIndice(string c, Dato mx, Dato mn, bool v) : campo(c), max(mx), min(mn), vacio(v) {};
      };

      //Guardamos iteradores para cada registro con iteradores a todas las estructuras en la cual se insertó (así basta con acceder por cualquiera de las 3 para borrar todo rastro del registro)
      struct IteradoresIndices {
        IteradoresIndices(Conj<Registro_tp3>::Iterador it) : itReg(it), itNat(NULL), itString(NULL) {}
        Conj<Registro_tp3>::Iterador itReg;   //Apunta a _registros
        Lista<Lista<IteradoresIndices>::Iterador> ::Iterador* itNat; //Campo indexado nat -> apunta a _indicesNat, NULL de no haberlo
        Lista<Lista<IteradoresIndices>::Iterador> ::Iterador* itString; // Idem para string
      };


    Lista<IteradoresIndices> _iteradores;
    //Lista de tuplas descriptas anteriormente, proveen acceso centralizado a todas las estructuras relacionadas a registros

    diccString<bool> _campos;

    diccString<Lista<Lista<IteradoresIndices>::Iterador> > _indicesString;
    diccNat<Lista<Lista<IteradoresIndices>::Iterador> > _indicesNat;
    //Significados de indices son listas de iteradores con siguiente a _iteradores para agilizar acceso
    //Las claves corresponden a valores para el dato indexado

    string _nombre;

    Conj<string> _claves;

    Lista<InfoIndice> _campoIndexadoNat;
    Lista<InfoIndice> _campoIndexadoString;

    unsigned int _cantAccesos;

    Conj<Registro_tp3> _registros;

};

}
#endif
