#ifndef PILA_H_INCLUDED
#define PILA_H_INCLUDED

#include "aed2.h"
#include <cassert>
#include <iostream>

using namespace std;

template<typename T>
class Pila {
  public:
    Pila();
    Pila(const Pila<T>& otra);
    //~Pila();
    Pila<T>& operator=(const Pila<T>& otra);

    void apilar(const T& elem);
    T desapilar();
    const T& tope() const;
    //T& desapilar();
    //T& tope() const;
    bool esVacia() const;

    template<typename Q>
    friend ostream& operator<<(ostream& os, const Pila<Q>& p);

  private:
    Lista<T> estr;
};

template<typename T>
std::ostream& operator << (std::ostream& os, const Pila<T>& p);

template<typename T>
Pila<T>::Pila() : estr(Lista<T>()) {};

template<typename T>
Pila<T>::Pila(const Pila<T>& otra) : estr(otra.estr) {}

//template<typename T>
//Pila<T>::~Pila(){
//  while (!esVacia()){
//    desapilar();
//  }
//}

template<typename T>
Pila<T>& Pila<T>::operator=(const Pila<T>& otra){
  estr = otra.estr;
  return *this;
}

template<typename T>
void Pila<T>::apilar(const T& elem){
  estr.AgregarAdelante(elem);
}

template<typename T>
T Pila<T>::desapilar(){
  T res = tope();
  estr.Fin();
  return res;
}

template<typename T>
const T& Pila<T>::tope() const{
  return estr.Primero();
}

template<typename T>
bool Pila<T>::esVacia() const{
  return estr.EsVacia();
}

template <typename T>
ostream& operator<<(ostream& os, const Pila<T>& p){
  return Mostrar(os, p.estr, '<', '>');
}

#endif
