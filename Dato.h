
#ifndef DATO_H_INCLUDED
#define DATO_H_INCLUDED

#include "TiposBasicos.h"

    template<class T>
    class Dato{
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

   
    bool operator==(const Dato& d1, const Dato<T>& d2);

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

	Dato::datoString(string s){
		nat?=false;
		valorStr=s;
		valorNat=0;
	}

	Dato::datoNat(unsigned int n){
		nat?=true;
		valorStr=n;
		valorNat=0;
	}

    bool mismoTipo?(Dato d1,Dato d2){
    	return (d1.esNat?==d2.esNat?);
    }

    Dato min(Conj(Dato) c){
    	Iterador it = c.crearIt();
    	Dato minimo = it.siguiente();
    	while(it.haySiguiente?()){
    		if ( menorOIgual(it.siguiente(),minimo)){
    			minimo=it.siguiente();
    		}
			it.Avanzar();
    	}
    	return minimo;
    }

      Dato max(Conj(Dato) c){
    	Iterador it = c.crearIt();
    	Dato maximo = it.siguiente();
    	while(it.haySiguiente?()){
    		if ( !menorOIgual(it.siguiente(),maximo)){
    			maximo=it.siguiente();
    		}
			it.Avanzar();
    	}
    	return maximo;
    }

bool menorOIgual(Dato d1, Dato d2){
	assert(mismoTipo?(d1,d2))
	if (d1.esNat?){
		return d1.datoNat<=d2.datoNat;
	}
	else{
		bool res=true;
		unsigned int i =0;
		while(res && i<min(longitud(d1.valorStr),longitud(e2.valorStr))){
			if (d1.valorStr[i]>d2.valorStr[i]){
				res=false;
			}
			i++;
		}
	}
	return res;
}

    bool operator==(const Dato& d1, const Dato d2&){
    	if (mismoTipo?(d1,d2)){
    		if (d1.esNat?){
    			return (d1.valorNat==d2.valorNat);
    		}else{
    			return (d1.valorStr==d2.valorStr);
    		}
    	}else{
    		return false;
    	}
    }
}



#endif
