#include "SyntaxAnalyzer.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "../Model/Tablero.h"
using namespace std;

string SyntaxAnalyzer::tipoToString(TipoToken t) {
    switch (t) {
        case TipoToken::TABLERO: return "TABLERO";
        case TipoToken::COLUMNA: return "COLUMNA";
        case TipoToken::TAREA: return "TAREA";
        case TipoToken::PRIORIDAD: return "prioridad";
        case TipoToken::RESPONSABLE: return "responsable";
        case TipoToken::FECHA_LIMITE: return "fecha_limite";
        case TipoToken::STRING: return "STRING";
        case TipoToken::DOS_PUNTOS: return ":";
        case TipoToken::COMA: return ",";
        case TipoToken::LLAVE_ABRE: return "{";
        case TipoToken::LLAVE_CIERRA: return "}";
        case TipoToken::CORCHETE_ABRE: return "[";
        case TipoToken::CORCHETE_CIERRA: return "]";
        case TipoToken::PUNTO_COMA: return ";";
        case TipoToken::ALTA: return "ALTA";
        case TipoToken::MEDIA: return "MEDIA";
        case TipoToken::BAJA: return "BAJA";
        default: return "TOKEN_DESCONOCIDO";
    }
}

string limpiar(string s) {
    if (s.size() >= 2 && s[0] == '"' && s[s.size()-1] == '"') {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

SyntaxAnalyzer::SyntaxAnalyzer(vector<Token> t) {
    tokens = t;
    pos = 0;
    current = tokens[pos];
    error.existe = false;
    tablero = Tablero();
    colActual = Columna();
    tareaActual = Tarea();
    dot  = "digraph ArbolDerivacion {\n";
    dot += "rankdir=TB;\n";
    dot += "node [shape=box, style=filled, fontname=\"Arial\"];\n";
}



string SyntaxAnalyzer::nuevoNodo(string label, string color, bool root) {

    string id = "n" + to_string(nodoId++);

    if (root) {
        dot += id + " [label=\"" + label + "\", fillcolor=\"" + color + "\", fontcolor=white];\n";
    } else {
        dot += id + " [label=\"" + label + "\", fillcolor=\"" + color + "\"];\n";
    }

    return id;
}

void SyntaxAnalyzer::agregarArista(string padre, string hijo) {
    dot += padre + " -> " + hijo + ";\n";
}


string SyntaxAnalyzer::toJSON() {
    string json = "{ \"tablero\": { ";

    json += "\"nombre\": \"" + tablero.nombre + "\",";
    json += "\"columnas\": [";

    for (int i = 0; i < tablero.columnas.size(); i++) {
        Columna col = tablero.columnas[i];

        json += "{";
        json += "\"nombre\": \"" + col.nombre + "\",";
        json += "\"tareas\": [";

        for (int j = 0; j < col.tareas.size(); j++) {
            Tarea t = col.tareas[j];

            json += "{";
            json += "\"nombre\": \"" + t.nombre + "\",";
            json += "\"prioridad\": \"" + t.prioridad + "\",";
            json += "\"responsable\": \"" + t.responsable + "\",";
            json += "\"fecha_limite\": \"" + t.fecha + "\"";
            json += "}";

            if (j < col.tareas.size() - 1) json += ",";
        }

        json += "]}";
        if (i < tablero.columnas.size() - 1) json += ",";
    }

    json += "]}}";
    return json;
}

void SyntaxAnalyzer::nextToken() {
    if (error.existe) return;

    pos++;
    if (pos < tokens.size()) {
        current = tokens[pos];
    }
}

void SyntaxAnalyzer::match(TipoToken esperado) {
    if (error.existe) return;

    if (current.tipo == esperado) {
        nextToken();
    } else {
        error.existe = true;
        error.esperado = tipoToString(esperado);
        error.encontrado = current.getTipoToken();
        error.lexema = current.lexema;
        error.fila = current.fila;
        error.columna = current.columna;
        error.mensaje =
            "Se esperaba '" + error.esperado + "' pero se encontró '" + error.encontrado + "'";
        return;
    }
}



string tareaNodeActual;
string prioridadNodeActual;


void SyntaxAnalyzer::parsePrograma() {

    string root = nuevoNodo("<programa>", "#2E75B6", true);

    if (error.existe) return;

    match(TipoToken::TABLERO);

    string n1 = nuevoNodo("TABLERO");
    agregarArista(root, n1);

    match(TipoToken::STRING);
    tablero.nombre = limpiar(tokens[pos-1].lexema);

    string n2 = nuevoNodo(tablero.nombre);
    agregarArista(n1, n2);

    match(TipoToken::LLAVE_ABRE);

    parseColumnas(n2);

    match(TipoToken::LLAVE_CIERRA);
    match(TipoToken::PUNTO_COMA);

    dot += "}\n";
}

void SyntaxAnalyzer::parseColumnas(string padre) {
    if (error.existe) return;

    parseColumna(padre);

    while (!error.existe && current.tipo == TipoToken::COLUMNA) {
        parseColumna(padre);
    }
}

void SyntaxAnalyzer::parseColumna(string padre) {

    if (error.existe) return;

    match(TipoToken::COLUMNA);

    string colNode = nuevoNodo("COLUMNA");
    agregarArista(padre, colNode);

    match(TipoToken::STRING);
    colActual.nombre = limpiar(tokens[pos-1].lexema);

    string nameNode = nuevoNodo(colActual.nombre);
    agregarArista(colNode, nameNode);

    match(TipoToken::LLAVE_ABRE);

    parseTareas(nameNode);

    match(TipoToken::LLAVE_CIERRA);
    match(TipoToken::PUNTO_COMA);

    tablero.columnas.push_back(colActual);
    colActual = Columna();
}

void SyntaxAnalyzer::parseTareas(string padre) {

    parseTarea(padre);

    while (!error.existe && current.tipo == TipoToken::COMA) {
        match(TipoToken::COMA);
        parseTarea(padre);
    }
}

void SyntaxAnalyzer::parseTarea(string padre) {

    if (error.existe) return;

    match(TipoToken::TAREA);
    match(TipoToken::DOS_PUNTOS);

    tareaNodeActual = nuevoNodo("TAREA");
    agregarArista(padre, tareaNodeActual);

    match(TipoToken::STRING);
    tareaActual.nombre = limpiar(tokens[pos-1].lexema);

    string nameNode = nuevoNodo(tareaActual.nombre);
    agregarArista(tareaNodeActual, nameNode);

    match(TipoToken::CORCHETE_ABRE);

    parseAtributos();

    match(TipoToken::CORCHETE_CIERRA);

    colActual.tareas.push_back(tareaActual);
    tareaActual = Tarea();
}

void SyntaxAnalyzer::parseAtributos() {

    parseAtributo();

    while (!error.existe && current.tipo == TipoToken::COMA) {
        match(TipoToken::COMA);
        parseAtributo();
    }
}

void SyntaxAnalyzer::parseAtributo() {

    if (current.tipo == TipoToken::PRIORIDAD) {
        match(TipoToken::PRIORIDAD);
        match(TipoToken::DOS_PUNTOS);
        parsePrioridad();
    }
    else if (current.tipo == TipoToken::RESPONSABLE) {

        match(TipoToken::RESPONSABLE);
        match(TipoToken::DOS_PUNTOS);

        match(TipoToken::STRING);
        tareaActual.responsable = limpiar(tokens[pos-1].lexema);
    }
    else if (current.tipo == TipoToken::FECHA_LIMITE) {

        match(TipoToken::FECHA_LIMITE);
        match(TipoToken::DOS_PUNTOS);

        match(TipoToken::FECHA);
        tareaActual.fecha = tokens[pos-1].lexema;
    }
}

void SyntaxAnalyzer::parsePrioridad() {

    if (error.existe) return;

    prioridadNodeActual = nuevoNodo("PRIORIDAD");
    agregarArista(tareaNodeActual, prioridadNodeActual);

    if (current.tipo == TipoToken::ALTA) {

        string n = nuevoNodo("ALTA");
        agregarArista(prioridadNodeActual, n);

        tareaActual.prioridad = current.lexema;
        match(TipoToken::ALTA);
    }
    else if (current.tipo == TipoToken::MEDIA) {

        string n = nuevoNodo("MEDIA");
        agregarArista(prioridadNodeActual, n);

        tareaActual.prioridad = current.lexema;
        match(TipoToken::MEDIA);
    }
    else if (current.tipo == TipoToken::BAJA) {

        string n = nuevoNodo("BAJA");
        agregarArista(prioridadNodeActual, n);

        tareaActual.prioridad = current.lexema;
        match(TipoToken::BAJA);
    }
}