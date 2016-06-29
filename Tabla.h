#ifndef TABLA_H_INCLUDED
#define TABLA_H_INCLUDED
#include "diccString.h"
#include "aed2.h"
#include <cassert>
#include <string>
#include <iostream>
#include "Dato.h"
#define Registro diccString<Dato>

using namespace std;

class Tabla {
public:

	Tabla(string nombre, Conj<string> claves, Registro columnas);
	agregarRegistro(Registro r);
    borrarRegistro(Registro cr);
    indexar(string c);
    string nombre();
    Conj<string>::Iterador claves();
    Lista<Registro> buscar(string campo, Dato d);
    Conj<string> indices();
    Conj<string>::Iterador campos();
    bool tipoCampo(string campo);
    Conj<Registro> registros();
    unsigned int cantidadDeAccesos();
    Dato minimo(string campo);
    Dato maximo(string campo);

private:

    struct Tupla {
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
    _claves = Conj<claves>();
    _campoIndexadoNat = Lista<Tupla>();
    _campoIndexadoString = Lista<Tupla>();
    _cantAccesos = 0;
    Lista<Registro>::Iterador iter = columnas.vistaDicc();
    while (iter.HaySiguiente()) {
        //REVISAR EN PDF LE FALTA A QUE DICC SE DEFINE
        definir(iter.siguiente().clave, nat(iter.Siguiente().significado, campos));
        iter.Avanzar();
    }

}

string Tabla::nombre() {
    return _nombre;
}

Conj<string>::Iterador Tabla::claves() {
    return _claves.CrearIt();
}

Conj<string> Tabla::indices() {
    Conj<string> res;
    if ( _campoIndexadoNat.Longitud() > 0) {
        res.AgregarRapido(_campoIndexadoNat.Primero().campo);
    }
    if (_campoIndexadoString.Longitud() > 0) {
         res.AgregarRapido(_campoIndexadoString.Primero().campo);
    }
    return res;
}

Conj<string>::Iterador Tabla::campos() {
    return Conj<string>::CrearIt(campos);
}

bool Tabla::tipoCampo(string c) {
    return _campos.obtener(c);
}

Conj<Registro> Tabla::registros() {
    return _registros;
}

unsigned int Tabla::cantidadDeAccesos() {
    return _cantAccesos;
}

 Tabla::borrarRegistro(Registro cr) {
    Registro::Iterador it = cr.vistaDicc();
    string clave = it.Siguiente().clave;
    Dato dato = it.actual().significado;

    if (_campoIndexadoNat.Primero().campo == clave) {
        if (_indicesNat.def(dato.valorNat())) {
            Conj<Conj<Registro>::Iterador>::Iterador iterador = _indicesNat.obtener(dato.valorNat()).CrearIt();
            iterador.Siguiente().EliminarSiguiente();
            _cantAccesos++;
            _indicesNat.borrar(dato.valorNat());
            diccNat<Conj<Conj<Registro>::Iterador> >::Iterador temp = _indicesNat.CrearIt();
            if (!temp.HaySiguiente()) {
                _campoIndexadoNat.Primero().vacio = true;
            } else {
                if (dato.valorNat() == _campoIndexadoNat.Primero().max) {
                    _campoIndexadoNat.Primero().max
                            = _indicesNat.maximo().fst().datoNat();
                }
                if (dato.valorNat() == _campoIndexadoNat.Primero().min) {
                    _campoIndexadoNat.Primero().min
                            = _indicesNat.minimo().fst().datoNat();
                }
            }
        }

    } else if (_campoIndexadoString.Primero().campo == clave) {
        if (_indicesString.def(dato.valorString())) {
            Conj<Conj<Registro>::Iterador>::Iterador iterador = _indicesString.obtener(valorString()).CrearIt();
            iterador.Siguiente().EliminarSiguiente();
            _cantAccesos++;
            _indicesString.borrar(dato.valorString());
            diccString<Conj<Conj<Registro>::Iterador> >::Iterador temp = _indicesString.CrearIt();
            if (!temp.HaySiguiente()) {
                _campoIndexadoString.Primero().vacio = true;
            } else {
                if (dato = _campoIndexadoString.Primero().max) {
                    _campoIndexadoString.Primero().max
                            = Maximo(_indicesString).fst().datoString();
                }
                if (dato = _campoIndexadoString.Primero().min) {
                    _campoIndexadoString.Primero().min
                            = minimo(_indicesString).fst().datoString();
                }

            }

        }

    } else {
        Conj<Registro>::Iterador iter = _registros.CrearIt();
        while (iter.HaySiguiente()) {
            if (iter.Siguiente.obtener(clave) == dato) {
                if (!_campoIndexadoNat.vacia()) {
                    Registro regi = iter.Siguiente();
                    unsigned int valorIndex = regi.obtener(_campoIndexadoNat.Primero().campo).valorNat();
                    ? ? conjIters = _indicesNat.obtener(valorIndex);
                    ? ? itDeIters = ::CrearIt(conjIters);
                    while (itDeIters.HaySiguiente()) {
                        if (itDeIters.Siguiente().Siguiente() == regi) {
                            itDeIters.EliminarSiguiente();
                        }
                        //FALTA AVANZAR?? FIJARSE!
                        itDeIters.Avanzar();
                    }
                } else if (!_campoIndexadoString.vacia()) {

                    Registro regi = iter.Siguiente();
                    string valorIndex = regi.obtener(_campoIndexadoString.Primero().campo).valorString();
                    ? ? conjIters = _indicesString.obtener(valorIndex);
                    ? ? itDeIters = ::CrearIt(conjIters);
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

 Tabla::agregarRegistro(Registro r) {
    cantAccesos++;
    Conj<Registro>::Iterador it = registros.agregarRapido(r);
    if (!_campoIndexadoNat.vacia()) {
        if (_campoIndexadoNat.Primero().vacio) {
            _campoIndexadoNat.Primero().min = r.obtener(
                    _campoIndexadoNat.Primero());
            _campoIndexadoNat.Primero().max = (_campoIndexadoNat.Primero()).min; //REVISAR LINEA!
            _campoIndexadoNat.Primero().vacio = false;

        } else {
            unsigned int nPaMinMax = r.obtener(
                    _campoIndexadoNat.Primero().campo);
            if (nPaMinMax > _campoIndexadoNat.Primero().min) {
                _campoIndexadoNat.Primero().min = nPaMinMax;
            }
            if (nPaMinMax > _campoIndexadoNat.Primero().max) {
                _campoIndexadoNat.Primero().max = nPaMinMax;
            }
        }
        Dato aux = r.obtener(_campoIndexadoNat.Primero());
        if (_indicesNat.def(aux)) {
            _indicesNat.obtener(aux).agregarRapido(it);
        } else {
            _indicesNat.definir(aux, Conj::vacio().agregarRapido(it));
        }
    }

    if (!_campoIndexadoString.vacia()) {
        if (_campoIndexadoString.Primero().vacio) {
            _campoIndexadoString.Primero().min = r.obtener(
                    _campoIndexadoString.Primero());
            _campoIndexadoString.Primero().max
                    = (_campoIndexadoString.Primero()).min; //REVISAR LINEA!
            _campoIndexadoString.Primero().vacio = false;

        } else {
            string sPaMinMax = r.obtener(_campoIndexadoString.Primero().campo);
            if (sPaMinMax > _campoIndexadoString.Primero().min) {
                _campoIndexadoString.Primero().min = sPaMinMax;
            }
            if (sPaMinMax > _campoIndexadoString.Primero().max) {
                _campoIndexadoString.Primero().max = sPaMinMax;
            }
        }
        Dato aux = r.obtener(_campoIndexadoString.Primero());
        if (_indicesString.def(aux)) {
            _indicesString.obtener(aux).agregarRapido(it);
        } else {
            _indicesString.definir(aux, Conj::vacio().agregarRapido(it));
        }
    }

}

 Tabla::indexar(string c) {
    if (this.tipoCampo(c)) {
        Dato dato = Dato::DatoNat(0);
        _campoIndexadoNat.agregarAdelante(Tupla(c, dato, dato, true));
        Conj<Registro>::Iterador it = _registros.CrearIt();
        if (it.HaySiguiente()) {
            _campoIndexadoNat.Primero().vacio = false;
            _campoIndexadoNat.Primero().max = it.Siguiente().obtener(c);
            _campoIndexadoNat.Primero().min = it.Siguiente().obtener(c);
        }
        while (it.HaySiguiente()) {
            unsigned int temp = it.Siguiente().obtener(c).valorNat();
            if (! ? ? ? .def(_indicesNat.obtener(temp))) {
                _indicesNat.definir(temp, ::vacio());
            }
            _indicesNat.obtener(temp).agregarRapido(it);
            if (it.Siguiente().obtener(c) > _campoIndexadoNat.Primero().max) {
                _campoIndexadoNat.Primero().max = it.siguiente().obtener(c);
            }
            if (it.siguiente().obtener(c) < _campoIndexadoNat.Primero().min) {
                _campoIndexadoNat.Primero().min = it.Siguiente().obtener(c);
            }
            it.Avanzar();

        }

    } else {
        Dato dato = Dato::datoString("temp");
        _campoIndexadoString.agregarAdelante(Tupla(c, dato, dato, true));
        Conj<Registro>::Iterador it = _registros.CrearIt();
        if (it.HaySiguiente()) {
            _campoIndexadoString.Primero().vacio = false;
            _campoIndexadoString.Primero().max = it.Siguiente().obtener(c);
            _campoIndexadoString.Primero().min = it.Siguiente().obtener(c);
        }

        while (it.HaySiguiente()) {
            string temp = it.Siguiente().obtener(c).valorString();
            if (! ? ? ? .def(_indicesString.obtener(temp))) {
                _indicesString.definir(temp, ::vacio());
            }
            _indicesString.obtener(temp).agregarRapido(it);
            if (it.Siguiente().obtener(c) > _campoIndexadoString.Primero().max) {
                _campoIndexadoString.Primero().max = it.siguiente().obtener(c);
            }
            if (it.siguiente().obtener(c) < _campoIndexadoString.Primero().min) {
                _campoIndexadoString.Primero().min = it.Siguiente().obtener(c);
            }
            it.Avanzar();

        }

    }
}

Lista<Registro> Tabla::buscar(string c, Dato d) {
    Lista<Registro> res;
    if (d.esNat()) {
        if (_campoIndexadoNat.Primero().campo = c) {
            if (_indicesNat.def(d.valorNat())) {
                Conj< Conj<Registro>::Iterador >::Iterador itConjIts = _indicesNat.obtener(d.valorNat()).CrearIt();
                while (itConjIts.HaySiguiente()) {
                    res.AgregarAtras(itConjIts.Siguiente().Siguiente());
                    itConjIts.Avanzar();
                }
            }
        } else {
            if (_campoIndexadoString.Primero().campo = c) {
                if (_indicesString.def(d.valorString())) {
                    Conj< Conj<Registro>::Iterador >::Iterador itConjIts2 = _indicesString.obtener(d.valorNat()).CrearIt();
                    while (itConjIts2.HaySiguiente()) {
                        res.AgregarAtras(itConjIts2.Siguiente().Siguiente());
                        itConjIts2.Avanzar();
                    }
                }



            } else {
                Conj<Registro>::Iterador it = _registros.CrearIt();
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

    Dato Tabla::minimo(string c) {
        if (c == _campoIndexadoNat.Primero().campo) {
            return _campoIndexadoNat.Primero().min;
        } else {
            return _campoIndexadoString.Primero().min;

        }
    }

    Dato Tabla::maximo(string c) {
        
        if (c == _campoIndexadoNat.Primero().campo) {
            return _campoIndexadoNat.Primero().max;
        } else {
            return _campoIndexadoString.Primero().max;

        }
    }

