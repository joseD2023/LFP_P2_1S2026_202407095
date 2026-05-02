/*Vamos a trabajar la logica de nuestro analizador lexico donde posteriormente vamos a trabajar con el parser*/

#include "LexicalAnalyzer.h"
#include "../Errores/LexicalError.h"
#include "vector"
#include "string"
#include "cctype"
#include <iostream>
using namespace std;

// Verifica si un caracter es válido dentro de una cadena de texto
bool esCaracterValidoParaString(char c) {

    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return true;

    // Números
    if (c >= '0' && c <= '9') {
        return true;
    }

    // Espacio en blanco
    if (c == ' ') {
        return true;
    }

    // Signos de puntuación normales
    if (c == '.' || c == ',' || c == '-' || c == '/') {
        return true;
    }
    if (c == ':' || c == ';' || c == '(' || c == ')' || c == '?' || c == '!') {
        return true;
    }

    // Tildes y letras especiales (á, é, í, ó, ú, ñ, etc.)
    // Estos caracteres tienen valor ASCII mayor a 127
    if ((unsigned char)c > 127) {
        return true;
    }

    // Si llegó aquí, el caracter NO es válido
    return false;
}



// ver si el caracter es válido para palabras
bool esCaracterValidoParaPalabra(char c) {
    return isalpha(c) || c == '_';
}

LexicalAnalyzer::LexicalAnalyzer(string entrada) {
    input = entrada;
    pos = 0;
    fila = 1;
    columna = 1;
}

/*Verificacion si una palabra reservada*/
bool esReservada(const string& palabra) {
    return palabra == "TABLERO" || palabra == "COLUMNA" || palabra == "tarea" || palabra == "prioridad" || palabra == "responsable" || palabra == "fecha_limite";
}

bool esPrioridad(const string& prioridad) {
    return prioridad == "ALTA" || prioridad == "MEDIA" || prioridad == "BAJA";
}

/*Utilizamos Herramientas que nos puede servir en el analizador lexico*/

void LexicalAnalyzer::mostrarAnalyze() {
    cout << "TOKENS ENCONTRADOS" << endl;

    for (auto& token : tokens) {
        cout << "Tipo: ";

        switch (token.tipo) {
            case TipoToken::TABLERO: cout << "TABLERO"; break;
            case TipoToken::COLUMNA: cout << "COLUMNA"; break;
            case TipoToken::TAREA: cout << "TAREA"; break;
            case TipoToken::PRIORIDAD: cout << "PRIORIDAD"; break;
            case TipoToken::RESPONSABLE: cout << "RESPONSABLE"; break;
            case TipoToken::FECHA_LIMITE: cout << "FECHA_LIMITE"; break;
            case TipoToken::ALTA: cout << "ALTA"; break;
            case TipoToken::MEDIA: cout << "MEDIA"; break;
            case TipoToken::BAJA: cout << "BAJA"; break;
            case TipoToken::STRING: cout << "STRING"; break;
            case TipoToken::FECHA: cout << "FECHA"; break;
            case TipoToken::ENTERO: cout << "ENTERO"; break;
            case TipoToken::LLAVE_ABRE: cout << "LLAVE_ABRE"; break;
            case TipoToken::LLAVE_CIERRA: cout << "LLAVE_CIERRA"; break;
            case TipoToken::CORCHETE_ABRE: cout << "CORCHETE_ABRE"; break;
            case TipoToken::CORCHETE_CIERRA: cout << "CORCHETE_CIERRA"; break;
            case TipoToken::DOS_PUNTOS: cout << "DOS_PUNTOS"; break;
            case TipoToken::COMA: cout << "COMA"; break;
            case TipoToken::PUNTO_COMA: cout << "PUNTO_COMA"; break;
            case TipoToken::PUNTO: cout << "PUNTO"; break;
            case TipoToken::FIN_ARCHIVO: cout << "EOF"; break;
            default: cout << "DESCONOCIDO"; break;
        }

        cout << "  | Lexema: " << token.lexema
             << "  | Fila: " << token.fila
             << "  | Columna: " << token.columna << endl;
    }

    if (!errores.empty()) {
        cout << "\nERRORES ENCONTRADOS:\n";
        cout << "Fila\tColumna\tLexema\tDescripcion\tGravedad" << endl;

        for (auto& error : errores) {
            cout << error.fila << "\t"
                 << error.columna << "\t"
                 << error.lexema << "\t"
                 << error.descripcion << "\t"
                 << error.gravedad << endl;
        }
    } else {
        cout << "\nNo se encontraron errores.\n";
    }
}

char LexicalAnalyzer::peek() {
    if (pos >= input.size()) {
        return '\0';
    }
    return input[pos];
}

char LexicalAnalyzer::avance() {
    char c = input[pos];
    pos++;
    if (c == '\n') {
        fila++;
        columna = 1;
    } else {
        columna++;
    }
    return c;
}

void LexicalAnalyzer::analyze() {
    /*Iniciamos lo que es nuestro analizador lexico*/
    int estado = 0;
    string lexeme;
    int inicioFila = 1;
    int inicioColumna = 1;

    while (true) {
        char c = peek();
        switch (estado) {
            case 0:
                if (isspace(c)) {
                    avance();
                    continue;
                }

                inicioFila = fila;
                inicioColumna = columna;

                if (isalpha(c)) {
                    lexeme += avance();
                    estado = 1;
                    break;
                }

                if (isdigit(c)) {
                    lexeme += avance();
                    estado = 2;
                    break;
                }

                if (c == '"') {
                    lexeme += avance();
                    estado = 3;
                    break;
                }

                if (c == '{') {
                    avance();
                    tokens.push_back(Token(TipoToken::LLAVE_ABRE, "{", inicioFila, inicioColumna));
                    break;
                }

                if (c == '}') {
                    avance();
                    tokens.push_back(Token(TipoToken::LLAVE_CIERRA, "}", inicioFila, inicioColumna));
                    break;
                }

                if (c == '[') {
                    avance();
                    tokens.push_back(Token(TipoToken::CORCHETE_ABRE, "[", inicioFila, inicioColumna));
                    break;
                }

                if (c == ']') {
                    avance();
                    tokens.push_back(Token(TipoToken::CORCHETE_CIERRA, "]", inicioFila, inicioColumna));
                    break;
                }

                if (c == ':') {
                    avance();
                    tokens.push_back(Token(TipoToken::DOS_PUNTOS, ":", inicioFila, inicioColumna));
                    break;
                }

                if (c == ',') {
                    avance();
                    tokens.push_back(Token(TipoToken::COMA, ",", inicioFila, inicioColumna));
                    break;
                }

                if (c == ';') {
                    avance();
                    tokens.push_back(Token(TipoToken::PUNTO_COMA, ";", inicioFila, inicioColumna));
                    break;
                }

                if (c == '\0') {
                    return;
                }

                lexeme += avance();
                errores.push_back(LexicalError(lexeme, fila, columna - 1,
                    "El caracter '" + string(1, c) + "' no esta permitido", "ERROR"));
                lexeme = "";
                break;

            case 1:
                if (c != '\0' && esCaracterValidoParaPalabra(c)) {
                    lexeme += avance();
                } else {
                    if (lexeme == "TABLERO") {
                        tokens.push_back(Token(TipoToken::TABLERO, lexeme, inicioFila, inicioColumna));
                    }
                    else if (lexeme == "COLUMNA") {
                        tokens.push_back(Token(TipoToken::COLUMNA, lexeme, inicioFila, inicioColumna));
                    }
                    else if (lexeme == "tarea") {
                        tokens.push_back(Token(TipoToken::TAREA, lexeme, inicioFila, inicioColumna));
                    }
                    else if (lexeme == "prioridad") {
                        tokens.push_back(Token(TipoToken::PRIORIDAD, lexeme, inicioFila, inicioColumna));
                    }
                    else if (lexeme == "responsable") {
                        tokens.push_back(Token(TipoToken::RESPONSABLE, lexeme, inicioFila, inicioColumna));
                    }
                    else if (lexeme == "fecha_limite") {
                        tokens.push_back(Token(TipoToken::FECHA_LIMITE, lexeme, inicioFila, inicioColumna));
                    }
                    else if (lexeme == "ALTA") {
                        tokens.push_back(Token(TipoToken::ALTA, lexeme, inicioFila, inicioColumna));
                    }
                    else if (lexeme == "MEDIA") {
                        tokens.push_back(Token(TipoToken::MEDIA, lexeme, inicioFila, inicioColumna));
                    }
                    else if (lexeme == "BAJA") {
                        tokens.push_back(Token(TipoToken::BAJA, lexeme, inicioFila, inicioColumna));
                    }
                    else {
                        errores.push_back(LexicalError(lexeme, inicioFila, inicioColumna,
                            "La palabra '" + lexeme + "' no es valida aqui", "ERROR"));
                    }

                    lexeme = "";
                    estado = 0;
                }
                break;

            case 2:
                if (isdigit(c)) {
                    lexeme += avance();
                }
                else if (c == '-' && lexeme.size() == 4) {
                    lexeme += avance();
                    estado = 4;
                    break;
                }
                else {
                    tokens.push_back(Token(TipoToken::ENTERO, lexeme, inicioFila, inicioColumna));
                    lexeme = "";
                    estado = 0;
                }
                break;

            case 3:
                if (c == '\0') {
                    errores.push_back(LexicalError(lexeme, inicioFila, inicioColumna,
                        "Faltan las comillas para cerrar el texto", "CRITICO"));
                    lexeme = "";
                    estado = 0;
                }
                else if (c == '\n') {
                    errores.push_back(LexicalError(lexeme, inicioFila, inicioColumna,
                        "El texto se quedo abierto y se encontro un salto de linea", "CRITICO"));
                    lexeme = "";
                    estado = 0;
                }
                else if (c == '"') {
                    lexeme += avance();
                    tokens.push_back(Token(TipoToken::STRING, lexeme, inicioFila, inicioColumna));
                    lexeme = "";
                    estado = 0;
                }
                else {
                    if (!esCaracterValidoParaString(c)) {
                        string charStr(1, c);
                        errores.push_back(LexicalError(charStr, fila, columna,
                            "El caracter '" + charStr + "' no se puede usar dentro del texto", "ERROR"));
                    }
                    lexeme += avance();
                }
                break;

            case 4:
                if (c != '\0' && (isdigit(c) || c == '-')) {
                    lexeme += avance();
                }
                else {
                    if (lexeme.length() == 10 && lexeme[4] == '-' && lexeme[7] == '-') {
                        int mes = stoi(lexeme.substr(5, 2));
                        int dia = stoi(lexeme.substr(8, 2));

                        if (mes >= 1 && mes <= 12) {
                            if (dia >= 1 && dia <= 31) {
                                tokens.push_back(Token(TipoToken::FECHA, lexeme, inicioFila, inicioColumna));
                            } else {
                                errores.push_back(LexicalError(lexeme, inicioFila, inicioColumna,
                                    "El dia " + to_string(dia) + " no existe", "ERROR"));
                            }
                        } else {
                            errores.push_back(LexicalError(lexeme, inicioFila, inicioColumna,
                                "El mes " + to_string(mes) + " no existe", "ERROR"));
                        }
                    } else {
                        errores.push_back(LexicalError(lexeme, inicioFila, inicioColumna,
                            "La fecha no esta en formato AAAA-MM-DD", "ERROR"));
                    }

                    lexeme = "";
                    estado = 0;
                    break;
                }
                break;
        }
    }
}