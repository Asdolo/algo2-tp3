#include <string>
#include <iostream>

#include "BaseDeDatos.h"
#include "mini_test.h"

Registro_tp3 sampleCampos0();
Registro_tp3 sampleCampos1();
aed2::Conj<std::string> clavesTabla0();
aed2::Conj<std::string> clavesTabla1();

Registro_tp3 registro_Tabla0_0();
Registro_tp3 registro_Tabla0_1();
Registro_tp3 registro_Tabla0_2();
Registro_tp3 registro_Tabla0_3();

Registro_tp3 registro_Tabla1_0();
Registro_tp3 registro_Tabla1_1();
Registro_tp3 registro_Tabla1_2();
Registro_tp3 registro_Tabla1_3();

tp3::Tabla Tabla0();
tp3::Tabla Tabla1();

void constructor_y_tablas() {
    tp3::BaseDeDatos db;
    tp3::Tabla t0 = Tabla0();
    tp3::Tabla t1 = Tabla1();


    db.agregarTabla(t0);
    ASSERT( (db.Tablas().Siguiente() == "Tabla0") );
    ASSERT(db.dameTabla("Tabla0").nombre() == "Tabla0");

    ASSERT_EQ(db.tablaMaxima(), "Tabla0");

    db.agregarTabla(t1);
    ASSERT(db.dameTabla("Tabla1").nombre() == "Tabla1");
}

void tablas_maximas() {
    tp3::BaseDeDatos db;
    tp3::Tabla t0 = Tabla0();
    tp3::Tabla t1 = Tabla1();

    db.agregarTabla(t0);
    db.agregarTabla(t1);

    db.insertarEntrada(registro_Tabla1_0(), "Tabla1");
    db.insertarEntrada(registro_Tabla1_1(), "Tabla1");

    ASSERT_EQ(db.tablaMaxima(), "Tabla1");

    db.insertarEntrada(registro_Tabla0_0(), "Tabla0");
    db.insertarEntrada(registro_Tabla0_1(), "Tabla0");
    db.insertarEntrada(registro_Tabla0_2(), "Tabla0");

    ASSERT_EQ(db.tablaMaxima(), "Tabla0");

    Registro_tp3 pablo;
    pablo.definir("claveStr0", tp3::Dato::datoString("a"));

    db.Borrar(pablo, "Tabla1");
    pablo.borrar("claveStr0");
    pablo.definir("claveStr0", tp3::Dato::datoString("b"));
    db.Borrar(pablo, "Tabla1");

    ASSERT_EQ(db.tablaMaxima(), "Tabla1");
}

void busquedaCriterio() {
    tp3::BaseDeDatos db;
    tp3::Tabla t0 = Tabla0();
    tp3::Tabla t1 = Tabla1();

    db.agregarTabla(t0);
    db.agregarTabla(t1);

    db.insertarEntrada(registro_Tabla0_0(), "Tabla0");
    db.insertarEntrada(registro_Tabla0_1(), "Tabla0");
    db.insertarEntrada(registro_Tabla0_2(), "Tabla0");
    db.insertarEntrada(registro_Tabla0_3(), "Tabla0");

    db.insertarEntrada(registro_Tabla1_0(), "Tabla1");
    db.insertarEntrada(registro_Tabla1_1(), "Tabla1");
    db.insertarEntrada(registro_Tabla1_2(), "Tabla1");
    db.insertarEntrada(registro_Tabla1_3(), "Tabla1");

    Registro_tp3 crit;

    crit.definir("str1_2", tp3::Dato::datoString("asd"));

    aed2::Conj<Registro_tp3> res = db.busquedaCriterio(crit, "Tabla1");

    ASSERT_EQ(res.Cardinal(), 2);

}

void crear_joins_simple() {
    tp3::BaseDeDatos db;
    tp3::Tabla t0 = Tabla0();
    tp3::Tabla t1 = Tabla1();

    db.agregarTabla(t0);
    db.agregarTabla(t1);

    ASSERT(!db.hayJoin("Tabla0","Tabla1"));
    ASSERT(!db.hayJoin("Tabla1","Tabla0"));

    aed2::Conj<Registro_tp3>::const_Iterador it0;
    it0 = db.generarVistaJoin("Tabla0","Tabla1", "claveNat0");

    ASSERT(db.hayJoin("Tabla0","Tabla1"));
    ASSERT(!db.hayJoin("Tabla1","Tabla0"));
    ASSERT_EQ(db.campoJoin("Tabla0","Tabla1"), "claveNat0");
    ASSERT(!it0.HaySiguiente());

    it0 = db.generarVistaJoin("Tabla1","Tabla0", "claveStr0");

    ASSERT(db.hayJoin("Tabla0","Tabla1"));
    ASSERT(db.hayJoin("Tabla1","Tabla0"));
    ASSERT_EQ(db.campoJoin("Tabla0","Tabla1"), "claveNat0");
    ASSERT_EQ(db.campoJoin("Tabla1","Tabla0"), "claveStr0");
    ASSERT(!it0.HaySiguiente());

    aed2::Conj<Registro_tp3>::const_Iterador it1;
    it1 = db.vistaJoin("Tabla0", "Tabla1");

    ASSERT(!it1.HaySiguiente());

    it1 = db.vistaJoin("Tabla1", "Tabla0");

    ASSERT(!it1.HaySiguiente());

    db.BorrarJoin("Tabla0", "Tabla1");

    ASSERT(!db.hayJoin("Tabla0","Tabla1"));
}

void crear_joins_con_datos() {
    tp3::BaseDeDatos db;
    tp3::Tabla t0 = Tabla0();
    tp3::Tabla t1 = Tabla1();

    db.agregarTabla(t0);
    db.agregarTabla(t1);

    db.insertarEntrada(registro_Tabla0_0(), "Tabla0");
    db.insertarEntrada(registro_Tabla0_1(), "Tabla0");
    db.insertarEntrada(registro_Tabla0_2(), "Tabla0");
    db.insertarEntrada(registro_Tabla0_3(), "Tabla0");

    db.insertarEntrada(registro_Tabla1_0(), "Tabla1");
    db.insertarEntrada(registro_Tabla1_1(), "Tabla1");
    db.insertarEntrada(registro_Tabla1_2(), "Tabla1");
    db.insertarEntrada(registro_Tabla1_3(), "Tabla1");

    aed2::Conj<Registro_tp3>::const_Iterador it0;
    it0 = db.generarVistaJoin("Tabla0","Tabla1", "claveNat0");

    ASSERT(it0.HaySiguiente());

    aed2::Conj<Registro_tp3>::const_Iterador it1;
    it1 = db.vistaJoin("Tabla0", "Tabla1");

    ASSERT(it1.HaySiguiente());

    Registro_tp3 r = it1.Siguiente();

    ASSERT(r.def("claveNat0"));
    ASSERT(r.def("Nat_Tabla0_2"));
    ASSERT(r.def("Nat_Tabla1_2"));
}

void operaciones_con_joins() {
    tp3::BaseDeDatos db;
    tp3::Tabla t0 = Tabla0();
    tp3::Tabla t1 = Tabla1();
    aed2::Conj<Registro_tp3>::const_Iterador it0;
    aed2::Conj<Registro_tp3>::const_Iterador it1;

    db.agregarTabla(t0);
    db.agregarTabla(t1);

    db.generarVistaJoin("Tabla0","Tabla1", "claveNat0");
    db.generarVistaJoin("Tabla1","Tabla0", "claveStr0");

    db.insertarEntrada(registro_Tabla0_0(), "Tabla0");


    //std::cout << "DESDE ACAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa" << std::endl;

    it0 = db.vistaJoin("Tabla0","Tabla1");
    it1 = db.vistaJoin("Tabla1","Tabla0");




    ASSERT(!it0.HaySiguiente());
    ASSERT(!it1.HaySiguiente());

    db.insertarEntrada(registro_Tabla1_0(), "Tabla1");

    it0 = db.vistaJoin("Tabla0","Tabla1");
    it1 = db.vistaJoin("Tabla1","Tabla0");


    //std::cout << "TABLA1: " << db.dameTabla("Tabla1").registros() << std::endl;
    //std::cout << "TABLA0: " << db.dameTabla("Tabla0").registros() << std::endl;

    //std::cout << "t0->t1 :" << db._registrosDelJoin.obtener("Tabla0").obtener("Tabla1") << std::endl;
    //std::cout << "t1->t0 :" << db._registrosDelJoin.obtener("Tabla1").obtener("Tabla0") << std::endl;

    ASSERT(it1.HaySiguiente());
    ASSERT(it0.HaySiguiente());
    ASSERT_EQ(it0.Siguiente().obtener("claveNat0").dame_valorNat(), 0);
    ASSERT_EQ(it1.Siguiente().obtener("claveNat0").dame_valorNat(), 0);

    it0.Avanzar();
    it1.Avanzar();

    ASSERT(!it0.HaySiguiente());
    ASSERT(!it1.HaySiguiente());

    db.insertarEntrada(registro_Tabla0_1(), "Tabla0");
    db.insertarEntrada(registro_Tabla0_2(), "Tabla0");
    db.insertarEntrada(registro_Tabla1_1(), "Tabla1");
    db.insertarEntrada(registro_Tabla1_2(), "Tabla1");

    it0 = db.vistaJoin("Tabla0","Tabla1");
    it1 = db.vistaJoin("Tabla1","Tabla0");

    //std::cout << "t0->t1 :" << db._registrosDelJoin.obtener("Tabla0").obtener("Tabla1") << std::endl;
    //std::cout << "t1->t0 :" << db._registrosDelJoin.obtener("Tabla1").obtener("Tabla0") << std::endl;

    Registro_tp3 pablo2;
    pablo2.definir("claveNat0", tp3::Dato::datoNat(0));
    //pablo2.definir("claveStr0", tp3::Dato::datoString("b"));
    db.Borrar(pablo2, "Tabla0");


    it1 = db.vistaJoin("Tabla1","Tabla0");
    it0 = db.vistaJoin("Tabla0","Tabla1");


    pablo2.borrar("claveNat0");
    pablo2.definir("claveStr0", tp3::Dato::datoString("b"));
    db.Borrar(pablo2, "Tabla1");
    pablo2.borrar("claveStr0");

    db.insertarEntrada(registro_Tabla0_3(), "Tabla0");
    db.insertarEntrada(registro_Tabla1_3(), "Tabla1");

    //std::cout << "t0->t1 :" << db._registrosDelJoin.obtener("Tabla0").obtener("Tabla1") << std::endl;
    //std::cout << "t1->t0 :" << db._registrosDelJoin.obtener("Tabla1").obtener("Tabla0") << std::endl;

    //std::cout << "t0->t1 :" << db._registrosDelJoin.obtener("Tabla0").obtener("Tabla1") << std::endl;
    //std::cout << "t1->t0 :" << db._registrosDelJoin.obtener("Tabla1").obtener("Tabla0") << std::endl;

    //std::cout << "Tabla 0: " << db.dameTabla("Tabla0") << std::endl;


    pablo2.definir("claveNat0", tp3::Dato::datoNat(3));
    //std::cout << endl << endl << "Pablo2: " <<pablo2 << std::endl;
    db.Borrar(pablo2, "Tabla0");


    //std::cout << "Tabla 0: " << db.dameTabla("Tabla0") << std::endl;
    //std::cout << "Tabla 1: " << db.dameTabla("Tabla1") << std::endl;

    it0 = db.vistaJoin("Tabla0","Tabla1");
    it1 = db.vistaJoin("Tabla1","Tabla0");

    //std::cout << "t0->t1 :" << db._registrosDelJoin.obtener("Tabla0").obtener("Tabla1") << std::endl;
    //std::cout << "t1->t0 :" << db._registrosDelJoin.obtener("Tabla1").obtener("Tabla0") << std::endl;

    ASSERT(it0.HaySiguiente());
    ASSERT(it1.HaySiguiente());

    //std::cout << "t0->t1 :" << db._registrosDelJoin.obtener("Tabla0").obtener("Tabla1") << std::endl;
    //std::cout << "t1->t0 :" << db._registrosDelJoin.obtener("Tabla1").obtener("Tabla0") << std::endl;

    //std::cout << "Prim it0: " << it0.Siguiente() << std::endl;
    //std::cout << "Prim it1: " << it1.Siguiente() << std::endl;

    ASSERT_EQ(it0.Siguiente().obtener("claveNat0").dame_valorNat(), 2);
    ASSERT_EQ(it1.Siguiente().obtener("claveNat0").dame_valorNat(), 2);
}

void testsDB(int, char**) {
    std::cout << "TEST DB" << std::endl;
    RUN_TEST(constructor_y_tablas);
    RUN_TEST(tablas_maximas);
    RUN_TEST(busquedaCriterio);
    RUN_TEST(crear_joins_simple)
    RUN_TEST(crear_joins_con_datos);
    RUN_TEST(operaciones_con_joins);
}


/* Auxiliares */

Registro_tp3 sampleCampos0() {
    Registro_tp3 r;

    r.definir("claveNat0", tp3::Dato::datoNat(48));
    r.definir("claveNat1", tp3::Dato::datoNat(99));
    r.definir("Nat_Tabla0_2", tp3::Dato::datoNat(0));

    r.definir("claveStr0", tp3::Dato::datoString("aoeu"));
    r.definir("claveStr1", tp3::Dato::datoString("hrlo"));
    r.definir("str0_2", tp3::Dato::datoString(""));

    return r;
}

Registro_tp3 sampleCampos1() {
    Registro_tp3 r;

    r.definir("claveNat0", tp3::Dato::datoNat(48));
    r.definir("claveNat1", tp3::Dato::datoNat(99));
    r.definir("Nat_Tabla1_2", tp3::Dato::datoNat(0));

    r.definir("claveStr0", tp3::Dato::datoString("aoeu"));
    r.definir("claveStr1", tp3::Dato::datoString("hrlo"));
    r.definir("str1_2", tp3::Dato::datoString(""));

    return r;
}

aed2::Conj<std::string> clavesTabla0() {
    aed2::Conj<std::string> claves;

    claves.AgregarRapido("claveNat0");
    claves.AgregarRapido("claveNat1");
    claves.AgregarRapido("claveStr0");
    claves.AgregarRapido("claveStr1");

    return claves;
}

aed2::Conj<std::string> clavesTabla1() {
    aed2::Conj<std::string> claves;

    claves.AgregarRapido("claveNat0");
    claves.AgregarRapido("claveNat1");
    claves.AgregarRapido("claveStr0");
    claves.AgregarRapido("claveStr1");

    return claves;
}

Registro_tp3 registro_Tabla0_0() {
    Registro_tp3 r;

    r.definir("claveNat0", tp3::Dato::datoNat(0));
    r.definir("claveNat1", tp3::Dato::datoNat(0));
    r.definir("Nat_Tabla0_2",      tp3::Dato::datoNat(0));

    r.definir("claveStr0", tp3::Dato::datoString("a"));
    r.definir("claveStr1", tp3::Dato::datoString("0"));
    r.definir("str0_2",      tp3::Dato::datoString(""));

    return r;
}

Registro_tp3 registro_Tabla0_1() {
    Registro_tp3 r;

    r.definir("claveNat0", tp3::Dato::datoNat(1));
    r.definir("claveNat1", tp3::Dato::datoNat(2));
    r.definir("Nat_Tabla0_2",      tp3::Dato::datoNat(0));

    r.definir("claveStr0", tp3::Dato::datoString("b"));
    r.definir("claveStr1", tp3::Dato::datoString("1"));
    r.definir("str0_2",      tp3::Dato::datoString("asd"));

    return r;
}


Registro_tp3 registro_Tabla0_2() {
    Registro_tp3 r;

    r.definir("claveNat0", tp3::Dato::datoNat(2));
    r.definir("claveNat1", tp3::Dato::datoNat(4));
    r.definir("Nat_Tabla0_2",      tp3::Dato::datoNat(42));

    r.definir("claveStr0", tp3::Dato::datoString("c"));
    r.definir("claveStr1", tp3::Dato::datoString("2"));
    r.definir("str0_2",      tp3::Dato::datoString("asd"));

    return r;
}


Registro_tp3 registro_Tabla0_3() {
    Registro_tp3 r;

    r.definir("claveNat0", tp3::Dato::datoNat(3));
    r.definir("claveNat1", tp3::Dato::datoNat(8));
    r.definir("Nat_Tabla0_2",      tp3::Dato::datoNat(42));

    r.definir("claveStr0", tp3::Dato::datoString("d"));
    r.definir("claveStr1", tp3::Dato::datoString("3"));
    r.definir("str0_2",      tp3::Dato::datoString("aoe"));

    return r;
}

Registro_tp3 registro_Tabla1_0() {
    Registro_tp3 r;

    r.definir("claveNat0", tp3::Dato::datoNat(0));
    r.definir("claveNat1", tp3::Dato::datoNat(0));
    r.definir("Nat_Tabla1_2",      tp3::Dato::datoNat(0));

    r.definir("claveStr0", tp3::Dato::datoString("a"));
    r.definir("claveStr1", tp3::Dato::datoString("0"));
    r.definir("str1_2",      tp3::Dato::datoString(""));

    return r;
}

Registro_tp3 registro_Tabla1_1() {
    Registro_tp3 r;

    r.definir("claveNat0", tp3::Dato::datoNat(1));
    r.definir("claveNat1", tp3::Dato::datoNat(2));
    r.definir("Nat_Tabla1_2",      tp3::Dato::datoNat(0));

    r.definir("claveStr0", tp3::Dato::datoString("b"));
    r.definir("claveStr1", tp3::Dato::datoString("1"));
    r.definir("str1_2",      tp3::Dato::datoString("asd"));

    return r;
}


Registro_tp3 registro_Tabla1_2() {
    Registro_tp3 r;

    r.definir("claveNat0", tp3::Dato::datoNat(2));
    r.definir("claveNat1", tp3::Dato::datoNat(4));
    r.definir("Nat_Tabla1_2",      tp3::Dato::datoNat(42));

    r.definir("claveStr0", tp3::Dato::datoString("c"));
    r.definir("claveStr1", tp3::Dato::datoString("2"));
    r.definir("str1_2",      tp3::Dato::datoString("asd"));

    return r;
}


Registro_tp3 registro_Tabla1_3() {
    Registro_tp3 r;

    r.definir("claveNat0", tp3::Dato::datoNat(3));
    r.definir("claveNat1", tp3::Dato::datoNat(8));
    r.definir("Nat_Tabla1_2",    tp3::Dato::datoNat(42));

    r.definir("claveStr0", tp3::Dato::datoString("d"));
    r.definir("claveStr1", tp3::Dato::datoString("3"));
    r.definir("str1_2",    tp3::Dato::datoString("aoe"));

    return r;
}

tp3::Tabla Tabla0() {
    Registro_tp3 campos = sampleCampos0();
    aed2::Conj<std::string> claves = clavesTabla0();

    tp3::Tabla t = tp3::Tabla("Tabla0", claves, campos);

    return t;
}

tp3::Tabla Tabla1() {
    Registro_tp3 campos = sampleCampos1();
    aed2::Conj<std::string> claves = clavesTabla1();

    tp3::Tabla t = tp3::Tabla("Tabla1", claves, campos);

    return t;
}
