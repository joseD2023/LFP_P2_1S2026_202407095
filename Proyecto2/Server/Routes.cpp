#include "Routes.h"
#include "../Lexer/LexicalAnalyzer.h"
#include "../Parser/SyntaxAnalyzer.h"
#include <iostream>
#include <map>
#include <fstream>

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

string escaparJSON(const string& s) {
    string resultado;
    for (char c : s) {
        if (c == '"') resultado += "\\\"";
        else if (c == '\\') resultado += "\\\\";
        else if (c == '\n') resultado += "\\n";
        else if (c == '\r') resultado += "\\r";
        else if (c == '\t') resultado += "\\t";
        else resultado += c;
    }
    return resultado;
}



//unificamos los errores sintacticos y lexicos
string generarReporteUnificado(LexicalAnalyzer& lexer, SyntaxAnalyzer& parser) {
    string json = "{ \"errores\": [";
    int contador = 1;

    // Errores LÉXICOS
    for (auto& e : lexer.errores) {
        json += "{";
        json += "\"no\":" + to_string(contador++) + ",";
        json += "\"lexema\":\"" + escaparJSON(e.lexema) + "\",";
        json += "\"tipo\":\"Léxico\",";
        json += "\"descripcion\":\"" + escaparJSON(e.descripcion) + "\",";
        json += "\"fila\":" + to_string(e.fila) + ",";
        json += "\"columna\":" + to_string(e.columna);
        json += "},";
    }

    // Error SINTÁCTICO (el primero que encontró el parser)
    if (parser.error.existe) {
        json += "{";
        json += "\"no\":" + to_string(contador) + ",";
        json += "\"lexema\":\"" + escaparJSON(parser.error.lexema) + "\",";
        json += "\"tipo\":\"Sintáctico\",";
        json += "\"descripcion\":\"" + escaparJSON(parser.error.mensaje) + "\",";
        json += "\"fila\":" + to_string(parser.error.fila) + ",";
        json += "\"columna\":" + to_string(parser.error.columna);
        json += "}";
    } else if (!lexer.errores.empty() && json.back() == ',') {
        json.pop_back();
    }

    json += "]}";
    return json;
}

string generarReporte2(LexicalAnalyzer& lexer) {
    map<string, InfoPersona> data;
    int totalTareas = 0;
    string responsableActual = "";
    string prioridadActual = "";
    bool enTarea = false;

    for (size_t i = 0; i < lexer.tokens.size(); i++) {
        string tipo = lexer.tokens[i].getTipoToken();

        if (tipo == "TAREA") {
            if (enTarea && !responsableActual.empty()) {
                data[responsableActual].total++;
                if (prioridadActual == "ALTA") data[responsableActual].alta++;
                else if (prioridadActual == "MEDIA") data[responsableActual].media++;
                else if (prioridadActual == "BAJA") data[responsableActual].baja++;
                totalTareas++;
            }
            responsableActual = "";
            prioridadActual = "";
            enTarea = true;
        }

        if (tipo == "PRIORIDAD" && enTarea) {
            if (i + 2 < lexer.tokens.size()) {
                prioridadActual = lexer.tokens[i + 2].getTipoToken();
            }
        }

        if (tipo == "RESPONSABLE" && enTarea) {
            if (i + 2 < lexer.tokens.size()) {
                responsableActual = limpiarComillas(lexer.tokens[i + 2].lexema);
            }
        }
    }

    if (enTarea && !responsableActual.empty()) {
        data[responsableActual].total++;
        if (prioridadActual == "ALTA") data[responsableActual].alta++;
        else if (prioridadActual == "MEDIA") data[responsableActual].media++;
        else if (prioridadActual == "BAJA") data[responsableActual].baja++;
        totalTareas++;
    }

    string json = "{ \"reporte2\": [";
    int i = 0;
    for (auto& entry : data) {
        string nombre = entry.first;
        InfoPersona info = entry.second;
        int porcentaje = (totalTareas == 0) ? 0 : (info.total * 100 / totalTareas);
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



//nuevo enpoint para visualizar

// Agrega esta función para generar el JSON de tokens
string generarReporteTokens(LexicalAnalyzer& lexer) {
    string json = "{ \"tokens\": [";

    for (size_t i = 0; i < lexer.tokens.size(); i++) {
        Token& t = lexer.tokens[i];
        json += "{";
        json += "\"no\":" + to_string(i + 1) + ",";
        json += "\"lexema\":\"" + escaparJSON(t.lexema) + "\",";
        json += "\"tipo\":\"" + t.getTipoToken() + "\",";
        json += "\"fila\":" + to_string(t.fila) + ",";
        json += "\"columna\":" + to_string(t.columna);
        json += "}";
        if (i < lexer.tokens.size() - 1) json += ",";
    }

    json += "]}";
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

        ofstream file("arbol.dot");
        file << parser.dot;
        file.close();

        if (parser.error.existe) {
            return response(
                "{\"estado\":\"error\",\"mensaje\":\"" + parser.error.mensaje + "\"}",
                "application/json"
            );
        }
        return response(parser.toJSON(), "application/json");
    }


    //endpoint tokens

    // Agrega esto en Routes.cpp dentro de Routes::handle()
    if (request.find("POST /getTokens") != string::npos) {
        string body = extraerBody(request);
        if (body.empty()) {
            return response("{\"tokens\":[]}", "application/json");
        }

        LexicalAnalyzer lexer(body);
        lexer.analyze();

        return response(generarReporteTokens(lexer), "application/json");
    }

    if (request.find("GET /download-dot") != string::npos) {
        ifstream file("arbol.dot");
        if (!file.is_open()) {
            return response("digraph {};", "text/plain");
        }
        string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        return response(content, "text/plain");
    }

    if (request.find("POST /report2") != string::npos) {
        string body = extraerBody(request);
        if (body.empty()) {
            return response("{\"reporte2\":[]}", "application/json");
        }
        LexicalAnalyzer lexer(body);
        lexer.analyze();
        return response(generarReporte2(lexer), "application/json");
    }

    // ENDPOINT /report3 - Devuelve errores léxicos + sintácticos
    if (request.find("POST /report3") != string::npos) {
        string body = extraerBody(request);
        if (body.empty()) {
            return response("{\"errores\":[]}", "application/json");
        }

        // Análisis Léxico
        LexicalAnalyzer lexer(body);
        lexer.analyze();

        // Análisis Sintáctico
        SyntaxAnalyzer parser(lexer.tokens);
        parser.parsePrograma();

        // Devolver errores unificados (léxicos + sintácticos)
        return response(generarReporteUnificado(lexer, parser), "application/json");
    }

    return "HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\n";
}