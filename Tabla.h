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
    ~Tabla();

    void agregarRegistro(Registro r);
    void borrarRegistro(Registro cr);
    void indexar(const string& c);
    string nombre() const;
    Conj<string> claves() const;
    Lista<Registro> buscar(const string& campo, const Dato& d) const;
    Conj<string> indices() const;
    const Conj<string> campos() const;
    bool tipoCampo(const string& campo) const; //const bool->no me cambien los tipos desde afuera
    const Conj<Registro>& registros() const;
    unsigned int cantidadDeAccesos() const;
    const Dato& minimo(string campo) const;
    const Dato& maximo(string campo) const;

    friend ostream& operator<<(ostream& os, const Tabla& t);
    Tabla operator=(const Tabla& t);

    struct InfoIndice {
      InfoIndice(string c, Dato mx, Dato mn, bool v) : campo(c), max(mx), min(mn), vacio(v) {};
      string campo;
      Dato max;
      Dato min;
      bool vacio;
    };
    struct IteradoresIndices {
      IteradoresIndices(Conj<Registro>::Iterador it) : itReg(it), itNat(NULL), itString(NULL) {}
      Conj<Registro>::Iterador itReg;
      Lista<Lista<IteradoresIndices>::Iterador> ::Iterador* itNat;
      Lista<Lista<IteradoresIndices>::Iterador> ::Iterador* itString;
    };
    Lista<IteradoresIndices> _iteradores;
private:

    diccString<bool> _campos;
    diccString<Lista<Lista<IteradoresIndices>::Iterador> > _indicesString;
    diccNat<Lista<Lista<IteradoresIndices>::Iterador> > _indicesNat;
    string _nombre;
    Conj<string> _claves;
    Lista<InfoIndice> _campoIndexadoNat;
    Lista<InfoIndice> _campoIndexadoString;
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

Tabla::~Tabla(){
  Lista<IteradoresIndices>::Iterador destructive_It = _iteradores.CrearIt();
  while ( destructive_It.HaySiguiente() ){
    if ( destructive_It.Siguiente().itNat != NULL ) delete destructive_It.Siguiente().itNat;
    if ( destructive_It.Siguiente().itString != NULL ) delete destructive_It.Siguiente().itString;
    destructive_It.Avanzar();
  }
}

Tabla Tabla::operator=(const Tabla& t){
  _cantAccesos = t._cantAccesos;
  _campos = t._campos;
  _nombre = t._nombre;
  _claves = t._claves;
  _campoIndexadoNat = t._campoIndexadoNat;
  _campoIndexadoString = t._campoIndexadoNat;
  Conj<Registro>::const_Iterador it = t._registros.CrearIt();
  while ( it.HaySiguiente() ) {
    std::cout << "AGREGADO:" << it.Siguiente() << std::endl;
    agregarRegistro(it.Siguiente());
    it.Avanzar();
  }
  return *this;
}

string Tabla::nombre() const {
    return _nombre;
}

Conj<string> Tabla::claves() const {
    return _claves;
}

Conj<string> Tabla::indices() const {
    Conj<string> res;
    if (_campoIndexadoNat.Longitud() > 0) {
        res.AgregarRapido(_campoIndexadoNat.Primero().campo);
    }
    if (_campoIndexadoString.Longitud() > 0) {
        res.AgregarRapido(_campoIndexadoString.Primero().campo);
    }
    return res;
}

const Conj<string> Tabla::campos() const {
  return _campos.claves();
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
            _cantAccesos++;
            Lista<Lista<IteradoresIndices>::Iterador> ::Iterador it = _indicesNat.obtener(dato.dame_valorNat()).CrearIt();
            Lista<IteradoresIndices>::Iterador iterador = it.Siguiente();

            iterador.Siguiente().itNat->EliminarSiguiente();
            delete iterador.Siguiente().itNat;
            _indicesNat.borrar(dato.dame_valorNat());

            Registro regi = iterador.Siguiente().itReg.Siguiente();
            if (iterador.Siguiente().itString != NULL){
                iterador.Siguiente().itString->EliminarSiguiente();
                delete iterador.Siguiente().itString;
                string valorIndex = regi.obtener(_campoIndexadoString.Primero().campo).dame_valorStr();
                if ( _indicesString.obtener(valorIndex).EsVacia() ){
                    _indicesString.borrar(valorIndex);
                }
            }
            iterador.Siguiente().itReg.EliminarSiguiente();
            iterador.EliminarSiguiente();


        }
    } else if (!_campoIndexadoString.EsVacia() && _campoIndexadoString.Primero().campo == clave) {
        if ( _indicesString.def(dato.dame_valorStr()) ) {
            _cantAccesos++;
            Lista<Lista<IteradoresIndices>::Iterador> ::Iterador it = _indicesString.obtener(dato.dame_valorStr()).CrearIt();
            Lista<IteradoresIndices>::Iterador iterador = it.Siguiente();

            iterador.Siguiente().itString->EliminarSiguiente();
            delete iterador.Siguiente().itString;
            _indicesString.borrar(dato.dame_valorStr());

            Registro regi = iterador.Siguiente().itReg.Siguiente();
            if (iterador.Siguiente().itNat != NULL){
                iterador.Siguiente().itNat->EliminarSiguiente();
                delete iterador.Siguiente().itNat;
                unsigned int valorIndex = regi.obtener(_campoIndexadoNat.Primero().campo).dame_valorNat();
                if ( _indicesNat.obtener(valorIndex).EsVacia() ){
                    _indicesNat.borrar(valorIndex);
                }
            }
            iterador.Siguiente().itReg.EliminarSiguiente();


        }
    } else {
        //Modificación fuerte respecto de diseño...
        Lista<IteradoresIndices>::Iterador iter = _iteradores.CrearIt();
        while (iter.HaySiguiente()) {
            Registro regi = iter.Siguiente().itReg.Siguiente();
            if (regi.obtener(clave) == dato) {
                if (iter.Siguiente().itNat != NULL){
                    iter.Siguiente().itNat->EliminarSiguiente();
                    delete iter.Siguiente().itNat;
                    unsigned int valorIndex = regi.obtener(_campoIndexadoNat.Primero().campo).dame_valorNat();
                    if ( _indicesNat.obtener(valorIndex).EsVacia() ){
                        _indicesNat.borrar(valorIndex);
                    }
                }
                if (iter.Siguiente().itString != NULL){
                    iter.Siguiente().itString->EliminarSiguiente();
                    delete iter.Siguiente().itString;
                    string valorIndex = regi.obtener(_campoIndexadoString.Primero().campo).dame_valorStr();
                    if ( _indicesString.obtener(valorIndex).EsVacia() ){
                        _indicesString.borrar(valorIndex);
                    }
                }
                iter.Siguiente().itReg.EliminarSiguiente();
                iter.EliminarSiguiente();
            }
            else iter.Avanzar();
        }
    }
    //Esto antes estaba en cada rama de indice, ahora es global
     if (!_campoIndexadoNat.EsVacia()) {
        diccNat<Lista<Lista<IteradoresIndices>::Iterador> >::Iterador temp_Nat = _indicesNat.crearIt();
        if (!temp_Nat.hayMas()) {
                _campoIndexadoNat.Primero().vacio = true;
        } else {
                Dato max = Dato::datoNat(_indicesNat.max().clave);
                _campoIndexadoNat.Primero().max = max;
                Dato min = Dato::datoNat(_indicesNat.min().clave);
                _campoIndexadoNat.Primero().min = min;
        }
    }
    if (!_campoIndexadoString.EsVacia()) {
        Lista<tupString<Lista<Lista<IteradoresIndices>::Iterador> > >::const_Iterador temp_Str = _indicesString.vistaDicc();
        if (! temp_Str.HaySiguiente() ) {
            _campoIndexadoString.Primero().vacio = true;
        } else {
                Dato max = Dato::datoString(_indicesString.max());
                _campoIndexadoString.Primero().max = max;
                Dato min = Dato::datoString(_indicesString.min());
                _campoIndexadoString.Primero().min = min;
        }
    }
}

void Tabla::agregarRegistro(Registro r) {
    _cantAccesos++;
    Conj<Registro>::Iterador iter = _registros.AgregarRapido(r);
    IteradoresIndices ag = IteradoresIndices(iter);
    Lista<IteradoresIndices>::Iterador it = _iteradores.AgregarAtras(ag);

    if (!_campoIndexadoNat.EsVacia()) {
        if (_campoIndexadoNat.Primero().vacio) {
            _campoIndexadoNat.Primero().min = r.obtener(_campoIndexadoNat.Primero().campo);
            _campoIndexadoNat.Primero().max = r.obtener(_campoIndexadoNat.Primero().campo);
            _campoIndexadoNat.Primero().vacio = false;
        } else {
            unsigned int nPaMinMax = r.obtener(_campoIndexadoNat.Primero().campo).dame_valorNat();
            if (nPaMinMax < _campoIndexadoNat.Primero().min.dame_valorNat()) {
                _campoIndexadoNat.Primero().min =r.obtener(_campoIndexadoNat.Primero().campo);
            }
            if (nPaMinMax > _campoIndexadoNat.Primero().max.dame_valorNat()) {
                _campoIndexadoNat.Primero().max = r.obtener(_campoIndexadoNat.Primero().campo);
            }
        }
        Dato aux = r.obtener(_campoIndexadoNat.Primero().campo);
        if (_indicesNat.def(aux.dame_valorNat())) {
            Lista<Lista<IteradoresIndices>::Iterador>::Iterador iter_nat = _indicesNat.obtener(aux.dame_valorNat()).AgregarAtras(it);
            it.Siguiente().itNat = new Lista<Lista<IteradoresIndices>::Iterador>::Iterador(iter_nat);
        } else {
            Lista<Lista<IteradoresIndices>::Iterador> nuevaLista;
            Lista<Lista<IteradoresIndices>::Iterador>::Iterador iter_nat = nuevaLista.AgregarAtras(it);
            it.Siguiente().itNat = new Lista<Lista<IteradoresIndices>::Iterador>::Iterador(iter_nat);
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
            if (sPaMinMax < _campoIndexadoString.Primero().min.dame_valorStr()) {
                _campoIndexadoString.Primero().min = r.obtener(_campoIndexadoString.Primero().campo);
            }
            if (sPaMinMax > _campoIndexadoString.Primero().max.dame_valorStr()) {
                _campoIndexadoString.Primero().max = r.obtener(_campoIndexadoString.Primero().campo);
            }
        }
        Dato aux = r.obtener(_campoIndexadoString.Primero().campo);
        if (_indicesString.def(aux.dame_valorStr())) {
            Lista<Lista<IteradoresIndices>::Iterador>::Iterador iter_str = _indicesString.obtener(aux.dame_valorStr()).AgregarAtras(it);
            it.Siguiente().itString = new Lista<Lista<IteradoresIndices>::Iterador>::Iterador(iter_str);
        } else {
            Lista<Lista<IteradoresIndices>::Iterador> nuevaLista;
            Lista<Lista<IteradoresIndices>::Iterador>::Iterador iter_str = nuevaLista.AgregarAtras(it);
            _indicesString.definir(aux.dame_valorStr(), nuevaLista);
        }
    }
}

void Tabla::indexar(const string& c) {
    if ( tipoCampo(c) ) {
        Dato dato = Dato::datoNat(0);
        _campoIndexadoNat.AgregarAdelante(InfoIndice(c, dato, dato, true));
        Lista<IteradoresIndices>::Iterador it = _iteradores.CrearIt();
        if (it.HaySiguiente()) {
            _campoIndexadoNat.Primero().vacio = false;
            _campoIndexadoNat.Primero().max = it.Siguiente().itReg.Siguiente().obtener(c);
            _campoIndexadoNat.Primero().min = it.Siguiente().itReg.Siguiente().obtener(c);
        }
        while (it.HaySiguiente()) {
            unsigned int temp = it.Siguiente().itReg.Siguiente().obtener(c).dame_valorNat();
            if (!_indicesNat.def(temp)) {
                Lista<Lista<IteradoresIndices>::Iterador> lista_iters = Lista<Lista<IteradoresIndices>::Iterador>();
                _indicesNat.definir(temp, lista_iters);
            }
            Lista<Lista<IteradoresIndices>::Iterador>::Iterador it_nat  = _indicesNat.obtener(temp).AgregarAtras(it);
            it.Siguiente().itNat = new Lista<Lista<IteradoresIndices>::Iterador>::Iterador(it_nat);

            if (it.Siguiente().itReg.Siguiente().obtener(c) > _campoIndexadoNat.Primero().max) {
                _campoIndexadoNat.Primero().max = it.Siguiente().itReg.Siguiente().obtener(c);
            }
            if (it.Siguiente().itReg.Siguiente().obtener(c) < _campoIndexadoNat.Primero().min) {
                _campoIndexadoNat.Primero().min = it.Siguiente().itReg.Siguiente().obtener(c);
            }
            it.Avanzar();
        }

    } else {
        Dato dato = Dato::datoString("temp");
        _campoIndexadoString.AgregarAdelante(InfoIndice(c, dato, dato, true));
        Lista<IteradoresIndices>::Iterador it = _iteradores.CrearIt();
        if (it.HaySiguiente()) {
            _campoIndexadoString.Primero().vacio = false;
            _campoIndexadoString.Primero().max = it.Siguiente().itReg.Siguiente().obtener(c);
            _campoIndexadoString.Primero().min = it.Siguiente().itReg.Siguiente().obtener(c);
        }

        while (it.HaySiguiente()) {
            string temp =  it.Siguiente().itReg.Siguiente().obtener(c).dame_valorStr();
            if (!_indicesString.def(temp)) {
                Lista<Lista<IteradoresIndices>::Iterador> lista_iters = Lista<Lista<IteradoresIndices>::Iterador>();
                _indicesString.definir(temp, lista_iters);
            }
            Lista<Lista<IteradoresIndices>::Iterador>::Iterador it_str = _indicesString.obtener(temp).AgregarAtras(it);
            it.Siguiente().itString = new Lista<Lista<IteradoresIndices>::Iterador>::Iterador(it_str);

            if (it.Siguiente().itReg.Siguiente().obtener(c) > _campoIndexadoString.Primero().max) {
                _campoIndexadoString.Primero().max = it.Siguiente().itReg.Siguiente().obtener(c);
            }
            if (it.Siguiente().itReg.Siguiente().obtener(c) < _campoIndexadoString.Primero().min) {
                _campoIndexadoString.Primero().min = it.Siguiente().itReg.Siguiente().obtener(c);
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
                Lista<Lista<IteradoresIndices>::Iterador>::const_Iterador itConjIts = _indicesNat.obtener(d.dame_valorNat()).CrearIt();
                while (itConjIts.HaySiguiente()) {
                    res.AgregarAtras(itConjIts.Siguiente().Siguiente().itReg.Siguiente());
                    itConjIts.Avanzar();
                }
                return res;
            }
        }
    } else {
        if (!_campoIndexadoString.EsVacia() && _campoIndexadoString.Primero().campo == c) {
            if (_indicesString.def(d.dame_valorStr())) {
                Lista<Lista<IteradoresIndices>::Iterador>::const_Iterador itConjIts2 = (_indicesString.obtener(d.dame_valorStr())).CrearIt();
                while (itConjIts2.HaySiguiente()) {
                    res.AgregarAtras(itConjIts2.Siguiente().Siguiente().itReg.Siguiente());
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
  os << "----------------------------" << endl;
  os << "     " << "Nombre: " << t.nombre() << endl;
  os << "----------------------------" << endl;

  os << "Cant Accesos: " << t.cantidadDeAccesos() << endl;

  os << "Campos: ";
  Conj<string>::const_Iterador it = t.campos().CrearIt();
  while(it.HaySiguiente()){
    os << " | " << it.Siguiente() << "(" << t.tipoCampo(it.Siguiente()) << ")";
    it.Avanzar();
  }
  os << " | " << endl;

  os << "Claves: " << t.claves() << endl;

  os << "Indices: " << t.indices() << endl;
  //Conj<string>::const_Iterador iti = indices.CrearIt();
  //while ( iti.HaySiguiente() ) { //itera dos veces nomás
  //  os << "Max/Min sobre " << iti.Siguiente() << ": " << t.maximo(iti.Siguiente()) << "/" << t.minimo(iti.Siguiente()) << endl;
  //  iti.Avanzar();
  //}
  if (!t._campoIndexadoNat.EsVacia() && !t._campoIndexadoNat.Primero().vacio){
    os << "Min/Max para " << t._campoIndexadoNat.Primero().campo << ": ";
    os << t._campoIndexadoNat.Primero().min << "/" << t._campoIndexadoNat.Primero().max << endl;
  }
  if (!t._campoIndexadoString.EsVacia() && !t._campoIndexadoString.Primero().vacio){
    os << "Min/Max para " << t._campoIndexadoString.Primero().campo << ": ";
    os << t._campoIndexadoString.Primero().min << "/" << t._campoIndexadoString.Primero().max << endl;
  }

  os << endl << "Registros:   " << endl;
  Conj<Registro>::const_Iterador iter = t.registros().CrearIt();
  while(iter.HaySiguiente()){
    os << "-> | ";
    Conj<string> camposReg = iter.Siguiente().claves();
    Conj<string>::const_Iterador superIt = camposReg.CrearIt();
    while(superIt.HaySiguiente()){
        Registro reg = iter.Siguiente();
        os << superIt.Siguiente() << ": " << reg.obtener(superIt.Siguiente());
        superIt.Avanzar();
        if (superIt.HaySiguiente() ) os << " | ";
    }
    os << " | " << endl;
    iter.Avanzar();
  }
  return os;
}

#endif
