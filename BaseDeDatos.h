
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

    /**
     * Crea una Base de datos.
     */
    BaseDeDatos();

    /**
     * Agrega una tabla a la base de datos.
     */
    agregarTabla(Tabla t);

    /**
     * Inserta un registro en una tabla
     */
    insertarEntrada(Registro r, string s);

    Borrar(Registro cr, string t);

    Conj<Registro> combinarRegistros(string t1, string t2, string campo);

    Conj<Registro>::Iterador generarVistaJoin(string t1, string t2, string campo);

    BorrarJoin(string t1, string t2);

    Conj<string>::Iterador Tablas();

    Tabla dameTabla(string t);

    bool hayJoin(string t1, string t2);

    string campoJoin(string t1, string t2);

    static Registro Merge(Registro r1, Registro r2);

    Conj<Registro>::Iterador vistaJoin(string t1, string t2);

    Conj<Registro> busquedaCriterio(Registro cr, string t);

    static bool coincidenTodosCrit(Registro crit, Registro r);

    string tablaMaxima();


private:

    struct tupBdd {
        string campoJoin;
        Lista<tupInterna> cambiosT1;
        Lista<tupInterna> cambiosT2;

    };

    struct tupInterna {
        Registro reg;
        bool agregar;
    };
    string* tablaMasAccedida;
    diccString<Tabla> nombreATabla;
    Conj<string> tablas;
    diccString<diccString<diccNat<Conj<Registro>::Iterador > > > joinPorCampoNat;
    diccString<diccString<diccString<Conj<Registro>::Iterador > > > joinPorCampoString;
    diccString<diccString<Conj<Registro > > > registrosDelJoin;
    diccString<diccString<tupBdd > > hayJoin;
};


bool operator==(const BaseDeDatos& b1, const BaseDeDatos& b2);

template<class T>
std::ostream& operator<<(std::ostream& os, const BaseDeDatos&);

BaseDeDatos::BaseDeDatos() : tablaMasAccedida(NULL), nombreATabla(diccString<Tabla>()), tablas(Conj<string>()), hayJoin(diccString<diccString<tupBdd > >()), joinPorCampoString(diccString<diccString<diccString<Conj<Registro>::Iterador > > >()), joinPorCampoNat(diccString<diccString<diccNat<Conj<Registro>::Iterador > > >()), registrosDelJoin(diccString<diccString<Conj<Registro > > >()) {
}

BaseDeDatos::agregarTabla(Tabla t) {
    if (tablaMasAccedida == NULL || nombreATabla.obtener(*tablaMasAccedida).cantidadDeAccesos() < t.cantidadDeAccesos()) {
        tablaMasAccedida = &(t.nombre);
    }
    nombreATabla.definir(t.nombre, t);
    tablas.AgregarRapido(t.nombre);
    hayJoin.definir(t.nombre(), diccString<tupBdd >());
    joinPorCampoNat.definir(t.nombre(), diccString<diccNat<Conj<Registro>::Iterador > >());
    joinPorCampoString.definir(t.nombre(), diccString<diccString<Conj<Registro>::Iterador > >());
    registrosDelJoin.definir(t.nombre(), diccString<Conj<Registro > >());
}

BaseDeDatos::insertarEntrada(Registro r, string s) {
    Tabla tabla = nombreATabla.obtener(s);
    tabla.agregarRegistro(r);
    Tabla tabMax = nombreATabla.obtener(*tablaMasAccedida);
    if (tabla.cantidadDeAccesos > tabMax.cantidadDeAccesos) {
        tablaMasAccedida = &s;
    }
    class Lista<struct tupString<tupBdd > >::const_Iterador iter = hayJoin.obtener(s).vistaDicc();

    while (iter.HaySiguiente()) {
        tupInterna tup;
        tup.reg = r;
        tup.agregar = true;
        iter.Siguiente().significado.cambiosT1.AgregarAdelante(tup);
        iter.Avanzar();
    }
    iter = tablas.CrearIt();
    while (iter.HaySiguiente()) {
        if (hayJoin.obtener(iter.Siguiente()).def(s)) {
            tupBdd cambios = hayJoin.obtener(iter.Siguiente()).obtener(s);
            tupInterna tup;
            tup.reg = r;
            tup.agregar = true;
            cambios.cambiosT2.AgregarAtras(tup);
        }
        iter.Avanzar();
    }
}

BaseDeDatos::Borrar(Registro cr, string t) {
    Tabla tabla = nombreATabla.obtener(t);
    tabla.borrarRegistro(cr);
    Tabla tabMax = nombreATabla.obtener(*tablaMasAccedida);
    if (tabla.cantidadDeAccesos > tabMax.cantidadDeAccesos) {
        tablaMasAccedida = &t;
    }
    class Lista<struct tupString<tupBdd > >::const_Iterador iter = hayJoin.obtener(t).vistaDicc();
    while (iter.HaySiguiente()) {
        tupInterna tup;
        tup.reg = cr;
        tup.agregar = false;
        iter.Siguiente().significado.cambiosT1.AgregarAdelante(tup);
        iter.Avanzar();
    }
    while (iter.HaySiguiente()) {
        if (hayJoin.obtener(iter.Siguiente()).def(t)) {
            tupBdd cambios = hayJoin.obtener(iter.Siguiente()).obtener(t);
            tupInterna tup;
            tup.reg = cr;
            tup.agregar = false;
            cambios.cambiosT2.AgregarAtras(tup);
        }
        iter.Avanzar();
    }

}

Conj<Registro> BaseDeDatos::combinarRegistros(string t1, string t2, string campo) {
    Tabla tabla1 = nombreATabla.obtener(t1);
    Tabla tabla2 = nombreATabla.obtener(t2);
    Conj<Registro>::Iterador it;
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

Conj<Registro>::Iterador BaseDeDatos::generarVistaJoin(string t1, string t2, string campo) {
    tupBdd aux;
    aux.campoJoin = campo;
    aux.cambiosT1 = Lista<tupInterna>();
    aux.cambiosT2 = Lista<tupInterna>();
    hayJoin.obtener(t1).definir(t2, aux);
    registrosDelJoin.obtener(t1).definir(t2, Conj<Registro>());
    Tabla tabla1 = nombreATabla.obtener(t1);
    Tabla tabla2 = nombreATabla.obtener(t2);
    if (tabla1.tipoCampo(campo)) {
        joinPorCampoNat.obtener(t1).definir(t2, diccNat < Conj<Registro>::Iterador);

        Conj<Registro> regsMergeados = combinarRegistros(t1, t2, campo);
        Conj<Registro>::Iterador it = regsMergeados.CrearIt();
        while (it.HaySiguiente()) {
            Dato d = it.Siguiente().obtener(campo);
            Conj<Registro>::Iterador iter = registrosDelJoin.obtener(t1).obtener(t2).AgregarRapido(it.Siguiente());
            unsigned int n = d.dame_valorNat();
            joinPorCampoNat.obtener(t1).obtener(t2).definir(n, iter);
            it.Avanzar();
        }
    } else {
        joinPorCampoString.obtener(t1).definir(t2, diccString < Conj<Registro>::Iterador);
        Conj<Registro> regsMergeados = combinarRegistros(t1, t2, campo);
        Conj<Registro>::Iterador it = regsMergeados.CrearIt();
        while (it.HaySiguiente()) {
            Dato d = it.Siguiente().obtener(campo);
            Conj<Registro>::Iterador iter = registrosDelJoin.obtener(t1).obtener(t2).AgregarRapido(it.Siguiente());
            string s = d.dame_valorStr();
            joinPorCampoNat.obtener(t1).obtener(t2).definir(s, iter);
            it.Avanzar();
        }
    }

    Conj<Registro>::Iterador res = registrosDelJoin.obtener(t1).obtener(t2).CrearIt();
    return res;
}

void BaseDeDatos::BorrarJoin(string t1, string t2) {
    hayJoin.obtener(t1).borrar(t2);
    registrosDelJoin.obtener(t1).borrar(t2);
    if (joinPorCampoNat.obtener(t1).def(t2)) {
        joinPorCampoNat.obtener(t1).borrar(t2);
    } else {
        joinPorCampoString.obtener(t1).borrar(t2);
    }

}

Conj<string>::Iterador BaseDeDatos::Tablas() {
    return tablas.CrearIt();
}

Tabla BaseDeDatos::dameTabla(string s) {
    return nombreATabla.obtener(s);
}

bool BaseDeDatos::hayJoin(string s1, string s2) {
    return hayJoin.obtener(s1).def(s2);
}

string BaseDeDatos::campoJoin(string s1, string s2) {
    return hayJoin.obtener(s1).obtener(s2).campoJoin;
}

Registro Merge(Registro r1, Registro r2) {
    Registro res = r1;
    class Lista<struct tupString<Dato> >::const_Iterador ite = r2.vistaDicc();
    while (ite.HaySiguiente()) {
        if (res.def(ite.Siguiente().clave)) {
            res.definir(ite.Siguiente().clave, ite.Siguiente().significado);
        }
        ite.Avanzar();
    }
}

Conj<Registro>::Iterador BaseDeDatos::vistaJoin(string s1, string s2) {
    //COMPLETAR!!
}

Conj<Registro> BaseDeDatos::busquedaCriterio(Registro crit, string t) {
    //COMPLETAR!!
}

bool coincidenTodosCrit(Registro crit, Registro r) {
    //COMPLETAR!!
}

string BaseDeDatos::tablaMaxima() {
    return (*tablaMasAccedida);
}




#endif
