
/*Aqui vamos a colocar las Estructuras de como deberia guardar yo estas herrmientas
 * para luego utilizarlo y mandarlo de manera JSON en lo que es a un Frontend
 */

#ifndef PROYECTO2_TABLERO_H
#define PROYECTO2_TABLERO_H
#include "string"
#include "vector"
using namespace std;

/*Construimos para poder guardar la informacion*/

struct Tarea {
    string nombre;
    string prioridad;
    string responsable;
    string fecha;
};

struct Columna {
    string nombre;
    vector<Tarea> tareas;
};

struct Tablero {
    string nombre;
    vector<Columna> columnas;
};
#endif //PROYECTO2_TABLERO_H