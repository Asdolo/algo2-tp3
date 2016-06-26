
#ifndef BASEDEDATOS_H_INCLUDED
#define BASEDEDATOS_H_INCLUDED

#include "TiposBasicos.h"

    class BaseDeDatos{

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
            insertarEntrada(Registro r, String s);

            Borrar(Registro cr,String t);

            Conj(Regisro) combinarRegistros(String t1,String t2,String campo);

            Iterador(Conj(Registro)) generarVistaJoin(String t1, String t2, String campo);

            BorrarJoin(String t1, String t2);

            Iterador(Conj(String)) Tablas();

            Tabla dameTabla(String t);

            bool hayJoin(String t1, String t2);

            String campoJoin(String t1, String t2);

            Registro Merge(Registro r1, Registro r2);

            Iterador(Conj(Registro)) vistaJoin(String t1, String t2);

           	Conj(Registro) busquedaCriterio(Registro cr, String t);

           	bool coincidenTodosCrit(Registro crit, Registro r);

           	String tablaMaxima();


        private:
            String* tablaMasAccedida;
            diccString(Tabla) nombreATabla;
            Conj(String) tablas;
            diccString(diccString(diccNat(Iterador(Conj(Registro))))) joinPorCampoNat;
            diccString(diccString(diccString(Iterador(Conj(Registro))))) joinPorCampoString;
            diccString(diccString(Conj(Registro))) registrosDelJoin;
            diccString(diccString(Tupla(String campoJoin,Lista(Tupla(Registro reg, bool agregar)) cambiosT1,Lista(Tupla(Registro reg, bool agregar)) cambiosT2))) hayJoin;
    };

   
    bool operator==(const Dato& d1, const Dato<T>& d2);

    template<class T>
    std::ostream& operator<<(std::ostream& os, const Dato&);


BaseDeDatos::BaseDeDatos() : tablaMasAccedida(NULL), nombreATabla(vacio()),tablas(vacia()),hayJoin(vacio()),joinPorCampoString(vacio()),joinPorCampoNat(vacio()),registrosDelJoin(vacio()) {}


void BaseDeDatos::agregarTabla(String t){
	if (tablaMasAccedida==NULL || nombreATabla(*tablaMasAccedida).cantidadDeAccesos < t.cantidadDeAccesos){
		tablaMasAccedida= &(t.nombre);

		}
		definir(t.nombre,t,nombreATabla);
		AgregarRapido(t.nombre,tablas);
		definir(t.nombre,vacio(),hayJoin);
		definir(t.nombre,vacio(),joinPorCampoNat);
		definir(t.nombre,vacio(),joinPorCampoString);
		definir(t.nombre,vacio(),registrosDelJoin);
}

void BaseDeDatos::insertarEntrada(){
	Tabla tabla = obtener(t,nombreATabla);
	tabla.agregarRegistro(r);
	Tabla tabMax = obtener(*(tablaMasAccedida),nombreATabla);
	if (tabla.cantidadDeAccesos>tabMax.cantidadDeAccesos)
	{
		tablaMasAccedida = &t;
	}
Iterador iter = vistaDicc(obtener(t,hayJoin));

while(iter.haySiguiente()){
	AgregarAtras(Tupla(r,true),(iter.Siguiente().significado.cambiosT1));
	iter.Avanzar();
}
iter = crearIt(tablas);
while(iter.haySiguiente()){
	if (def(t,obtener(iter.Siguiente(),hayJoin)))
	{
		??? cambios = obtener(t,obtener(iter.Siguiente(),hayJoin));
	AgregarAtras(Tupla(r,true),(iter.Siguiente().significado.cambiosT2));
	}
	iter.Avanzar();
}
}

void BaseDeDatos::borrar(Registro cr, String t){
	Tabla tabla = obtener(t,nombreATabla);
	t.borrarRegistro(cr);
	Tabla tabMax = obtener(*(tablaMasAccedida),nombreATabla);
	if (tabla.cantidadDeAccesos>tabMax.cantidadDeAccesos)
	{
		tablaMasAccedida = &t;
	}
Iterador iter = vistaDicc(obtener(t,hayJoin));
while(iter.haySiguiente()){
	AgregarAtras(Tupla(r,false),(iter.Siguiente().significado.cambiosT1));
	iter.Avanzar();
}
while(iter.haySiguiente()){
	if (def(t,obtener(iter.Siguiente(),hayJoin)))
	{
		??? cambios = obtener(t,obtener(iter.Siguiente(),hayJoin));
	AgregarAtras(Tupla(r,false),(iter.Siguiente().significado.cambiosT2));
	}
	iter.Avanzar();
}

}





}



#endif
