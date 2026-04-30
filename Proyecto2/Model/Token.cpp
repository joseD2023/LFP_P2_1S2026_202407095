/*Entonces aqui vamos a ir colcoando la logica del sistema del
 * lo que son nuestro token
 */

#include "Token.h"

#include <bits/std_function.h>


//inicializamos nuestra clase de tokens
Token::Token(TipoToken tipo, string lexema, int fila, int columna) {
    this -> tipo = tipo;
    this -> lexema = lexema;
    this -> fila = fila;
    this -> columna = columna;
}


string Token::getTipoToken() const {
    switch (tipo) {
        case TipoToken::TABLERO:
            return "TABLERO";
        case TipoToken::COLUMNA:
            return "COLUMNA";
        case TipoToken::TAREA:
            return "TAREA";
        case TipoToken::PRIORIDAD:
            return "PRIORIDAD";
        case TipoToken::RESPONSABLE:
            return "RESPONSABLE";
        case TipoToken::FECHA_LIMITE:
            return "FECHA_LIMITE";
        case TipoToken::ALTA:
            return "ALTA";
        case TipoToken::MEDIA:
            return "MEDIA";
        case TipoToken::BAJA:
            return "BAJA";
        case TipoToken::STRING:
            return "STRING";
        case TipoToken::FECHA:
            return "FECHA";
        case TipoToken::ENTERO:
            return "ENTERO";
        case TipoToken::DOS_PUNTOS:
            return "DOS_PUNTOS";
        case TipoToken::COMA:
            return "COMA";
        case TipoToken::CORCHETE_ABRE:
            return "CORCHETE_ABRE";
        case TipoToken::CORCHETE_CIERRA:
            return "CORCHETE_CIERRA";
        case TipoToken::LLAVE_ABRE:
            return "LLAVE_ABRE";
        case TipoToken::LLAVE_CIERRA:
            return "LLAVE_CIERRA";
        case TipoToken::PUNTO_COMA:
            return "PUNTO_COMA";
        case TipoToken::PUNTO:
            return "PUNTO";
        case TipoToken::FIN_ARCHIVO:
            return "FIN_ARCHIVO";
        case TipoToken::DESCONOCIDO:
            return "DESCONOCIDO";
        default:
            return "UNKNOWN";
    }
}
