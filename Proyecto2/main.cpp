#include <iostream>
#include "../Proyecto2/Lexer/LexicalAnalyzer.h"
#include "string"
using namespace std;
int main() {

    string texto= "TABLERO { $";
    LexicalAnalyzer lexical(texto);
    lexical.analyze();
    lexical.mostrarAnalyze();

    return 0;
}