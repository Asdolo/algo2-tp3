#ifndef diccString_H_INCLUDED
#define diccString_H_INCLUDED

#include <string>
#include "aed2.h"
#include <cassert>
#include <iostream>
template<class Significado>
class diccString {

public:

	/**
	 * Crea un dicc vacio.
	 */
	public diccString();
	/**
	 * define un string,significado en el dicc.
	 */
	definir(String clave,Significado significado);
	bool def(String clave);
	Significado obtener(String clave);Conj(String) claves();borrar(String clave);
	Iterador vistaDicc();
	String min();
	String max();
	//FIJARSE SI NECESITAMOS COPIAR!
	diccString(const diccString& otro);

private:
    struct tup{
      string clave;
      Significado significado;
    };
	trie nodoTrie;
        Lista<tup> valores;
	struct trie {
		Iterador* valor;
		Array<trie*> hijos;
		unsigned int cantHijos;
	};
};

bool operator==(const diccString& d1, const diccString<T>& d2);

template<class Significado>
std::ostream& operator<<(std::ostream& os, const diccString& d);

diccString::diccString() :
	nodoTrie(new trie), valores(It(new List)) {
}

template<class Significado>
void diccString<Significado>::definir(string clave, Significado significado) {

	<string, Significado> entrada = <clave, significado>;
	itList(<string, Significado>) iter = (this->valores).AgregarAdelante(entrada);
	trie* actual = &(this->nodoTrie);

	for (int i = 0; i < clave.lenght(); i++) {
		if ((actual->hijos)[clave[i]] == NULL) {
			(actual->hijos)[clave[i]] = new trie;
			(actual->cantHijos)++;
		}
		actual = (actual->hijos)[clave[i]];
	}
	actual->valor = &iter;

}

template<class Significado>
bool diccString<significado>::def?(string clave) {
	trie* actual = &(this->nodoTrie);
	bool res = true;
	int i = 0;

	while (i < clave.length() && res ) {
		if (actual->cantHijos > 0) {
			if (actual->hijos[clave[i]] == NULL) {
				res = false;
			}
			else {
				actual = (actual->hijos[clave[i]]);
			}
			i++;
		}
		else {
			res = false;
		}
	}

	if (res) res = !(actual->valor == NULL);
	return res;
}

template<class Significado>
significado diccString<significado>::obtener(string clave) {
	trie* actual = &(this->nodoTrie);
	for (int i = 0; i < clave.length(); i++) {
		actual = (actual->hijos)[clave[i]];
	}
	res = *(actual->valor).siguiente().significado;
	return res;
}

template<class Significado>
conj<string> claves() {
	itList(<string, significado>) it = crearIt(this->valores);
	conj < string > res = Vacio();
	while (it.haySiguiente()) {
res	.agregarRapido(res, it.siguiente().clave)
	it.avanzar();
}
return res;
}

template<class Significado>
void diccString<significado>::borrar(string clave) {
	trie* actual = &(this->nodoTrie);
	bool listo = false;
	for (int i = 0; i < clave.length(); i++) {
		trie* temp = (actual->hijos)[clave[i]];
		if (temp->cantHijos == 0) {
			delete temp;
			(actual->hijos)[clave[i]] = NULL;
		}
		actual = temp;
	}
	*(actual->valor).eliminarSiguiente();

	actual->valor = NULL;
	actual = &(this->nodoTrie);
	i = 0;
	while (i < clave.length() - 1 && !listo) {
		if (actual->hijos[clave[i]]->cantHijos > 0) {
			actual = actual->hijos[clave[i]];
		} else {
			actual->hijos[clave[i]] = NULL;
			listo = true;
		}
		i++;
	}
}

template<class Significado>
itList<string, significado> diccString<significado>::vistaDicc() {
	return crearIt(this->valores);
}

template<class Significado>
string diccString<significado>::min() {
	trie* actual = &(this->nodoTrie);
	bool termine = false;
	while (!termine) {
		if (actual->valor == NULL) {
			for (int i = 0; i < 256; i++) {
				if (actual->hijos[i] != NULL)
					actual = actual->hijos[i];
			}
		} else {
			termine = true;
			string res = *(actual->valor).siguiente().clave;
		}
	}
	return res;
}

template<class Significado>
string diccString<significado>::max() {
	trie* actual = &(this->nodoTrie);
	bool termine = false;
	while (!termine) {
		if (actual->cantHijos == 0) {
			string res = *(actual->valor).siguiente().clave;
			termine = true;
		} else {
			bool seguir = true;
			for (int i = 255; i >= 0 && seguir; i++) {
				if (actual->hijos[i] != NULL) {
					actual = (actual->hijos[i]);
					seguir = false;
				}
			}
			i++;
		}
	}
	return res;
}

/* vale para constructor por copia tambien*/
template<class Significado>
diccString<significado>& diccString<significado>::operator =(
		const diccString<significado>& other) {
	itList(<string, significado>) it = crearIt(other.valores);
	diccString < significado > res;
	while it.haySiguiente() {
		res.definir(it.siguiente().clave, it.siguiente().significado, res);
		it.avanzar();
	}
	return res;
}

}

#endif
