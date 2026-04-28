/*Definimos la parte de nuestra clase del analizado lexico*/

#ifndef PROYECTO2_LEXICALANALYZER_H
#define PROYECTO2_LEXICALANALYZER_H
#include "string"
#include "../Model/Token.h"
#include "../Errores/LexicalError.h"
#include "vector"
using namespace std;



class LexicalAnalyzer {
private:
    string input;
    int pos;
    int fila;
    int columna;

    char peek();
    char avance();

public:


    vector<Token> tokens;
    vector<LexicalError> errores;
    LexicalAnalyzer(string text);
    void analyze();
    void mostrarAnalyze();




};
#endif //PROYECTO2_LEXICALANALYZER_H