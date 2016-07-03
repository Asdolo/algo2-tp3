
#ifndef BASEDEDATOS_H_INCLUDED
#define BASEDEDATOS_H_INCLUDED


#include "aed2.h"
#include <cassert>
#include <string>
#include <iostream>
#include "Dato.h"
#include "diccString.h"
#include "Tabla.h"

#define Registro diccString<Dato>

using namespace std;

class BaseDeDatos {
public:

    BaseDeDatos();

    void agregarTabla(Tabla t);

    void insertarEntrada(Registro r, string s);

    void Borrar(Registro cr, string t);

    Conj<Registro> combinarRegistros(string t1, string t2, string campo) const;

    Conj<Registro>::const_Iterador generarVistaJoin(string t1, string t2, string campo);

    void BorrarJoin(string t1, string t2);

    Conj<string>::const_Iterador Tablas() const;

    Tabla dameTabla(string t) const;

    bool hayJoin(string t1, string t2) const;

    string campoJoin(string t1, string t2) const;

    Conj<Registro>::const_Iterador vistaJoin(string t1, string t2);

    string tablaMaxima() const;

private:

    static Registro Merge(Registro r1, Registro r2);

    static bool coincidenTodosCrit(Registro crit, Registro r);

    Conj<Registro> busquedaCriterio(Registro cr, string t) const;

    struct tupInterna {
      Registro reg;
      bool agregar;
    };

    struct tupBdd {
      string campoJoin;
      Lista<tupInterna> cambiosT1;
      Lista<tupInterna> cambiosT2;
    };

    string* _tablaMasAccedida;
    diccString<Tabla> _nombreATabla;
    Conj<string> _tablas;
    diccString<diccString<diccNat<Conj<Registro>::Iterador > > > _joinPorCampoNat;
    diccString<diccString<diccString<Conj<Registro>::Iterador > > > _joinPorCampoString;
    diccString<diccString<Conj<Registro > > > _registrosDelJoin;
    diccString<diccString<tupBdd > > _hayJoin;
};


bool operator==(const BaseDeDatos& b1, const BaseDeDatos& b2); //Se usa?

template<class T>
std::ostream& operator<<(std::ostream& os, const BaseDeDatos&); //Por qué el template?

BaseDeDatos::BaseDeDatos() :
  _tablaMasAccedida(NULL),
  _nombreATabla(diccString<Tabla>()),
  _tablas(Conj<string>()),
  _hayJoin(diccString<diccString<tupBdd > >()),
  _joinPorCampoString(diccString<diccString<diccString<Conj<Registro>::Iterador > > >()),
  _joinPorCampoNat(diccString<diccString<diccNat<Conj<Registro>::Iterador > > >()),
  _registrosDelJoin(diccString<diccString<Conj<Registro > > >())
{}

void BaseDeDatos::agregarTabla(Tabla t) {
    if (_tablaMasAccedida == NULL || _nombreATabla.obtener(*_tablaMasAccedida).cantidadDeAccesos() < t.cantidadDeAccesos()) {
        _tablaMasAccedida = new string(t.nombre());
    }
    _nombreATabla.definir(t.nombre(), t);
    _tablas.AgregarRapido(t.nombre());
    _hayJoin.definir(t.nombre(), diccString<tupBdd >());
    _joinPorCampoNat.definir(t.nombre(), diccString<diccNat<Conj<Registro>::Iterador > >());
    _joinPorCampoString.definir(t.nombre(), diccString<diccString<Conj<Registro>::Iterador > >());
    _registrosDelJoin.definir(t.nombre(), diccString<Conj<Registro > >());
}

void BaseDeDatos::insertarEntrada(Registro r, string s) {
    Tabla tabla = _nombreATabla.obtener(s);
    tabla.agregarRegistro(r);
    Tabla tabMax = _nombreATabla.obtener(*_tablaMasAccedida);
    if (tabla.cantidadDeAccesos() > tabMax.cantidadDeAccesos()) {
        _tablaMasAccedida = &s;
    }
    class Lista<struct tupString<tupBdd > >::const_Iterador iter = _hayJoin.obtener(s).vistaDicc(); //PROBLEMAAAAAAAAAAAA

    while (iter.HaySiguiente()) {
        tupInterna tup;
        tup.reg = r;
        tup.agregar = true;
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

void BaseDeDatos::Borrar(Registro cr, string t) {
    Tabla tabla = _nombreATabla.obtener(t);
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

Conj<Registro> BaseDeDatos::combinarRegistros(string t1, string t2, string campo) const {
    Tabla tabla1 = _nombreATabla.obtener(t1);
    Tabla tabla2 = _nombreATabla.obtener(t2);
    Conj<Registro>::const_Iterador it;
    Tabla tablaBusq = tabla1;
    if (tabla1.indices().Pertenece(campo)) {
        Tabla tablaIt = tabla2;
        it = tablaIt.registros().CrearIt();
    } else {
        Tabla tablaIt = tabla1;
        it = tablaIt.registros().CrearIt();
        tablaBusq = tabla2;
    }
    Conj<Registro> res;
    Registro regMergeado;
    while (it.HaySiguiente()) {
        Dato d = it.Siguiente().obtener(campo);
        Lista<Registro> coincis = tablaBusq.buscar(campo, d);
        if (!coincis.EsVacia()) {
            if (tablaBusq.nombre() == t1) {
                regMergeado = Merge(coincis.Primero(), it.Siguiente());

            } else {
                regMergeado = Merge(it.Siguiente(), coincis.Primero());
            }
        }
        res.AgregarRapido(regMergeado);
        it.Avanzar();
    }
    return res;
}

Conj<Registro>::const_Iterador BaseDeDatos::generarVistaJoin(string t1, string t2, string campo) {
    tupBdd aux;
    aux.campoJoin = campo;
    aux.cambiosT1 = Lista<tupInterna>();
    aux.cambiosT2 = Lista<tupInterna>();
    _hayJoin.obtener(t1).definir(t2, aux);
    _registrosDelJoin.obtener(t1).definir(t2, Conj<Registro>());
    Tabla tabla1 = _nombreATabla.obtener(t1);
    Tabla tabla2 = _nombreATabla.obtener(t2);
    if (tabla1.tipoCampo(campo)) {
        _joinPorCampoNat.obtener(t1).definir(t2, diccNat < Conj<Registro>::Iterador>());

        Conj<Registro> regsMergeados = combinarRegistros(t1, t2, campo);
        Conj<Registro>::Iterador it = regsMergeados.CrearIt();
        while (it.HaySiguiente()) {
            Dato d = it.Siguiente().obtener(campo);
            Conj<Registro>::Iterador iter = _registrosDelJoin.obtener(t1).obtener(t2).AgregarRapido(it.Siguiente());
            unsigned int n = d.dame_valorNat();
            _joinPorCampoNat.obtener(t1).obtener(t2).definir(n, iter);
            it.Avanzar();
        }
    } else {
        _joinPorCampoString.obtener(t1).definir(t2, diccString < Conj<Registro>::Iterador>());
        Conj<Registro> regsMergeados = combinarRegistros(t1, t2, campo);
        Conj<Registro>::Iterador it = regsMergeados.CrearIt();
        while (it.HaySiguiente()) {
            Dato d = it.Siguiente().obtener(campo);
            Conj<Registro>::Iterador iter = _registrosDelJoin.obtener(t1).obtener(t2).AgregarRapido(it.Siguiente());
            string s = d.dame_valorStr();
            _joinPorCampoString.obtener(t1).obtener(t2).definir(s, iter);
            it.Avanzar();
        }
    }

    Conj<Registro>::const_Iterador res = _registrosDelJoin.obtener(t1).obtener(t2).CrearIt();
    return res;
}

void BaseDeDatos::BorrarJoin(string t1, string t2) {
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
    return _nombreATabla.obtener(s);
}

bool BaseDeDatos::hayJoin(string s1, string s2) const {
    return _hayJoin.obtener(s1).def(s2);
}

string BaseDeDatos::campoJoin(string s1, string s2) const {
    return _hayJoin.obtener(s1).obtener(s2).campoJoin;
}

Registro BaseDeDatos::Merge(Registro r1, Registro r2) {
    Registro res = r1;
    class Lista<struct tupString<Dato> >::const_Iterador ite = r2.vistaDicc();
    while (ite.HaySiguiente()) {
        if (res.def(ite.Siguiente().clave)) {
            res.definir(ite.Siguiente().clave, ite.Siguiente().significado);
        }
        ite.Avanzar();
    }
    return res;
}

Conj<Registro>::const_Iterador BaseDeDatos::vistaJoin(string s1, string s2) {
    //COMPLETAR!!
}

Conj<Registro> BaseDeDatos::busquedaCriterio(Registro crit, string t) const {
    //COMPLETAR!!
}

bool BaseDeDatos::coincidenTodosCrit(Registro crit, Registro r) {
    //COMPLETAR!!
}

string BaseDeDatos::tablaMaxima() const {
    return (*_tablaMasAccedida);
}

#endif
