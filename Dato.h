
#ifndef DATO_H_INCLUDED
#define DATO_H_INCLUDED


#include "Conj.h"
#include "Lista.h"

#include <cassert>
#include <string>
#include <iostream>

using namespace std;
namespace tp3{
    class Dato{

        public:
            //Operators, constructores, etc
            bool operator<(const Dato& d) const;
            bool operator>(const Dato& d) const;
            bool operator<=(const Dato& d) const;
            bool operator>=(const Dato& d) const;
            bool operator==(const Dato& d) const;
            bool operator!=(const Dato& d) const;

            //Interfaz
            static Dato datoString(string s)    { return Dato(false, s, 0);      };
            static Dato datoNat(unsigned int n) { return Dato(true, "vacio", n); };
            bool esNat() const;
            bool esString() const;
            unsigned int dame_valorNat() const;
            const string& dame_valorStr() const;

            static bool mismoTipo(const Dato& d1,const Dato& d2);
            static const Dato min(const aed2::Conj<Dato>& cd);
            static const Dato max(const aed2::Conj<Dato>& cd);
            friend ostream& operator<<(ostream& os, Dato& d);

        private:
            // Campos
            bool nat;
            string valorStr;
            unsigned int valorNat;

            //Constructor auxiliar
            Dato(bool es_nat, string s, unsigned int n) : nat(es_nat), valorStr(s), valorNat(n) {};
    };
}
#endif
