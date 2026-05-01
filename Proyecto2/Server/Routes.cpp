#include "Routes.h"
#include "../Lexer/LexicalAnalyzer.h"
#include "../Parser/SyntaxAnalyzer.h"
#include <iostream>

using namespace std;


string response(const string& body, const string& type) {

    return
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: " + type + "\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n"
        "Connection: close\r\n"
        "\r\n" + body;
}


string extraerBody(const string& request) {

    size_t pos = request.find("\r\n\r\n");

    if (pos == string::npos) {
        cout << "ERROR: sin body HTTP\n";
        return "";
    }

    string body = request.substr(pos + 4);

    cout << "\n=== DEBUG BODY ===\n" << body << endl;

    return body;
}

/*Aqui vamos a hacer los reportes De Errores Lexicos*/
string generarReporteErroresLexicos(LexicalAnalyzer& lexer) {

    cout << "\n=== GENERANDO ERRORES ===\n";
    cout << "Cantidad: " << lexer.errores.size() << endl;

    string json = "{ \"errores\": [";
    int i = 1;

    for (auto& e : lexer.errores) {

        cout << "\nERROR " << i << endl;
        cout << "lexema: " << e.lexema << endl;

        json += "{";
        json += "\"no\":" + to_string(i++) + ",";
        json += "\"lexema\":\"" + e.lexema + "\",";
        cout << "Lexema: " << e.lexema << endl;
        json += "\"tipo\":\"Léxico\",";
        json += "\"descripcion\":\"" + e.descripcion + "\",";
        cout << "Descripcion: " << e.descripcion << endl;
        json += "\"fila\":" + to_string(e.fila) + ",";
        cout << "Fila: " << e.fila << endl;
        json += "\"columna\":" + to_string(e.columna) + ",";
        cout << "Columna: " << e.columna << endl;
        json += "\"gravedad\":\"" + e.gravedad + "\"";
        cout << "Gravedad: " << e.gravedad << endl;
        json += "},";
    }

    if (!lexer.errores.empty())
        json.pop_back();

    json += "]}";

    cout << "\nJSON FINAL:\n" << json << endl;

    return json;
}


string Routes::handle(string request) {

    cout << "\n=== ROUTE HIT ===\n";

    /* ANALYZE */
    if (request.find("POST /analyze") != string::npos) {

        string body = extraerBody(request);

        if (body.empty()) {
            cout << "BODY VACÍO ANALYZE\n";
            return response("{\"error\":\"body vacío\"}", "application/json");
        }

        LexicalAnalyzer lexer(body);
        lexer.analyze();

        cout << "TOKENS: " << lexer.tokens.size() << endl;

        SyntaxAnalyzer parser(lexer.tokens);
        parser.parsePrograma();

        if (parser.error.existe) {
            return response(
                "{\"estado\":\"error\",\"mensaje\":\"" + parser.error.mensaje + "\"}",
                "application/json"
            );
        }

        return response(parser.toJSON(), "application/json");
    }

    /* reporte 3 de errores */
    if (request.find("POST /report3") != string::npos) {

        cout << "\n    REPORT3    \n";

        string body = extraerBody(request);

        if (body.empty()) {
            cout << "BODY VACÍO REPORT3\n";
            return response("{\"errores\":[]}", "application/json");
        }

        LexicalAnalyzer lexer(body);
        lexer.analyze();

        cout << "TOKENS REPORT3: " << lexer.tokens.size() << endl;
        cout << "ERRORES REPORT3: " << lexer.errores.size() << endl;

        return response(
            generarReporteErroresLexicos(lexer),
            "application/json"
        );
    }

    return "HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\n";
}