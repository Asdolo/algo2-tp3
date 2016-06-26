#ifndef TABLA_H_INCLUDED
#define TABLA_H_INCLUDED

#include "TiposBasicos.h"

    class Tabla{

        public:
        	String nombre();
        	Iterador(String) claves();
        	Lista(Registro) buscar(Strin campo,Dato d);
        	Conj(String) Indices();
        	Iterador(String) campos();
        	bool tipoCampo(String campo);
        	Conj(Registro) registros();
        	unsigned int cantidadDeAccesos();
        	nuevaTabla(String nombre, Conj(String) campos,Registro columnas);
        	agregarRegistro(Registro r);
        	borrarRegistro(Registro cr);
        	indexar(Strin c);
        	Dato minimo(String campo);
        	Dato maximo(String campo);

        private:
        	diccString(Conj(Iterador(Conj(Registro)))) indicesString;
        	diccNat(Conj(Iterador(Conj(Registro)))) indicesNat;
        	String nombre;
        	diccString(bool esNat) campos;
        	Conj(String) claves;
        	lista(Tupla(String campo,Dato max,Dato min, bool vacio)) campoIndexadoNat;
        	lista(Tupla(String campo,Dato max,Dato min, bool vacio)) campoIndexadoString;
        	unsigned int cantAccesos;



 };

 Tabla::nuevaTabla(String nombre,Conj(String) claves,Registro columnas) : indicesString(Vacio()), indicesNat(Vacio()),registros(Vacio()),nombre(copiar(nombre)),campos(Vacio()),claves(copiar(claves)),campoIndexadoNat(Vacio()),campoIndexadoString(Vacio()),cantAccesos(0) {
 	Iterador iter = columnas.vistaDicc();
 	while(iter.HaySiguiente()){
 		definir(iter.siguiente().clave,nat(iter.Siguiente().significado,campos));
 		iter.Avanzar();
 	}

 }

 //COMPLETAR ALGORITMOS!!