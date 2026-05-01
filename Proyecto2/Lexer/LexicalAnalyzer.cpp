/*Vamos a trabajar la logica de nuestro analizador lexico donde posteriormente vamos a trabajar con el parser*/


#include "LexicalAnalyzer.h"
#include "../Errores/LexicalError.h"
#include "vector"
#include "string"
#include "cctype"
#include <iostream>
using namespace std;


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
    if (pos > input.size()) {
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
    }else {
        columna++;
    }
    return c;
}


void LexicalAnalyzer::analyze() {
    /*Iniciamos lo que es nuestro analizador lexico*/
    int estado = 0;
    string lexeme;

    while (true) {
        char c = peek();
        switch (estado) {
            case 0:
                if (isspace(c)) { //continuar si existe un espacio en blanco
                    avance();
                    continue;
                }

                if (isalpha(c)) { //si encontramos el primer caracter entonces avanzamos a un estado de palabras
                    lexeme += avance();
                    estado = 1;
                    break;
                }

                if (isdigit(c)) { // si viene un digito redirigirnos a un estado donde trabajemos con ello
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
                    tokens.push_back(Token(TipoToken::LLAVE_ABRE, "{", fila, columna));
                    break;
                }

                if (c == '}') {
                    avance();
                    tokens.push_back(Token(TipoToken::LLAVE_CIERRA, "}", fila, columna));
                    break;
                }

                if (c == '[') {
                    avance();
                    tokens.push_back(Token(TipoToken::CORCHETE_ABRE, "[", fila, columna));
                    break;
                }

                if (c == ']') {
                    avance();
                    tokens.push_back(Token(TipoToken::CORCHETE_CIERRA, "]", fila, columna));
                    break;
                }

                if (c == ':') {
                    avance();
                    tokens.push_back(Token(TipoToken::DOS_PUNTOS, ":", fila, columna));
                    break;
                }

                if (c == ',') {
                    avance();
                    tokens.push_back(Token(TipoToken::COMA, ",", fila, columna));
                    break;
                }

                if (c == ';') {
                    avance();
                    tokens.push_back(Token(TipoToken::PUNTO_COMA, ";", fila, columna));
                    break;
                }

                if (c == '\0') {
                    return;
                }

                //si no es ninguno de estos entonces debe venir un error
                lexeme += avance();
                errores.push_back(LexicalError(lexeme, fila, columna, "Caracter No reconocido", "ERROR"));
                lexeme = "";
                break; //terminamos esta parte dle break

            case 1:
                // Construcción del lexema (igual que ya lo hacías)
                if (isalpha(c) || c == '_') {
                    lexeme += avance();
                } else {

                    if (lexeme == "TABLERO") {
                        tokens.push_back(Token(TipoToken::TABLERO, lexeme, fila, columna));
                    }
                    else if (lexeme == "COLUMNA") {
                        tokens.push_back(Token(TipoToken::COLUMNA, lexeme, fila, columna));
                    }
                    else if (lexeme == "tarea") {
                        tokens.push_back(Token(TipoToken::TAREA, lexeme, fila, columna));
                    }
                    else if (lexeme == "prioridad") {
                        tokens.push_back(Token(TipoToken::PRIORIDAD, lexeme, fila, columna));
                    }
                    else if (lexeme == "responsable") {
                        tokens.push_back(Token(TipoToken::RESPONSABLE, lexeme, fila, columna));
                    }
                    else if (lexeme == "fecha_limite") {
                        tokens.push_back(Token(TipoToken::FECHA_LIMITE, lexeme, fila, columna));
                    }

                    else if (lexeme == "ALTA") {
                        tokens.push_back(Token(TipoToken::ALTA, lexeme, fila, columna));
                    }
                    else if (lexeme == "MEDIA") {
                        tokens.push_back(Token(TipoToken::MEDIA, lexeme, fila, columna));
                    }
                    else if (lexeme == "BAJA") {
                        tokens.push_back(Token(TipoToken::BAJA, lexeme, fila, columna));
                    }
                    else {
                        errores.push_back(LexicalError(lexeme, fila, columna, "Token no reconocido", "ERROR"));
                    }

                    // Reset
                    lexeme = "";
                    estado = 0;
                }
                break;

            case 2:
                /*aqui mayormente vamos a trabajar con lo que son los numero o las fechas pero como tal no viene numeros
                 * solos sino que vienen parte de las fechas
                 */


                if (isdigit(c)) {
                    lexeme += avance();
                } else if (c == '-' && lexeme.size() == 4) {
                    lexeme += avance();
                    estado = 4;
                    break;
                }else {
                    tokens.push_back(Token(TipoToken::ENTERO, lexeme, fila, columna));
                    lexeme = "";
                    estado = 0;
                }

                break;

            case 3:
                /*Ahora vamos analizar la parte de lo que es un String para comprobar que lo sea
                 * Entonces basicamente ya deberia pasar digamos "Proyecto LFP"  entonces va guardando todo
                 * hasta que aparezca otra '"' entonces debemos tener el control
                 */

                if (c == '\0') {
                    errores.push_back(LexicalError(lexeme, fila, columna, "Cadena No Cerrada", "CRITICO"));
                    lexeme = "";
                    estado = 0;
                }
                else if (c == '\n') {
                    errores.push_back(LexicalError(lexeme, fila, columna, "Cadena No Cerrada", "CRITICO"));
                    lexeme = "";
                    estado = 0;
                }
                else if (c == '"') {
                    lexeme += avance(); // consumir la comilla final
                    tokens.push_back(Token(TipoToken::STRING, lexeme, fila, columna));
                    lexeme = "";
                    estado = 0;
                }
                else {
                    lexeme += avance(); // seguir acumulando contenido
                }
                break;

            case 4:
                /*Aquí vamos con la continuacion de lo que es la fecha y la cual vamos a trabjar
                 * por el lexema ya viene tipo 2026- con 4 digitios
                 */

                if (isdigit(c) || c == '-') {
                    lexeme += avance(); /*aqui va a llegar a 2026-06 pero cuando aparezca */
                }else {


                    if (lexeme.length() == 10 && lexeme[4] == '-' && lexeme[7] == '-' ) {
                        //si tiene la estructura correcta entonces podemos seguir adelante
                        // cuando termine de tomar todos los digitos entonces venimos nosotros y comparamos cuando termine
                        int meStr = stoi(lexeme.substr(5,2));
                        int diaStr = stoi(lexeme.substr(8,2));
                        if (meStr>= 1 && meStr <= 12) {
                            if (diaStr>=1 && diaStr <= 31) {
                                //si se cumple bien entonces el Token es valido
                                tokens.push_back(Token(TipoToken::FECHA, lexeme, fila, columna));
                                lexeme = "";
                                estado = 0;
                            }else {
                                errores.push_back(LexicalError(lexeme, fila, columna, "Dia Invalido", "ERROR"));
                                lexeme = "";
                                estado = 0;

                            }

                        }else {
                            errores.push_back(LexicalError(lexeme, fila, columna, "Mes Invalido", "ERROR"));
                            lexeme = "";
                            estado = 0;
                        }

                    }else {
                        errores.push_back(LexicalError(lexeme, fila, columna, "Formato Invalido AAAA-MM-DD", "ERROR"));
                        lexeme = "";
                        estado = 0;
                    }

                    break;
                }
        }

    }
}
