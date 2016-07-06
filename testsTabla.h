#include <string>
#include <iostream>

#include "Tabla.h"
#include "mini_test.h"


Registro_tp3 sampleCamposTabla();
aed2::Conj<std::string> sampleClavesTabla();

Registro_tp3 sampleRegistro_Nro0();
Registro_tp3 sampleRegistro_Nro1();
Registro_tp3 sampleRegistro_Nro2();
Registro_tp3 sampleRegistro_Nro3();

void constructorDeTabla() {
    Registro_tp3 campos = sampleCamposTabla();
    aed2::Conj<std::string> claves = sampleClavesTabla();

    tp3::Tabla t = tp3::Tabla("TablaTest", claves, campos);

    ASSERT_EQ(t.nombre(), "TablaTest");
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
    Registro_tp3 campos = sampleCamposTabla();
    aed2::Conj<std::string> claves = sampleClavesTabla();

    tp3::Tabla t = tp3::Tabla("TablaTest", claves, campos);

    t.agregarRegistro(sampleRegistro_Nro0());

    ASSERT_EQ(t.cantidadDeAccesos(), 1);
    ASSERT(t.registros().CrearIt().Siguiente() == sampleRegistro_Nro0());

    t.agregarRegistro(sampleRegistro_Nro1());
    t.agregarRegistro(sampleRegistro_Nro2());
    t.agregarRegistro(sampleRegistro_Nro3());

    ASSERT_EQ(t.cantidadDeAccesos(), 4);
    Registro_tp3 critB;
    critB.definir("ClaveNat_0", tp3::Dato::datoNat(3));
    t.borrarRegistro(critB);

    ASSERT_EQ(t.cantidadDeAccesos(), 5);

    Registro_tp3 critA;
    critA.definir("ClaveString_0", tp3::Dato::datoString("no existe"));
    t.borrarRegistro(critA);

    ASSERT_EQ(t.cantidadDeAccesos(), 5);

}

void indexar() {
    Registro_tp3 campos = sampleCamposTabla();
    aed2::Conj<std::string> claves = sampleClavesTabla();

    tp3::Tabla t = tp3::Tabla("TablaTest", claves, campos);

    ASSERT(!t.indices().Pertenece("ClaveNat_0"));
    ASSERT(!t.indices().Pertenece("ClaveString_0"));

    t.indexar("ClaveNat_0");

    ASSERT(t.indices().Pertenece("ClaveNat_0"));
    ASSERT(!t.indices().Pertenece("ClaveString_0"));

    t.indexar("ClaveString_0");

    ASSERT(t.indices().Pertenece("ClaveNat_0"));
    ASSERT(t.indices().Pertenece("ClaveString_0"));

    t.agregarRegistro(sampleRegistro_Nro0());

    ASSERT_EQ(t.maximo("ClaveNat_0").dame_valorNat(), 0);
    ASSERT_EQ(t.minimo("ClaveNat_0").dame_valorNat(), 0);
    ASSERT_EQ(t.maximo("ClaveString_0").dame_valorStr(), "0Zero");
    ASSERT_EQ(t.minimo("ClaveString_0").dame_valorStr(), "0Zero");

    t.agregarRegistro(sampleRegistro_Nro1());
    t.agregarRegistro(sampleRegistro_Nro2());
    t.agregarRegistro(sampleRegistro_Nro3());

    ASSERT_EQ(t.maximo("ClaveNat_0").dame_valorNat(), 3);
    ASSERT_EQ(t.minimo("ClaveNat_0").dame_valorNat(), 0);
    ASSERT_EQ(t.maximo("ClaveString_0").dame_valorStr(), "3Tres");
    ASSERT_EQ(t.minimo("ClaveString_0").dame_valorStr(), "0Zero");

    Registro_tp3 pablo;
    pablo.definir("ClaveNat_0", tp3::Dato::datoNat(0));

    t.borrarRegistro(pablo);
    pablo.borrar("ClaveNat_0");
    pablo.definir("ClaveString_0", tp3::Dato::datoString("3Tres"));
    t.borrarRegistro(pablo);
    pablo.borrar("ClaveString_0");

    ASSERT_EQ(t.maximo("ClaveNat_0").dame_valorNat(), 2);
    ASSERT_EQ(t.minimo("ClaveNat_0").dame_valorNat(), 1);
    ASSERT_EQ(t.maximo("ClaveString_0").dame_valorStr(), "2Dos");
    ASSERT_EQ(t.minimo("ClaveString_0").dame_valorStr(), "1Uno");


    pablo.definir("ClaveNat_0", tp3::Dato::datoNat(020));
    t.borrarRegistro(pablo);
    pablo.borrar("ClaveNat_0");
    pablo.definir("ClaveString_0", tp3::Dato::datoString("no existe"));
    t.borrarRegistro(pablo);


    tp3::Tabla q = tp3::Tabla("TablaTest", claves, campos);

    q.agregarRegistro(sampleRegistro_Nro1());
    q.agregarRegistro(sampleRegistro_Nro2());
    q.agregarRegistro(sampleRegistro_Nro3());

    q.indexar("ClaveNat_0");
    q.indexar("ClaveString_0");

    ASSERT_EQ(q.maximo("ClaveNat_0").dame_valorNat(), 3);
    ASSERT_EQ(q.minimo("ClaveNat_0").dame_valorNat(), 1);
    ASSERT_EQ(q.maximo("ClaveString_0").dame_valorStr(), "3Tres");
    ASSERT_EQ(q.minimo("ClaveString_0").dame_valorStr(), "1Uno");

    q.agregarRegistro(sampleRegistro_Nro0());

    ASSERT_EQ(q.maximo("ClaveNat_0").dame_valorNat(), 3);
    ASSERT_EQ(q.minimo("ClaveNat_0").dame_valorNat(), 0);
    ASSERT_EQ(q.maximo("ClaveString_0").dame_valorStr(), "3Tres");
    ASSERT_EQ(q.minimo("ClaveString_0").dame_valorStr(), "0Zero");

}

void buscarDeTabla() {
    Registro_tp3 campos = sampleCamposTabla();
    aed2::Conj<std::string> claves = sampleClavesTabla();

    tp3::Tabla t = tp3::Tabla("TablaTest", claves, campos);

    t.agregarRegistro(sampleRegistro_Nro0());
    t.agregarRegistro(sampleRegistro_Nro1());
    t.agregarRegistro(sampleRegistro_Nro2());
    t.agregarRegistro(sampleRegistro_Nro3());

    aed2::Lista<Registro_tp3> res0 = t.buscar("ClaveNat_0", tp3::Dato::datoNat(0));

    ASSERT_EQ(res0.Longitud(), 1);

    const Registro_tp3 res0_r0 = res0.CrearIt().Siguiente();

    ASSERT_EQ(res0_r0.obtener("ClaveNat_0").dame_valorNat(), 0);
    ASSERT_EQ(res0_r0.obtener("ClaveNat_1").dame_valorNat(), 0);
    ASSERT_EQ(res0_r0.obtener("ClaveString_0").dame_valorStr(), "0Zero");
    ASSERT_EQ(res0_r0.obtener("ClaveString_1").dame_valorStr(), "0");


    aed2::Lista<Registro_tp3> res1 = t.buscar("String_2", tp3::Dato::datoString("stringGenerico"));

    ASSERT_EQ(res1.Longitud(), 2);
    aed2::Lista<Registro_tp3>::const_Iterador it1 = res1.CrearIt();
    const Registro_tp3 res1_r0 = it1.Siguiente();
    it1.Avanzar();
    const Registro_tp3 res1_r1 = it1.Siguiente();

    ASSERT_EQ(res1_r0.obtener("ClaveNat_0").dame_valorNat(), 2);
    ASSERT_EQ(res1_r1.obtener("ClaveNat_0").dame_valorNat(), 1);


    aed2::Lista<Registro_tp3> res2 = t.buscar("String_2", tp3::Dato::datoString("no existe"));

    ASSERT_EQ(res2.Longitud(), 0);


    t.indexar("ClaveNat_0");
    t.indexar("ClaveString_0");

    aed2::Lista<Registro_tp3> res3 = t.buscar("ClaveString_0", tp3::Dato::datoString("0Zero"));

    ASSERT_EQ(res3.Longitud(), 1);

    const Registro_tp3 res3_r0 = res3.CrearIt().Siguiente();

    ASSERT_EQ(res3_r0.obtener("ClaveNat_0").dame_valorNat(), 0);


    aed2::Lista<Registro_tp3> res4 = t.buscar("ClaveNat_0", tp3::Dato::datoNat(020));

    ASSERT_EQ(res4.Longitud(), 0);

}
/*************************************************************/
/*************************************************************/
/**************MAIN*******************************************/

void testsTabla(int, char**) {
    std::cout << "TEST TABLA" << std::endl;
    RUN_TEST(constructorDeTabla) ;
    RUN_TEST(insertar) ;
    RUN_TEST(indexar) ;
    RUN_TEST(buscarDeTabla) ;
}

/*************************************************************/
/*************************************************************/
Registro_tp3 sampleCamposTabla() {
    Registro_tp3 r;

    r.definir("ClaveNat_0", tp3::Dato::datoNat(48));
    r.definir("ClaveNat_1", tp3::Dato::datoNat(99));
    r.definir("Nat_2", tp3::Dato::datoNat(0));

    r.definir("ClaveString_0", tp3::Dato::datoString("string1"));
    r.definir("ClaveString_1", tp3::Dato::datoString("string2"));
    r.definir("String_2", tp3::Dato::datoString("string3"));

    return r;
}

aed2::Conj<std::string> sampleClavesTabla() {
    aed2::Conj<std::string> claves;

    claves.AgregarRapido("ClaveNat_0");
    claves.AgregarRapido("ClaveNat_1");
    claves.AgregarRapido("ClaveString_0");
    claves.AgregarRapido("ClaveString_1");

    return claves;
}

Registro_tp3 sampleRegistro_Nro0() {
    Registro_tp3 r;

    r.definir("ClaveNat_0", tp3::Dato::datoNat(0));
    r.definir("ClaveNat_1", tp3::Dato::datoNat(0));
    r.definir("Nat_2",      tp3::Dato::datoNat(0));

    r.definir("ClaveString_0", tp3::Dato::datoString("0Zero"));
    r.definir("ClaveString_1", tp3::Dato::datoString("0"));
    r.definir("String_2",      tp3::Dato::datoString("WIFI"));

    return r;
}

Registro_tp3 sampleRegistro_Nro1() {
    Registro_tp3 r;

    r.definir("ClaveNat_0", tp3::Dato::datoNat(1));
    r.definir("ClaveNat_1", tp3::Dato::datoNat(2));
    r.definir("Nat_2",      tp3::Dato::datoNat(0));

    r.definir("ClaveString_0", tp3::Dato::datoString("1Uno"));
    r.definir("ClaveString_1", tp3::Dato::datoString("1"));
    r.definir("String_2",      tp3::Dato::datoString("stringGenerico"));

    return r;
}


Registro_tp3 sampleRegistro_Nro2() {
    Registro_tp3 r;

    r.definir("ClaveNat_0", tp3::Dato::datoNat(2));
    r.definir("ClaveNat_1", tp3::Dato::datoNat(4));
    r.definir("Nat_2",      tp3::Dato::datoNat(42));

    r.definir("ClaveString_0", tp3::Dato::datoString("2Dos"));
    r.definir("ClaveString_1", tp3::Dato::datoString("2"));
    r.definir("String_2",      tp3::Dato::datoString("stringGenerico"));

    return r;
}


Registro_tp3 sampleRegistro_Nro3() {
    Registro_tp3 r;

    r.definir("ClaveNat_0", tp3::Dato::datoNat(3));
    r.definir("ClaveNat_1", tp3::Dato::datoNat(8));
    r.definir("Nat_2",      tp3::Dato::datoNat(42));

    r.definir("ClaveString_0", tp3::Dato::datoString("3Tres"));
    r.definir("ClaveString_1", tp3::Dato::datoString("3"));
    r.definir("String_2",      tp3::Dato::datoString("Carlos"));

    return r;
}
