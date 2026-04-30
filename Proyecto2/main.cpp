#include <iostream>
#include "../Proyecto2/Lexer/LexicalAnalyzer.h"
#include "../Proyecto2/Parser/SyntaxAnalyzer.h"
#include "string"
#include "../Proyecto2/Server/Server.h"
using namespace std;
int main() {

    string texto = R"(TABLERO "Proyecto LFP" {
    COLUMNA "Por Hacer" {
        tarea: "GIT"  [prioridad: ALTA, responsable: "Jorge",
            fecha_limite: 2026-05-01]
    };
};)";
    //LexicalAnalyzer lexical(texto);
    //lexical.analyze();
    //lexical.mostrarAnalyze();
    //SyntaxAnalyzer parser(lexical.tokens);
    //parser.parsePrograma();

    Server server;
    server.start(8080);

    return 0;
}