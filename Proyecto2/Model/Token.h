//
// Created by josev on 27/04/2026.
//

#ifndef PROYECTO2_TOKEN_H
#define PROYECTO2_TOKEN_H
#include "string"
using namespace std;

enum class TipoToken {
    PALABRARESEVADA, //Palabra Reservadas
    NUMBER, //Entero
    FECHA, //Fecha
    STRING, //Cadena
    DELIMITADORES, //Delimitadores
    PRIORIDADES,  //Enum Prioridades
    DESCONOCIDO // ningun tipo Token

};


class Token {
public:
    TipoToken tipo;
    string lexema;
    int columna;
    int fila;

    Token(TipoToken tipo, string lexema, int fila, int columna);
    string getTipoToken() const;
};



#endif //PROYECTO2_TOKEN_H