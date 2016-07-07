#include "Tabla.h"

using namespace std;
using namespace aed2;
using namespace tp3;


Tabla::Tabla(const string& nombre, Conj<string> claves, Registro_tp3 columnas) : _nombre(nombre), _claves(claves), _cantAccesos(0) {
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
    if ( destructive_It.Siguiente().itNat != NULL ) delete destructive_It.Siguiente().itNat; destructive_It.Siguiente().itNat = NULL;
    if ( destructive_It.Siguiente().itString != NULL ) delete destructive_It.Siguiente().itString;  destructive_It.Siguiente().itString = NULL;
    destructive_It.Avanzar();
  }
}

Tabla& Tabla::operator=(const Tabla& t){
  _campos = t._campos;
  _nombre = t._nombre;
  _claves = t._claves;
  _campoIndexadoNat = t._campoIndexadoNat;
  _campoIndexadoString = t._campoIndexadoString;
  Conj<Registro_tp3>::const_Iterador it = t._registros.CrearIt();
  while ( it.HaySiguiente() ) {
    agregarRegistro(it.Siguiente());
    it.Avanzar();
  }
  _cantAccesos = t._cantAccesos;
  return *this;
}

Tabla::Tabla(const Tabla& other){
  *this = other;
}

const string& Tabla::nombre() const {
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

const Conj<Registro_tp3>& Tabla::registros() const {
    return _registros;
}

unsigned int Tabla::cantidadDeAccesos() const {
    return _cantAccesos;
}

void Tabla::borrarRegistro(Registro_tp3 cr) {

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
            iterador.Siguiente().itNat = NULL;
            _indicesNat.borrar(dato.dame_valorNat());

           Registro_tp3 regi = iterador.Siguiente().itReg.Siguiente();
            if (iterador.Siguiente().itString != NULL){
                iterador.Siguiente().itString->EliminarSiguiente();
                delete iterador.Siguiente().itString;
                iterador.Siguiente().itString = NULL;
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
            iterador.Siguiente().itString = NULL;
            _indicesString.borrar(dato.dame_valorStr());

           Registro_tp3 regi = iterador.Siguiente().itReg.Siguiente();
            if (iterador.Siguiente().itNat != NULL){
                iterador.Siguiente().itNat->EliminarSiguiente();
                delete iterador.Siguiente().itNat;
                iterador.Siguiente().itNat = NULL;
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
           Registro_tp3 regi = iter.Siguiente().itReg.Siguiente();
            if (regi.obtener(clave) == dato) {
                if (iter.Siguiente().itNat != NULL){
                    iter.Siguiente().itNat->EliminarSiguiente();
                    delete iter.Siguiente().itNat;
                    iter.Siguiente().itNat = NULL;
                    unsigned int valorIndex = regi.obtener(_campoIndexadoNat.Primero().campo).dame_valorNat();
                    if ( _indicesNat.obtener(valorIndex).EsVacia() ){
                        _indicesNat.borrar(valorIndex);
                    }
                }
                if (iter.Siguiente().itString != NULL){
                    iter.Siguiente().itString->EliminarSiguiente();
                    delete iter.Siguiente().itString;
                    iter.Siguiente().itString = NULL;
                    string valorIndex = regi.obtener(_campoIndexadoString.Primero().campo).dame_valorStr();
                    if ( _indicesString.obtener(valorIndex).EsVacia() ){
                        _indicesString.borrar(valorIndex);
                    }
                }
                iter.Siguiente().itReg.EliminarSiguiente();
                iter.EliminarSiguiente();
                _cantAccesos++;
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

void Tabla::agregarRegistro(Registro_tp3 r) {
    _cantAccesos++;
    Conj<Registro_tp3>::Iterador iter = _registros.AgregarRapido(r);
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
            _indicesNat.definir(aux.dame_valorNat(), nuevaLista);
            Lista<Lista<IteradoresIndices>::Iterador>::Iterador iter_nat = _indicesNat.obtener(aux.dame_valorNat()).AgregarAtras(it);
            it.Siguiente().itNat = new Lista<Lista<IteradoresIndices>::Iterador>::Iterador(iter_nat);
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
            _indicesString.definir(aux.dame_valorStr(), nuevaLista);
            Lista<Lista<IteradoresIndices>::Iterador>::Iterador iter_str = _indicesString.obtener(aux.dame_valorStr()).AgregarAtras(it);
            it.Siguiente().itString = new Lista<Lista<IteradoresIndices>::Iterador>::Iterador(iter_str);
        }
    }
}

void Tabla::indexar(const string& c) {
    assert( tipoCampo(c)? _campoIndexadoNat.EsVacia() : _campoIndexadoString.EsVacia() );

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

Lista<Registro_tp3> Tabla::buscar(const string& c, const Dato& d) const {
    Lista<Registro_tp3> res;
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

    Conj<Registro_tp3>::const_Iterador it = _registros.CrearIt();
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

/* namespace tp3{

}
*/
