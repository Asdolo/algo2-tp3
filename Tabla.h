#ifndef TABLA_H_INCLUDED
#define TABLA_H_INCLUDED
#include "diccString.h"
#define Registro diccString<Dato>
#include "aed2.h"
#include <cassert>
#include <string>
#include <iostream>


using namespace std;

class Tabla {
public:
    string nombre();
    Conj<string>::Iterador claves();
    Lista<Registro> buscar(string campo, Dato d);
    Conj<string> indices();
    Conj<string>::Iterador campos();
    bool tipoCampo(string campo);
    Conj<Registro> registros();
    unsigned int cantidadDeAccesos();
    Tabla(string nombre, Conj<string> claves, Registro columnas);
    agregarRegistro(Registro r);
    borrarRegistro(Registro cr);
    indexar(string c);
    Dato minimo(string campo);
    Dato maximo(string campo);

private:

    struct Tupla {
        string campo;
        Dato max;
        Dato min;
        bool vacio;
    };
    diccString<Conj<Iterador(Conj<Registro>) >> _indicesString;
    diccNat<Conj<Iterador<Conj<Registro>>>> _indicesNat;
    string _nombre;
    diccString<bool> _campos;
    Conj<string> _claves;
    Lista<Tupla> _campoIndexadoNat;
    Lista<Tupla> _campoIndexadoString;
    unsigned int _cantAccesos;
    Conj<Registro> _registros;

};

Tabla::Tabla(string nombre, Conj<string> claves, Registro columnas) {
    _indicesString = diccString();
    _indicesNat = diccNat();
    _registros = Conj();
    _nombre = nombre;
    _campos = diccString();
    _claves = Conj(claves);
    _campoIndexadoNat = Lista<Tupla>;
    _campoIndexadoString = Lista<Tupla>;
    _cantAccesos = 0;
    diccString::Iterador iter = columnas.vistaDicc();
    while (iter.hayMas()) {
        //REVISAR EN PDF LE FALTA A QUE DICC SE DEFINE
        definir(iter.siguiente().clave, nat(iter.Siguiente().significado, campos));
        iter.avanzar();
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

void Tabla::borrarRegistro(Registro criterio) {
    diccString::Iterador it = criterio.vistaDicc();
    ? ? ? clave = it.Siguiente().clave;
    Dato dato = it.actual().significado;

    if (_campoIndexadoNat.Primero().nombre == clave) {
        if (_indicesNat.def(dato.valorNat())) {
            Iterador iterador = ::CrearIt(_indicesNat.obtener(dato.valorNat()));
            iterador.Siguiente().EliminarSiguiente();
            _cantAccesos++;
            _indicesNat.borrar(dato.valorNat());
            Iterador temp = ::CrearIt(_indicesNat);
            if (!temp.HaySiguiente()) {
                _campoIndexadoNat.primero().vacio = true;
            } else {
                if (dato.valorNat() == _campoIndexadoNat.primero().max) {
                    _campoIndexadoNat.primero().max
                            = _indicesNat.maximo().fst().datoNat();
                }
                if (dato.valorNat() == _campoIndexadoNat.primero().min) {
                    _campoIndexadoNat.primero().min
                            = _indicesNat.minimo().fst().datoNat();
                }
            }
        }

    } else if (_campoIndexadoString.primero().nombre == clave) {
        if (_indicesString.def(dato.valorString())) {
            Iterador iterador = ::CrearIt(_indicesString.obtener(valorString()));
            iterador.Siguiente().EliminarSiguiente();
            _cantAccesos++;
            _indicesString.borrar(dato.valorString());
            Iterador temp = ::CrearIt(_indicesString);
            if (!temp.HaySiguiente()) {
                _campoIndexadoString.primero().vacio = true;
            } else {
                if (dato = _campoIndexadoString.primero().max) {
                    _campoIndexadoString.primero().max
                            = Maximo(_indicesString).fst().datoString();
                }
                if (dato = _campoIndexadoString.primero().min) {
                    _campoIndexadoString.primero().min
                            = minimo(_indicesString).fst().datoString();
                }

            }

        }

    } else {
        Iterador iter = Registro::CrearIt(registros);
        while (iter.HaySiguiente()) {
            if (iter.Siguiente.obtener(clave) == dato) {
                if (!_campoIndexadoNat.vacia()) {
                    Registro regi = iter.Siguiente();
                    ? ? valorIndex = regi.obtener(_campoIndexadoNat.primero().nombre).valorNat();
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
                    ? ? valorIndex = regi.obtener(_campoIndexadoString.primero().nombre).valorString();
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

void Tabla::agregarRegistro(Registro r) {
    cantAccesos++;
    Iterador it = registros.agregarRapido(r);
    if (!_campoIndexadoNat.vacia()) {
        if (_campoIndexadoNat.primero().vacio) {
            _campoIndexadoNat.primero().min = r.obtener(
                    _campoIndexadoNat.primero());
            _campoIndexadoNat.primero().max = (_campoIndexadoNat.primero()).min; //REVISAR LINEA!
            _campoIndexadoNat.primero().vacio = false;

        } else {
            unsigned int nPaMinMax = r.obtener(
                    _campoIndexadoNat.primero().nombre);
            if (nPaMinMax > _campoIndexadoNat.primero().min) {
                _campoIndexadoNat.primero().min = nPaMinMax;
            }
            if (nPaMinMax > _campoIndexadoNat.primero().max) {
                _campoIndexadoNat.primero().max = nPaMinMax;
            }
        }
        ? ? aux = r.obtener(_campoIndexadoNat.primero());
        if (_indicesNat.def(aux)) {
            _indicesNat.obtener(aux).agregarRapido(it);
        } else {
            _indicesNat.definir(aux, Conj::vacio().agregarRapido(it));
        }
    }

    if (!_campoIndexadoString.vacia()) {
        if (_campoIndexadoString.primero().vacio) {
            _campoIndexadoString.primero().min = r.obtener(
                    _campoIndexadoString.primero());
            _campoIndexadoString.primero().max
                    = (_campoIndexadoString.primero()).min; //REVISAR LINEA!
            _campoIndexadoString.primero().vacio = false;

        } else {
            string sPaMinMax = r.obtener(_campoIndexadoString.primero().nombre);
            if (sPaMinMax > _campoIndexadoString.primero().min) {
                _campoIndexadoString.primero().min = sPaMinMax;
            }
            if (sPaMinMax > _campoIndexadoString.primero().max) {
                _campoIndexadoString.primero().max = sPaMinMax;
            }
        }
        ? ? aux = r.obtener(_campoIndexadoString.primero());
        if (_indicesString.def(aux)) {
            _indicesString.obtener(aux).agregarRapido(it);
        } else {
            _indicesString.definir(aux, Conj::vacio().agregarRapido(it));
        }
    }

}

void Tabla::indexar(string c) {
    if (this.tipoCampo(c)) {
        Dato dato = Dato::DatoNat(0);
        _campoIndexadoNat.agregarAdelante(Tupla(c, dato, dato, true));
        Iterador it = ::CrearIt(registros);
        if (it.HaySiguiente()) {
            _campoIndexadoNat.primero().vacio = false;
            _campoIndexadoNat.primero().max = it.Siguiente().obtener(c);
            _campoIndexadoNat.primero().min = it.Siguiente().obtener(c);
        }
        while (it.HaySiguiente()) {
            unsigned int temp = it.Siguiente().obtener(c).valorNat();
            if (! ? ? ? .def(_indicesNat.obtener(temp))) {
                _indicesNat.definir(temp, ::vacio());
            }
            _indicesNat.obtener(temp).agregarRapido(it);
            if (it.Siguiente().obtener(c) > _campoIndexadoNat.primero().max) {
                _campoIndexadoNat.primero().max = it.siguiente().obtener(c);
            }
            if (it.siguiente().obtener(c) < _campoIndexadoNat.primero().min) {
                _campoIndexadoNat.primero().min = it.Siguiente().obtener(c);
            }
            it.Avanzar();

        }

    } else {
        Dato dato = Dato::datoString("temp");
        _campoIndexadoString.agregarAdelante(Tupla(c, dato, dato, true));
        Iterador it = ::CrearIt(registros);
        if (it.HaySiguiente()) {
            _campoIndexadoString.primero().vacio = false;
            _campoIndexadoString.primero().max = it.Siguiente().obtener(c);
            _campoIndexadoString.primero().min = it.Siguiente().obtener(c);
        }

        while (it.HaySiguiente()) {
            string temp = it.Siguiente().obtener(c).valorString();
            if (! ? ? ? .def(_indicesString.obtener(temp))) {
                _indicesString.definir(temp, ::vacio());
            }
            _indicesString.obtener(temp).agregarRapido(it);
            if (it.Siguiente().obtener(c) > _campoIndexadoString.primero().max) {
                _campoIndexadoString.primero().max = it.siguiente().obtener(c);
            }
            if (it.siguiente().obtener(c) < _campoIndexadoString.primero().min) {
                _campoIndexadoString.primero().min = it.Siguiente().obtener(c);
            }
            it.Avanzar();

        }

    }
}

Lista<Registro> Tabla::buscar(string c, Dato d) {
    Lista res = Lista::vacia();
    if (d.esNat) {
        if (_campoIndexadoNat.primero().nombre = c) {
            if (_indicesNat.def(d.valorNat())) {
                Iterador itConjIts = _indicesNat.obtener(d.valorNat())::CrearIt();
                while (itConjIts.HaySiguiente()) {
                    res.agregarAtras(itConjIts.Siguiente().Siguiente());
                    itConjIts.Avanzar();
                }
            }
        } else {
            if (_campoIndexadoString.primero().nombre = c) {
                if (_indicesString.def(d.valorString())) {
                    Iterador itConjIts = _indicesNat.obtener(d.valorNat())::CrearIt();
                    while (itConjIts.HaySiguiente()) {
                        res.agregarAtras(itConjIts.Siguiente().Siguiente());
                        itConjIts.Avanzar();
                    }
                }



            } else {
                Iterador it = registros.CrearIt();
                while (it.HaySiguiente()) {
                    if (it.Siguiente().obtener(c) == d) {
                        res.agregarAtras(it.Siguiente());
                    }
                    it.Avanzar();
                }
            }
        }
    }

    Dato Tabla::minimo(string c) {
        Dato res;
        if (c == _campoIndexadoNat.primero().nombre) {
            res = _campoIndexadoNat.primero().min;
        } else {
            res = _campoIndexadoString.primero().min;

        }
    }

    Dato Tabla::maximo(string c) {
        Dato res;
        if (c == _campoIndexadoNat.primero().nombre) {
            res = _campoIndexadoNat.primero().max;
        } else {
            res = _campoIndexadoString.primero().max;

        }
    }

