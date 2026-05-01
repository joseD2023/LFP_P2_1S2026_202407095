#include "SyntaxAnalyzer.h"
#include <string>
#include <vector>
#include <iostream>
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

/* ========= CONTROL TOKENS ========= */

void SyntaxAnalyzer::nextToken() {
    if (error.existe) return;

    pos++;
    if (pos < tokens.size()) {
        current = tokens[pos];
    }
}

/* 🔥 MATCH MEJORADO */
void SyntaxAnalyzer::match(TipoToken esperado) {
    if (error.existe) return;

    if (current.tipo == esperado) {
        nextToken();
    } else {

        error.existe = true;

        error.esperado = tipoToString(esperado);
        error.encontrado = current.getTipoToken();

        // 🔥 NUEVO (CLAVE)
        error.lexema = current.lexema;
        error.fila = current.fila;
        error.columna = current.columna;

        error.mensaje =
            "Se esperaba '" + error.esperado +
            "' pero se encontró '" + error.encontrado + "'";

        return;
    }
}

/* ========= GRAMÁTICA ========= */

void SyntaxAnalyzer::parsePrograma() {
    if (error.existe) return;

    match(TipoToken::TABLERO);
    match(TipoToken::STRING);

    tablero.nombre = limpiar(tokens[pos-1].lexema);

    match(TipoToken::LLAVE_ABRE);

    parseColumnas();

    match(TipoToken::LLAVE_CIERRA);
    match(TipoToken::PUNTO_COMA);
}

void SyntaxAnalyzer::parseColumnas() {
    if (error.existe) return;

    parseColumna();

    while (!error.existe && current.tipo == TipoToken::COLUMNA) {
        parseColumna();
    }
}

void SyntaxAnalyzer::parseColumna() {
    if (error.existe) return;

    match(TipoToken::COLUMNA);
    match(TipoToken::STRING);

    colActual.nombre = limpiar(tokens[pos-1].lexema);

    match(TipoToken::LLAVE_ABRE);

    parseTareas();

    match(TipoToken::LLAVE_CIERRA);
    match(TipoToken::PUNTO_COMA);

    tablero.columnas.push_back(colActual);
    colActual = Columna();
}

void SyntaxAnalyzer::parseTareas() {
    if (error.existe) return;

    parseTarea();

    while (!error.existe && current.tipo == TipoToken::COMA) {
        match(TipoToken::COMA);
        parseTarea();
    }
}

void SyntaxAnalyzer::parseTarea() {
    if (error.existe) return;

    match(TipoToken::TAREA);
    match(TipoToken::DOS_PUNTOS);

    match(TipoToken::STRING);
    tareaActual.nombre = limpiar(tokens[pos-1].lexema);

    match(TipoToken::CORCHETE_ABRE);

    parseAtributos();

    match(TipoToken::CORCHETE_CIERRA);

    colActual.tareas.push_back(tareaActual);
    tareaActual = Tarea();
}

void SyntaxAnalyzer::parseAtributos() {
    if (error.existe) return;

    parseAtributo();

    while (!error.existe && current.tipo == TipoToken::COMA) {
        match(TipoToken::COMA);
        parseAtributo();
    }
}

void SyntaxAnalyzer::parseAtributo() {
    if (error.existe) return;

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
    else {

        error.existe = true;
        error.lexema = current.lexema;
        error.fila = current.fila;
        error.columna = current.columna;

        error.mensaje = "Atributo no válido: " + current.getTipoToken();
        return;
    }
}

void SyntaxAnalyzer::parsePrioridad() {
    if (error.existe) return;

    if (current.tipo == TipoToken::ALTA) {
        tareaActual.prioridad = current.lexema;
        match(TipoToken::ALTA);
    }
    else if (current.tipo == TipoToken::MEDIA) {
        tareaActual.prioridad = current.lexema;
        match(TipoToken::MEDIA);
    }
    else if (current.tipo == TipoToken::BAJA) {
        tareaActual.prioridad = current.lexema;
        match(TipoToken::BAJA);
    }
    else {
        error.existe = true;
        error.lexema = current.lexema;
        error.fila = current.fila;
        error.columna = current.columna;

        error.mensaje =
            "Se esperaba prioridad válida (ALTA, MEDIA, BAJA) y se encontró '" +
            current.getTipoToken() + "'";
        return;
    }
}