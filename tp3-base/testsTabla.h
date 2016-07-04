#include <string>
#include <iostream>

#include "../Tabla.h"
#include "mini_test.h"

using namespace std;
namespace testsTabla
{
    Registro_tp3 sampleCampos();
    aed2::Conj<std::string> sampleClaves();

    Registro_tp3 sampleReg0();
    Registro_tp3 sampleReg1();
    Registro_tp3 sampleReg2();
    Registro_tp3 sampleReg3();

    void constructor() {
        Registro_tp3 campos = sampleCampos();
        aed2::Conj<std::string> claves = sampleClaves();

        tp3::Tabla t = tp3::Tabla("nombre", claves, campos);

        ASSERT_EQ(t.nombre(), "nombre");
        ASSERT(t.campos() == campos.claves() );

        aed2::Lista<struct tp3::tupString<tp3::Dato> >::const_Iterador it = campos.vistaDicc();
        while( it.HaySiguiente() ) {
            std::string c = it.Siguiente().clave;
            tp3::Dato d = it.Siguiente().significado;

            ASSERT_EQ(t.claves().Pertenece(c), claves.Pertenece(c));
            ASSERT(!t.indices().Pertenece(c));
            ASSERT_EQ(t.tipoCampo(c), d.esNat());

            it.Avanzar();
        }

        ASSERT_EQ(t.cantidadDeAccesos(), 0);
        ASSERT(!t.registros().CrearIt().HaySiguiente());
    }

    void insertar() {
        Registro_tp3 campos = sampleCampos();
        aed2::Conj<std::string> claves = sampleClaves();

        tp3::Tabla t = tp3::Tabla("nombre", claves, campos);

        t.agregarRegistro(sampleReg0());

        ASSERT_EQ(t.cantidadDeAccesos(), 1);
        ASSERT(t.registros().CrearIt().Siguiente() == sampleReg0());

        t.agregarRegistro(sampleReg1());
        t.agregarRegistro(sampleReg2());
        t.agregarRegistro(sampleReg3());

        ASSERT_EQ(t.cantidadDeAccesos(), 4);
        Registro_tp3 critB;
        critB.definir("claveNat0", tp3::Dato::datoNat(3));
        t.borrarRegistro(critB);

        ASSERT_EQ(t.cantidadDeAccesos(), 5);

        Registro_tp3 critA;
        critA.definir("claveStr0", tp3::Dato::datoString("no existe"));
        t.borrarRegistro(critA);

        ASSERT_EQ(t.cantidadDeAccesos(), 5);

    }

    void indexar() {
        Registro_tp3 campos = sampleCampos();
        aed2::Conj<std::string> claves = sampleClaves();

        // Indexar sobre vacío

        tp3::Tabla t = tp3::Tabla("nombre", claves, campos);

        ASSERT(!t.indices().Pertenece("claveNat0"));
        ASSERT(!t.indices().Pertenece("claveStr0"));

        t.indexar("claveNat0");

        ASSERT(t.indices().Pertenece("claveNat0"));
        ASSERT(!t.indices().Pertenece("claveStr0"));

        t.indexar("claveStr0");

        ASSERT(t.indices().Pertenece("claveNat0"));
        ASSERT(t.indices().Pertenece("claveStr0"));

        // Inserciones y max/min

        t.agregarRegistro(sampleReg0());

        ASSERT_EQ(t.maximo("claveNat0").dame_valorNat(), 0);
        ASSERT_EQ(t.minimo("claveNat0").dame_valorNat(), 0);
        ASSERT_EQ(t.maximo("claveStr0").dame_valorStr(), "a");
        ASSERT_EQ(t.minimo("claveStr0").dame_valorStr(), "a");

        t.agregarRegistro(sampleReg1());
        t.agregarRegistro(sampleReg2());
        t.agregarRegistro(sampleReg3());

        ASSERT_EQ(t.maximo("claveNat0").dame_valorNat(), 3);
        ASSERT_EQ(t.minimo("claveNat0").dame_valorNat(), 0);
        ASSERT_EQ(t.maximo("claveStr0").dame_valorStr(), "d");
        ASSERT_EQ(t.minimo("claveStr0").dame_valorStr(), "a");

        // Borrar indexados
        Registro_tp3 pablo;
        pablo.definir("claveNat0", tp3::Dato::datoNat(0));

        t.borrarRegistro(pablo);
        pablo.borrar("claveNat0");
        pablo.definir("claveStr0", tp3::Dato::datoString("d"));
        t.borrarRegistro(pablo);
        pablo.borrar("claveStr0");

        ASSERT_EQ(t.maximo("claveNat0").dame_valorNat(), 2);
        ASSERT_EQ(t.minimo("claveNat0").dame_valorNat(), 1);
        ASSERT_EQ(t.maximo("claveStr0").dame_valorStr(), "c");
        ASSERT_EQ(t.minimo("claveStr0").dame_valorStr(), "b");

        // Borrar inexistentes por indice

        pablo.definir("claveNat0", tp3::Dato::datoNat(0xDEADBEEF));
        t.borrarRegistro(pablo);
        pablo.borrar("claveNat0");
        pablo.definir("claveStr0", tp3::Dato::datoString("no existe"));
        t.borrarRegistro(pablo);

        // Indexar con registros indexados

        tp3::Tabla q = tp3::Tabla("nombre", claves, campos);

        q.agregarRegistro(sampleReg1());
        q.agregarRegistro(sampleReg2());
        q.agregarRegistro(sampleReg3());

        q.indexar("claveNat0");
        q.indexar("claveStr0");

        ASSERT_EQ(q.maximo("claveNat0").dame_valorNat(), 3);
        ASSERT_EQ(q.minimo("claveNat0").dame_valorNat(), 1);
        ASSERT_EQ(q.maximo("claveStr0").dame_valorStr(), "d");
        ASSERT_EQ(q.minimo("claveStr0").dame_valorStr(), "b");

        q.agregarRegistro(sampleReg0());

        ASSERT_EQ(q.maximo("claveNat0").dame_valorNat(), 3);
        ASSERT_EQ(q.minimo("claveNat0").dame_valorNat(), 0);
        ASSERT_EQ(q.maximo("claveStr0").dame_valorStr(), "d");
        ASSERT_EQ(q.minimo("claveStr0").dame_valorStr(), "a");

    }

    void buscar() {
        Registro_tp3 campos = sampleCampos();
        aed2::Conj<std::string> claves = sampleClaves();

        tp3::Tabla t = tp3::Tabla("nombre", claves, campos);

        t.agregarRegistro(sampleReg0());
        t.agregarRegistro(sampleReg1());
        t.agregarRegistro(sampleReg2());
        t.agregarRegistro(sampleReg3());

        // Buscar sin indices, un registro
        aed2::Lista<Registro_tp3> res0 = t.buscar("claveNat0", tp3::Dato::datoNat(0));

        ASSERT_EQ(res0.Longitud(), 1);

        const Registro_tp3 res0_r0 = res0.CrearIt().Siguiente();

        ASSERT_EQ(res0_r0.obtener("claveNat0").dame_valorNat(), 0);
        ASSERT_EQ(res0_r0.obtener("claveNat1").dame_valorNat(), 0);
        ASSERT_EQ(res0_r0.obtener("claveStr0").dame_valorStr(), "a");
        ASSERT_EQ(res0_r0.obtener("claveStr1").dame_valorStr(), "0");

        // Buscar sin indices, varios registros

        aed2::Lista<Registro_tp3> res1 = t.buscar("str2", tp3::Dato::datoString("asd"));

        ASSERT_EQ(res1.Longitud(), 2);
        aed2::Lista<Registro_tp3>::const_Iterador it1 = res1.CrearIt();
        const Registro_tp3 res1_r0 = it1.Siguiente();
        it1.Avanzar();
        const Registro_tp3 res1_r1 = it1.Siguiente();

        ASSERT_EQ(res1_r0.obtener("claveNat0").dame_valorNat(), 2);
        ASSERT_EQ(res1_r1.obtener("claveNat0").dame_valorNat(), 1);

        // Buscar sin resultados

        aed2::Lista<Registro_tp3> res2 = t.buscar("str2", tp3::Dato::datoString("no existe"));

        ASSERT_EQ(res2.Longitud(), 0);

        // Buscar indexados

        t.indexar("claveNat0");
        t.indexar("claveStr0");

        aed2::Lista<Registro_tp3> res3 = t.buscar("claveStr0", tp3::Dato::datoString("a"));

        ASSERT_EQ(res3.Longitud(), 1);

        const Registro_tp3 res3_r0 = res3.CrearIt().Siguiente();

        ASSERT_EQ(res3_r0.obtener("claveNat0").dame_valorNat(), 0);

        // Buscar indexados sin resultado

        aed2::Lista<Registro_tp3> res4 = t.buscar("claveNat0", tp3::Dato::datoNat(0xDEADBEEF));

        ASSERT_EQ(res4.Longitud(), 0);

    }

    void main(int, char**) {
        std::cout << "******** tabla *********" << std::endl;
        constructor();
        insertar();
        indexar();
        buscar();
    }

    /* Auxiliares */

    Registro_tp3 sampleCampos() {
        Registro_tp3 r;

        r.definir("claveNat0", tp3::Dato::datoNat(48));
        r.definir("claveNat1", tp3::Dato::datoNat(99));
        r.definir("nat2", tp3::Dato::datoNat(0));

        r.definir("claveStr0", tp3::Dato::datoString("aoeu"));
        r.definir("claveStr1", tp3::Dato::datoString("hrlo"));
        r.definir("str2", tp3::Dato::datoString(" "));

        return r;
    }

    aed2::Conj<std::string> sampleClaves() {
        aed2::Conj<std::string> claves;

        claves.AgregarRapido("claveNat0");
        claves.AgregarRapido("claveNat1");
        claves.AgregarRapido("claveStr0");
        claves.AgregarRapido("claveStr1");

        return claves;
    }

    Registro_tp3 sampleReg0() {
        Registro_tp3 r;

        r.definir("claveNat0", tp3::Dato::datoNat(0));
        r.definir("claveNat1", tp3::Dato::datoNat(0));
        r.definir("nat2",      tp3::Dato::datoNat(0));

        r.definir("claveStr0", tp3::Dato::datoString("a"));
        r.definir("claveStr1", tp3::Dato::datoString("0"));
        r.definir("str2",      tp3::Dato::datoString(""));

        return r;
    }

    Registro_tp3 sampleReg1() {
        Registro_tp3 r;

        r.definir("claveNat0", tp3::Dato::datoNat(1));
        r.definir("claveNat1", tp3::Dato::datoNat(2));
        r.definir("nat2",      tp3::Dato::datoNat(0));

        r.definir("claveStr0", tp3::Dato::datoString("b"));
        r.definir("claveStr1", tp3::Dato::datoString("1"));
        r.definir("str2",      tp3::Dato::datoString("asd"));

        return r;
    }


    Registro_tp3 sampleReg2() {
        Registro_tp3 r;

        r.definir("claveNat0", tp3::Dato::datoNat(2));
        r.definir("claveNat1", tp3::Dato::datoNat(4));
        r.definir("nat2",      tp3::Dato::datoNat(42));

        r.definir("claveStr0", tp3::Dato::datoString("c"));
        r.definir("claveStr1", tp3::Dato::datoString("2"));
        r.definir("str2",      tp3::Dato::datoString("asd"));

        return r;
    }


    Registro_tp3 sampleReg3() {
        Registro_tp3 r;

        r.definir("claveNat0", tp3::Dato::datoNat(3));
        r.definir("claveNat1", tp3::Dato::datoNat(8));
        r.definir("nat2",      tp3::Dato::datoNat(42));

        r.definir("claveStr0", tp3::Dato::datoString("d"));
        r.definir("claveStr1", tp3::Dato::datoString("3"));
        r.definir("str2",      tp3::Dato::datoString("aoe"));

        return r;
    }



}
