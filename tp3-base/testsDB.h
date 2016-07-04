#include <string>
#include <iostream>

#include "../BaseDeDatos.h"
#include "mini_test.h"

namespace testsDB
{
    Registro_tp3 sampleCampos0();
    Registro_tp3 sampleCampos1();
    aed2::Conj<std::string> sampleClaves0();
    aed2::Conj<std::string> sampleClaves1();

    Registro_tp3 sampleReg0_0();
    Registro_tp3 sampleReg0_1();
    Registro_tp3 sampleReg0_2();
    Registro_tp3 sampleReg0_3();

    Registro_tp3 sampleReg1_0();
    Registro_tp3 sampleReg1_1();
    Registro_tp3 sampleReg1_2();
    Registro_tp3 sampleReg1_3();

    tp3::Tabla sampleTabla0();
    tp3::Tabla sampleTabla1();

    void constructor_y_tablas() {
        tp3::BaseDeDatos db;
        tp3::Tabla t0 = sampleTabla0();
        tp3::Tabla t1 = sampleTabla1();


        db.agregarTabla(t0);
        ASSERT( (db.Tablas().Siguiente() == "tabla0") );
        ASSERT(db.dameTabla("tabla0").nombre() == "tabla0");

        ASSERT_EQ(db.tablaMaxima(), "tabla0");

        db.agregarTabla(t1);
        ASSERT(db.dameTabla("tabla1").nombre() == "tabla1");
    }

    void tablas_maximas() {
        tp3::BaseDeDatos db;
        tp3::Tabla t0 = sampleTabla0();
        tp3::Tabla t1 = sampleTabla1();

        db.agregarTabla(t0);
        db.agregarTabla(t1);

        db.insertarEntrada(sampleReg1_0(), "tabla1");
        db.insertarEntrada(sampleReg1_1(), "tabla1");

        ASSERT_EQ(db.tablaMaxima(), "tabla1");

        db.insertarEntrada(sampleReg0_0(), "tabla0");
        db.insertarEntrada(sampleReg0_1(), "tabla0");
        db.insertarEntrada(sampleReg0_2(), "tabla0");

        ASSERT_EQ(db.tablaMaxima(), "tabla0");

        Registro_tp3 pablo;
        pablo.definir("claveStr0", tp3::Dato::datoString("a"));

        db.Borrar(pablo, "tabla1");
        pablo.borrar("claveStr0");
        pablo.definir("claveStr0", tp3::Dato::datoString("b"));
        db.Borrar(pablo, "tabla1");

        ASSERT_EQ(db.tablaMaxima(), "tabla1");
    }

    void buscar() {
        tp3::BaseDeDatos db;
        tp3::Tabla t0 = sampleTabla0();
        tp3::Tabla t1 = sampleTabla1();

        db.agregarTabla(t0);
        db.agregarTabla(t1);

        db.insertarEntrada(sampleReg0_0(), "tabla0");
        db.insertarEntrada(sampleReg0_1(), "tabla0");
        db.insertarEntrada(sampleReg0_2(), "tabla0");
        db.insertarEntrada(sampleReg0_3(), "tabla0");

        db.insertarEntrada(sampleReg1_0(), "tabla1");
        db.insertarEntrada(sampleReg1_1(), "tabla1");
        db.insertarEntrada(sampleReg1_2(), "tabla1");
        db.insertarEntrada(sampleReg1_3(), "tabla1");

        Registro_tp3 crit;

        crit.definir("str1_2", tp3::Dato::datoString("asd"));

        aed2::Conj<Registro_tp3> res = db.busquedaCriterio(crit, "tabla1");

        ASSERT_EQ(res.Cardinal(), 2);

    }

    void crear_joins_simple() {
        tp3::BaseDeDatos db;
        tp3::Tabla t0 = sampleTabla0();
        tp3::Tabla t1 = sampleTabla1();

        db.agregarTabla(t0);
        db.agregarTabla(t1);

        ASSERT(!db.hayJoin("tabla0","tabla1"));
        ASSERT(!db.hayJoin("tabla1","tabla0"));

        aed2::Conj<Registro_tp3>::const_Iterador it0;
        it0 = db.generarVistaJoin("tabla0","tabla1", "claveNat0");

        ASSERT(db.hayJoin("tabla0","tabla1"));
        ASSERT(!db.hayJoin("tabla1","tabla0"));
        ASSERT_EQ(db.campoJoin("tabla0","tabla1"), "claveNat0");
        ASSERT(!it0.HaySiguiente());

        it0 = db.generarVistaJoin("tabla1","tabla0", "claveStr0");

        ASSERT(db.hayJoin("tabla0","tabla1"));
        ASSERT(db.hayJoin("tabla1","tabla0"));
        ASSERT_EQ(db.campoJoin("tabla0","tabla1"), "claveNat0");
        ASSERT_EQ(db.campoJoin("tabla1","tabla0"), "claveStr0");
        ASSERT(!it0.HaySiguiente());

        aed2::Conj<Registro_tp3>::const_Iterador it1;
        it1 = db.vistaJoin("tabla0", "tabla1");

        ASSERT(!it1.HaySiguiente());

        it1 = db.vistaJoin("tabla1", "tabla0");

        ASSERT(!it1.HaySiguiente());

        db.BorrarJoin("tabla0", "tabla1");

        ASSERT(!db.hayJoin("tabla0","tabla1"));
    }

    void crear_joins_con_datos() {
        tp3::BaseDeDatos db;
        tp3::Tabla t0 = sampleTabla0();
        tp3::Tabla t1 = sampleTabla1();

        db.agregarTabla(t0);
        db.agregarTabla(t1);

        db.insertarEntrada(sampleReg0_0(), "tabla0");
        db.insertarEntrada(sampleReg0_1(), "tabla0");
        db.insertarEntrada(sampleReg0_2(), "tabla0");
        db.insertarEntrada(sampleReg0_3(), "tabla0");

        db.insertarEntrada(sampleReg1_0(), "tabla1");
        db.insertarEntrada(sampleReg1_1(), "tabla1");
        db.insertarEntrada(sampleReg1_2(), "tabla1");
        db.insertarEntrada(sampleReg1_3(), "tabla1");

        aed2::Conj<Registro_tp3>::const_Iterador it0;
        it0 = db.generarVistaJoin("tabla0","tabla1", "claveNat0");

        ASSERT(it0.HaySiguiente());

        aed2::Conj<Registro_tp3>::const_Iterador it1;
        it1 = db.vistaJoin("tabla0", "tabla1");

        ASSERT(it1.HaySiguiente());

        Registro_tp3 r = it1.Siguiente();

        ASSERT(r.def("claveNat0"));
        ASSERT(r.def("nat0_2"));
        ASSERT(r.def("nat1_2"));
    }

    void operaciones_con_joins() {
        tp3::BaseDeDatos db;
        tp3::Tabla t0 = sampleTabla0();
        tp3::Tabla t1 = sampleTabla1();
        aed2::Conj<Registro_tp3>::const_Iterador it0;
        aed2::Conj<Registro_tp3>::const_Iterador it1;

        db.agregarTabla(t0);
        db.agregarTabla(t1);

        db.generarVistaJoin("tabla0","tabla1", "claveNat0");
        db.generarVistaJoin("tabla1","tabla0", "claveStr0");

        db.insertarEntrada(sampleReg0_0(), "tabla0");


        //std::cout << "DESDE ACAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa" << std::endl;

        it0 = db.vistaJoin("tabla0","tabla1");
        it1 = db.vistaJoin("tabla1","tabla0");




        ASSERT(!it0.HaySiguiente());
        ASSERT(!it1.HaySiguiente());

        db.insertarEntrada(sampleReg1_0(), "tabla1");

        it0 = db.vistaJoin("tabla0","tabla1");
        it1 = db.vistaJoin("tabla1","tabla0");


        //std::cout << "TABLA1: " << db.dameTabla("tabla1").registros() << std::endl;
        //std::cout << "TABLA0: " << db.dameTabla("tabla0").registros() << std::endl;

        //std::cout << "t0->t1 :" << db._registrosDelJoin.obtener("tabla0").obtener("tabla1") << std::endl;
        //std::cout << "t1->t0 :" << db._registrosDelJoin.obtener("tabla1").obtener("tabla0") << std::endl;

        ASSERT(it1.HaySiguiente());
        ASSERT(it0.HaySiguiente());
        ASSERT_EQ(it0.Siguiente().obtener("claveNat0").dame_valorNat(), 0);
        ASSERT_EQ(it1.Siguiente().obtener("claveNat0").dame_valorNat(), 0);

        it0.Avanzar();
        it1.Avanzar();

        ASSERT(!it0.HaySiguiente());
        ASSERT(!it1.HaySiguiente());

        db.insertarEntrada(sampleReg0_1(), "tabla0");
        db.insertarEntrada(sampleReg0_2(), "tabla0");
        db.insertarEntrada(sampleReg1_1(), "tabla1");
        db.insertarEntrada(sampleReg1_2(), "tabla1");

        it0 = db.vistaJoin("tabla0","tabla1");
        it1 = db.vistaJoin("tabla1","tabla0");

        //std::cout << "t0->t1 :" << db._registrosDelJoin.obtener("tabla0").obtener("tabla1") << std::endl;
        //std::cout << "t1->t0 :" << db._registrosDelJoin.obtener("tabla1").obtener("tabla0") << std::endl;

        Registro_tp3 pablo2;
        pablo2.definir("claveNat0", tp3::Dato::datoNat(0));
        //pablo2.definir("claveStr0", tp3::Dato::datoString("b"));
        db.Borrar(pablo2, "tabla0");


        it1 = db.vistaJoin("tabla1","tabla0");
        it0 = db.vistaJoin("tabla0","tabla1");


        pablo2.borrar("claveNat0");
        pablo2.definir("claveStr0", tp3::Dato::datoString("b"));
        db.Borrar(pablo2, "tabla1");
        pablo2.borrar("claveStr0");

        db.insertarEntrada(sampleReg0_3(), "tabla0");
        db.insertarEntrada(sampleReg1_3(), "tabla1");

        //std::cout << "t0->t1 :" << db._registrosDelJoin.obtener("tabla0").obtener("tabla1") << std::endl;
        //std::cout << "t1->t0 :" << db._registrosDelJoin.obtener("tabla1").obtener("tabla0") << std::endl;

        //std::cout << "t0->t1 :" << db._registrosDelJoin.obtener("tabla0").obtener("tabla1") << std::endl;
        //std::cout << "t1->t0 :" << db._registrosDelJoin.obtener("tabla1").obtener("tabla0") << std::endl;

        //std::cout << "Tabla 0: " << db.dameTabla("tabla0") << std::endl;


        pablo2.definir("claveNat0", tp3::Dato::datoNat(3));
        //std::cout << endl << endl << "Pablo2: " <<pablo2 << std::endl;
        db.Borrar(pablo2, "tabla0");


        //std::cout << "Tabla 0: " << db.dameTabla("tabla0") << std::endl;
        //std::cout << "Tabla 1: " << db.dameTabla("tabla1") << std::endl;

        it0 = db.vistaJoin("tabla0","tabla1");
        it1 = db.vistaJoin("tabla1","tabla0");

        //std::cout << "t0->t1 :" << db._registrosDelJoin.obtener("tabla0").obtener("tabla1") << std::endl;
        //std::cout << "t1->t0 :" << db._registrosDelJoin.obtener("tabla1").obtener("tabla0") << std::endl;

        ASSERT(it0.HaySiguiente());
        ASSERT(it1.HaySiguiente());

        //std::cout << "t0->t1 :" << db._registrosDelJoin.obtener("tabla0").obtener("tabla1") << std::endl;
        //std::cout << "t1->t0 :" << db._registrosDelJoin.obtener("tabla1").obtener("tabla0") << std::endl;

        //std::cout << "Prim it0: " << it0.Siguiente() << std::endl;
        //std::cout << "Prim it1: " << it1.Siguiente() << std::endl;

        ASSERT_EQ(it0.Siguiente().obtener("claveNat0").dame_valorNat(), 2);
        ASSERT_EQ(it1.Siguiente().obtener("claveNat0").dame_valorNat(), 2);
    }

    void main(int, char**) {
        std::cout << "******** DB ************" << std::endl;
        constructor_y_tablas();
        tablas_maximas();
        buscar();
        crear_joins_simple();
        crear_joins_con_datos();
        operaciones_con_joins();
    }


    /* Auxiliares */

    Registro_tp3 sampleCampos0() {
        Registro_tp3 r;

        r.definir("claveNat0", tp3::Dato::datoNat(48));
        r.definir("claveNat1", tp3::Dato::datoNat(99));
        r.definir("nat0_2", tp3::Dato::datoNat(0));

        r.definir("claveStr0", tp3::Dato::datoString("aoeu"));
        r.definir("claveStr1", tp3::Dato::datoString("hrlo"));
        r.definir("str0_2", tp3::Dato::datoString(""));

        return r;
    }

    Registro_tp3 sampleCampos1() {
        Registro_tp3 r;

        r.definir("claveNat0", tp3::Dato::datoNat(48));
        r.definir("claveNat1", tp3::Dato::datoNat(99));
        r.definir("nat1_2", tp3::Dato::datoNat(0));

        r.definir("claveStr0", tp3::Dato::datoString("aoeu"));
        r.definir("claveStr1", tp3::Dato::datoString("hrlo"));
        r.definir("str1_2", tp3::Dato::datoString(""));

        return r;
    }

    aed2::Conj<std::string> sampleClaves0() {
        aed2::Conj<std::string> claves;

        claves.AgregarRapido("claveNat0");
        claves.AgregarRapido("claveNat1");
        claves.AgregarRapido("claveStr0");
        claves.AgregarRapido("claveStr1");

        return claves;
    }

    aed2::Conj<std::string> sampleClaves1() {
        aed2::Conj<std::string> claves;

        claves.AgregarRapido("claveNat0");
        claves.AgregarRapido("claveNat1");
        claves.AgregarRapido("claveStr0");
        claves.AgregarRapido("claveStr1");

        return claves;
    }

    Registro_tp3 sampleReg0_0() {
        Registro_tp3 r;

        r.definir("claveNat0", tp3::Dato::datoNat(0));
        r.definir("claveNat1", tp3::Dato::datoNat(0));
        r.definir("nat0_2",      tp3::Dato::datoNat(0));

        r.definir("claveStr0", tp3::Dato::datoString("a"));
        r.definir("claveStr1", tp3::Dato::datoString("0"));
        r.definir("str0_2",      tp3::Dato::datoString(""));

        return r;
    }

    Registro_tp3 sampleReg0_1() {
        Registro_tp3 r;

        r.definir("claveNat0", tp3::Dato::datoNat(1));
        r.definir("claveNat1", tp3::Dato::datoNat(2));
        r.definir("nat0_2",      tp3::Dato::datoNat(0));

        r.definir("claveStr0", tp3::Dato::datoString("b"));
        r.definir("claveStr1", tp3::Dato::datoString("1"));
        r.definir("str0_2",      tp3::Dato::datoString("asd"));

        return r;
    }


    Registro_tp3 sampleReg0_2() {
        Registro_tp3 r;

        r.definir("claveNat0", tp3::Dato::datoNat(2));
        r.definir("claveNat1", tp3::Dato::datoNat(4));
        r.definir("nat0_2",      tp3::Dato::datoNat(42));

        r.definir("claveStr0", tp3::Dato::datoString("c"));
        r.definir("claveStr1", tp3::Dato::datoString("2"));
        r.definir("str0_2",      tp3::Dato::datoString("asd"));

        return r;
    }


    Registro_tp3 sampleReg0_3() {
        Registro_tp3 r;

        r.definir("claveNat0", tp3::Dato::datoNat(3));
        r.definir("claveNat1", tp3::Dato::datoNat(8));
        r.definir("nat0_2",      tp3::Dato::datoNat(42));

        r.definir("claveStr0", tp3::Dato::datoString("d"));
        r.definir("claveStr1", tp3::Dato::datoString("3"));
        r.definir("str0_2",      tp3::Dato::datoString("aoe"));

        return r;
    }

    Registro_tp3 sampleReg1_0() {
        Registro_tp3 r;

        r.definir("claveNat0", tp3::Dato::datoNat(0));
        r.definir("claveNat1", tp3::Dato::datoNat(0));
        r.definir("nat1_2",      tp3::Dato::datoNat(0));

        r.definir("claveStr0", tp3::Dato::datoString("a"));
        r.definir("claveStr1", tp3::Dato::datoString("0"));
        r.definir("str1_2",      tp3::Dato::datoString(""));

        return r;
    }

    Registro_tp3 sampleReg1_1() {
        Registro_tp3 r;

        r.definir("claveNat0", tp3::Dato::datoNat(1));
        r.definir("claveNat1", tp3::Dato::datoNat(2));
        r.definir("nat1_2",      tp3::Dato::datoNat(0));

        r.definir("claveStr0", tp3::Dato::datoString("b"));
        r.definir("claveStr1", tp3::Dato::datoString("1"));
        r.definir("str1_2",      tp3::Dato::datoString("asd"));

        return r;
    }


    Registro_tp3 sampleReg1_2() {
        Registro_tp3 r;

        r.definir("claveNat0", tp3::Dato::datoNat(2));
        r.definir("claveNat1", tp3::Dato::datoNat(4));
        r.definir("nat1_2",      tp3::Dato::datoNat(42));

        r.definir("claveStr0", tp3::Dato::datoString("c"));
        r.definir("claveStr1", tp3::Dato::datoString("2"));
        r.definir("str1_2",      tp3::Dato::datoString("asd"));

        return r;
    }


    Registro_tp3 sampleReg1_3() {
        Registro_tp3 r;

        r.definir("claveNat0", tp3::Dato::datoNat(3));
        r.definir("claveNat1", tp3::Dato::datoNat(8));
        r.definir("nat1_2",    tp3::Dato::datoNat(42));

        r.definir("claveStr0", tp3::Dato::datoString("d"));
        r.definir("claveStr1", tp3::Dato::datoString("3"));
        r.definir("str1_2",    tp3::Dato::datoString("aoe"));

        return r;
    }

    tp3::Tabla sampleTabla0() {
        Registro_tp3 campos = sampleCampos0();
        aed2::Conj<std::string> claves = sampleClaves0();

        tp3::Tabla t = tp3::Tabla("tabla0", claves, campos);

        return t;
    }

    tp3::Tabla sampleTabla1() {
        Registro_tp3 campos = sampleCampos1();
        aed2::Conj<std::string> claves = sampleClaves1();

        tp3::Tabla t = tp3::Tabla("tabla1", claves, campos);

        return t;
    }
}
