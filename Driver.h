#ifndef DRIVER_H_
#define DRIVER_H_

#include "Tipos.h"
#include "Conj.h"
#include "TiposBasicos.h"
#include "BaseDeDatos.h"

namespace aed2
{

class Driver
{
  public:

    /**
     * Esta clase representa un dato, que puede tomar valores de String o de Nat.
     */
    class Dato
    {
      public:

        Dato(const aed2::Nat& x);
        Dato(const aed2::String& x);

        bool esNat() const;
        bool esString() const;
        TipoCampo tipo() const;

        const aed2::Nat& dameNat() const;
        const aed2::String& dameString() const;

        bool operator == (const Dato& otro) const;
        bool operator != (const Dato& otro) const;

      private:

        TipoCampo tipo_;

        aed2::Nat nat_;
        aed2::String str_;
    };

    /**
     * Esta clase representa un registro, es decir, un mapeo de nombres de columna a valores.
     */
    typedef aed2::Dicc<NombreCampo, Dato> Registro;

  // Base de datos

    /**
     * inicializa una base de datos vacía.
     */
    Driver();

    /**
     * destruye el contenido de la base de datos, liberando la memoria dinámica alocada.
     */
    ~Driver();

  // Tabla

    /**
     * Crea una tabla nueva en la base de datos.
     *
     * PRE: 'nombre' no pertenece a las tablas de la base de datos.
     * PRE: las 'claves' están incluídas en los nombres de las 'columnas'.
     */
    void crearTabla(const NombreTabla& nombre, const aed2::Conj<Columna>& columnas, const aed2::Conj<NombreCampo>& claves);

    /**
     * Inserta un registro en una tabla de la base de datos.
     *
     * PRE: idem especificación TAD Driver :: insertarEntrada
     */
    void insertarRegistro(const NombreTabla& tabla, const Registro& registro);

    /**
     * Borra una serie de registros de una tabla de la base de datos.
     *
     * PRE:
     *  - 'tabla' pertenece a las tablas de la base de datos.
     *  - 'columna' es una columna de dicha tabla.
     *  - el tipo de 'valor' coincide con el tipo de dicha columna.
     */
    void borrarRegistro(const NombreTabla& tabla, const NombreCampo& columna, const Dato& valor);

    /**
     * Devuelve el conjunto de columnas de la tabla.
     *
     * PRE: 'tabla' pertenece a las tablas de la base de datos.
     */
    aed2::Conj<Columna> columnasDeTabla(const NombreTabla& tabla) const;

    /**
     * Devuelve el conjunto de columnas clave de la tabla.
     *
     * PRE: 'tabla' pertenece a las tablas de la base de datos.
     */
    aed2::Conj<NombreCampo> columnasClaveDeTabla(const NombreTabla& tabla) const;

    /**
     * Devuelve el conjunto de registros de la tabla.
     *
     * PRE: 'tabla' pertenece a las tablas de la base de datos.
     */
    aed2::Conj<Registro> registrosDeTabla(const NombreTabla& tabla) const;

    /**
     * Devuelve la cantidad de accesos que se hicieron a la tabla.
     *
     * PRE: 'tabla' pertenece a las tablas de la base de datos.
     */
    aed2::Nat cantidadDeAccesosDeTabla(const NombreTabla& tabla) const;

    /**
     * Devuelve el minimo valor de una columna de la tabla.
     *
     * PRE:
     *  - 'tabla' pertenece a las tablas de la base de datos.
     *  - idem especificación TAD Tabla :: minimo
     */
    Dato minimo(const NombreTabla& tabla, const NombreCampo& columna) const;

    /**
     * Devuelve el maximo valor de una columna de la tabla.
     *
     * PRE:
     *  - 'tabla' pertenece a las tablas de la base de datos.
     *  - idem especificación TAD Tabla :: maximo
     */
    Dato maximo(const NombreTabla& tabla, const NombreCampo& columna) const;

    /**
     * Devuelve el conjunto de registros de la tabla que sean iguales al criterio.
     *
     * PRE: 'tabla' pertenece a las tablas de la base de datos.
     */
    aed2::Conj<Registro> buscar(const NombreTabla& tabla, const Registro& criterio) const;

    /**
     * Devuelve los nombres de todas las tablas de la base de datos.
     */
    aed2::Conj<NombreTabla> tablas() const;

    /**
     * Devuelve el nombre de la tabla con la mayor cantidad de accesos.
     *
     * PRE: existe al menos una tabla en la base de datos.
     */
    NombreTabla tablaMaxima() const;

  // Indices

    /**
     * Indica si una tabla tiene índice definido sobre un campo de tipo Nat.
     *
     * PRE: 'tabla' pertenece a las tablas de la base de datos.
     */
    bool tieneIndiceNat(const NombreTabla& tabla) const;

    /**
     * Indica si una tabla tiene índice definido sobre un campo de tipo String.
     *
     * PRE: 'tabla' pertenece a las tablas de la base de datos.
     */
    bool tieneIndiceString(const NombreTabla& tabla) const;

    /**
     * Devuelve el campo sobre el cuál está definido el índice de tipo Nat.
     *
     * PRE:
     *  - 'tabla' pertenece a las tablas de la base de datos.
     *  - existe un índice de tipo Nat en dicha tabla.
     */
    NombreCampo campoIndiceNat(const NombreTabla& tabla) const;

    /**
     * Devuelve el campo sobre el cuál está definido el índice de tipo String.
     *
     * PRE:
     *  - 'tabla' pertenece a las tablas de la base de datos.
     *  - existe un índice de tipo String en dicha tabla.
     */
    NombreCampo campoIndiceString(const NombreTabla& tabla) const;

    /**
     * Crea un índicede tipo Nat sobre un campo de la tabla.
     *
     * PRE:
     *  - 'tabla' pertenece a las tablas de la base de datos.
     *  - no existe un índice de tipo Nat en dicha tabla.
     *  - 'campo' pertenece a las columnas de dicha tabla, y la columna es de tipo Nat.
     */
    void crearIndiceNat(const NombreTabla& tabla, const NombreCampo& campo);

    /**
     * Crea un índicede tipo String sobre un campo de la tabla.
     *
     * PRE:
     *  - 'tabla' pertenece a las tablas de la base de datos.
     *  - no existe un índice de tipo String en dicha tabla.
     *  - 'campo' pertenece a las columnas de dicha tabla, y la columna es de tipo String.
     */
    void crearIndiceString(const NombreTabla& tabla, const NombreCampo& campo);

  // Joins

    /**
     * Indica si existe un join entre 'tabla1' y 'tabla2'.
     */
    bool hayJoin(const NombreTabla& tabla1, const NombreTabla& tabla2) const;

    /**
     * Devuelve el campo sobre el cuál se hace el join entre dos tablas.
     *
     * PRE: existe un Join entre las tablas 'tabla1' y 'tabla2'.
     */
    NombreCampo campoJoin(const NombreTabla& tabla1, const NombreTabla& tabla2) const;

    /**
     * Genera un Join entre dos tablas de la base de datos, sobre el 'campo' especificado.
     *
     * PRE: idem especificación TAD Driver :: generarVistaJoin
     */
    void generarVistaJoin(const NombreTabla& tabla1, const NombreTabla& tabla2, const NombreCampo& campo);

    /**
     * Borra un join de la base de datos.
     *
     * PRE: existe un Join entre las tablas 'tabla1' y 'tabla2'.
     */
    void borrarVistaJoin(const NombreTabla& tabla1, const NombreTabla& tabla2);

    /**
     * Devuelve el resultado de un join.
     * Idealmente debería ser 'const', pero como en la materia
     * no manejamos el concepto de 'mutable' lo dejamos comentado.
     *
     * PRE: existe un Join entre las tablas 'tabla1' y 'tabla2'.
     */
    aed2::Conj<Registro> vistaJoin(const NombreTabla& tabla1, const NombreTabla& tabla2)/* const*/;

  private:

    tp3::BaseDeDatos db;


    /************************************************************************
     * TODO: Va a ser necesario instanciar privadamente el Modulo principal *
     * con el cuál interactuar. Además, pueden declarar todas las           *
     * funciones auxiliares que les hagan falta.                            *
     ************************************************************************/

}; // class Driver

}; // namespace aed2

#endif // DRIVER_H_
