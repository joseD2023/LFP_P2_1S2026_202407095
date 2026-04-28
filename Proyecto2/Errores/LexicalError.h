//
// Created by josev on 27/04/2026.
//

#ifndef PROYECTO2_LEXICALERROR_H
#define PROYECTO2_LEXICALERROR_H
#include "string"
using namespace std;

class LexicalError {
public:
    string lexema;
    int fila;
    int columna;
    string descripcion;
    string gravedad;

    LexicalError(string lexema, int fila, int columna, string descripcion, string gravedad);


};

#endif //PROYECTO2_LEXICALERROR_H