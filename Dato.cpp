#include "Dato.h"

using namespace std;
using namespace aed2;
using namespace tp3;


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

bool Dato::mismoTipo(const Dato& d1,const Dato& d2){
  return (d1.esNat()==d2.esNat());
};

 const Dato min(const aed2::Conj<Dato>& cd){
	aed2::Conj<Dato>::const_Iterador it = cd.CrearIt();
	Dato minimo = it.Siguiente();
	while(it.HaySiguiente()){
		if ( it.Siguiente() <= minimo ){
			minimo = it.Siguiente();
		}
	it.Avanzar();
	}
	return minimo;
}


 const Dato Dato::max(const aed2::Conj<Dato>& cd){
  aed2::Conj<Dato>::const_Iterador it = cd.CrearIt();
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
  bool seguir;
  if ( esNat() ){
		return this->valorNat <= otro.valorNat;
	}
  else {
		res = true;
    seguir = true;
		unsigned int i = 0;
		while( seguir && i < (valorStr).length() && i < (otro.valorStr).length() ){
			if ( valorStr[i] > otro.valorStr[i] ){
				res = false;
        seguir = false;
			} else if (  valorStr[i] < otro.valorStr[i] ){
        res = true;
        seguir = false;
      } else if ( i+1 == valorStr.length() || i+1 == otro.valorStr.length() ){
        res = (valorStr.length() <= otro.valorStr.length());
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

namespace tp3{
	ostream& operator<<(ostream& os, Dato& d){
		d.esNat()?	os << d.dame_valorNat():
								os << d.dame_valorStr();
		return os;
	}
}
