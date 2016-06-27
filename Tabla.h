#ifndef TABLA_H_INCLUDED
#define TABLA_H_INCLUDED
#define Registro diccString<Dato>
#include "aed2.h"
#include <cassert>
#include <string>
#include <iostream>

using namespace std;

class Tabla {

public:
	string nombre();Iterador(string) claves();Lista(Registro) buscar(Strin campo,Dato d);Conj(string) Indices();Iterador(string) campos();
	bool tipoCampo(string campo);Conj(Registro) registros();
	unsigned int cantidadDeAccesos();Tabla(string nombre, Conj(string) campos,Registro columnas);agregarRegistro(Registro r);borrarRegistro(Registro cr);indexar(string c);
	Dato minimo(string campo);
	Dato maximo(string campo);

private:
	diccString(Conj(Iterador(Conj(Registro)))) indicesString;diccNat(Conj(Iterador(Conj(Registro)))) indicesNat;
	string nombre;diccString(bool esNat) campos;Conj(string) claves;lista(Tupla(string campo,Dato max,Dato min, bool vacio)) campoIndexadoNat;lista(Tupla(string campo,Dato max,Dato min, bool vacio)) campoIndexadoString;
	unsigned int cantAccesos;

};

Tabla::Tabla(string nombre, Conj<string> claves, Registro columnas) :
	indicesString(Vacio()), indicesNat(Vacio()), registros(Vacio()),
			nombre(copiar(nombre)), campos(Vacio()), claves(copiar(claves)),
			campoIndexadoNat(Vacio()), campoIndexadoString(Vacio()),
			cantAccesos(0) {
	Iterador iter = columnas.vistaDicc();
	while (iter.HaySiguiente()) {
		definir(iter.siguiente().clave,
				nat(iter.Siguiente().significado, campos));
		iter.Avanzar();
	}

}

string Tabla::nombre() {
	return nombre;
}

Iterador(Conj(string)) Tabla::claves() {
	return CrearIt(claves);
}

Conj<string> Tabla::indices() {
	Conj(string) res;
	if (campoIndexadoNat.longitud() > 0) {
		res.agregarRapido(campoIndexadoNat.primero().nombre, res);

	}
	if (campoIndexadoString.longitud() > 0) {
		res.agregarRapido(campoIndexadoString.primero().nombre, res);
	}
	return res;
}

Conj<string>::Iterador Tabla::campos() {
	return Conj<string>::CrearIt(campos);
}

bool Tabla::tipoCampo(string c) {
	return campos.obtener(c);
}

Conj<Registro> Tabla::registros() {
	return registros;
}

unsigned int Tabla::cantidadDeAccesos() {
	return cantAccesos;
}

void Tabla::borrarRegistro(Registro criterio) {
	Iterador it = Conj<>::CrearIt(criterio.vistaDicc());
	??? clave = it.Siguiente().clave;
	Dato dato = it.Siguiente().significado;

	if (campoIndexadoNat.primero().nombre == clave) {
		if (indicesNat.def(dato.valorNat())) {
			Iterador iterador = ::CrearIt(indicesNat.obtener(dato.valorNat()));
			iterador.Siguiente().EliminarSiguiente();
			cantAccesos++;
			indicesNat.borrar(dato.valorNat());
			Iterador temp = ::CrearIt(indicesNat);
			if (!temp.HaySiguiente()) {
				campoIndexadoNat.primero().vacio = true;
			} else {
				if (dato.valorNat() == campoIndexadoNat.primero().max) {
					campoIndexadoNat.primero().max
							= indicesNat.maximo().fst().datoNat();
				}
				if (dato.valorNat() == campoIndexadoNat.primero().min) {
					campoIndexadoNat.primero().min
							= indicesNat.minimo().fst().datoNat();
				}
			}
		}

	} else if (campoIndexadoString.primero().nombre == clave) {
		if (indicesString.def(dato.valorString())) {
			Iterador iterador = ::CrearIt(indicesString.obtener(valorString()));
			iterador.Siguiente().EliminarSiguiente();
			cantAccesos++;
			indicesString.borrar(dato.valorString());
			Iterador temp = ::CrearIt(indicesString);
			if (!temp.HaySiguiente()) {
				campoIndexadoString.primero().vacio = true;
			} else {
				if (dato = campoIndexadoString.primero().max) {
					campoIndexadoString.primero().max
							= Maximo(indicesString).fst().datoString();
				}
				if (dato = campoIndexadoString.primero().min) {
					campoIndexadoString.primero().min
							= minimo(indicesString).fst().datoString();
				}

			}

		}

	} else {
		Iterador iter = Registro::CrearIt(registros);
		while (iter.HaySiguiente()) {
			if (iter.Siguiente.obtener(clave) == dato) {
				if (!campoIndexadoNat.vacia()) {
					Registro regi = iter.Siguiente();
					?? valorIndex = regi.obtener(campoIndexadoNat.primero().nombre).valorNat();
					?? conjIters = indicesNat.obtener(valorIndex);
					?? itDeIters = ::CrearIt(conjIters);
					while (itDeIters.HaySiguiente()) {
						if (itDeIters.Siguiente().Siguiente() == regi) {
							itDeIters.EliminarSiguiente();
						}
						//FALTA AVANZAR?? FIJARSE!
						itDeIters.Avanzar();
					}
				} else if (!campoIndexadoString.vacia()) {

					Registro regi = iter.Siguiente();
					?? valorIndex = regi.obtener(campoIndexadoString.primero().nombre).valorString();
					?? conjIters = indicesString.obtener(valorIndex);
					?? itDeIters = ::CrearIt(conjIters);
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
	if (!campoIndexadoNat.vacia()) {
		if (campoIndexadoNat.primero().vacio) {
			campoIndexadoNat.primero().min = r.obtener(
					campoIndexadoNat.primero());
			campoIndexadoNat.primero().max = (campoIndexadoNat.primero()).min;//REVISAR LINEA!
			campoIndexadoNat.primero().vacio = false;

		} else {
			unsigned int nPaMinMax = r.obtener(
					campoIndexadoNat.primero().nombre);
			if (nPaMinMax > campoIndexadoNat.primero().min) {
				campoIndexadoNat.primero().min = nPaMinMax;
			}
			if (nPaMinMax > campoIndexadoNat.primero().max) {
				campoIndexadoNat.primero().max = nPaMinMax;
			}
		}
		?? aux = r.obtener(campoIndexadoNat.primero());
		if (indicesNat.def(aux)) {
			indicesNat.obtener(aux).agregarRapido(it);
		} else {
			indicesNat.definir(aux, Conj::vacio().agregarRapido(it));
		}
	}

	if (!campoIndexadoString.vacia()) {
		if (campoIndexadoString.primero().vacio) {
			campoIndexadoString.primero().min = r.obtener(
					campoIndexadoString.primero());
			campoIndexadoString.primero().max
					= (campoIndexadoString.primero()).min;//REVISAR LINEA!
			campoIndexadoString.primero().vacio = false;

		} else {
			string sPaMinMax = r.obtener(campoIndexadoString.primero().nombre);
			if (sPaMinMax > campoIndexadoString.primero().min) {
				campoIndexadoString.primero().min = sPaMinMax;
			}
			if (sPaMinMax > campoIndexadoString.primero().max) {
				campoIndexadoString.primero().max = sPaMinMax;
			}
		}
		?? aux = r.obtener(campoIndexadoString.primero());
		if (indicesString.def(aux)) {
			indicesString.obtener(aux).agregarRapido(it);
		} else {
			indicesString.definir(aux, Conj::vacio().agregarRapido(it));
		}
	}

}

void Tabla::indexar(string c) {
	if (this.tipoCampo(c)) {
		Dato dato = Dato::DatoNat(0);
		campoIndexadoNat.agregarAdelante(Tupla(c, dato, dato, true));
		Iterador it = ::CrearIt(registros);
		if (it.HaySiguiente()) {
			campoIndexadoNat.primero().vacio = false;
			campoIndexadoNat.primero().max = it.Siguiente().obtener(c);
			campoIndexadoNat.primero().min = it.Siguiente().obtener(c);
		}
		while (it.HaySiguiente()) {
			unsigned int temp = it.Siguiente().obtener(c).valorNat();
			if (! ???.def(indicesNat.obtener(temp))) {
				indicesNat.definir(temp, ::vacio());
			}
			indicesNat.obtener(temp).agregarRapido(it);
			if (it.Siguiente().obtener(c) > campoIndexadoNat.primero().max) {
				campoIndexadoNat.primero().max = it.siguiente().obtener(c);
			}
			if (it.siguiente().obtener(c) < campoIndexadoNat.primero().min) {
				campoIndexadoNat.primero().min = it.Siguiente().obtener(c);
			}
			it.Avanzar();

		}

	} else {
		Dato dato = Dato::datoString("temp");
		campoIndexadoString.agregarAdelante(Tupla(c, dato, dato, true));
		Iterador it = ::CrearIt(registros);
		if (it.HaySiguiente()) {
			campoIndexadoString.primero().vacio = false;
			campoIndexadoString.primero().max = it.Siguiente().obtener(c);
			campoIndexadoString.primero().min = it.Siguiente().obtener(c);
		}

		while (it.HaySiguiente()) {
			string temp = it.Siguiente().obtener(c).valorString();
			if (! ???.def(indicesString.obtener(temp))) {
				indicesString.definir(temp, ::vacio());
			}
			indicesString.obtener(temp).agregarRapido(it);
			if (it.Siguiente().obtener(c) > campoIndexadoString.primero().max) {
				campoIndexadoString.primero().max = it.siguiente().obtener(c);
			}
			if (it.siguiente().obtener(c) < campoIndexadoString.primero().min) {
				campoIndexadoString.primero().min = it.Siguiente().obtener(c);
			}
			it.Avanzar();

		}

	}
}

Lista(Registro) Tabla::buscar(string c,Dato d){
	Lista res = Lista::vacia();
	if (d.esNat)
	{
		if (campoIndexadoNat.primero().nombre=c)
		{
			if (indicesNat.def(d.valorNat()))
			{
				Iterador itConjIts = indicesNat.obtener(d.valorNat())::CrearIt();
				while(itConjIts.HaySiguiente()){
					res.agregarAtras(itConjIts.Siguiente().Siguiente());
					itConjIts.Avanzar();
				}
			}
		}else{
			if(campoIndexadoString.primero().nombre=c)
		{
			if (indicesString.def(d.valorString()))
			{
				Iterador itConjIts = indicesNat.obtener(d.valorNat())::CrearIt();
				while(itConjIts.HaySiguiente()){
					res.agregarAtras(itConjIts.Siguiente().Siguiente());
					itConjIts.Avanzar();
				}
			}



		}else{
			Iterador it = registros.CrearIt();
			while(it.HaySiguiente()){
				if (it.Siguiente().obtener(c)==d)
				{
					res.agregarAtras(it.Siguiente());
				}
				it.Avanzar();
			}
		}
	}
}


Dato Tabla::minimo(string c){
	Dato res;
	if (c== campoIndexadoNat.primero().nombre)
	{
		res = campoIndexadoNat.primero().min;
	}else{
				res = campoIndexadoString.primero().min;

	}
}


Dato Tabla::maximo(string c){
	Dato res;
	if (c== campoIndexadoNat.primero().nombre)
	{
		res = campoIndexadoNat.primero().max;
	}else{
				res = campoIndexadoString.primero().max;

	}
}

