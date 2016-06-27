
#ifndef DATO_H_INCLUDED
#define DATO_H_INCLUDED

#include "aed2.h"
#include <cassert>
#include <string>
#include <iostream>

using namespace std;

    class Dato{

        public:
            //Operators, constructores, etc
            bool operator<(const Dato& d) const;
            bool operator>(const Dato& d) const;
            bool operator<=(const Dato& d) const;
            bool operator>=(const Dato& d) const;
            bool operator==(const Dato& d) const;
            bool operator!=(const Dato& d) const;
            friend ostream& operator<<(ostream& os, const Dato& d);

            //Interfaz
            static Dato datoString(string s)    { return Dato(false, s, 0);      };
            static Dato datoNat(unsigned int n) { return Dato(true, "vacio", n); };
            bool esNat() const;
            bool esString() const;
            unsigned int dame_valorNat() const;
            const string& dame_valorStr() const;

            static bool mismoTipo(const Dato& d1,const Dato& d2);
            static const Dato min(Conj<const Dato&> cd);
            static const Dato max(Conj<const Dato&> cd);

        private:
            // Campos
            bool nat;
            string valorStr;
            unsigned int valorNat;

            //Constructor auxiliar
            Dato(bool es_nat, string s, unsigned int n) : nat(es_nat), valorStr(s), valorNat(n) {};
    };

    bool Dato::esNat() const{
    	return nat;
    }

    unsigned int Dato::dame_valorNat() const{
    	assert(nat);
    	return valorNat;
    }

    const string& Dato::dame_valorStr() const{
    	assert(!nat);
    	return valorStr;
    }

    bool Dato::esString() const{
    	return !(nat);
    }

    bool mismoTipo(const Dato& d1,const Dato& d2){
      return d1.esNat()==d2.esNat();
    }

    const Dato min(Conj<Dato> c){
    	Conj<Dato>::Iterador it = c.CrearIt();
    	Dato minimo = it.Siguiente();
    	while(it.HaySiguiente()){
    		if ( it.Siguiente() <= minimo ){
    			minimo = it.Siguiente();
    		}
    	it.Avanzar();
    	}
    	return minimo;
    }


    const Dato max(Conj<Dato> c){
      Conj<Dato>::Iterador it = c.CrearIt();
      Dato maximo = it.Siguiente();
      while(it.HaySiguiente()){
      	if ( it.Siguiente() > maximo ){
      		maximo = it.Siguiente();
      	}
      it.Avanzar();
      }
      return maximo;
    }

    bool Dato::operator<=(const Dato& otro) const{
    	assert( mismoTipo(*this, otro) );
      bool res;
      if ( this->esNat() ){
    		return this->valorNat <= otro.valorNat;
    	}
      else {
    		res = true;
    		unsigned int i = 0;
    		while( res && i < (this->valorStr).length() && i < (otro.valorStr).length() ){
    			if ( this->valorStr[i] > otro.valorStr[i] ){
    				res = false;
    			}
    			i++;
    		}
    	}
    	return res;
    }

    bool Dato::operator<(const Dato& otro) const{
      return (*this <= otro) && !(*this == otro );
    }

    bool Dato::operator>(const Dato& otro) const{
      return (otro < *this);
    }

    bool Dato::operator>=(const Dato& otro) const{
      return (otro <= *this);
    }


    bool Dato::operator==(const Dato& otro) const{
    	if (mismoTipo(*this, otro)){
    		if (this->esNat()){
    			return (this->valorNat == otro.valorNat);
    		} else {
    			return (this->valorStr == otro.valorStr);
    		}
    	}else{
    		return false;
    	}
    }

    bool Dato::operator!=(const Dato& otro) const{
      return !(*this == otro);
    }

    ostream& operator<<(ostream& os, const Dato& d){
      d.esNat()?  os << d.valorNat << endl:
                  os << d.valorStr << endl;
      return os;
    }



#endif
