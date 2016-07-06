#include <string>
#include <iostream>

#include "Driver.h"
#include "mini_test.h"


aed2::Conj<aed2::Columna> Columnas_Alumnos();
aed2::Conj<aed2::Columna> Columnas_Alumnas();
aed2::Conj<aed2::NombreCampo> Claves_Alumnos();
aed2::Conj<aed2::NombreCampo> Claves_Alumnas();
aed2::Driver::Registro Reg_Alumnos_1();
aed2::Driver::Registro Reg_Alumnos_2();
aed2::Driver::Registro Reg_Alumnos_lu_match();
aed2::Driver::Registro Reg_Alumnas_lu_match();
aed2::Driver::Registro Reg_Alumnas_1();
aed2::Driver::Registro Reg_Alumnas_2();

void variado()
{
    aed2::Driver bd;
    ASSERT_EQ(bd.tablas().Cardinal(), 0);
    aed2::Conj<aed2::Columna> columnas_Alumnos = Columnas_Alumnos();
    aed2::Conj<aed2::NombreCampo> columnas_clave_Alumnos = Claves_Alumnos();

    bd.crearTabla("Alumnos", columnas_Alumnos, columnas_clave_Alumnos);
    ASSERT_EQ(bd.tablas().Cardinal(), 1);

    bd.crearTabla("Alumnas", columnas_Alumnos, columnas_clave_Alumnos);
    ASSERT_EQ(bd.tablas().Cardinal(), 2);

    ASSERT(bd.tablas().Pertenece("Alumnas"));
    ASSERT(bd.tablas().Pertenece("Alumnos"));

    aed2::Driver::Registro Alumno = Reg_Alumnos_1();

    bd.insertarRegistro("Alumnos", Alumno);

    aed2::Conj<aed2::Columna> columnas = bd.columnasDeTabla("Alumnos");

    ASSERT_EQ(columnas.Cardinal(), 3);

    aed2::Conj<aed2::NombreCampo> columnas_nombre;
    aed2::Conj<aed2::Columna>::const_Iterador it = columnas.CrearIt();
    while(it.HaySiguiente()) {
        columnas_nombre.Agregar(it.Siguiente().nombre);
        it.Avanzar();
    }
    ASSERT(columnas_nombre.Pertenece("LU"));
    ASSERT(columnas_nombre.Pertenece("nombre"));
    ASSERT(columnas_nombre.Pertenece("apellido"));

    aed2::Conj<aed2::NombreCampo> claves = bd.columnasClaveDeTabla("Alumnos");

    ASSERT_EQ(claves.Cardinal(), 1);
    ASSERT_EQ(claves.CrearIt().Siguiente(), "LU");

    bd.borrarRegistro("Alumnos", "LU", aed2::Driver::Dato(1));

    aed2::Conj<aed2::Driver::Registro> registros = bd.registrosDeTabla("Alumnos");

    ASSERT(registros.EsVacio());

    bd.insertarRegistro("Alumnos", Alumno);

    registros = bd.registrosDeTabla("Alumnos");

    ASSERT_EQ(registros.Cardinal(), 1);
    ASSERT(registros.CrearIt().Siguiente().Significado("LU") == aed2::Driver::Dato(1));

    ASSERT_EQ(bd.cantidadDeAccesosDeTabla("Alumnos"), 3);
}

void buscar()
{
    aed2::Driver bd;

    //AlumnoS
    aed2::Conj<aed2::Columna> columnas_Alumnos = Columnas_Alumnos();
    aed2::Conj<aed2::NombreCampo> columnas_clave_Alumnos = Claves_Alumnos();

    bd.crearTabla("Alumnos", columnas_Alumnos, columnas_clave_Alumnos);
    aed2::Driver::Registro Alumno = Reg_Alumnos_1();

    //PERSONITAS
    aed2::Conj<aed2::Columna> columnas_Alumnas = Columnas_Alumnas();
    aed2::Conj<aed2::NombreCampo> columnas_clave_Alumnas = Claves_Alumnas();

    bd.crearTabla("Alumnas", columnas_Alumnas, columnas_clave_Alumnas);
    aed2::Driver::Registro Alumna = Reg_Alumnas_1();

    //Búsquedas...
    aed2::Driver::Registro crit;
    crit.Definir("LU", aed2::Driver::Dato(1));

    bd.insertarRegistro("Alumnos", Alumno);

    ASSERT(bd.buscar("Alumnos", crit).Cardinal() == 1);
    ASSERT(bd.buscar("Alumnos", crit).CrearIt().Siguiente() == Reg_Alumnos_1() );

    ASSERT_EQ(bd.buscar("Alumnas", crit).Cardinal(), 0);

    bd.borrarRegistro("Alumnos", "LU", aed2::Driver::Dato(1));
    ASSERT_EQ(bd.buscar("Alumnos", crit).Cardinal(), 0);

    //Con índice
    bd.insertarRegistro("Alumnos", Alumno);
    bd.crearIndiceNat("Alumnos", "LU");
    ASSERT(bd.buscar("Alumnos", crit).Cardinal() == 1);

    bd.borrarRegistro("Alumnos", "LU", aed2::Driver::Dato(1));
    ASSERT_EQ(bd.buscar("Alumnos", crit).Cardinal(), 0);


    bd.crearIndiceString("Alumnas", "nombre");
    bd.insertarRegistro("Alumnas", Alumna);

    aed2::Driver::Registro crit2;
    crit2.Definir("LU", aed2::Driver::Dato(3));

    ASSERT(bd.buscar("Alumnas", crit2).Cardinal() == 1);
    bd.borrarRegistro("Alumnas", "LU", aed2::Driver::Dato(3));
    ASSERT_EQ(bd.buscar("Alumnas", crit2).Cardinal(), 0);
    //

}


void indices() {
    aed2::Driver bd;

    aed2::Conj<aed2::Columna> columnas_Alumnos = Columnas_Alumnos();
    aed2::Conj<aed2::NombreCampo> columnas_clave_Alumnos = Claves_Alumnos();

    bd.crearTabla("Alumnos", columnas_Alumnos, columnas_clave_Alumnos);
    bd.crearIndiceNat("Alumnos", "LU");
    bd.crearIndiceString("Alumnos", "nombre");

    ASSERT(bd.tieneIndiceNat("Alumnos"));
    ASSERT(bd.tieneIndiceString("Alumnos"));

    ASSERT_EQ(bd.campoIndiceNat("Alumnos"), "LU");
    ASSERT_EQ(bd.campoIndiceString("Alumnos"), "nombre");

}


void joins()
{
  aed2::Driver bd;

  //AlumnoS
  aed2::Conj<aed2::Columna> columnas_Alumnos = Columnas_Alumnos();
  aed2::Conj<aed2::NombreCampo> columnas_clave_Alumnos = Claves_Alumnas();  //incorpora apellido además de LU

  bd.crearTabla("Alumnos", columnas_Alumnos, columnas_clave_Alumnos);
  aed2::Driver::Registro Alumno = Reg_Alumnos_2();
  bd.insertarRegistro("Alumnos", Alumno);

  //PERSONITAS
  aed2::Conj<aed2::Columna> columnas_Alumnas = Columnas_Alumnas();
  aed2::Conj<aed2::NombreCampo> columnas_clave_Alumnas = Claves_Alumnas();

  bd.crearTabla("Alumnas", columnas_Alumnas, columnas_clave_Alumnas);

  //CREACION/ELIMINACION...

  ASSERT(! bd.hayJoin("Alumnas", "Alumnos"));
  ASSERT(! bd.hayJoin("Alumnos", "Alumnas"));

  bd.generarVistaJoin("Alumnos", "Alumnas", "LU");

  ASSERT(! bd.hayJoin("Alumnas", "Alumnos"));
  ASSERT( bd.hayJoin("Alumnos", "Alumnas"));
  ASSERT_EQ( bd.campoJoin("Alumnos", "Alumnas"), "LU" );

  bd.borrarVistaJoin("Alumnos", "Alumnas");

  ASSERT(! bd.hayJoin("Alumnas", "Alumnos"));
  ASSERT(! bd.hayJoin("Alumnos", "Alumnas"));

  //vistaJoin

  bd.generarVistaJoin("Alumnos", "Alumnas", "LU");
  bd.generarVistaJoin("Alumnas", "Alumnos", "apellido");

  ASSERT_EQ( bd.campoJoin("Alumnas", "Alumnos"), "apellido" );

  ASSERT_EQ( bd.vistaJoin("Alumnos", "Alumnas").Cardinal(), 0 );
  ASSERT_EQ( bd.vistaJoin("Alumnas", "Alumnos").Cardinal(), 0 );

  aed2::Driver::Registro Alumna = Reg_Alumnas_2();
  bd.insertarRegistro("Alumnas", Alumna);

  aed2::Conj<Driver::Registro> join = bd.vistaJoin("Alumnos", "Alumnas");

  ASSERT_EQ( bd.vistaJoin("Alumnos", "Alumnas").Cardinal(), 0 );
  ASSERT_EQ( bd.vistaJoin("Alumnas", "Alumnos").Cardinal(), 1 );

  bd.borrarVistaJoin("Alumnos", "Alumnas");
  ASSERT( bd.hayJoin("Alumnas", "Alumnos"));
  ASSERT(! bd.hayJoin("Alumnos", "Alumnas"));

  bd.borrarVistaJoin("Alumnas", "Alumnos");
  ASSERT(! bd.hayJoin("Alumnas", "Alumnos"));
  ASSERT(! bd.hayJoin("Alumnos", "Alumnas"));

  bd.generarVistaJoin("Alumnos", "Alumnas", "LU");
  bd.generarVistaJoin("Alumnas", "Alumnos", "apellido");

  bd.insertarRegistro("Alumnas", Reg_Alumnas_1() );
  bd.insertarRegistro("Alumnas", Reg_Alumnas_lu_match() );
  bd.insertarRegistro("Alumnos", Reg_Alumnos_lu_match() );

  ASSERT_EQ( bd.vistaJoin("Alumnos", "Alumnas").Cardinal(), 1 );
  ASSERT_EQ( bd.vistaJoin("Alumnas", "Alumnos").Cardinal(), 2 );
}

/**********************************************\
===================> MAIN <=====================
\**********************************************/

void driverTest(int, char**)
{
    std::cout << "DRIVER TEST" << std::endl;
    RUN_TEST( variado );
    RUN_TEST( buscar );
    RUN_TEST( indices );
    RUN_TEST( joins );
}


/* Auxiliares */

aed2::Conj<aed2::Columna> Columnas_Alumnos() {
    aed2::Conj<aed2::Columna> columnas_Alumnos;

    aed2::Columna columna_lu;
    columna_lu.nombre = "LU";
    columna_lu.tipo = aed2::NAT;

    aed2::Columna columna_nombre;
    columna_nombre.nombre = "nombre";
    columna_nombre.tipo = aed2::STR;

    aed2::Columna columna_apellido;
    columna_apellido.nombre = "apellido";
    columna_apellido.tipo = aed2::STR;

    columnas_Alumnos.Agregar( columna_lu );
    columnas_Alumnos.Agregar( columna_nombre );
    columnas_Alumnos.Agregar( columna_apellido );

    return columnas_Alumnos;
}

//COLUMNAS <===============================

aed2::Conj<aed2::Columna> Columnas_Alumnas() {
    aed2::Conj<aed2::Columna> columnas_Alumnas;

    aed2::Columna columna_lu;
    columna_lu.nombre = "LU";
    columna_lu.tipo = aed2::NAT;

    aed2::Columna columna_nombre;
    columna_nombre.nombre = "nombre";
    columna_nombre.tipo = aed2::STR;

    aed2::Columna columna_apellido;
    columna_apellido.nombre = "apellido";
    columna_apellido.tipo = aed2::STR;

    columnas_Alumnas.Agregar( columna_lu );
    columnas_Alumnas.Agregar( columna_nombre );
    columnas_Alumnas.Agregar( columna_apellido );

    return columnas_Alumnas;
}

//COLUMNAS CLAVES <===============================

aed2::Conj<aed2::NombreCampo> Claves_Alumnos() {
    aed2::Conj<aed2::NombreCampo> columnas_clave_Alumnos;
    columnas_clave_Alumnos.Agregar("LU");

    return columnas_clave_Alumnos;
}

aed2::Conj<aed2::NombreCampo> Claves_Alumnas() {
    aed2::Conj<aed2::NombreCampo> columnas_clave_Alumnas;
    columnas_clave_Alumnas.Agregar("LU");
    columnas_clave_Alumnas.Agregar("apellido");

    return columnas_clave_Alumnas;
}

//AlumnoS REGS <===============================

aed2::Driver::Registro Reg_Alumnos_lu_match() {
    aed2::Driver::Registro Alumno;
    Alumno.Definir("LU", aed2::Driver::Dato(0));
    Alumno.Definir("nombre", aed2::Driver::Dato("Alumno"));
    Alumno.Definir("apellido", aed2::Driver::Dato("Zero"));

    return Alumno;
}

aed2::Driver::Registro Reg_Alumnos_1() {
    aed2::Driver::Registro Alumno;
    Alumno.Definir("LU", aed2::Driver::Dato(1));
    Alumno.Definir("nombre", aed2::Driver::Dato("Alumno"));
    Alumno.Definir("apellido", aed2::Driver::Dato("Uno"));

    return Alumno;
}

aed2::Driver::Registro Reg_Alumnos_2() {
    aed2::Driver::Registro Alumno;
    Alumno.Definir("LU", aed2::Driver::Dato(2));
    Alumno.Definir("nombre", aed2::Driver::Dato("Alumno"));
    Alumno.Definir("apellido", aed2::Driver::Dato("Dos"));

    return Alumno;
}

//PERSONITAS REGS <===============================

aed2::Driver::Registro Reg_Alumnas_lu_match() {
    aed2::Driver::Registro Alumna;
    Alumna.Definir("LU", aed2::Driver::Dato(0));
    Alumna.Definir("nombre", aed2::Driver::Dato("Alumna"));
    Alumna.Definir("apellido", aed2::Driver::Dato("Zero"));

    return Alumna;
}

aed2::Driver::Registro Reg_Alumnas_1() {
    aed2::Driver::Registro Alumna;
    Alumna.Definir("LU", aed2::Driver::Dato(3));
    Alumna.Definir("nombre", aed2::Driver::Dato("Alumna"));
    Alumna.Definir("apellido", aed2::Driver::Dato("Uno"));

    return Alumna;
}

aed2::Driver::Registro Reg_Alumnas_2() {
    aed2::Driver::Registro Alumna;
    Alumna.Definir("LU", aed2::Driver::Dato(4));
    Alumna.Definir("nombre", aed2::Driver::Dato("Alumna"));
    Alumna.Definir("apellido", aed2::Driver::Dato("Dos"));

    return Alumna;
}
