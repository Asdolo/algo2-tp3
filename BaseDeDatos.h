
#ifndef BASEDEDATOS_H_INCLUDED
#define BASEDEDATOS_H_INCLUDED


#include "tp3.h"
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

    Tabla dameTabla(string t) const;

    bool hayJoin(string t1, string t2) const;

    string campoJoin(string t1, string t2) const;

    Conj<Registro_tp3>::const_Iterador vistaJoin(string t1, string t2);

    string tablaMaxima() const;

    friend ostream& operator<<(ostream& os, const BaseDeDatos& b);

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
      Lista<tupInterna> cambiosT1;
      Lista<tupInterna> cambiosT2;
    };

    diccString<diccString<Conj<Registro_tp3 > > > _registrosDelJoin;
    string* _tablaMasAccedida;
    diccString<Tabla> _nombreATabla;
    Conj<string> _tablas;
    diccString<diccString<diccNat<Conj<Registro_tp3>::Iterador > > > _joinPorCampoNat;
    diccString<diccString<diccString<Conj<Registro_tp3>::Iterador > > > _joinPorCampoString;
    diccString<diccString<tupBdd > > _hayJoin;
};


bool operator==(const BaseDeDatos& b1, const BaseDeDatos& b2); //Se usa?

ostream& operator<<(ostream& os, const BaseDeDatos& b) {
    os << "----------------------------" << endl;
    os << "     " << "Tablas: " << endl;
    Conj<string>::const_Iterador itTab = b.Tablas();
    while (itTab.HaySiguiente()) {
        os << b.dameTabla(itTab.Siguiente());
        itTab.Avanzar();
    }
    os << endl;
    os << "----------------------------" << endl;


    os << "Tabla más accedida: " << b.tablaMaxima() << endl;
    os << "----------------------------" << endl;
    itTab = b.Tablas();
    while (itTab.HaySiguiente()) {
        Conj<string>::const_Iterador itTabSecund = b.Tablas();

        while (itTabSecund.HaySiguiente()) {
            if (b.hayJoin(itTab.Siguiente(), itTabSecund.Siguiente())) {
                os << "Join " << itTab.Siguiente() << " --> " << itTabSecund.Siguiente() << endl;
              Conj<Registro_tp3> registrosDelJoin = b._registrosDelJoin.obtener(itTab.Siguiente()).obtener(itTabSecund.Siguiente());
              Conj<Registro_tp3>::Iterador itReg= registrosDelJoin.CrearIt();
              while(itReg.HaySiguiente()){
                  os << itReg.Siguiente() << endl;
                  itReg.Avanzar();
              }

            }
            itTabSecund.Avanzar();
        }
        itTab.Avanzar();
    }
    os << "----------------------------" << endl;

    return os;
}

BaseDeDatos::BaseDeDatos() :
  _tablaMasAccedida   (NULL),
  _nombreATabla       (diccString<Tabla>()),
  _tablas             (Conj<string>()),
  _hayJoin            (diccString<diccString<tupBdd > >()),
  _joinPorCampoString (diccString<diccString<diccString<Conj<Registro_tp3>::Iterador > > >()),
  _joinPorCampoNat    (diccString<diccString<diccNat<Conj<Registro_tp3>::Iterador > > >()),
  _registrosDelJoin   (diccString<diccString<Conj<Registro_tp3 > > >())
{}

BaseDeDatos::~BaseDeDatos(){
  if (_tablaMasAccedida != NULL) delete _tablaMasAccedida;
}

void BaseDeDatos::agregarTabla(Tabla t) {
    assert(!_nombreATabla.def(t.nombre()));

    if (_tablaMasAccedida == NULL || _nombreATabla.obtener(*_tablaMasAccedida).cantidadDeAccesos() < t.cantidadDeAccesos()) {
        _tablaMasAccedida = new string(t.nombre());
      }
    _nombreATabla.definir(t.nombre(), t);
    _tablas.AgregarRapido(t.nombre());
    _hayJoin.definir(t.nombre(), diccString<tupBdd >());
    _joinPorCampoNat.definir(t.nombre(), diccString<diccNat<Conj<Registro_tp3>::Iterador > >());
    _joinPorCampoString.definir(t.nombre(), diccString<diccString<Conj<Registro_tp3>::Iterador > >());
    _registrosDelJoin.definir(t.nombre(), diccString<Conj<Registro_tp3 > >());
}

void BaseDeDatos::insertarEntrada(Registro_tp3 r, string s) {
    assert(_nombreATabla.def(s) && r.claves() == _nombreATabla.obtener(s).campos() );

    Tabla& tabla = _nombreATabla.obtener(s);
    tabla.agregarRegistro(r);
    Tabla tabMax = _nombreATabla.obtener(*_tablaMasAccedida);
    if (tabla.cantidadDeAccesos() > tabMax.cantidadDeAccesos()) {
        _tablaMasAccedida = &s;
    }
    class Lista<struct tupString<tupBdd > >::const_Iterador iter = _hayJoin.obtener(s).vistaDicc();

    while (iter.HaySiguiente()) {
        tupInterna tup;
        tup.reg = r;
        tup.agregar = true;
        //MOD
        _hayJoin.obtener(s).obtener(iter.Siguiente().clave).cambiosT1.AgregarAdelante(tup);
        //iter.Siguiente().significado.cambiosT1.AgregarAdelante(tup);
        iter.Avanzar();
    }
    Conj<string>::Iterador iterTablas =_tablas.CrearIt();
    while (iterTablas.HaySiguiente()) {
        if (_hayJoin.obtener(iterTablas.Siguiente()).def(s)) {
            tupBdd cambios = _hayJoin.obtener(iterTablas.Siguiente()).obtener(s);
            tupInterna tup;
            tup.reg = r;
            tup.agregar = true;
            cambios.cambiosT2.AgregarAtras(tup);
        }
        iterTablas.Avanzar();
    }
}

void BaseDeDatos::Borrar(Registro_tp3 cr, string t) {
    assert(_nombreATabla.def(t));

    Tabla& tabla = _nombreATabla.obtener(t);
    tabla.borrarRegistro(cr);
    Tabla tabMax = _nombreATabla.obtener(*_tablaMasAccedida);
    if (tabla.cantidadDeAccesos() > tabMax.cantidadDeAccesos()) {
        _tablaMasAccedida = &t;
    }
    class Lista<struct tupString<tupBdd > >::const_Iterador iter = _hayJoin.obtener(t).vistaDicc();
    while (iter.HaySiguiente()) {
        tupInterna tup;
        tup.reg = cr;
        tup.agregar = false;
        //MOD
        _hayJoin.obtener(t).obtener(iter.Siguiente().clave).cambiosT1.AgregarAdelante(tup);
        //iter.Siguiente().significado.cambiosT1.AgregarAdelante(tup);
        iter.Avanzar();
    }
    Conj<string>::Iterador iterTablas =_tablas.CrearIt();
    while (iterTablas.HaySiguiente()) {
        if (_hayJoin.obtener(iterTablas.Siguiente()).def(t)) {
            tupBdd cambios = _hayJoin.obtener(iterTablas.Siguiente()).obtener(t);
            tupInterna tup;
            tup.reg = cr;
            tup.agregar = false;
            cambios.cambiosT2.AgregarAtras(tup);
        }
        iterTablas.Avanzar();
    }
}

Conj<Registro_tp3> BaseDeDatos::combinarRegistros(string t1, string t2, string campo) const {
    assert( _nombreATabla.def(t1) && _nombreATabla.def(t2) );
    assert( _nombreATabla.obtener(t1).campos().Pertenece(campo) );
    assert( _nombreATabla.obtener(t2).campos().Pertenece(campo) );

    Tabla tabla1 = _nombreATabla.obtener(t1);
    Tabla tabla2 = _nombreATabla.obtener(t2);
    Conj<Registro_tp3>::const_Iterador it;

    Tabla tablaIt = (tabla1.indices().Pertenece(campo))? tabla2 : tabla1;
    Tabla tablaBusq = (tabla1.indices().Pertenece(campo))? tabla1 : tabla2;
    it = tablaIt.registros().CrearIt();

    Conj<Registro_tp3> res;

    while (it.HaySiguiente()) {
        Registro_tp3 regMergeado;
        Dato d = it.Siguiente().obtener(campo);
        Lista<Registro_tp3> coincis = tablaBusq.buscar(campo, d);
        if (!coincis.EsVacia()) {
            if (tablaBusq.nombre() == t1) {
                regMergeado = Merge(coincis.Primero(), it.Siguiente());
            } else {
                regMergeado = Merge(it.Siguiente(), coincis.Primero());
            }
        }
        Lista<struct tupString<Dato> >::const_Iterador ver_Vacio = regMergeado.vistaDicc();
        if (ver_Vacio.HaySiguiente()) res.AgregarRapido(regMergeado);
        it.Avanzar();
    }
    return res;
}

Conj<Registro_tp3>::const_Iterador BaseDeDatos::generarVistaJoin(string t1, string t2, string campo) {
  assert( _nombreATabla.def(t1) && _nombreATabla.def(t2) );
  assert( _nombreATabla.obtener(t1).claves().Pertenece(campo) );
  assert( _nombreATabla.obtener(t2).claves().Pertenece(campo) );

    tupBdd aux;
    aux.campoJoin = campo;
    aux.cambiosT1 = Lista<tupInterna>();
    aux.cambiosT2 = Lista<tupInterna>();
    _hayJoin.obtener(t1).definir(t2, aux);
    _registrosDelJoin.obtener(t1).definir(t2, Conj<Registro_tp3>());
    Tabla tabla1 = _nombreATabla.obtener(t1);
    Tabla tabla2 = _nombreATabla.obtener(t2);
    if (tabla1.tipoCampo(campo)) {
        _joinPorCampoNat.obtener(t1).definir(t2, diccNat < Conj<Registro_tp3>::Iterador>());

        Conj<Registro_tp3> regsMergeados = combinarRegistros(t1, t2, campo);
        Conj<Registro_tp3>::Iterador it = regsMergeados.CrearIt();
        while (it.HaySiguiente()) {
            Dato d = it.Siguiente().obtener(campo);
            Conj<Registro_tp3>::Iterador iter = _registrosDelJoin.obtener(t1).obtener(t2).AgregarRapido(it.Siguiente());
            unsigned int n = d.dame_valorNat();
            _joinPorCampoNat.obtener(t1).obtener(t2).definir(n, iter);
            it.Avanzar();
        }
    } else {
        _joinPorCampoString.obtener(t1).definir(t2, diccString < Conj<Registro_tp3>::Iterador>());
        Conj<Registro_tp3> regsMergeados = combinarRegistros(t1, t2, campo);
        Conj<Registro_tp3>::Iterador it = regsMergeados.CrearIt();
        while (it.HaySiguiente()) {
            Dato d = it.Siguiente().obtener(campo);
            Conj<Registro_tp3>::Iterador iter = _registrosDelJoin.obtener(t1).obtener(t2).AgregarRapido(it.Siguiente());
            string s = d.dame_valorStr();
            _joinPorCampoString.obtener(t1).obtener(t2).definir(s, iter);
            it.Avanzar();
        }
    }
    Conj<Registro_tp3>::const_Iterador res = _registrosDelJoin.obtener(t1).obtener(t2).CrearIt();
    return res;
}

void BaseDeDatos::BorrarJoin(string t1, string t2) {
    assert( hayJoin(t1,t2) );

    _hayJoin.obtener(t1).borrar(t2);
    _registrosDelJoin.obtener(t1).borrar(t2);
    if (_joinPorCampoNat.obtener(t1).def(t2)) {
        _joinPorCampoNat.obtener(t1).borrar(t2);
    } else {
        _joinPorCampoString.obtener(t1).borrar(t2);
    }

}

Conj<string>::const_Iterador BaseDeDatos::Tablas() const {
    return _tablas.CrearIt();
}

Tabla BaseDeDatos::dameTabla(string s) const {
    assert( _nombreATabla.def(s) );

    return _nombreATabla.obtener(s);
}

bool BaseDeDatos::hayJoin(string s1, string s2) const {
    assert (_nombreATabla.def(s1) && _nombreATabla.def(s2));

    return _hayJoin.obtener(s1).def(s2);
}

string BaseDeDatos::campoJoin(string s1, string s2) const {
    assert( hayJoin(s1,s2) );

    return _hayJoin.obtener(s1).obtener(s2).campoJoin;
}

Registro_tp3 BaseDeDatos::Merge(Registro_tp3 r1, Registro_tp3 r2) {
 Registro_tp3 res = r1;
 class Lista<struct tupString<Dato> >::const_Iterador ite = r2.vistaDicc();
 while (ite.HaySiguiente()) {
        if (!(r1.def(ite.Siguiente().clave))) {
            res.definir(ite.Siguiente().clave, ite.Siguiente().significado);
        }
        ite.Avanzar();
    }
    return res;
}

Conj<Registro_tp3>::const_Iterador BaseDeDatos::vistaJoin(string s1, string s2) {
    assert( hayJoin(s1,s2) );

    string campito = _hayJoin.obtener(s1).obtener(s2).campoJoin;
    Tabla tabla1 = _nombreATabla.obtener(s1);
    bool esNat = tabla1.tipoCampo(campito);
    Tabla tabla2 = _nombreATabla.obtener(s2);

    if ( esNat ) {
      diccNat<Conj<Registro_tp3>::Iterador > diccDeIters = _joinPorCampoNat.obtener(s1).obtener(s2);
      Lista<tupInterna>::Iterador itT1 = _hayJoin.obtener(s1).obtener(s2).cambiosT1.CrearIt();

      while ( itT1.HaySiguiente() ) {
        tupInterna tupSiguiente = itT1.Siguiente();
        Dato claveNat = tupSiguiente.reg.obtener(campito);
        Lista<Registro_tp3> coincidencias = tabla2.buscar(campito, claveNat);

        if ( !coincidencias.EsVacia() ) {
          Registro_tp3 regT2 = coincidencias.Primero();

          if ( tupSiguiente.agregar ) {
            Registro_tp3 registroMergeado = Merge(tupSiguiente.reg, regT2);
            Conj<Registro_tp3>::Iterador iter = _registrosDelJoin.obtener(s1).obtener(s2).AgregarRapido(registroMergeado);
            diccDeIters.definir(claveNat.dame_valorNat(), iter);
          } else {
            diccDeIters.obtener(claveNat.dame_valorNat()).EliminarSiguiente();
            diccDeIters.borrar(claveNat.dame_valorNat());
          }
        }

        itT1.EliminarSiguiente();
      }

      Lista<tupInterna>::Iterador itT2 = _hayJoin.obtener(s1).obtener(s2).cambiosT2.CrearIt();

      while ( itT2.HaySiguiente() ) {
        tupInterna tupSiguiente = itT2.Siguiente();
        Dato claveNat = tupSiguiente.reg.obtener(campito);
        Lista<Registro_tp3> coincidencias = tabla1.buscar(campito, claveNat);

        if ( !coincidencias.EsVacia() && !diccDeIters.def(claveNat.dame_valorNat()) ) {
          Registro_tp3 regT1 = coincidencias.Primero();

          if ( tupSiguiente.agregar ) {
            Registro_tp3 registroMergeado = Merge(regT1, tupSiguiente.reg);
            Conj<Registro_tp3>::Iterador iter = _registrosDelJoin.obtener(s1).obtener(s2).AgregarRapido(registroMergeado);
            diccDeIters.definir(claveNat.dame_valorNat(), iter);
          } else {
            diccDeIters.obtener(claveNat.dame_valorNat()).EliminarSiguiente();
            diccDeIters.borrar(claveNat.dame_valorNat());
          }
        }
        itT2.EliminarSiguiente();
      }

    } else {

      diccString<Conj<Registro_tp3>::Iterador > diccDeIters = _joinPorCampoString.obtener(s1).obtener(s2);
      Lista<tupInterna>::Iterador itT1 = _hayJoin.obtener(s1).obtener(s2).cambiosT1.CrearIt();

      while ( itT1.HaySiguiente() ) {
        tupInterna tupSiguiente = itT1.Siguiente();
        Dato claveString = tupSiguiente.reg.obtener(campito);
        Lista<Registro_tp3> coincidencias = tabla2.buscar(campito, claveString);

        if ( !coincidencias.EsVacia() ) {
          Registro_tp3 regT2 = coincidencias.Primero();

          if ( tupSiguiente.agregar ) {
            Registro_tp3 registroMergeado = Merge(tupSiguiente.reg, regT2);
            Conj<Registro_tp3>::Iterador iter = _registrosDelJoin.obtener(s1).obtener(s2).AgregarRapido(registroMergeado);
            diccDeIters.definir(claveString.dame_valorStr(), iter);
          } else {
            diccDeIters.obtener(claveString.dame_valorStr()).EliminarSiguiente();
            diccDeIters.borrar(claveString.dame_valorStr());
          }
        }

        itT1.EliminarSiguiente();
      }

      Lista<tupInterna>::Iterador itT2 = _hayJoin.obtener(s1).obtener(s2).cambiosT2.CrearIt();

      while ( itT2.HaySiguiente() ) {
        tupInterna tupSiguiente = itT2.Siguiente();
        Dato claveString = tupSiguiente.reg.obtener(campito);
        Lista<Registro_tp3> coincidencias = tabla1.buscar(campito, claveString);

        if ( !coincidencias.EsVacia() && !diccDeIters.def(claveString.dame_valorStr()) ) {
          Registro_tp3 regT1 = coincidencias.Primero();

          if ( tupSiguiente.agregar ) {
            Registro_tp3 registroMergeado = Merge(regT1, tupSiguiente.reg);
            Conj<Registro_tp3>::Iterador iter = _registrosDelJoin.obtener(s1).obtener(s2).AgregarRapido(registroMergeado);
            diccDeIters.definir(claveString.dame_valorStr(), iter);
          } else {
            diccDeIters.obtener(claveString.dame_valorStr()).EliminarSiguiente();
            diccDeIters.borrar(claveString.dame_valorStr());
          }
        }
        itT2.EliminarSiguiente();
      }
    }
    Conj<Registro_tp3>::const_Iterador res = _registrosDelJoin.obtener(s1).obtener(s2).CrearIt();
    return res;
}

Conj<Registro_tp3> BaseDeDatos::busquedaCriterio(Registro_tp3 crit, string t) const {
    assert( _nombreATabla.def(t) );

    Tabla tabla = _nombreATabla.obtener(t);
    bool termine = false;
    Conj<Registro_tp3> res;
    class Lista<struct tupString<Dato> >::const_Iterador itCrit = crit.vistaDicc();
    while (itCrit.HaySiguiente() && !termine) {
        string campoCrit = itCrit.Siguiente().clave;
        if (! tabla.campos().Pertenece(campoCrit) ) {
            termine = true;
        }
        itCrit.Avanzar();
    }
    Conj<string>::Iterador itIndices = tabla.indices().CrearIt();
    while (itIndices.HaySiguiente() && !termine) {
        if (crit.def(itIndices.Siguiente())) {
            string campoIndice = itIndices.Siguiente();
            Dato valorCampo = crit.obtener(campoIndice);
            Lista<Registro_tp3> coincis = tabla.buscar(campoIndice,valorCampo);
            Lista<Registro_tp3>::const_Iterador itCoincis = coincis.CrearIt();
            while (itCoincis.HaySiguiente()) {
                if (coincidenTodosCrit(itCoincis.Siguiente(),crit)) {
                    res.AgregarRapido(itCoincis.Siguiente());
                }
                itCoincis.Avanzar();
            }
            termine = true;
            itIndices.Avanzar();
        }
    }

    if (!termine) {
        Conj<Registro_tp3>::const_Iterador itRegs =  tabla.registros().CrearIt();
        while (itRegs.HaySiguiente()) {
            if (coincidenTodosCrit(itRegs.Siguiente(),crit)){
                res.AgregarRapido(itRegs.Siguiente());

            }
            itRegs.Avanzar();
        }
    }

    return res;
}

bool BaseDeDatos::coincidenTodosCrit(Registro_tp3 crit, Registro_tp3 r) {
    class Lista<struct tupString<Dato> >::const_Iterador itCrit =crit.vistaDicc();
    bool res = true;
    while(itCrit.HaySiguiente() && res){
        tupString<Dato> tuplaCrit = itCrit.Siguiente();
        if( r.obtener(tuplaCrit.clave) != tuplaCrit.significado){
            res=false;
        }
        itCrit.Avanzar();
    }
    return res;
}

string BaseDeDatos::tablaMaxima() const {
    assert (_tablaMasAccedida != NULL);
    return (*_tablaMasAccedida);
}

}
#endif
