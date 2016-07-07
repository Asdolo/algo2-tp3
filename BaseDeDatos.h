
#ifndef BASEDEDATOS_H_INCLUDED
#define BASEDEDATOS_H_INCLUDED

#include "Tabla.h"
#include <cassert>
#include <string>
#include <iostream>


using namespace aed2;
using namespace std;

namespace tp3{

class BaseDeDatos {
public:

    BaseDeDatos();

    ~BaseDeDatos();

    void agregarTabla(Tabla t);

    void insertarEntrada(Registro_tp3 r, string s);

    void Borrar(Registro_tp3 cr, string t);

    Conj<Registro_tp3>::const_Iterador generarVistaJoin(string t1, string t2, string campo);

    void BorrarJoin(string t1, string t2);

    Conj<string>::const_Iterador Tablas() const;

    Tabla& dameTabla(string t) const;

    bool hayJoin(string t1, string t2) const;

    string campoJoin(string t1, string t2) const;

    Conj<Registro_tp3>::const_Iterador vistaJoin(string t1, string t2);

    string tablaMaxima() const;

    Conj<Registro_tp3> busquedaCriterio(Registro_tp3 cr, string t) const;



private:

    Conj<Registro_tp3> combinarRegistros(string t1, string t2, string campo) const;

    static Registro_tp3 Merge(Registro_tp3 r1, Registro_tp3 r2);

    static bool coincidenTodosCrit(Registro_tp3 crit, Registro_tp3 r);


    struct tupInterna {
      Registro_tp3 reg;
      bool agregar;
    };

    struct tupBdd {
      string campoJoin;
      Lista<tupInterna> cambiosT1;  //Buffer con cambios realizados en T1 desde que se llamó a generarVistaJoin/vistaJoin por última vez
      Lista<tupInterna> cambiosT2;  //Idem para T2
    };

    string* _tablaMasAccedida;

    diccString<Tabla> _nombreATabla;

    Conj<string> _tablas;

    diccString<diccString<diccNat<Conj<Registro_tp3>::Iterador > > > _joinPorCampoNat;
    diccString<diccString<diccString<Conj<Registro_tp3>::Iterador > > > _joinPorCampoString;
    // Se accede por (Tabla 1 -> Tabla 2 -> Diccionario indexado sobre el campo del join) a iteradores sobre _registrosDelJoin

    diccString<diccString<tupBdd > > _hayJoin;

    diccString<diccString<Conj<Registro_tp3 > > > _registrosDelJoin;
    // Conjunto de registros del join T1 -> T2

};

};
#endif
