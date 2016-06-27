#ifndef TABLA_H_INCLUDED
#define TABLA_H_INCLUDED

#include "aed2.h"
#include <cassert>
#include <string>
#include <iostream>

using namespace std;

    class Tabla{

        public:
        	string nombre();
        	Iterador(string) claves();
        	Lista(Registro) buscar(Strin campo,Dato d);
        	Conj(string) Indices();
        	Iterador(string) campos();
        	bool tipoCampo(string campo);
        	Conj(Registro) registros();
        	unsigned int cantidadDeAccesos();
        	nuevaTabla(string nombre, Conj(string) campos,Registro columnas);
        	agregarRegistro(Registro r);
        	borrarRegistro(Registro cr);
        	indexar(Strin c);
        	Dato minimo(string campo);
        	Dato maximo(string campo);

        private:
        	diccString(Conj(Iterador(Conj(Registro)))) indicesString;
        	diccNat(Conj(Iterador(Conj(Registro)))) indicesNat;
        	string nombre;
        	diccString(bool esNat) campos;
        	Conj(string) claves;
        	lista(Tupla(string campo,Dato max,Dato min, bool vacio)) campoIndexadoNat;
        	lista(Tupla(string campo,Dato max,Dato min, bool vacio)) campoIndexadoString;
        	unsigned int cantAccesos;



 };

 Tabla::nuevaTabla(string nombre,Conj(string) claves,Registro columnas) : indicesString(Vacio()), indicesNat(Vacio()),registros(Vacio()),nombre(copiar(nombre)),campos(Vacio()),claves(copiar(claves)),campoIndexadoNat(Vacio()),campoIndexadoString(Vacio()),cantAccesos(0) {
 	Iterador iter = columnas.vistaDicc();
 	while(iter.HaySiguiente()){
 		definir(iter.siguiente().clave,nat(iter.Siguiente().significado,campos));
 		iter.Avanzar();
 	}

 }

 //COMPLETAR ALGORITMOS!!