#include "Driver.h"

bool aed2::operator == (const aed2::Columna& c1, const aed2::Columna& c2)
{
  return c1.nombre == c2.nombre and c1.tipo == c2.tipo;
}

using namespace aed2;

////////////////////////////////////////////////////////////////////////////////
// Dato
////////////////////////////////////////////////////////////////////////////////

Driver::Dato::Dato(const aed2::Nat& x)
  : tipo_( NAT ), nat_( x )
{}

Driver::Dato::Dato(const aed2::String& x)
  : tipo_( STR ), str_( x )
{}

bool Driver::Dato::esNat() const
{
  return tipo_ == NAT;
}

bool Driver::Dato::esString() const
{
  return tipo_ == STR;
}

TipoCampo Driver::Dato::tipo() const
{
  return tipo_;
}

const aed2::Nat& Driver::Dato::dameNat() const
{
  assert( esNat() );
  return nat_;
}

const aed2::String& Driver::Dato::dameString() const
{
  assert( esString() );
  return str_;
}

bool Driver::Dato::operator == (const Dato& otro) const
{
  return tipo_ == otro.tipo_ and (
    ( tipo_ == NAT and nat_ == otro.nat_ ) or
    ( tipo_ == STR and str_ == otro.str_ )
  );
}

bool Driver::Dato::operator != (const Dato& otro) const
{
  return not (*this == otro);
}

////////////////////////////////////////////////////////////////////////////////
// Base de datos
////////////////////////////////////////////////////////////////////////////////

Driver::Driver()
{
  db = BaseDeDatos();
}

Driver::~Driver()
{
  // TODO ...
  assert(false);
}

// Tablas

void Driver::crearTabla(const NombreTabla& nombre, const aed2::Conj<Columna>& columnas, const aed2::Conj<NombreCampo>& claves)
{
    // AED CONJ COLUMNAS -> TP3 REG
    diccString<tp3::Dato> reg;
    aed2::Conj<Columna>::const_Iterador it = columnas.CrearIt();
    while ( it.HaySiguiente() ){
      bool esNat = (it.Siguiente().tipo == NAT);
      reg.definir( it.Siguiente().nombre , esNat );
      it.Avanzar();
    }
    Tabla tabla = Tabla(nombre, claves, reg);
    db.agregarTabla(tabla);
}

void Driver::insertarRegistro(const NombreTabla& tabla, const Registro& registro)
{
  //AED REG -> TP3 REG
  diccString<Dato> reg;
  aed2::Dicc<NombreCampo, Dato>::const_Iterador it = registro.CrearIt();
  while ( it.HaySiguiente() ){
    Dato dato = it.SiguienteSignificado().esNat?  Dato::datoNat(it.SiguienteSignificado().dameNat) :
                                                  Dato::datoString(it.SiguienteSignificado().dameString);
    reg.definir( it.SiguienteClave() , dato );
    it.Avanzar();
  }

  db.insertarEntrada(tabla, reg);
}

void Driver::borrarRegistro(const NombreTabla& tabla, const NombreCampo& columna, const Dato& valor)
{
  //AED DATO -> TP3 DATO
  tp3::Dato dato = valor.esNat? tp3::Dato::datoNat(valor.dameNat) : tp3::Dato::datoString(valor.dameString);
  tp3::diccString<Dato> crit;
  crit.definir(columna, dato);

  db.Borrar(crit, tabla);
}

aed2::Conj<Columna> Driver::columnasDeTabla(const NombreTabla& tabla) const
{
  //  CAMPOS EN COLOMUNAS
  tp3::Tabla t = db.dameTabla(tabla);
  tp3::Conj<string> campos = t.campos();
  tp3::Conj<string>::const_Iterador it = campos.CrearIt();
  aed2::Conj<Columna> res;
  while ( it.HaySiguiente() ){
    aed2::Columna actual = {it.Siguiente(), t.tipoCampo(it.Siguiente())? NAT : STR};
    res.AgregarRapido( actual );
    it.Avanzar();
  }
  return res;
}

aed2::Conj<NombreCampo> Driver::columnasClaveDeTabla(const NombreTabla& tabla) const
{
  tp3::Tabla t = db.dameTabla(tabla);
  aed2::Conj<Columna> res;
  aed2::Conj<Columna> todos = columnasDeTabla(tabla);
  aed2::Conj<Columna>::const_Iterador it = todos.CrearIt();
  while ( it.HaySiguiente() ){
    string nombre = it.Siguiente().nombre;
    if ( t.claves().Pertenece(nombre) ) {
      res.AgregarRapido(it.Siguiente().nombre);
    }
    it.Avanzar();
  }
  return res;
}

aed2::Conj<Driver::Registro> Driver::registrosDeTabla(const NombreTabla& tabla) const
{
  tp3::Tabla t = db.dameTabla(tabla);
  aed2::Conj<Driver::Registro> res;

  Conj<tp3::diccString<Dato>> regs = t.registros();
  Conj<tp3::diccString<Dato>>::const_Iterador it = regs.CrearIt();
  while ( it.HaySiguiente() ){
    // TP3 REG -> AED REG
    Driver::Registro reg;
    tp3::diccString<Dato> actual = it.Siguiente();
    tp3::Lista<struct tupString<tp3::Dato> >::const_Iterador vistaReg = actual.vistaDicc();
    while ( vistaReg.HaySiguiente() ){
      tp3::Dato dato_tp = vistaReg.Siguiente().significado;
      Driver::Dato dato = dato_tp.esNat? Driver::Dato(dato_tp.dame_valorNat()) : Driver::Dato(dato_tp.dame_valorStr()) ;
      reg.Definir( vistaReg.Siguiente().clave, dato);
      vistaReg.Avanzar();
    }
    res.Agregar(reg);
    it.Avanzar();
  }
  return res;
}

aed2::Nat Driver::cantidadDeAccesosDeTabla(const NombreTabla& tabla) const
{
  tp3::Tabla t = db.dameTabla(tabla);
  return t.cantidadDeAccesos();
}

Driver::Dato Driver::minimo(const NombreTabla& tabla, const NombreCampo& columna) const
{
  tp3::Tabla t = db.dameTabla(tabla);

  tp3::Dato dato_tp = t.minimo(columna);
  Driver::Dato dato = dato_tp.esNat? Driver::Dato(dato_tp.dame_valorNat()) : Driver::Dato(dato_tp.dame_valorStr()) ;
  return dato;
}

Driver::Dato Driver::maximo(const NombreTabla& tabla, const NombreCampo& columna) const
{
  tp3::Tabla t = db.dameTabla(tabla);

  tp3::Dato dato_tp = t.maximo(columna);
  Driver::Dato dato = dato_tp.esNat? Driver::Dato(dato_tp.dame_valorNat()) : Driver::Dato(dato_tp.dame_valorStr()) ;
  return dato;
}

aed2::Conj<Driver::Registro> Driver::buscar(const NombreTabla& tabla, const Registro& criterio) const
{
  aed2::Conj<Driver::Registro> res;
  res = db.busquedaCriterio(criterio,tabla);
  return res;
}

aed2::Conj<NombreTabla> Driver::tablas() const
{
  return db.Tablas();
}

NombreTabla Driver::tablaMaxima() const
{
  return db.tablaMaxima();
}

// Indices

bool Driver::tieneIndiceNat(const NombreTabla& tabla) const
{
   bool res = false;
  tp3::Tabla t = db.dameTabla(tabla);
  Conj<string> indices = t.indices();
  Conj<string>::const_Iterador it = indices.CrearIt();
  while(it.HaySiguiente()){
    if (t.tipoCampo(it.Siguiente()))
    {
      res=true;
    }
    it.Avanzar();
  }
  return res;
}

bool Driver::tieneIndiceString(const NombreTabla& tabla) const
{
  bool res = false;
  tp3::Tabla t = db.dameTabla(tabla);
  Conj<string> indices = t.indices();
  Conj<string>::const_Iterador it = indices.CrearIt();
  while(it.HaySiguiente()){
    if (!t.tipoCampo(it.Siguiente()))
    {
      res=true;
    }
    it.Avanzar();
  }
  return res;
}

NombreCampo Driver::campoIndiceNat(const NombreTabla& tabla) const
{
  tp3::Tabla t = db.dameTabla(tabla);
  Driver::NombreCampo res;
  Conj<string> indices = t.indices();
  Conj<string>::const_Iterador it = indices.CrearIt();
  while(it.HaySiguiente()){
    if (t.tipoCampo(it.Siguiente()))
    {
      res=it.Siguiente();
    }
    it.Avanzar();
  }
  return res;
}

NombreCampo Driver::campoIndiceString(const NombreTabla& tabla) const
{
  tp3::Tabla t = db.dameTabla(tabla);
  Driver::NombreCampo res;
  Conj<string> indices = t.indices();
  Conj<string>::const_Iterador it = indices.CrearIt();
  while(it.HaySiguiente()){
    if (!t.tipoCampo(it.Siguiente()))
    {
      res=it.Siguiente();
    }
    it.Avanzar();
  }
  return res;
}

void Driver::crearIndiceNat(const NombreTabla& tabla, const NombreCampo& campo)
{
  tp3::Tabla t& = db.dameTabla(tabla);
  t.indexar(campo);
}

void Driver::crearIndiceString(const NombreTabla& tabla, const NombreCampo& campo)
{
  tp3::Tabla& t = db.dameTabla(tabla);
  t.indexar(campo);
}

// Joins

bool Driver::hayJoin(const NombreTabla& tabla1, const NombreTabla& tabla2) const
{
  db.hayJoin(tabla1,tabla2);
}

NombreCampo Driver::campoJoin(const NombreTabla& tabla1, const NombreTabla& tabla2) const
{
return db.campoJoin(tabla1,tabla2);
}

void Driver::generarVistaJoin(const NombreTabla& tabla1, const NombreTabla& tabla2, const NombreCampo& campo)
{
  db.generarVistaJoin(tabla1,tabla2,campo);
}

void Driver::borrarVistaJoin(const NombreTabla& tabla1, const NombreTabla& tabla2)
{
  db.borrarJoin(tabla1,tabla2);
}

aed2::Conj<Driver::Registro> Driver::vistaJoin(const NombreTabla& tabla1, const NombreTabla& tabla2)/* const*/
{
  tp3::diccString<tp3::Dato>::const_Iterador it = db.vistaJoin(tabla1.nombre(),tabla2.nombre());
  aed2::Conj<Driver::Registro> res;
  while(it.HaySiguiente()){
    res.AgregarRapido(it.Siguiente());
    it.Avanzar();
  }
  return res;
}
