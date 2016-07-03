#ifndef AED2_TIPOS_H_
#define AED2_TIPOS_H_

#include "aed2/TiposBasicos.h"

namespace aed2
{
  typedef String NombreTabla;
  typedef String NombreCampo;

  enum TipoCampo { NAT, STR };

  typedef struct {NombreCampo nombre; TipoCampo tipo;} Columna;
  bool operator == (const Columna& c1, const Columna& c2);
}

#endif // AED2_TIPOS_H_
