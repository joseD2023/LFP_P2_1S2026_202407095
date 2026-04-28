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
            case TipoToken::PALABRARESEVADA: cout << "PALABRA RESERVADA"; break;
            case TipoToken::PRIORIDADES: cout << "PRIORIDAD"; break;
            case TipoToken::STRING: cout << "STRING"; break;
            case TipoToken::DELIMITADORES: cout << "DELIMITADOR"; break;
            case TipoToken::NUMBER: cout << "NUMERO"; break;
            case TipoToken::FECHA: cout << "FECHA"; break;
            case TipoToken::DESCONOCIDO: cout << "DESCONOCIDO"; break;
            default: cout << "DESCONOCIDO"; break;
        }

        cout << "  | Lexema: " << token.lexema
             << "  | Fila: " << token.fila
             << "  | Columna: " << token.columna << endl;
    }

    if (!errores.empty()) {
        cout << "\nERRORES ENCONTRADOS:\n";
        cout << "Fila\tColumna\tLexema\tDescripcion\t       Gravedad" << endl;

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

                if (c == ':' || c == '[' || c == ']' || c == '{' || c == '}' || c == ',' || c == ':' || c == ';' || c == '-') {
                    lexeme += avance();
                    tokens.push_back(Token(TipoToken::DELIMITADORES, lexeme, fila, columna));
                    lexeme = "";
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
                /*Entonces vamos a trabajar con el primer caso que es el de si es un caracter de string entonces trabajamos aquí
                 * Basicamente es la construccion de los lexemas como TABLERO, COLUMNA solo debemos tener cuidado con lo que es la
                 * fecha
                 */

                if (isalpha(c) || c == '_') { //basicamente construimos las palabras en este apartado
                    lexeme += avance();
                }else {

                    if (esReservada(lexeme)) {
                        tokens.push_back(Token(TipoToken::PALABRARESEVADA, lexeme, fila, columna));
                        lexeme = "";
                        estado = 0;
                        break;
                    }else {
                        errores.push_back(LexicalError(lexeme, fila, columna, "Palabra Reservada No reconocida", "ERROR"));
                        lexeme = "";
                        estado = 0;
                    }

                    if (esPrioridad(lexeme)) {
                        tokens.push_back(Token(TipoToken::PRIORIDADES, lexeme, fila, columna));
                        lexeme = "";
                        estado = 0;


                    }else {
                        errores.push_back(LexicalError(lexeme, fila, columna, "Prioridad No reconocida", "ERROR"));
                        lexeme = "";
                        estado = 0;
                    }

                }

            case 2:
                /*aqui mayormente vamos a trabajar con lo que son los numero o las fechas pero como tal no viene numeros
                 * solos sino que vienen parte de las fechas
                 */

                if (isdigit(c)) {
                    lexeme += avance();
                }

                if (c == '-' && lexeme.size() == 4) {
                    lexeme += avance();
                    estado = 4;
                    break;
                }

            case 4:
                /*Aquí vamos con la continuacion de lo que es la fecha y la cual vamos a trabjar
                 * por el lexema ya viene tipo 2026- con 4 digitios
                 */

                if (isdigit(c)) {
                    lexeme += avance(); /*aqui va a llegar a 2026-06 pero cuando aparezca */
                    if (lexeme.length()-5 == 2) {
                        string mes = lexeme.substr(5,2);
                        int mesN = stoi(mes);
                        if (mesN >= 1 && mesN <= 12) {
                            if (c == '-') {
                                lexeme += avance();
                                estado = 5; /*Luego pasamos a otro estado para seguir con el numero*/
                            }else {
                                errores.push_back(LexicalError(lexeme, fila, columna, "Formato de Fecha Invalido", "ERROR"));
                                lexeme = "";
                                estado = 0;
                            }
                        }else {
                            errores.push_back(LexicalError(lexeme, fila, columna, "Mes Invalido dentro Formato", "ERROR"));
                            estado = 0;
                            lexeme = "";
                        }
                    }

                }

            case 5:
                /*ahora necesitamos concluir la parte de la fecha despues 2026-06-34 fecha validas*/

                if (isdigit(c)) {

                }





        }



    }










}
