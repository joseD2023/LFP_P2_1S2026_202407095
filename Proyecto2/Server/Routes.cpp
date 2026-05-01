#include "Routes.h"
#include "../Lexer/LexicalAnalyzer.h"
#include "../Parser/SyntaxAnalyzer.h"
#include <iostream>
#include <map>

using namespace std;


struct InfoPersona {
    int total = 0;
    int alta = 0;
    int media = 0;
    int baja = 0;
};


string limpiarComillas(string s) {
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

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


string generarReporteErroresLexicos(LexicalAnalyzer& lexer) {

    string json = "{ \"errores\": [";
    int i = 1;

    for (auto& e : lexer.errores) {

        json += "{";
        json += "\"no\":" + to_string(i++) + ",";
        json += "\"lexema\":\"" + e.lexema + "\",";
        json += "\"tipo\":\"Léxico\",";
        json += "\"descripcion\":\"" + e.descripcion + "\",";
        json += "\"fila\":" + to_string(e.fila) + ",";
        json += "\"columna\":" + to_string(e.columna) + ",";
        json += "\"gravedad\":\"" + e.gravedad + "\"";
        json += "},";
    }

    if (!lexer.errores.empty())
        json.pop_back();

    json += "]}";

    return json;
}

string generarReporte2(LexicalAnalyzer& lexer) {

    map<string, InfoPersona> data;
    int totalTareas = 0;

    string responsable = "";

    for (size_t i = 0; i < lexer.tokens.size(); i++) {

        string tipo = lexer.tokens[i].getTipoToken();


        if (tipo == "RESPONSABLE") {
            if (i + 2 < lexer.tokens.size() &&
                lexer.tokens[i + 2].getTipoToken() == "STRING") {
                responsable = limpiarComillas(lexer.tokens[i + 2].lexema);
            }
        }


        if (tipo == "PRIORIDAD") {
            if (i + 2 < lexer.tokens.size()) {

                string p = lexer.tokens[i + 2].getTipoToken();

                if (p == "ALTA" || p == "MEDIA" || p == "BAJA") {

                    if (responsable != "") {

                        data[responsable].total++;

                        if (p == "ALTA") data[responsable].alta++;
                        else if (p == "MEDIA") data[responsable].media++;
                        else if (p == "BAJA") data[responsable].baja++;

                        totalTareas++;
                    }
                }
            }
        }
    }

    string json = "{ \"reporte2\": [";
    int i = 0;

    for (auto& entry : data) {

        string nombre = entry.first; // ya viene limpio
        InfoPersona info = entry.second;

        int porcentaje = (totalTareas == 0)
            ? 0
            : (info.total * 100 / totalTareas);

        json += "{";
        json += "\"responsable\":\"" + nombre + "\",";
        json += "\"total\":" + to_string(info.total) + ",";
        json += "\"ALTA\":" + to_string(info.alta) + ",";
        json += "\"MEDIA\":" + to_string(info.media) + ",";
        json += "\"BAJA\":" + to_string(info.baja) + ",";
        json += "\"porcentaje\":" + to_string(porcentaje);
        json += "}";

        if (++i < data.size()) json += ",";
    }

    json += "] }";

    return json;
}

string Routes::handle(string request) {

    cout << "\n=== ROUTE HIT ===\n";

    if (request.find("POST /analyze") != string::npos) {

        string body = extraerBody(request);

        if (body.empty()) {
            return response("{\"error\":\"body vacío\"}", "application/json");
        }

        LexicalAnalyzer lexer(body);
        lexer.analyze();

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

    if (request.find("POST /report2") != string::npos) {

        string body = extraerBody(request);

        if (body.empty()) {
            return response("{\"reporte2\":[]}", "application/json");
        }

        LexicalAnalyzer lexer(body);
        lexer.analyze();

        return response(
            generarReporte2(lexer),
            "application/json"
        );
    }

    if (request.find("POST /report3") != string::npos) {

        string body = extraerBody(request);

        if (body.empty()) {
            return response("{\"errores\":[]}", "application/json");
        }

        LexicalAnalyzer lexer(body);
        lexer.analyze();

        return response(
            generarReporteErroresLexicos(lexer),
            "application/json"
        );
    }

    return "HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\n";
}