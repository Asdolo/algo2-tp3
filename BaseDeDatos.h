
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

Conj(Registro) combinarRegistros(String t1, String t2, String campo){
	Tabla tabla1= obtener(t1,nombreATabla);
		Tabla tabla2= obtener(t2,nombreATabla);
		Tabla tablaIt;
		Tabla tablaBusq;
		if (Indices(tabla1).pertenece(campo))
		{
			 tablaIt=tabla2;
			 tablaBusq=tabla1;		}
			 else{
				tablaIt=tabla1;
			 tablaBusq=tabla2;	

			}
			Conj(Registro) res= vacio();
			Iterador it = crearIt(Registros(tablaIt));
			Registro regMergeado;
			while(it.haySiguiente()){
				Dato d = obtener(c,it.Siguiente());
				?? coincis = buscar(campo,d,tablaBusq);
				if (!vacia?(coincis))
				{
					
					if (tablaBusq.nombre()==t1)
					{
						regMergeado= Merge(Prim(coincis),it.Siguiente());

					}else{
						regMergeado= Merge(it.Siguiente(),Prim(coincis));
					}
				}
				AgregarRapido(regMergeado,res);
				it.Avanzar();
			}

return res;			
}

Iterador(Conj(Registro)) BaseDeDatos::generarVistaJoin(String t1, String t2, String campo){
	Tupla aux = Tupla(campo,vacio(),vacio());
	definir(t2,aux,obtener(t1,hayJoin));
	definir(t2,vacio(),obtener(t1,registrosDelJoin));
	Tabla tabla1= obtener(t1,nombreATabla);
		Tabla tabla2= obtener(t2,nombreATabla);
		if (tabla1.tipoCampo(campo))
		{
			definir(t2,vacio(),obtener(t1,joinPorCampoNat));
			?? regsMergeados = combinarRegistros(t1,t2,campo);
			Iterador it = crearIt(regsMergeados);
			while(it.haySiguiente()){
				Dato d = obtener(campo,it.Siguiente());
				Iterador iter = AgregarRapido(it.Siguiente(),obtener(t2,obtener(t1,registrosDelJoin)));
				unsigned int n = d.valorNat();
				definir(n,iter,obtener(t2,obtener(t1,joinPorCampoNat)));
				it.Avanzar();
			}
		}else{
			definir(t2,vacio(),obtener(t1,joinPorCampoString));
			?? regsMergeados = combinarRegistros(t1,t2,campo);
			Iterador it = crearIt(regsMergeados);
			while(it.haySiguiente()){
				Dato d = obtener(campo,it.Siguiente());
				Iterador iter = AgregarRapido(it.Siguiente(),obtener(t2,obtener(t1,registrosDelJoin)));
				String s = d.valorStr();
				definir(s,iter,obtener(t2,obtener(t1,joinPorCampoString)));
				it.Avanzar();
			}
		}

		Iterador(Conj(Registro)) res = crearIt(obtener(t2,obtener(t1,registrosDelJoin)));
		return res;
}

void BaseDeDatos::BorrarJoin(String t1, String t2){
borrar(t2,obtener(t1,hayJoin));
borrar(t2,obtener(t1,registrosDelJoin));
if (def(t2,obtener(t1,joinPorCampoNat)))
{
	borrar(t2,obtener(t1,joinPorCampoNat));
}else{
	borrar(t2,obtener(t1,joinPorCampoString));
}

}

Iterador(String) BaseDeDatos::tablas(){
return crearIt(tablas);
}

Tabla BaseDeDatos::dameTabla(String s){
return obtener(s,nombreATabla);
}

bool BaseDeDatos::hayJoin(String s1,String s2){
return def(s2,obtener(s1,hayJoin));
}

String BaseDeDatos::campoJoin(String s1, String s2){

return (obtener(s2,obtener(s1,hayJoin))).campoJoin;
}

Registro Merge(Registro r1, Registro r2){
	Registro res = copiar(r1);
	Iterador ite = vistaDicc(r2);
	while(ite.haySiguiente()){
		if (!def(ite.Siguiente().clave,res))
		{
			definir(ite.Siguiente().clave,ite.Siguiente().significado,res);
		}
		ite.Avanzar();
	}
}


Iterador(Conj(Registro)) BaseDeDatos::vistaJoin(String s1, String s2){
//COMPLETAR!!
}

Conj(Registro) BaseDeDatos::busquedaCriterio(Registro crit,String t){
	//COMPLETAR!!
}

bool coincidenTodosCrit(Registro crit,Registro r){
	//COMPLETAR!!
}

String BaseDeDatos::tablaMaxima(){
	return (*tablaMasAccedida);
}
}



#endif
