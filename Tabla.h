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

    Tabla(const string& nombre, Conj<string> claves, Registro columnas);

    void agregarRegistro(Registro r);
    void borrarRegistro(Registro cr);
    void indexar(const string& c);
    string nombre() const;
    Conj<string>::const_Iterador claves() const;
    Lista<Registro> buscar(const string& campo, const Dato& d) const;
    const Conj<string> indices() const;
    Lista<tupString<bool> >::const_Iterador campos() const;
    bool tipoCampo(const string& campo) const; //const bool->no me cambien los tipos desde afuera
    const Conj<Registro>& registros() const;
    unsigned int cantidadDeAccesos() const;
    const Dato& minimo(string campo) const;
    const Dato& maximo(string campo) const;

    friend ostream& operator<<(ostream& os, const Tabla& t);

private:

    struct Tupla {
        Tupla(string c, Dato mx, Dato mn, bool v) : campo(c), max(mx), min(mn), vacio(v) {};
        string campo;
        Dato max;
        Dato min;
        bool vacio;
    };
    diccString<bool> _campos;
    diccString<Lista<Conj<Registro>::Iterador> > _indicesString;
    diccNat<Lista<Conj<Registro>::Iterador> > _indicesNat;
    string _nombre;
    Conj<string> _claves;
    Lista<Tupla> _campoIndexadoNat;
    Lista<Tupla> _campoIndexadoString;
    unsigned int _cantAccesos;
    Conj<Registro> _registros;
};

Tabla::Tabla(const string& nombre, Conj<string> claves, Registro columnas) : _nombre(nombre), _claves(claves), _cantAccesos(0) {
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
    Conj<string> res;
    if (_campoIndexadoNat.Longitud() > 0) {
        res.AgregarRapido(_campoIndexadoNat.Primero().campo);
    }
    if (_campoIndexadoString.Longitud() > 0) {
        res.AgregarRapido(_campoIndexadoString.Primero().campo);
    }
    return res;
}

Lista<tupString<bool> >::const_Iterador Tabla::campos() const { //Ver si hay manera de devolver el iterador acá
  return _campos.vistaDicc();
}

bool Tabla::tipoCampo(const string& c) const {
    return _campos.obtener(c);
}

const Conj<Registro>& Tabla::registros() const {
    return _registros;
}

unsigned int Tabla::cantidadDeAccesos() const {
    return _cantAccesos;
}

void Tabla::borrarRegistro(Registro cr) {
    class Lista<struct tupString<Dato> >::const_Iterador it = cr.vistaDicc();
    string clave = it.Siguiente().clave;
    Dato dato = it.Siguiente().significado;

    if (!_campoIndexadoNat.EsVacia() && _campoIndexadoNat.Primero().campo == clave) {
        if (_indicesNat.def(dato.dame_valorNat())) {
            Lista<Conj<Registro>::Iterador > flash = _indicesNat.obtener(dato.dame_valorNat());
            Lista<Conj<Registro>::Iterador >::Iterador iterador = flash.CrearIt();
            iterador.Siguiente().EliminarSiguiente();
            _cantAccesos++;
            _indicesNat.borrar(dato.dame_valorNat());
            diccNat<Lista<Conj<Registro>::Iterador > >::Iterador temp = _indicesNat.crearIt();
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

    } else if (!_campoIndexadoString.EsVacia() && _campoIndexadoString.Primero().campo == clave) {
        if (_indicesString.def(dato.dame_valorStr())) {
            Lista<Conj<Registro>::Iterador>::Iterador iterador = _indicesString.obtener(dato.dame_valorStr()).CrearIt();
            iterador.Siguiente().EliminarSiguiente();
            _cantAccesos++;
            _indicesString.borrar(dato.dame_valorStr());
            //NO HAY ITERADOR DE DICCSTRING, HAY QUE USAR VISTADICC??
            Lista<tupString<Lista<Conj<diccString<Dato> >::Iterador> > >::const_Iterador temp = _indicesString.vistaDicc();
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
                    Lista<Conj<Registro>::Iterador> conjIters = _indicesNat.obtener(valorIndex);
                    Lista<Conj<Registro>::Iterador>::Iterador itDeIters = conjIters.CrearIt();
                    while (itDeIters.HaySiguiente()) {
                        if (itDeIters.Siguiente().Siguiente() == regi) {
                            itDeIters.EliminarSiguiente();
                        }
                        else itDeIters.Avanzar();
                        //FALTA AVANZAR?? FIJARSE!
                    }
                } else if (!_campoIndexadoString.EsVacia()) {

                    Registro regi = iter.Siguiente();
                    string valorIndex = regi.obtener(_campoIndexadoString.Primero().campo).dame_valorStr();
                    Lista<Conj<Registro>::Iterador> conjIters = _indicesString.obtener(valorIndex);
                    Lista<Conj<Registro>::Iterador>::Iterador itDeIters = conjIters.CrearIt();
                    while (itDeIters.HaySiguiente()) {
                        if (itDeIters.Siguiente().Siguiente() == regi) {
                            itDeIters.EliminarSiguiente();
                        }
                        else itDeIters.Avanzar();
                        //FALTA AVANZAR?? FIJARSE!
                    }

                }
                iter.EliminarSiguiente();
            }
            else iter.Avanzar();
        }

    }
}

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
            _indicesNat.obtener(aux.dame_valorNat()).AgregarAtras(it);
        } else {
            Lista< Conj<Registro>::Iterador > nuevaLista;
            nuevaLista.AgregarAtras(it);
            _indicesNat.definir(aux.dame_valorNat(), nuevaLista);
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
            _indicesString.obtener(aux.dame_valorStr()).AgregarAtras(it);
        } else {
            Lista< Conj<Registro>::Iterador > nuevaLista;
            nuevaLista.AgregarAtras(it);
            _indicesString.definir(aux.dame_valorStr(), nuevaLista);
        }
    }
}

void Tabla::indexar(const string& c) {
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
                Lista<Conj<Registro>::Iterador> lista_iters = Lista<Conj<Registro>::Iterador>();
                _indicesNat.definir(temp, lista_iters);
            }
            _indicesNat.obtener(temp).AgregarAtras(it);
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
                Lista<Conj<Registro>::Iterador> lista_iters = Lista<Conj<Registro>::Iterador>();
                _indicesString.definir(temp, lista_iters);
            }
            _indicesString.obtener(temp).AgregarAtras(it);
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

Lista<Registro> Tabla::buscar(const string& c, const Dato& d) const {
    Lista<Registro> res;
    if (d.esNat()) {
        if (!_campoIndexadoNat.EsVacia() && _campoIndexadoNat.Primero().campo == c) {
            if (_indicesNat.def(d.dame_valorNat())) {
                Lista< Conj<Registro>::Iterador >::const_Iterador itConjIts = _indicesNat.obtener(d.dame_valorNat()).CrearIt();
                while (itConjIts.HaySiguiente()) {
                    res.AgregarAtras(itConjIts.Siguiente().Siguiente());
                    itConjIts.Avanzar();
                }
                return res;
            }
        }
    } else {
        if (!_campoIndexadoString.EsVacia() && _campoIndexadoString.Primero().campo == c) {
            if (_indicesString.def(d.dame_valorStr())) {
                Lista< Conj<Registro>::Iterador >::const_Iterador itConjIts2 = (_indicesString.obtener(d.dame_valorStr())).CrearIt();
                while (itConjIts2.HaySiguiente()) {
                    res.AgregarAtras(itConjIts2.Siguiente().Siguiente());
                    itConjIts2.Avanzar();
                }
                return res;
            }
        }
    }

    Conj<Registro>::const_Iterador it = _registros.CrearIt();
    while (it.HaySiguiente()) {
        if (it.Siguiente().obtener(c) == d) {
            res.AgregarAtras(it.Siguiente());
        }
        it.Avanzar();
    }

    return res;
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

ostream& operator<<(ostream& os, const Tabla& t) {
  os << "Nombre: " << t.nombre() << endl;
  os << "Cant Accesos: " << t.cantidadDeAccesos() << endl;
  Lista<tupString<bool> >::const_Iterador it = t.campos();
  os << "Campos:";
  while(it.HaySiguiente()){
    os << " | " << it.Siguiente().clave << "(" << it.Siguiente().significado << ")";
    it.Avanzar();
  }
  os << " | " << endl;
  os << "Indices: " << t.indices() << endl;
  os << "" << endl;
  os << "Registros:   " << endl;
  Conj<Registro>::const_Iterador iter = t.registros().CrearIt();
  while(iter.HaySiguiente()){
    std::cout << "-> | ";
    Conj<string> camposReg = iter.Siguiente().claves();
    Conj<string>::const_Iterador superIt = camposReg.CrearIt();
    while(superIt.HaySiguiente()){
        Registro reg = iter.Siguiente();
        os << reg.obtener(superIt.Siguiente());
        superIt.Avanzar();
        if (superIt.HaySiguiente() ) os << " | ";
    }
    os << " | " << endl;
    iter.Avanzar();
  }
  return os;
}

#endif
