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
        case TipoToken::PALABRARESEVADA:
            return "PALABRARESERVADA";
        case TipoToken::NUMBER:
            return "NUMBER";
        case TipoToken::STRING:
            return "STRING";
        case TipoToken::DELIMITADORES:
            return "DELIMITADORES";
        case TipoToken::FECHA:
            return "FECHA";
        case TipoToken::PRIORIDADES:
            return "PRIORIDADES";
        case TipoToken::DESCONOCIDO:
            return "DESCONOCIDO";
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
        default:
            return "UNKNOWN";
    }
}
