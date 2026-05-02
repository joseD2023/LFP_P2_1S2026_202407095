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
    vector<Token> tokens;
    int pos;
    Token current;

public:
    Tablero tablero;
    Columna colActual;
    Tarea tareaActual;
    SyntaxError error;
    vector<SyntaxError> listaErroresSintacticos;  // <-- AGREGAR ESTO
    string dot;
    int nodoId = 0;

    SyntaxAnalyzer(vector<Token> t);
    void nextToken();
    string tipoToString(TipoToken t);
    void match(TipoToken esperando);
    void parsePrograma();
    void parseColumnas(string padre);
    void parseColumna(string padre);
    void parseTareas(string padre);
    void parseTarea(string padre);
    void parseAtributos();
    void parseAtributo();
    void parsePrioridad();
    string toJSON();
    string nuevoNodo(string label, string color = "#D6EAF8", bool root = false);
    void agregarArista(string padre, string hijo);
    string getErroresSintacticosJSON();  // <-- AGREGAR ESTO
};

#endif //PROYECTO2_SYNTAXANALYZER_H