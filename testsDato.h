#include <string>
#include <iostream>

#include "Dato.h"
#include "mini_test.h"


void constructor() {
    tp3::Dato datoNatural = tp3::Dato::datoNat(42);
    tp3::Dato datoString = tp3::Dato::datoString("Luis");

    ASSERT(datoNatural.esNat());
    ASSERT(!datoNatural.esString());

    ASSERT(!datoString.esNat());
    ASSERT(datoString.esString());

    ASSERT_EQ(datoNatural.dame_valorNat(), 42);
    ASSERT_EQ(datoString.dame_valorStr(), "Luis");
}

void comparaciones() {
    tp3::Dato datoNatural0 = tp3::Dato::datoNat(0);
    tp3::Dato datoNatural1 = tp3::Dato::datoNat(42);
    tp3::Dato datoNatural2 = tp3::Dato::datoNat(42);
    tp3::Dato datoString0 = tp3::Dato::datoString("");
    tp3::Dato datoString1 = tp3::Dato::datoString("Luis");
    tp3::Dato datoString2 = tp3::Dato::datoString("Luis");

    ASSERT(datoNatural0 != datoNatural1);
    ASSERT(datoNatural1 == datoNatural2);

    ASSERT(datoString0 != datoString1);
    ASSERT(datoString1 == datoString2);

    ASSERT(datoNatural0 != datoString0);
}

void orden() {
    tp3::Dato datoNatural0 = tp3::Dato::datoNat(0);
    tp3::Dato datoNatural1 = tp3::Dato::datoNat(42);
    tp3::Dato datoNatural2 = tp3::Dato::datoNat(42);
    tp3::Dato datoString0 = tp3::Dato::datoString("");
    tp3::Dato datoString1 = tp3::Dato::datoString("Luis");
    tp3::Dato datoString2 = tp3::Dato::datoString("Luis");

    ASSERT(datoNatural0 < datoNatural1);
    ASSERT(!(datoNatural1 < datoNatural2));

    ASSERT(datoString0 < datoString1);
    ASSERT(!(datoString1 < datoString2));

}

void testsDato(int, char**) {
    std::cout << "TEST DATO" << std::endl;
    RUN_TEST(constructor) ;
    RUN_TEST(comparaciones) ;
    RUN_TEST(orden) ;
}
