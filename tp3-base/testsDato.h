#include <string>
#include <iostream>

#include "../Dato.h"
#include "mini_test.h"

namespace testsDato
{
    void constructor() {
        tp3::Dato dN = tp3::Dato::datoNat(42);
        tp3::Dato dS = tp3::Dato::datoString("asd");

        ASSERT(dN.esNat());
        ASSERT(!dN.esString());

        ASSERT(!dS.esNat());
        ASSERT(dS.esString());

        ASSERT_EQ(dN.dame_valorNat(), 42);
        ASSERT_EQ(dS.dame_valorStr(), "asd");
    }

    void cmp() {
        tp3::Dato dN0 = tp3::Dato::datoNat(0);
        tp3::Dato dN1 = tp3::Dato::datoNat(42);
        tp3::Dato dN2 = tp3::Dato::datoNat(42);
        tp3::Dato dS0 = tp3::Dato::datoString("");
        tp3::Dato dS1 = tp3::Dato::datoString("asd");
        tp3::Dato dS2 = tp3::Dato::datoString("asd");

        ASSERT(dN0 != dN1);
        ASSERT(dN1 == dN2);

        ASSERT(dS0 != dS1);
        ASSERT(dS1 == dS2);

        ASSERT(dN0 != dS0);
    }

    void ord() {
        tp3::Dato dN0 = tp3::Dato::datoNat(0);
        tp3::Dato dN1 = tp3::Dato::datoNat(42);
        tp3::Dato dN2 = tp3::Dato::datoNat(42);
        tp3::Dato dS0 = tp3::Dato::datoString("");
        tp3::Dato dS1 = tp3::Dato::datoString("asd");
        tp3::Dato dS2 = tp3::Dato::datoString("asd");

        ASSERT(dN0 < dN1);
        ASSERT(!(dN1 < dN2));

        ASSERT(dS0 < dS1);
        ASSERT(!(dS1 < dS2));

    }

    void main(int, char**) {
        std::cout << "******** dato **********" << std::endl;
        constructor() ;
        cmp() ;
        ord() ;
    }
}
