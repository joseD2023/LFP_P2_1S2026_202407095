//
// Created by josev on 27/04/2026.
//

#ifndef PROYECTO2_TOKEN_H
#define PROYECTO2_TOKEN_H
#include "string"
using namespace std;

enum class TipoToken {
    /*Palabras Reservadas dentro del Lenguaje*/
    TABLERO,
    COLUMNA,
    TAREA,
    PRIORIDAD,
    RESPONSABLE,
    FECHA_LIMITE, //Fecha

    /*Enumeracion de Prioridad*/
    ALTA,
    BAJA,
    MEDIA,

    /*Literales del Lenguaje*/
    STRING, //Cadena
    FECHA,
    ENTERO,

    /*Delimitadores*/
    LLAVE_ABRE,
    LLAVE_CIERRA,
    CORCHETE_ABRE,
    CORCHETE_CIERRA,
    DOS_PUNTOS,
    COMA,
    PUNTO_COMA,
    PUNTO,
    FIN_ARCHIVO,
    DESCONOCIDO // ningun tipo Token
};


class Token {
public:
    TipoToken tipo;
    string lexema;
    int columna;
    int fila;

    Token() { //Nos va ayudar para utilizarlo en el parser
        tipo = TipoToken::DESCONOCIDO;
        lexema = "";
        fila = 0;
        columna = 0;
    }

    Token(TipoToken tipo, string lexema, int fila, int columna);
    string getTipoToken() const;
};



#endif //PROYECTO2_TOKEN_H