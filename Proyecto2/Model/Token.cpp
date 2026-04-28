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
        default:
            return "UNKNOWN";
    }
}
