/*Definir los errores Sintacticos dentro lo que es el parser*/

#ifndef PROYECTO2_SYNTAXERROR_H
#define PROYECTO2_SYNTAXERROR_H
#include "string"
using namespace std;

struct SyntaxError {
    bool existe = false;
    string mensaje;
    string esperado;
    string encontrado;

    //le agregamos para continuar con el reporte
    string lexema;
    int fila = 0;
    int columna = 0;
};

#endif //PROYECTO2_SYNTAXERROR_H