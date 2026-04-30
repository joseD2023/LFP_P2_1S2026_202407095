

#ifndef PROYECTO2_SYNTAXANALYZER_H
#define PROYECTO2_SYNTAXANALYZER_H
#include "../Model/Token.h"
#include "../Errores/SyntaxError.h"
#include "string"
#include "vector"
#include "../Model/Tablero.h"
using namespace std;


class SyntaxAnalyzer {
private:
    vector<Token> tokens; //los que nos dio la parte del analizador lexico
    int pos; //en que token me voy a encontrar para trabajar

    Token current; //Token actual es como un peek();
    Tablero tablero;
    Columna colActual;
    Tarea tareaActual;


public :
    SyntaxError error;
    SyntaxAnalyzer(vector<Token> t); //constructor como tal
    void nextToken(); //avanza al siguiente token
    void match(TipoToken esperando); // si el token es el esperado avanzamos si no lo es nos detenemos
    void parsePrograma();
    void parseColumnas();
    void parseColumna();
    void parseTareas();
    void parseTarea();
    void parseAtributos();
    void parseAtributo();
    void parsePrioridad();
    string toJSON();


};

#endif //PROYECTO2_SYNTAXANALYZER_H