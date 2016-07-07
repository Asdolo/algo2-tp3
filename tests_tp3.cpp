	// compilar usando:
//   g++ -o tests tests.cpp Driver.cpp <lista de archivos *.cpp propios>

#include "Driver.h"
#include "mini_test.h"

#include <string>
#include <iostream>

// Algunos datos de prueba que voy a usar reiteradamente.

#define NOMBRE_TABLA_PERSONAS "personas"

aed2::Conj<aed2::Columna> columnas_personas;
aed2::Conj<aed2::NombreCampo> columnas_clave_personas;

aed2::Driver::Registro persona_1;
aed2::Driver::Registro persona_2;
aed2::Driver::Registro persona_3;

#define NOMBRE_TABLA_TRABAJOS "trabajos"

aed2::Conj<aed2::Columna> columnas_trabajos;
aed2::Conj<aed2::NombreCampo> columnas_clave_trabajos;

aed2::Driver::Registro trabajo_1;
aed2::Driver::Registro trabajo_3;

void inicializarDatos()
{
  // tabla personas

  columnas_personas.Agregar({"DNI", aed2::NAT});
  columnas_personas.Agregar({"nombre", aed2::STR});
  columnas_personas.Agregar({"apellido", aed2::STR});

  columnas_clave_personas.Agregar("DNI");

  persona_1.Definir("DNI", aed2::Driver::Dato(1));
  persona_1.Definir("nombre", aed2::Driver::Dato("Juan"));
  persona_1.Definir("apellido", aed2::Driver::Dato("Perez"));

  persona_2.Definir("DNI", aed2::Driver::Dato(2));
  persona_2.Definir("nombre", aed2::Driver::Dato("José"));
  persona_2.Definir("apellido", aed2::Driver::Dato("Perez"));

  persona_3.Definir("DNI", aed2::Driver::Dato(3));
  persona_3.Definir("nombre", aed2::Driver::Dato("Carlos"));
  persona_3.Definir("apellido", aed2::Driver::Dato("Ramirez"));

  // tabla trabajos

  columnas_trabajos.Agregar({"DNI", aed2::NAT});
  columnas_trabajos.Agregar({"tipo", aed2::STR});

  columnas_clave_trabajos.Agregar("DNI");

  trabajo_1.Definir("DNI", aed2::Driver::Dato(1));
  trabajo_1.Definir("tipo", aed2::Driver::Dato("Soldador"));

  trabajo_3.Definir("DNI", aed2::Driver::Dato(3));
  trabajo_3.Definir("tipo", aed2::Driver::Dato("Piloto"));
}

#define CREAR_TABLA_PERSONAS( bd ) (bd).crearTabla(NOMBRE_TABLA_PERSONAS, columnas_personas, columnas_clave_personas)
#define CREAR_TABLA_TRABAJOS( bd ) (bd).crearTabla(NOMBRE_TABLA_TRABAJOS, columnas_trabajos, columnas_clave_trabajos)

////////////////////////////////////////////////////////////////////////////////
// funciones auxiliares

/**
 * @brief indica si conj1 está incluído o es igual a conj2.
 */
template<typename T>
bool incluidoEn(const aed2::Conj<T>& conj1, const aed2::Conj<T>& conj2)
{
  typename aed2::Conj<T>::const_Iterador it1 = conj1.CrearIt();
  while( it1.HaySiguiente() )
  {
    if (not conj2.Pertenece( it1.Siguiente() ))
      return false;

    it1.Avanzar();
  }

  return true;
}

/**
 * @brief indica si dicc1 está incluído o es igual a dicc2.
 */
template<typename K, typename V>
bool incluidoEn(const aed2::Dicc<K,V>& dicc1, const aed2::Dicc<K,V>& dicc2)
{
  typename aed2::Dicc<K,V>::const_Iterador it1 = dicc1.CrearIt();
  while( it1.HaySiguiente() )
  {
    if (not dicc2.Definido(it1.SiguienteClave()) or dicc2.Significado(it1.SiguienteClave()) != it1.SiguienteSignificado() )
      return false;

    it1.Avanzar();
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////

void bd_vacia()
{
  aed2::Driver bd;
  ASSERT_EQ(bd.tablas().Cardinal(), 0);
}

void insertar_una_tabla_vacia()
{
  aed2::Driver bd;

  CREAR_TABLA_PERSONAS( bd );

  {
    aed2::Conj<aed2::NombreTabla> nombres_tablas;
    nombres_tablas.Agregar(NOMBRE_TABLA_PERSONAS);

    // TODO el ASSERT_EQ deberia usar el ==
    assert(bd.tablas() == nombres_tablas);
  }

  // TODO el ASSERT_EQ deberia usar el ==
  assert( bd.columnasDeTabla(NOMBRE_TABLA_PERSONAS) == columnas_personas );

  // TODO el ASSERT_EQ deberia usar el ==
  assert( bd.columnasClaveDeTabla(NOMBRE_TABLA_PERSONAS) ==  columnas_clave_personas );
}

void insertar_registros_en_una_tabla()
{
  aed2::Driver bd;

  CREAR_TABLA_PERSONAS( bd );
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_1);
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_2);

  {
    aed2::Conj<aed2::Driver::Registro> registros;
    registros.Agregar( persona_1 );
    registros.Agregar( persona_2 );

    // TODO el ASSERT_EQ deberia usar el ==
    assert( bd.registrosDeTabla(NOMBRE_TABLA_PERSONAS) == registros );
  }
}

void borrar_un_campo_clave_en_una_tabla()
{
  aed2::Driver bd;

  CREAR_TABLA_PERSONAS( bd );
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_1);
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_2);

  bd.borrarRegistro(NOMBRE_TABLA_PERSONAS, "DNI", aed2::Driver::Dato(1));

  {
    aed2::Conj<aed2::Driver::Registro> registros;
    registros.Agregar( persona_2 );

    aed2::Conj<aed2::Driver::Registro> registros2;
    registros2.Agregar( persona_2 );
    registros2.Agregar( persona_1 );

    // TODO el ASSERT_EQ deberia usar el ==
    assert( bd.registrosDeTabla(NOMBRE_TABLA_PERSONAS) == registros );

    bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_1);
    assert( bd.registrosDeTabla(NOMBRE_TABLA_PERSONAS) == registros2 );

    bd.borrarRegistro(NOMBRE_TABLA_PERSONAS, "DNI", aed2::Driver::Dato(1));
    assert( bd.registrosDeTabla(NOMBRE_TABLA_PERSONAS) == registros );
  }
}

/*void borrar_un_campo_no_clave_en_una_tabla()
{
  aed2::Driver bd;

  CREAR_TABLA_PERSONAS( bd );
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_1);
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_2);

  bd.borrarRegistro(NOMBRE_TABLA_PERSONAS, "apellido", aed2::Driver::Dato("Perez"));

  // TODO el ASSERT_EQ deberia usar el ==
  assert( bd.registrosDeTabla(NOMBRE_TABLA_PERSONAS) == aed2::Conj<aed2::Driver::Registro>() );
}*/

void contar_cantidad_de_accesos()
{
  aed2::Driver bd;

  CREAR_TABLA_PERSONAS( bd );

  ASSERT_EQ(bd.cantidadDeAccesosDeTabla(NOMBRE_TABLA_PERSONAS), 0);

  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_1);
  ASSERT_EQ(bd.cantidadDeAccesosDeTabla(NOMBRE_TABLA_PERSONAS), 1);

  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_2);
  ASSERT_EQ(bd.cantidadDeAccesosDeTabla(NOMBRE_TABLA_PERSONAS), 2);

  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_3);
  ASSERT_EQ(bd.cantidadDeAccesosDeTabla(NOMBRE_TABLA_PERSONAS), 3);

  // Solo se borra por campo clave parece, asi que borro esto
  //bd.borrarRegistro(NOMBRE_TABLA_PERSONAS, "apellido", aed2::Driver::Dato("Perez"));
  //ASSERT_EQ(bd.cantidadDeAccesosDeTabla(NOMBRE_TABLA_PERSONAS), 5);

  bd.borrarRegistro(NOMBRE_TABLA_PERSONAS, "DNI", aed2::Driver::Dato(1));
  ASSERT_EQ(bd.cantidadDeAccesosDeTabla(NOMBRE_TABLA_PERSONAS), 4);
}

void tabla_maxima()
{
  aed2::Driver bd;

  CREAR_TABLA_PERSONAS( bd );

  ASSERT_EQ(bd.cantidadDeAccesosDeTabla(NOMBRE_TABLA_PERSONAS), 0);

  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_1);
  ASSERT_EQ(bd.cantidadDeAccesosDeTabla(NOMBRE_TABLA_PERSONAS), 1);

  assert(bd.tablaMaxima() ==NOMBRE_TABLA_PERSONAS);

  CREAR_TABLA_TRABAJOS( bd );

  bd.insertarRegistro(NOMBRE_TABLA_TRABAJOS, trabajo_1);
  bd.insertarRegistro(NOMBRE_TABLA_TRABAJOS, trabajo_3);
  ASSERT_EQ(bd.cantidadDeAccesosDeTabla(NOMBRE_TABLA_TRABAJOS), 2);

  assert(bd.tablaMaxima() ==NOMBRE_TABLA_TRABAJOS);

  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_2);
  ASSERT_EQ(bd.cantidadDeAccesosDeTabla(NOMBRE_TABLA_PERSONAS), 2);
  bd.borrarRegistro(NOMBRE_TABLA_PERSONAS, "DNI", aed2::Driver::Dato(1));

  ASSERT_EQ(bd.cantidadDeAccesosDeTabla(NOMBRE_TABLA_PERSONAS), 3);
  assert(bd.tablaMaxima() ==NOMBRE_TABLA_PERSONAS);

}

void indice_sobre_campo_clave_nat()
{
  aed2::Driver bd;

  CREAR_TABLA_PERSONAS( bd );
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_2);
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_3);
  assert(not bd.tieneIndiceNat(NOMBRE_TABLA_PERSONAS));

  bd.crearIndiceNat(NOMBRE_TABLA_PERSONAS,"DNI");
  assert( bd.tieneIndiceNat(NOMBRE_TABLA_PERSONAS));
  assert( bd.campoIndiceNat(NOMBRE_TABLA_PERSONAS)=="DNI");
}

void minimo_de_tabla()
{
  aed2::Driver bd;

  CREAR_TABLA_PERSONAS( bd );
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_2);
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_3);
  bd.crearIndiceNat(NOMBRE_TABLA_PERSONAS,"DNI");
  assert( bd.minimo(NOMBRE_TABLA_PERSONAS,"DNI")==2);

  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_1);
  assert( bd.minimo(NOMBRE_TABLA_PERSONAS,"DNI")==1);

  bd.borrarRegistro(NOMBRE_TABLA_PERSONAS, "DNI", aed2::Driver::Dato(1));
  assert( bd.minimo(NOMBRE_TABLA_PERSONAS,"DNI")==2);
}

void maximo_de_tabla()
{
  aed2::Driver bd;

  CREAR_TABLA_PERSONAS( bd );
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_1);
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_2);
  bd.crearIndiceNat(NOMBRE_TABLA_PERSONAS,"DNI");
  assert( bd.maximo(NOMBRE_TABLA_PERSONAS,"DNI")==2);

  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_3);
  assert( bd.maximo(NOMBRE_TABLA_PERSONAS,"DNI")==3);

  bd.borrarRegistro(NOMBRE_TABLA_PERSONAS, "DNI", aed2::Driver::Dato(3));
  assert( bd.maximo(NOMBRE_TABLA_PERSONAS,"DNI")==2);
}

void indice_sobre_campo_clave_string()
{
  aed2::Driver bd;

  CREAR_TABLA_PERSONAS( bd );
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_1);
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_2);
  assert(not bd.tieneIndiceString(NOMBRE_TABLA_PERSONAS));

  bd.crearIndiceString(NOMBRE_TABLA_PERSONAS,"nombre");
  assert( bd.tieneIndiceString(NOMBRE_TABLA_PERSONAS));
  assert( bd.campoIndiceString(NOMBRE_TABLA_PERSONAS)=="nombre");
}

void buscar_registro_perteneciente_a_tabla()
{
  aed2::Driver bd;

  CREAR_TABLA_PERSONAS( bd );
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_1);
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_2);
  //bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_3);

  aed2::Driver::Registro criterio_persona_1;
  criterio_persona_1.Definir("DNI", aed2::Driver::Dato(1));
  criterio_persona_1.Definir("nombre", aed2::Driver::Dato("Juan"));
  criterio_persona_1.Definir("apellido", aed2::Driver::Dato("Perez"));

  {
    aed2::Conj<aed2::Driver::Registro> registros;
    registros.Agregar( persona_1 );

    aed2::Conj<aed2::Driver::Registro>::Iterador it = registros.CrearIt();
    it.Avanzar();

    //EL SEGUNDO QUE DEVUELVE TIENE BASURA
    //std::cout << "CARDINAL: " << bd.buscar(NOMBRE_TABLA_PERSONAS,criterio_persona_1).Cardinal() << std::endl;
    assert(  bd.buscar(NOMBRE_TABLA_PERSONAS,criterio_persona_1) == registros );
  }
}

void buscar_registro_no_perteneciente_a_tabla()
{
  aed2::Driver bd;

  CREAR_TABLA_PERSONAS( bd );
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_1);
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_2);
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_3);

  {
    aed2::Driver::Registro criterio_persona_2;
    criterio_persona_2.Definir("DNI", aed2::Driver::Dato(4));

    assert(  bd.buscar(NOMBRE_TABLA_PERSONAS,criterio_persona_2).EsVacio() );
  }

  {
    aed2::Driver::Registro criterio_persona_2;
    criterio_persona_2.Definir("nombre", aed2::Driver::Dato("Juan"));
    criterio_persona_2.Definir("apellido", aed2::Driver::Dato("Ramirez"));

    assert(  bd.buscar(NOMBRE_TABLA_PERSONAS,criterio_persona_2).EsVacio() );
  }
}

////////////////////////////////////////////////////////////////////////////////

void hacer_un_join()
{
  aed2::Driver bd;

  CREAR_TABLA_PERSONAS( bd );
  CREAR_TABLA_TRABAJOS( bd );

  ASSERT( not bd.hayJoin(NOMBRE_TABLA_PERSONAS, NOMBRE_TABLA_TRABAJOS) );

  bd.generarVistaJoin(NOMBRE_TABLA_PERSONAS, NOMBRE_TABLA_TRABAJOS, "DNI");

  ASSERT( bd.hayJoin(NOMBRE_TABLA_PERSONAS, NOMBRE_TABLA_TRABAJOS) );
  ASSERT( bd.campoJoin(NOMBRE_TABLA_PERSONAS, NOMBRE_TABLA_TRABAJOS)== "DNI" );
}

void actualizar_join_despues_de_insertar()
{
  aed2::Driver bd;

  CREAR_TABLA_PERSONAS( bd );
  CREAR_TABLA_TRABAJOS( bd );

  bd.generarVistaJoin(NOMBRE_TABLA_PERSONAS, NOMBRE_TABLA_TRABAJOS, "DNI");

  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_1);
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_2);
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_3);

  bd.insertarRegistro(NOMBRE_TABLA_TRABAJOS, trabajo_1);
  bd.insertarRegistro(NOMBRE_TABLA_TRABAJOS, trabajo_3);

  /**
   * Hasta acá BD:
   *
   * PERSONAS
   *   DNI* | nombre | apellido
   *  ====== ======== ==========
   *   1      Juan     Perez
   *   2      José     Perez
   *   3      Carlos   Ramirez
   *
   * TRABAJOS
   *   DNI* | tipo
   *  ====== ======
   *   1      Soldador
   *   3      Piloto
   *
   * PERSONAS + TRABAJOS (DNI*)
   *   DNI* | nombre | apellido | tipo
   *  ====== ======== ========== ======
   *   1      Juan     Perez      Soldador
   *   3      Carlos   Ramirez    Piloto
   */

  aed2::Conj<aed2::Driver::Registro> join = bd.vistaJoin(NOMBRE_TABLA_PERSONAS, NOMBRE_TABLA_TRABAJOS);

  assert( join.Cardinal() == 2 );

  aed2::Conj<aed2::Driver::Registro>::const_Iterador it = join.CrearIt();
  while( it.HaySiguiente() )
  {
    ASSERT(
      ( incluidoEn(persona_1, it.Siguiente()) and incluidoEn(trabajo_1, it.Siguiente()) ) or
      ( incluidoEn(persona_3, it.Siguiente()) and incluidoEn(trabajo_3, it.Siguiente()) )
    );
    it.Avanzar();
  }
}

void actualizar_join_despues_de_borrar()
{
  aed2::Driver bd;

  CREAR_TABLA_PERSONAS( bd );
  CREAR_TABLA_TRABAJOS( bd );

  bd.generarVistaJoin(NOMBRE_TABLA_PERSONAS, NOMBRE_TABLA_TRABAJOS, "DNI");

  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_1);
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_2);
  bd.insertarRegistro(NOMBRE_TABLA_PERSONAS, persona_3);

  bd.insertarRegistro(NOMBRE_TABLA_TRABAJOS, trabajo_1);
  bd.insertarRegistro(NOMBRE_TABLA_TRABAJOS, trabajo_3);

  aed2::Conj<aed2::Driver::Registro> join = bd.vistaJoin(NOMBRE_TABLA_PERSONAS, NOMBRE_TABLA_TRABAJOS);

  /**
   * Hasta acá BD:
   *
   * PERSONAS
   *   DNI* | nombre | apellido
   *  ====== ======== ==========
   *   1      Juan     Perez
   *   2      José     Perez
   *   3      Carlos   Ramirez
   *
   * TRABAJOS
   *   DNI* | tipo
   *  ====== ======
   *   1      Soldador
   *   3      Piloto
   *
   * PERSONAS + TRABAJOS (DNI*)
   *   DNI* | nombre | apellido | tipo
   *  ====== ======== ========== ======
   *   1      Juan     Perez      Soldador
   *   3      Carlos   Ramirez    Piloto
   */

  bd.borrarRegistro(NOMBRE_TABLA_PERSONAS, "DNI", aed2::Driver::Dato(1));
  aed2::Conj<aed2::Driver::Registro> join2 = bd.vistaJoin(NOMBRE_TABLA_PERSONAS, NOMBRE_TABLA_TRABAJOS);

  /**
   * Hasta acá BD:
   *
   * PERSONAS
   *   DNI* | nombre | apellido
   *  ====== ======== ==========
   *   2      José     Perez
   *   3      Carlos   Ramirez
   *
   * TRABAJOS
   *   DNI* | tipo
   *  ====== ======
   *   1      Soldador
   *   3      Piloto
   *
   * PERSONAS + TRABAJOS (DNI*)
   *   DNI* | nombre | apellido | tipo
   *  ====== ======== ========== ======
   *   3      Carlos   Ramirez    Piloto
   */

  assert( join2.Cardinal() == 1 );

  aed2::Conj<aed2::Driver::Registro>::const_Iterador it2 = join2.CrearIt();
  while( it2.HaySiguiente() )
  {
    assert(incluidoEn(persona_3, it2.Siguiente()) and incluidoEn(trabajo_3, it2.Siguiente()));
    it2.Avanzar();
  }

  bd.borrarRegistro( NOMBRE_TABLA_TRABAJOS, "DNI", aed2::Driver::Dato(3));
  aed2::Conj<aed2::Driver::Registro> join3 = bd.vistaJoin(NOMBRE_TABLA_PERSONAS, NOMBRE_TABLA_TRABAJOS);

  /**
   * Hasta acá BD:
   *
   * PERSONAS
   *   DNI* | nombre | apellido
   *  ====== ======== ==========
   *   2      José     Perez
   *   3      Carlos   Ramirez
   *
   * TRABAJOS
   *   DNI* | tipo
   *  ====== ======
   *   1      Soldador
   *
   * PERSONAS + TRABAJOS (DNI*)
   *   DNI* | nombre | apellido | tipo
   *  ====== ======== ========== ======
   */

  assert( join3.Cardinal() == 0 );
}

void joins_no_son_simetricos()
{
  aed2::Driver bd;

  CREAR_TABLA_PERSONAS( bd );
  CREAR_TABLA_TRABAJOS( bd );

  ASSERT( not bd.hayJoin(NOMBRE_TABLA_PERSONAS, NOMBRE_TABLA_TRABAJOS) );
  ASSERT( not bd.hayJoin(NOMBRE_TABLA_TRABAJOS, NOMBRE_TABLA_PERSONAS) );

  bd.generarVistaJoin(NOMBRE_TABLA_PERSONAS, NOMBRE_TABLA_TRABAJOS, "DNI");

  ASSERT( bd.hayJoin(NOMBRE_TABLA_PERSONAS, NOMBRE_TABLA_TRABAJOS) );
  ASSERT( not bd.hayJoin(NOMBRE_TABLA_TRABAJOS, NOMBRE_TABLA_PERSONAS) );

  bd.generarVistaJoin(NOMBRE_TABLA_TRABAJOS, NOMBRE_TABLA_PERSONAS, "DNI");
  ASSERT( bd.hayJoin(NOMBRE_TABLA_PERSONAS, NOMBRE_TABLA_TRABAJOS) );
  ASSERT(bd.hayJoin(NOMBRE_TABLA_TRABAJOS, NOMBRE_TABLA_PERSONAS) );

  bd.borrarVistaJoin(NOMBRE_TABLA_PERSONAS, NOMBRE_TABLA_TRABAJOS);
  ASSERT(not bd.hayJoin(NOMBRE_TABLA_PERSONAS, NOMBRE_TABLA_TRABAJOS) );
  ASSERT(bd.hayJoin(NOMBRE_TABLA_TRABAJOS, NOMBRE_TABLA_PERSONAS) );
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
  inicializarDatos();

  //////////////////////////////////////////////////////////////////////////////
  // Dato
  //////////////////////////////////////////////////////////////////////////////

  RUN_TEST( bd_vacia );

  //////////////////////////////////////////////////////////////////////////////
  // Tabla
  //////////////////////////////////////////////////////////////////////////////

  RUN_TEST( insertar_una_tabla_vacia );
  RUN_TEST( insertar_registros_en_una_tabla );
  RUN_TEST( borrar_un_campo_clave_en_una_tabla );
  // RUN_TEST( borrar_un_campo_no_clave_en_una_tabla );
  RUN_TEST( contar_cantidad_de_accesos );
  RUN_TEST(tabla_maxima);
  RUN_TEST(indice_sobre_campo_clave_nat);
  RUN_TEST(indice_sobre_campo_clave_string);
  RUN_TEST(minimo_de_tabla);
  RUN_TEST(maximo_de_tabla);

  RUN_TEST(buscar_registro_perteneciente_a_tabla);
  RUN_TEST(buscar_registro_no_perteneciente_a_tabla);

  //////////////////////////////////////////////////////////////////////////////
  // BD
  //////////////////////////////////////////////////////////////////////////////

  RUN_TEST( hacer_un_join );
  RUN_TEST( actualizar_join_despues_de_insertar );
  RUN_TEST( actualizar_join_despues_de_borrar );
  RUN_TEST( joins_no_son_simetricos );

  return 0;
}
