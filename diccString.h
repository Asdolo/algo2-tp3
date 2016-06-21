
#ifndef diccString_H_INCLUDED
#define diccString_H_INCLUDED

#include "TiposBasicos.h"
    template<class significado>
    class diccString{

        public:

            /**
             * Crea un dicc vacio.
             */
            vacio();

           /**
             * define un string,significado en el dicc.
             */
            definir(String clave,significado significado);
            bool def?(String clave);
            significado obtener(String clave);
            Conj(String) claves();
            borrar(String clave);
            Iterador vistaDicc();
            copiar??
            String min();
            String max();
            diccString(const diccString& otro);


        private:
            trie nodoTrie;
            list(<String clave,significado significado>) valores;
           struct trie
           {
            Iterador* valor;
            Array<*trie> hijos;
            unsigned int cantHijos; 
           };
    };

   
    bool operator==(const diccString& d1, const diccString<T>& d2);

    template<class significado>
    std::ostream& operator<<(std::ostream& os, const diccString& d);


diccString::vacio() : nodoTrie(new trie), valores(crearIt(new List)){}

}



#endif
