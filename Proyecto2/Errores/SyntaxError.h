/*Definir los errores Sintacticos dentro lo que es el parser*/

#ifndef PROYECTO2_SYNTAXERROR_H
#define PROYECTO2_SYNTAXERROR_H
#include "string"
using namespace std;

struct SyntaxError {
    bool existe = false;
    string mensaje = "";
};

#endif //PROYECTO2_SYNTAXERROR_H