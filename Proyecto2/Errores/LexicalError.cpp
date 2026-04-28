/*Inicializamos la parte de nuestro
 * analizador de Errores
 */

#include "LexicalError.h"

LexicalError::LexicalError(string lexema, int fila, int columna, string descripcion, string gravedad) {
    this -> lexema = lexema;
    this -> fila = fila;
    this -> columna = columna;
    this -> descripcion = descripcion;
    this -> gravedad = gravedad;
}
