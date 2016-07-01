#ifndef TABLA_H_INCLUDED
#define TABLA_H_INCLUDED
#include "diccString.h"
#include "aed2.h"
#include <cassert>
#include <string>
#include <iostream>
#include "Dato.h"
#include "diccNat.h"
#include "diccString.h"

#define Registro diccString<Dato>

using namespace std;

class Tabla {
public:

    Tabla(string nombre, Conj<string> claves, Registro columnas);
    void agregarRegistro(Registro r);
    //void borrarRegistro(Registro cr);
    void indexar(string c);
    string nombre() const;
    Conj<string>::const_Iterador claves() const;
    Lista<Registro> buscar(string campo, const Dato& d) const;
    const Conj<string> indices() const;
    Conj<string>::const_Iterador campos() const;
    const bool& tipoCampo(string campo) const; //const bool->no me cambien los tipos desde afuera
    const Conj<Registro>& registros() const;
    unsigned int cantidadDeAccesos() const;
    const Dato& minimo(string campo) const;
    const Dato& maximo(string campo) const;

private:

    struct Tupla {
        Tupla(string c, Dato mx, Dato mn, bool v) : campo(c), max(mx), min(mn), vacio(v) {};
        string campo;
        Dato max;
        Dato min;
        bool vacio;
    };
    diccString<Conj<Conj<Registro>::Iterador> > _indicesString;
    diccNat<Conj<Conj<Registro>::Iterador> > _indicesNat;
    string _nombre;
    diccString<bool> _campos;
    Conj<string> _claves;
    Lista<Tupla> _campoIndexadoNat;
    Lista<Tupla> _campoIndexadoString;
    unsigned int _cantAccesos;
    Conj<Registro> _registros;
};

Tabla::Tabla(string nombre, Conj<string> claves, Registro columnas) {
    _indicesString = diccString<Conj<Conj<Registro>::Iterador> >();
    _indicesNat = diccNat<Conj<Conj<Registro>::Iterador> >();
    _registros = Conj<Registro>();
    _nombre = nombre;
    _campos = diccString<bool>();
    _claves = claves;
    _campoIndexadoNat = Lista<Tupla>();
    _campoIndexadoString = Lista<Tupla>();
    _cantAccesos = 0;
    class Lista<tupString<Dato> >::const_Iterador iter = columnas.vistaDicc();
    while (iter.HaySiguiente()) {
        bool es_Nat = iter.Siguiente().significado.esNat();
        _campos.definir(iter.Siguiente().clave, es_Nat);
        iter.Avanzar();
    }
}


string Tabla::nombre() const {
    return _nombre;
}

Conj<string>::const_Iterador Tabla::claves() const {
    return _claves.CrearIt();
}

const Conj<string> Tabla::indices() const {
    //Quizás res debería ser una referencia, pero no es por copia devuelve basura
    Conj<string> res;
    if (_campoIndexadoNat.Longitud() > 0) {
        res.AgregarRapido(_campoIndexadoNat.Primero().campo);
    }
    if (_campoIndexadoString.Longitud() > 0) {
        res.AgregarRapido(_campoIndexadoString.Primero().campo);
    }
    return res;
}

Conj<string>::const_Iterador Tabla::campos() const {
    return _campos.claves().CrearIt();
}

const bool& Tabla::tipoCampo(string c) const {
    return _campos.obtener(c);
}

const Conj<Registro>& Tabla::registros() const {
    return _registros;
}

unsigned int Tabla::cantidadDeAccesos() const {
    return _cantAccesos;
}
/*
void Tabla::borrarRegistro(Registro cr) {
    class Lista<struct tupString<Dato> >::const_Iterador it = cr.vistaDicc();
    string clave = it.Siguiente().clave;
    Dato dato = it.Siguiente().significado;

    if (_campoIndexadoNat.Primero().campo == clave) {
        if (_indicesNat.def(dato.dame_valorNat())) {
            Conj<Conj<Registro>::Iterador > flash = _indicesNat.obtener(dato.dame_valorNat());
            Conj<Conj<Registro>::Iterador >::Iterador iterador = flash.CrearIt();
            iterador.Siguiente().EliminarSiguiente();
            _cantAccesos++;
            _indicesNat.borrar(dato.dame_valorNat());
            diccNat<Conj<Conj<Registro>::Iterador > >::Iterador temp = _indicesNat.crearIt();
            if (!temp.hayMas()) {
                _campoIndexadoNat.Primero().vacio = true;
            } else {
                if (dato.dame_valorNat() == _campoIndexadoNat.Primero().max.dame_valorNat()) {
                    Dato max = Dato::datoNat(_indicesNat.max().clave);
                    _campoIndexadoNat.Primero().max = max;
                }
                if (dato.dame_valorNat() == _campoIndexadoNat.Primero().min.dame_valorNat()) {
                    Dato min = Dato::datoNat(_indicesNat.min().clave);
                    _campoIndexadoNat.Primero().max = min;
                }
            }
        }

    } else if (_campoIndexadoString.Primero().campo == clave) {
        if (_indicesString.def(dato.dame_valorStr())) {
            Conj<Conj<Registro>::Iterador>::Iterador iterador = _indicesString.obtener(dato.dame_valorStr()).CrearIt();
            iterador.Siguiente().EliminarSiguiente();
            _cantAccesos++;
            _indicesString.borrar(dato.dame_valorStr());
            //NO HAY ITERADOR DE DICCSTRING, HAY QUE USAR VISTADICC??
            Lista<tupString<Conj<Conj<diccString<Dato> >::Iterador> > >::const_Iterador temp = _indicesString.vistaDicc();
            if (! temp.HaySiguiente() ) {
                _campoIndexadoString.Primero().vacio = true;
            } else {
                if (dato == _campoIndexadoString.Primero().max) {
                    Dato max = Dato::datoString(_indicesString.max());
                    _campoIndexadoString.Primero().max = max;
                }
                if (dato == _campoIndexadoString.Primero().min) {
                    Dato min = Dato::datoString(_indicesString.min());
                    _campoIndexadoString.Primero().min = min;
                }

            }

        }

    } else {
        Conj<Registro>::Iterador iter = _registros.CrearIt();
        while (iter.HaySiguiente()) {
            if (iter.Siguiente().obtener(clave) == dato) {
                if (!_campoIndexadoNat.EsVacia()) {
                    Registro regi = iter.Siguiente();
                    unsigned int valorIndex = regi.obtener(_campoIndexadoNat.Primero().campo).dame_valorNat();
                    Conj<Conj<Registro>::Iterador> conjIters = _indicesNat.obtener(valorIndex);
                    Conj<Conj<Registro>::Iterador>::Iterador itDeIters = conjIters.CrearIt();
                    while (itDeIters.HaySiguiente()) {
                        if (itDeIters.Siguiente().Siguiente() == regi) {
                            itDeIters.EliminarSiguiente();
                        }
                        //FALTA AVANZAR?? FIJARSE!
                        itDeIters.Avanzar();
                    }
                } else if (!_campoIndexadoString.EsVacia()) {

                    Registro regi = iter.Siguiente();
                    string valorIndex = regi.obtener(_campoIndexadoString.Primero().campo).dame_valorStr();
                    Conj<Conj<Registro>::Iterador> conjIters = _indicesString.obtener(valorIndex);
                    Conj<Conj<Registro>::Iterador>::Iterador itDeIters = conjIters.CrearIt();
                    while (itDeIters.HaySiguiente()) {
                        if (itDeIters.Siguiente().Siguiente() == regi) {
                            itDeIters.EliminarSiguiente();
                        }
                        //FALTA AVANZAR?? FIJARSE!
                        itDeIters.Avanzar();
                    }

                }
                iter.EliminarSiguiente();
            }
            iter.Avanzar();
        }

    }
}
*/

void Tabla::agregarRegistro(Registro r) {
    _cantAccesos++;
    Conj<Registro>::Iterador it = _registros.AgregarRapido(r);
    if (!_campoIndexadoNat.EsVacia()) {
        if (_campoIndexadoNat.Primero().vacio) {
            _campoIndexadoNat.Primero().min = r.obtener(_campoIndexadoNat.Primero().campo);
            _campoIndexadoNat.Primero().max = r.obtener(_campoIndexadoNat.Primero().campo); 
            _campoIndexadoNat.Primero().vacio = false;

        } else {
            unsigned int nPaMinMax = r.obtener(_campoIndexadoNat.Primero().campo).dame_valorNat();
            if (nPaMinMax > _campoIndexadoNat.Primero().min.dame_valorNat()) {
                _campoIndexadoNat.Primero().min =r.obtener(_campoIndexadoNat.Primero().campo); 
            }
            if (nPaMinMax > _campoIndexadoNat.Primero().max.dame_valorNat()) {
                _campoIndexadoNat.Primero().max =r.obtener(_campoIndexadoNat.Primero().campo);
            }
        }
        Dato aux = r.obtener(_campoIndexadoNat.Primero().campo);
        if (_indicesNat.def(aux.dame_valorNat())) {
            _indicesNat.obtener(aux.dame_valorNat()).AgregarRapido(it);
        } else {
            Conj< Conj<Registro>::Iterador > nuevoConj;
            nuevoConj.AgregarRapido(it);
            _indicesNat.definir(aux.dame_valorNat(), nuevoConj);
        }
    }

    if (!_campoIndexadoString.EsVacia()) {
        if (_campoIndexadoString.Primero().vacio) {
            _campoIndexadoString.Primero().min = r.obtener(_campoIndexadoString.Primero().campo);
            _campoIndexadoString.Primero().max = r.obtener(_campoIndexadoString.Primero().campo); 
            _campoIndexadoString.Primero().vacio = false;

        } else {
            string sPaMinMax = r.obtener(_campoIndexadoString.Primero().campo).dame_valorStr();
            if (sPaMinMax > _campoIndexadoString.Primero().min.dame_valorStr()) {
                _campoIndexadoString.Primero().min = r.obtener(_campoIndexadoString.Primero().campo);
            }
            if (sPaMinMax > _campoIndexadoString.Primero().max.dame_valorStr()) {
                _campoIndexadoString.Primero().max = r.obtener(_campoIndexadoString.Primero().campo);
            }
        }
        Dato aux = r.obtener(_campoIndexadoString.Primero().campo);
        if (_indicesString.def(aux.dame_valorStr())) {
            _indicesString.obtener(aux.dame_valorStr()).AgregarRapido(it);
        } else {
             Conj< Conj<Registro>::Iterador > nuevoConj;
            nuevoConj.AgregarRapido(it);
            _indicesString.definir(aux.dame_valorStr(), nuevoConj);
        }
    }

}
void Tabla::indexar(string c) {
    if (this->tipoCampo(c)) {
        Dato dato = Dato::datoNat(0);
        _campoIndexadoNat.AgregarAdelante(Tupla(c, dato, dato, true));
        Conj<Registro>::Iterador it = _registros.CrearIt();
        if (it.HaySiguiente()) {
            _campoIndexadoNat.Primero().vacio = false;
            _campoIndexadoNat.Primero().max = it.Siguiente().obtener(c);
            _campoIndexadoNat.Primero().min = it.Siguiente().obtener(c);
        }
        while (it.HaySiguiente()) {
            unsigned int temp = it.Siguiente().obtener(c).dame_valorNat();
            if (!_indicesNat.def(temp)) {
                Conj<Conj<Registro>::Iterador> c_iters = Conj<Conj<Registro>::Iterador>();
                _indicesNat.definir(temp, c_iters);
            }
            _indicesNat.obtener(temp).AgregarRapido(it);
            if (it.Siguiente().obtener(c) > _campoIndexadoNat.Primero().max) {
                _campoIndexadoNat.Primero().max = it.Siguiente().obtener(c);
            }
            if (it.Siguiente().obtener(c) < _campoIndexadoNat.Primero().min) {
                _campoIndexadoNat.Primero().min = it.Siguiente().obtener(c);
            }
            it.Avanzar();

        }

    } else {
        Dato dato = Dato::datoString("temp");
        _campoIndexadoString.AgregarAdelante(Tupla(c, dato, dato, true));
        Conj<Registro>::Iterador it = _registros.CrearIt();
        if (it.HaySiguiente()) {
            _campoIndexadoString.Primero().vacio = false;
            _campoIndexadoString.Primero().max = it.Siguiente().obtener(c);
            _campoIndexadoString.Primero().min = it.Siguiente().obtener(c);
        }

        while (it.HaySiguiente()) {
            string temp = it.Siguiente().obtener(c).dame_valorStr();
            if (!_indicesString.def(temp)) {
                  Conj<Conj<Registro>::Iterador> c_iters = Conj<Conj<Registro>::Iterador>();
                _indicesString.definir(temp, c_iters);
            }
            _indicesString.obtener(temp).AgregarRapido(it);
            if (it.Siguiente().obtener(c) > _campoIndexadoString.Primero().max) {
                _campoIndexadoString.Primero().max = it.Siguiente().obtener(c);
            }
            if (it.Siguiente().obtener(c) < _campoIndexadoString.Primero().min) {
                _campoIndexadoString.Primero().min = it.Siguiente().obtener(c);
            }
            it.Avanzar();

        }

    }
}

Lista<Registro> Tabla::buscar(string c, const Dato& d) const {
    Lista<Registro> res;
    if (d.esNat()) {
        if (_campoIndexadoNat.Primero().campo == c) {
            if (_indicesNat.def(d.dame_valorNat())) {
                Conj< Conj<Registro>::Iterador >::const_Iterador itConjIts = _indicesNat.obtener(d.dame_valorNat()).CrearIt();
                while (itConjIts.HaySiguiente()) {
                    res.AgregarAtras(itConjIts.Siguiente().Siguiente());
                    itConjIts.Avanzar();
                }
            }
        } else {
            if (_campoIndexadoString.Primero().campo == c) {
                if (_indicesString.def(d.dame_valorStr())) {
                    Conj< Conj<Registro>::Iterador >::const_Iterador itConjIts2 = (_indicesString.obtener(d.dame_valorStr())).CrearIt();
                    while (itConjIts2.HaySiguiente()) {
                        res.AgregarAtras(itConjIts2.Siguiente().Siguiente());
                        itConjIts2.Avanzar();
                    }
                }
            } else {
                Conj<Registro>::const_Iterador it = _registros.CrearIt();
                while (it.HaySiguiente()) {
                    if (it.Siguiente().obtener(c) == d) {
                        res.AgregarAtras(it.Siguiente());
                    }
                    it.Avanzar();
                }
            }
        }
    }
}

const Dato& Tabla::minimo(string c) const {
    if (c == _campoIndexadoNat.Primero().campo) {
        return _campoIndexadoNat.Primero().min;
    } else {
        return _campoIndexadoString.Primero().min;

    }
}

const Dato& Tabla::maximo(string c) const {

    if (c == _campoIndexadoNat.Primero().campo) {
        return _campoIndexadoNat.Primero().max;
    } else {
        return _campoIndexadoString.Primero().max;

    }
}

#endif
