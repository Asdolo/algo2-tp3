#ifndef PILA_H_INCLUDED
#define PILA_H_INCLUDED

#include "Conj.h"
#include "Lista.h"
#include <cassert>
#include <iostream>

using namespace std;

namespace tp3
{

//Pila minimal implementada sobre módulo Lista (enlazada)
// (LIFO)


template<typename T>
class Pila {
  public:
    Pila();
    Pila(const tp3::Pila<T>& otra);
    tp3::Pila<T>& operator=(const tp3::Pila<T>& otra);

    void apilar(const T& elem);

    T desapilar();

    const T& tope() const;

    bool esVacia() const;

    template<typename Q>
    friend ostream& operator<<(ostream& os, const tp3::Pila<Q>& p);

  private:
    aed2::Lista<T> estr;
};

template<typename T>
std::ostream& operator << (std::ostream& os, const tp3::Pila<T>& p);

template<typename T>
tp3::Pila<T>::Pila() : estr(aed2::Lista<T>()) {};

template<typename T>
tp3::Pila<T>::Pila(const tp3::Pila<T>& otra) : estr(otra.estr) {}

template<typename T>
tp3::Pila<T>& tp3::Pila<T>::operator=(const tp3::Pila<T>& otra){
  estr = otra.estr;
  return *this;
}

template<typename T>
void tp3::Pila<T>::apilar(const T& elem){
  estr.AgregarAdelante(elem);
}

template<typename T>
T tp3::Pila<T>::desapilar(){
  T res = tope();
  estr.Fin();
  return res;
}

template<typename T>
const T& tp3::Pila<T>::tope() const{
  return estr.Primero();
}

template<typename T>
bool tp3::Pila<T>::esVacia() const{
  return estr.EsVacia();
}

template <typename T>
ostream& operator<<(ostream& os, const tp3::Pila<T>& p){
  return Mostrar(os, p.estr, '<', '>');
}

}
#endif
