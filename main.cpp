#include<iostream>
#include<fstream>
#include<cmath>

using namespace std;


//Estructuras de las cartas, el valor card tendra el token de caracteres y despues se evaluara y se colocara el palo y valor
struct Card {
    char* card;
    int palo;
    int valor;
};
//Estructura de la mano, un arreglo de mano para tener acceso a la memoria de la mano
struct Mano {
    int cantidad;
    Card* mano; 
};
//Estructura del Mazo, con su cantidad respectiva
struct Mazo {
    int cantidad;
    Card* mazo;
};
//Enum de los Jokers para identificarlos de forma sencilla cuando toque marcar los tokens de los Jokers
enum Jokers {
    joker = 1, //+4 en Mult
    joker_alegre = 2, //+8 en Mult si la mano contiene al menos un par
    joker_demente = 3, //+10 en Mult si la mano contiene dos pares
    joker_gracioso = 4, //+10 de Mult si la mano contiene flush
    joker_habilidoso = 5, //+50 puntos al valor de la mano si tiene al menos un par
    joker_taimado = 6, //+100 puntos al valor de la mano si tiene dos pares
    cuatro_dedos = 7, //Los flush y straight pueden hacerse con 4 cartas
    punno_elevado = 8, //Agrega el valor de la carta menor de la mano al Mult
    fibonacci = 9, //Cada As, 2, 3, 5 u 8, que contenga la mano, otorga +2 al Mult
    negativo = 10, //Aumenta la capacidad máxima de comodines en 1
    reflectante = 11, //Otorga un bono fijo de +50 puntos al valor base de la mano
};
//Slots de Joker con el mismo principio de los Mazos
struct Inventario {
    int capacidad;
    int cantidad;
    Jokers* jokers;
};



int main(int argc, char* argv[]) {
    
    //ruta del archivo .in
    char* input_file = argv[1];
    //ruta del archivo .out
    //char* output_file = argv[2];
    
    ifstream input(input_file);
    //ofstream output(output_file);

    if (!input.is_open()) {
        cout << "ERROR: Archivo de entrada inexistente \n";
        return 1;
    }
    /*
    if (!output.is_open()) {
        cout << "ERROR: No se pudo crear el archivo de salida \n";
        return 1;
    }
    */

    //Variable para leer cada valor separado por espacios o enter
    char token[50];
    
    // el operador >> es para saltar de espacio en espacio leyendo cada token
    while (input >> token) {

        //suma de los valores de los caracteres para determinar su "peso"
        int sum_ascii = 0;
        for (int i = 0; token[i] != '\0'; i++ ) {
            sum_ascii += token[i];
        }

        //Evaluacion, debido a que el valor mas bajo de las cartas es 21, que sumando sus valores ascii por caracter seria 
        //'2' = 50 y '1' = 49, entonces: 50 + 49 = 99, cuando el valor sea menor de 49 entonces estaremos empezando a leer los Jokers.
        //Tambien hay que buscar el limite por encima de suma de caracteres ascii para averiguar cuando no se trata de cartas ni de jokers para empezar con las ciegas
        if (sum_ascii > 99){
            cout << "Token leído: " << token << "\n";
        }
    }   

    //Se cierra el ifstream de input
    input.close();

}