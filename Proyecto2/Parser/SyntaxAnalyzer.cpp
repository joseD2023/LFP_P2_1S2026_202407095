#include "SyntaxAnalyzer.h"
#include <string>
#include <vector>
#include <iostream>
#include "../Model/Tablero.h"
#include <cstdlib>
#include "vector"
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
    this->tokens = t;
    this->pos = 0;
    this->current = tokens[pos];
    this->error.existe = false;
    this->error.mensaje= "";
    this->tablero = Tablero();
    this->colActual = Columna();
    this->tareaActual = Tarea();
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

/* Empezamos el programa  */

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
        error.mensaje = "Error sintactico: Token Invalido";
        error.esperado = tipoToString(esperado);
        error.encontrado = current.getTipoToken();
        error.existe = true;
        return;
    }
}

/* Comenzamos con la Gramatica */

void SyntaxAnalyzer::parsePrograma() {
    if (error.existe) return;

    match(TipoToken::TABLERO);
    if (error.existe) return;

    tablero.nombre = limpiar(current.lexema);
    match(TipoToken::STRING);
    if (error.existe) return;

    match(TipoToken::LLAVE_ABRE);

    parseColumnas();
    if (error.existe) return;

    match(TipoToken::LLAVE_CIERRA);
    if (error.existe) return;

    match(TipoToken::PUNTO_COMA);

    cout << "Analisis Sintactico terminado Exitosamente" << endl;
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
    if (error.existe) return;

    colActual.nombre = limpiar(current.lexema);
    match(TipoToken::STRING);
    if (error.existe) return;

    match(TipoToken::LLAVE_ABRE);
    parseTareas();
    if (error.existe) return;

    match(TipoToken::LLAVE_CIERRA);
    if (error.existe) return;

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
    if (error.existe) return;

    match(TipoToken::DOS_PUNTOS);
    if (error.existe) return;

    tareaActual.nombre = limpiar(current.lexema);
    match(TipoToken::STRING);
    if (error.existe) return;

    match(TipoToken::CORCHETE_ABRE);
    parseAtributos();
    if (error.existe) return;

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
        if (error.existe) return;

        match(TipoToken::DOS_PUNTOS);
        parsePrioridad();
    }
    else if (current.tipo == TipoToken::RESPONSABLE) {

        match(TipoToken::RESPONSABLE);
        if (error.existe) return;

        match(TipoToken::DOS_PUNTOS);

        tareaActual.responsable = limpiar(current.lexema);
        match(TipoToken::STRING);
    }
    else if (current.tipo == TipoToken::FECHA_LIMITE) {

        match(TipoToken::FECHA_LIMITE);
        if (error.existe) return;

        match(TipoToken::DOS_PUNTOS);

        tareaActual.fecha = current.lexema;
        match(TipoToken::FECHA);
    }
    else {
        error.mensaje = "Error: atributo no valido -> " + current.getTipoToken();
        error.existe = true;
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
        error.mensaje = "Error: prioridad no valida -> " + current.getTipoToken();
        error.existe= true;
        return;
    }
}