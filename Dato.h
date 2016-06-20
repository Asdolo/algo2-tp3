
#ifndef DATO_H_INCLUDED
#define DATO_H_INCLUDED

#include "TiposBasicos.h"

    template<class T>
    class Arreglo{
        bool mismoTipo?(Dato d1,Dato d2);
        Dato min(Conj(Dato) c);
        Dato max(Conj(Dato) c)
        bool menorOIgual(Dato d1,Dato d2);
        bool sonIguales?(Dato d1,Dato d2);

        public:

            /**
             * Crea un dato de tipo string.
             */
            datoString(string s);

           /**
             * Crea un dato de tipo Nat.
             */
            datoNat(unsigned int n);

            /**
             * Devuelve true si el dato es de tipo nat.
             */
            bool esNat?();

            unsigned int valorNat();

            const string& valorStr();

            bool esString?();

            Dato(const Dato& otro);


        private:
            bool nat?;
            String valorStr;
            unsigned int valorNat;
    };

   
    bool operator==(const Dato&, const Arreglo<T>&);

    template<class T>
    std::ostream& operator<<(std::ostream& os, const Dato&);


Dato::datoNat(unsigned int n) : nat?(true), valorStr(""),valorNat(n) {}

Dato::datoString(String s) : nat?(false), valorStr(s),valorNat(0) {}
Dato::Dato(const Dato& otro) : nat?(otro.nat?),valorStr(otro.valorStr),valorNat(otro.valorNat){}


bool Dato::esNat?(){
	return this.nat?;
}

unsigned int Dato::valorNat(){
	assert(nat?);
	return this.valorNat;
}

const string& Dato::valorStr(){
	assert(!nat?);
	return this.valorStr;
}

bool Dato::esString?(){
	return !(this.nat?);
}
}



#endif
