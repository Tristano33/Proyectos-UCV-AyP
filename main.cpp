#include <iostream>
#include <fstream>
#include<cmath>
d
// Funciones para convertir caracteres a sus valores enteros correspondientes 
int obtenerValorNumerico(char c) {
    if (c >= '1' && c <= '9') return c - '0';
    switch (c) {
        case 'T': return 10;
        case 'J': return 11;
        case 'Q': return 12;
        case 'K': return 13;
        case 'A': return 15;
        default:  return 0;
    }
}

int obtenerPaloNumerico(char c) {
    return c - '0';
}

bool esFlush(const int manoAJugar_PALOS[5]) {
    for (int i = 1; i < 5; i++) {
        if (manoAJugar_PALOS[i] != manoAJugar_PALOS[0]) return false;
    }
    return true;
}

bool esStraight(int manoAJugar_CARTAS[5]) {
    // Ordenadas de menor a mayor
    for (int i = 0; i < 4; i++) {
        if (manoAJugar_CARTAS[i + 1] != manoAJugar_CARTAS[i] + 1) return false;
    }
    return true;

int calcularValorDeLaMano() {
    int valorTotalDeLaMano = 0;
    return valorTotalDeLaMano;
}

int calcularValordelasCartas(int mano[5]) {
    int valorTotaldelasCartas = 0;
    for (int i = 0; i < 5; i++) {
        valorTotaldelasCartas += mano[i];
    }
    return valorTotaldelasCartas;
}

int calcularMultiplicador(int comodines[5], int manoAJugar_CARTAS[5], bool hayFlush, bool hayDosPares) {
    int Multiplicador = 1;
    for (int i = 0; i < 5; i++) {

        switch (comodines[i])
        {
        case 1: // Joker, +4 en mult
            Multiplicador += 4;
            break;

        case 2: // Joker alegre, +8 en mult
            Multiplicador += 8;
            break;

        case 3: //  Joker Demente, +10 de mult si la mano tiene 2 pares
            if (hayDosPares) {
                Multiplicador += 10;
            }
            break;

        case 4: // Joker Gracioso, +10 de Mult si la mano contiene flush
            if (hayFlush) {
                Multiplicador += 10;
            }
            break;

        case 8: { // Puño Elevado, Agrega el valor de la carta menor de la mano al Mult
            int cartaMenorDeLaMano = manoAJugar_CARTAS[0];
            for (int j = 1; j < 5; j++) {
                if (manoAJugar_CARTAS[j] < cartaMenorDeLaMano) {
                    cartaMenorDeLaMano = manoAJugar_CARTAS[j];
                }
            }
            Multiplicador += cartaMenorDeLaMano;
            break;
        }

        case 9: // Fibonacci, Cada As, 2, 3, 5 u 8, que contenga la mano, otorga +2 al Mult
            for (int j = 0; j < 5; j++) {
                if (manoAJugar_CARTAS[j] == 15 || manoAJugar_CARTAS[j] == 2 || 
                    manoAJugar_CARTAS[j] == 3 || manoAJugar_CARTAS[j] == 5 || 
                    manoAJugar_CARTAS[j] == 8) {
                    Multiplicador += 2;
                }
            }
            break;

        default:
            break;
        }
    }
    return Multiplicador;
}


int calcularpuntaje(int valordelaMano, int valorTotaldelasCartas, int Multiplicador) {
    int PuntajeFinal = (valordelaMano + valorTotaldelasCartas) * Multiplicador;
    return PuntajeFinal;
        
int main(int argc, char* argv[]) {
    // Verificar que se hayan pasado los argumentos requeridos desde la terminal
    if (argc < 3) {
        std::cout << "Uso correcto: " << argv[0] << " <archivo_entrada.in> <archivo_salida.out>" << std::endl;
        return 1;
    }

    // Abrir el archivo de entrada dinámicamente desde argv[1]
    std::ifstream Entrada(argv[1]);
    if (!Entrada.is_open()) {
        std::cout << "Error al abrir el archivo de entrada: " << argv[1] << std::endl;
        return 1;
    }

    // Abrir el archivo de salida dinámicamente desde argv[2]
    std::ofstream archivoSalida(argv[2]);
    if (!archivoSalida.is_open()) {
        std::cout << "Error al crear/abrir el archivo de salida: " << argv[2] << std::endl;
        Entrada.close();
        return 1;
    }

    // Abrir el archivo 'partida.sav'
    std::ofstream archivoSav("partida.sav");

    char mazoInicial[52][2];
    int numeroDeCarta[52];
    int paloDeCarta[52];

    int mazoSecundario_ROBO__CARTAS[8];
    int mazoSecundario_ROBO__PALOS[8];

    int manoAJugar_CARTAS[5];
    int manoAJugar_PALOS[5];

    int pilaDeDescarte_CARTAS[52];
    int pilaDeDescarte_Palos[52];
    
    for (int i = 0; i < 52; i++) {
        char extractorDeValores[3];
        Entrada >> extractorDeValores; // Lee el texto como "K1", "71", "24"

        mazoInicial[i][0] = extractorDeValores[0];
        mazoInicial[i][1] = extractorDeValores[1];

        numeroDeCarta[i] = obtenerValorNumerico(mazoInicial[i][0]);
        paloDeCarta[i]   = obtenerPaloNumerico(mazoInicial[i][1]);
    }

    int comodines[6];
        if(bool Negativo == false){
            int comodines[5]=0;}
    for (int i = 0; i < 5; i++) {
        Entrada >> comodines[i];
    }

    int mazoSecundario_ROBO__CARTAS[8] = numeroDeCarta[i];
    int mazoSecundario_ROBO__PALOS[8] = paloDeCarta[i];
    
    int ciegas[10]; // 9 posibles iteraciones maximas, se añade un extra por si acaso
    int totalCiegas = 0;

    // Lee las ciegas del archivo de entrada
    while (totalCiegas < 10 && Entrada >> ciegas[totalCiegas]) {
        totalCiegas++;
    }

    Entrada.close();

for(int c=0; int c < ]; c++)
    Salida.close();
    PartidaSav.close();

    return 0;
}

    //Se cierra el ifstream de input
    input.close();

}
