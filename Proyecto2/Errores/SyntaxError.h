#ifndef PROYECTO2_SYNTAXERROR_H
#define PROYECTO2_SYNTAXERROR_H
#include "string"
using namespace std;

struct SyntaxError {
    bool existe = false;
    string mensaje;
    string esperado;
    string encontrado;
    string lexema;
    int fila = 0;
    int columna = 0;

    // Constructor por defecto
    SyntaxError() {}

    // Constructor con parámetros
    SyntaxError(string lex, int f, int c, string msg, string esp, string enc) {
        existe = true;
        lexema = lex;
        fila = f;
        columna = c;
        mensaje = msg;
        esperado = esp;
        encontrado = enc;
    }
};

#endif //PROYECTO2_SYNTAXERROR_H