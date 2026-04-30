#include "Routes.h"
#include "../Lexer/LexicalAnalyzer.h"
#include "../Parser/SyntaxAnalyzer.h"
#include <iostream>

using namespace std;

string extraerBody(string request) {
    size_t pos = request.find("\r\n\r\n");
    if (pos != string::npos) {
        return request.substr(pos + 4);
    }
    return "";
}

string Routes::handle(string request) {

    if (request.find("POST /analyze") != string::npos) {

        string body = extraerBody(request);

        /*El analizador lexico se pone en marca y analiza cualquier caracter */
        LexicalAnalyzer lexer(body);
        lexer.analyze();

        /*Entonces el parse empieza a verificar esos tokens*/
        SyntaxAnalyzer parser(lexer.tokens);
        parser.parsePrograma();

        /*Si eixste algun error entonces entra al formato Json y da el error*/
        if (parser.error.existe) {

            string errorJson =
                "{ \"status\": \"error\", "
                "\"mensaje\": \"" + parser.error.mensaje + "\" }";

            return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + errorJson;
        }

        /*Si no existe algun manda
         * JSON Bueno
         */
        string json = parser.toJSON();


        return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + json;
    }

    /*Not Fount si no encuentra nada*/
    return "HTTP/1.1 404 Not Found\r\n\r\n";
}