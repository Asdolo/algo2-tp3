#include <string>
#include <iostream>

#include "../Driver.h"
#include "../mini_test.h"

namespace testsDriver
{
    aed2::Conj<aed2::Columna> sampleColumnas();
    aed2::Conj<aed2::NombreCampo> sampleClaves();
    aed2::Driver::Registro sampleReg();

    void ejemplo_simple()
    {
        aed2::Driver bd;
        ASSERT_EQ(bd.tablas().Cardinal(), 0);

        aed2::Conj<aed2::Columna> columnas_personas = sampleColumnas();
        aed2::Conj<aed2::NombreCampo> columnas_clave_personas = sampleClaves();

        bd.crearTabla("personas", columnas_personas, columnas_clave_personas);
        ASSERT_EQ(bd.tablas().Cardinal(), 1);

        aed2::Driver::Registro persona = sampleReg();

        bd.insertarRegistro("personas", persona);

        aed2::Conj<aed2::Columna> columnas = bd.columnasDeTabla("personas");

        ASSERT_EQ(columnas.Cardinal(), 3);

        aed2::Conj<aed2::NombreCampo> columnas_nombre;
        aed2::Conj<aed2::Columna>::const_Iterador it = columnas.CrearIt();
        while(it.HaySiguiente()) {
            columnas_nombre.Agregar(it.Siguiente().nombre);
            it.Avanzar();
        }
        ASSERT(columnas_nombre.Pertenece("DNI"));
        ASSERT(columnas_nombre.Pertenece("nombre"));
        ASSERT(columnas_nombre.Pertenece("apellido"));

        aed2::Conj<aed2::NombreCampo> claves = bd.columnasClaveDeTabla("personas");

        ASSERT_EQ(claves.Cardinal(), 1);
        ASSERT_EQ(claves.CrearIt().Siguiente(), "DNI");

        bd.borrarRegistro("personas", "DNI", aed2::Driver::Dato(1));

        aed2::Conj<aed2::Driver::Registro> registros = bd.registrosDeTabla("personas");

        ASSERT(registros.EsVacio());

        bd.insertarRegistro("personas", persona);

        registros = bd.registrosDeTabla("personas");

        ASSERT_EQ(registros.Cardinal(), 1);
        ASSERT(registros.CrearIt().Siguiente().Significado("DNI") ==
                aed2::Driver::Dato(1));

        ASSERT_EQ(bd.cantidadDeAccesosDeTabla("personas"), 3);
    }

    void main(int, char**)
    {
        std::cout << "******** driver ********" << std::endl;
        RUN_TEST( ejemplo_simple );
    }


    /* Auxiliares */

    aed2::Conj<aed2::Columna> sampleColumnas() {
        aed2::Conj<aed2::Columna> columnas_personas;

        aed2::Columna columna_dni;
        columna_dni.nombre = "DNI";
        columna_dni.tipo = aed2::NAT;

        aed2::Columna columna_nombre;
        columna_nombre.nombre = "nombre";
        columna_nombre.tipo = aed2::STR;

        aed2::Columna columna_apellido;
        columna_apellido.nombre = "apellido";
        columna_apellido.tipo = aed2::STR;

        columnas_personas.Agregar( columna_dni );
        columnas_personas.Agregar( columna_nombre );
        columnas_personas.Agregar( columna_apellido );

        return columnas_personas;
    }

    aed2::Conj<aed2::NombreCampo> sampleClaves() {
        aed2::Conj<aed2::NombreCampo> columnas_clave_personas;
        columnas_clave_personas.Agregar("DNI");

        return columnas_clave_personas;
    }

    aed2::Driver::Registro sampleReg() {
        aed2::Driver::Registro persona;
        persona.Definir("DNI", aed2::Driver::Dato(1));
        persona.Definir("nombre", aed2::Driver::Dato("Juan"));
        persona.Definir("apellido", aed2::Driver::Dato("Perez"));

        return persona;
    }
}
