#include <iostream>
#include <fstream>
#include <cmath>

// Funciones para convertir caracteres a enteros
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

char obtenerCaracterValor(int valor) {
    if (valor >= 1 && valor <= 9) return valor + '0';
    switch (valor) {
        case 10: return 'T';
        case 11: return 'J';
        case 12: return 'Q';
        case 13: return 'K';
        case 15: return 'A';
        default: return '0';
    }
}

// funciones de la Tabla Mano-Valor

bool esFlush(const int manoAJugar_PALOS[5]) {
    for (int i = 1; i < 5; i++) {
        if (manoAJugar_PALOS[i] != manoAJugar_PALOS[0]) return false;
    }
    return true;
}

bool esStraight(int manoAJugar_CARTAS[5]) {
    // Asume que manoAJugar_CARTAS ya está ordenada de menor a mayor
    for (int i = 0; i < 4; i++) {
        if (manoAJugar_CARTAS[i + 1] != manoAJugar_CARTAS[i] + 1) return false;
    }
    return true;
}

bool esStraightFlush(int manoAJugar_CARTAS[5], int manoAJugar_PALOS[5]) {
    return esFlush(manoAJugar_PALOS) && esStraight(manoAJugar_CARTAS);
}

bool esRoyalFlush(int manoAJugar_CARTAS[5], int manoAJugar_PALOS[5]) {
    return esStraightFlush(manoAJugar_CARTAS, manoAJugar_PALOS) && manoAJugar_CARTAS[4] == 15;
}

void contarCoincidencias(const int manoAJugar_CARTAS[5], int coincidencias[16]) {
    for (int i = 0; i < 16; i++) coincidencias[i] = 0;
    for (int i = 0; i < 5; i++) {
        if (manoAJugar_CARTAS[i] >= 0 && manoAJugar_CARTAS[i] < 16) {
            coincidencias[manoAJugar_CARTAS[i]]++;
        }
    }
}

// Asigna el valor base según la Tabla de Mano-Valor
int evaluarTipoMano(int manoAJugar_CARTAS[5], int manoAJugar_PALOS[5]) {
    int coincidencias[16]; // Basicamente, un contador sobre que tantas cartas aparecen mas de 1 vez
    contarCoincidencias(manoAJugar_CARTAS, coincidencias);

    bool hayFlush = esFlush(manoAJugar_PALOS);
    bool hayStraight = esStraight(manoAJugar_CARTAS);

    int maxCoinci = 0;
    int pares = 0;
    bool hayTrio = false;

    for (int i = 0; i < 16; i++) {
        if (coincidencias[i] > maxCoinci) maxCoinci = coincidencias[i];
        if (coincidencias[i] == 2) pares++;
        if (coincidencias[i] == 3) hayTrio = true;
    }

    if (hayFlush && hayStraight && manoAJugar_CARTAS[4] == 15) return 100; // Royal Flush
    if (hayFlush && hayStraight) return 80;                                 // Straight Flush
    if (maxCoinci == 4) return 75;                                           // Four of a Kind
    if (hayTrio && pares == 1) return 60;                                  // Full House
    if (hayFlush) return 50;                                               // Flush
    if (hayStraight) return 40;                                            // Straight
    if (hayTrio) return 30;                                                // Three of a Kind
    if (pares == 2) return 20;                                             // Two Pair
    if (pares == 1) return 10;                                             // One Pair
    return 0;                                                              // High Card
}

int calcularValordelasCartas(int manoAjugar_CARTAS[5]) {
    int valorTotaldelasCartas = 0;
    for (int i = 0; i < 5; i++) {
        valorTotaldelasCartas += manoAjugar_CARTAS[i];
    }
    return valorTotaldelasCartas;
}

// -------------------------------------------------------------
// SELECTOR DE LA MEJOR JUGADA (56 COMBINACIONES)
// -------------------------------------------------------------
int selectorDeJugada(int mazoSecundario_ROBO__CARTAS[8], int mazoSecundario_ROBO__PALOS[8],
                     int mejorJugada__CARTAS[5], int mejorJugada__PALOS[5]) {
    int mejorValorBase = -1;
    int mejorSumaCartas = -1;

    // 56 combinaciones posibles (8 tomadas de 5 en 5)
    for (int a = 0; a < 4; a++) {
        for (int b = a + 1; b < 5; b++) {
            for (int c = b + 1; c < 6; c++) {
                for (int d = c + 1; d < 7; d++) {
                    for (int e = d + 1; e < 8; e++) {

                        int tempCartas[5] = {
                            mazoSecundario_ROBO__CARTAS[a],
                            mazoSecundario_ROBO__CARTAS[b],
                            mazoSecundario_ROBO__CARTAS[c],
                            mazoSecundario_ROBO__CARTAS[d],
                            mazoSecundario_ROBO__CARTAS[e]
                        };

                        int tempPalos[5] = {
                            mazoSecundario_ROBO__PALOS[a],
                            mazoSecundario_ROBO__PALOS[b],
                            mazoSecundario_ROBO__PALOS[c],
                            mazoSecundario_ROBO__PALOS[d],
                            mazoSecundario_ROBO__PALOS[e]
                        };

                        // Ordenar la mano de 5 cartas sincronizando valores y palos
                        for (int i = 0; i < 4; i++) {
                            for (int j = i + 1; j < 5; j++) {
                                if (tempCartas[i] > tempCartas[j]) {
                                    int auxC = tempCartas[i];
                                    tempCartas[i] = tempCartas[j];
                                    tempCartas[j] = auxC;

                                    int auxP = tempPalos[i];
                                    tempPalos[i] = tempPalos[j];
                                    tempPalos[j] = auxP;
                                }
                            }
                        }

                        int valorActual = evaluarTipoMano(tempCartas, tempPalos);
                        int sumaCartasActual = calcularValordelasCartas(tempCartas);

                        // Criterio de selección: mayor valor base o desempate por suma de cartas
                        if (valorActual > mejorValorBase || 
                           (valorActual == mejorValorBase && sumaCartasActual > mejorSumaCartas)) {
                            mejorValorBase = valorActual;
                            mejorSumaCartas = sumaCartasActual;

                            for (int k = 0; k < 5; k++) {
                                mejorJugada__CARTAS[k] = tempCartas[k];
                                mejorJugada__PALOS[k]  = tempPalos[k];
                            }
                        }

                    }
                }
            }
        }
    }
    return mejorValorBase;
}


// calculadora del mult
int calcularMultiplicador(int comodines[5], int manoAJugar_CARTAS[5], bool hayFlush, bool hayDosPares) {
    int Multiplicador = 1;
    for (int i = 0; i < 5; i++) {

        switch (comodines[i]) {
            case 1: // Joker, +4 en mult
                Multiplicador += 4;
                break;

            case 2: // Joker alegre, +8 en mult
                Multiplicador += 8;
                break;

            case 3: // Joker Demente, +10 de mult si la mano tiene 2 pares
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

            case 9: { // Fibonacci, Cada As, 2, 3, 5 u 8 otorga +2 al Mult
                for (int j = 0; j < 5; j++) {
                    if (manoAJugar_CARTAS[j] == 15 || manoAJugar_CARTAS[j] == 2 || 
                        manoAJugar_CARTAS[j] == 3 || manoAJugar_CARTAS[j] == 5 || 
                        manoAJugar_CARTAS[j] == 8) {
                        Multiplicador += 2;
                    }
                }
                break;
            }

            default:
                break;
        }
    }
    return Multiplicador;
}
// formula de puntaje
int calcularpuntaje(int valordelaMano, int valorTotaldelasCartas, int Multiplicador) {
    int PuntajeFinal = (valordelaMano + valorTotaldelasCartas) * Multiplicador;
    return PuntajeFinal;
}

// Main

int main(int argc, char* argv[]) {
    // Verificar que se hayan pasado los argumentos requeridos desde la terminal
    if (argc < 3) {
        std::cout << "Uso correcto: " << argv[0] << " <archivo_entrada.in> <archivo_salida.out>" << std::endl;
        return 1;
    }

    
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

    // Abrir el archivo de persistencia 'partida.sav'
    std::ofstream archivoSav("partida.sav");

    // Lectura del mazo
    
    char mazoInicial[52][2];
    int numeroDeCarta[52];
    int paloDeCarta[52];

    int mazoSecundario_ROBO__CARTAS[8];
    int mazoSecundario_ROBO__PALOS[8];

    int manoAJugar_CARTAS[5];
    int manoAJugar_PALOS[5];

    int pilaDeDescarte_CARTAS[52];
    int pilaDeDescarte_PALOS[52];

    for (int i = 0; i < 52; i++) {
        char extractorDeValores[3];
        Entrada >> extractorDeValores; // Lee el texto como "K1", "71", "24"

        mazoInicial[i][0] = extractorDeValores[0];
        mazoInicial[i][1] = extractorDeValores[1];

        numeroDeCarta[i] = obtenerValorNumerico(mazoInicial[i][0]);
        paloDeCarta[i]   = obtenerPaloNumerico(mazoInicial[i][1]);
    }

    for (int i = 0; i < 8; i++) { // Asignación de Cartas y palos desde mazoInicial
        mazoSecundario_ROBO__CARTAS[i] = numeroDeCarta[i];
        mazoSecundario_ROBO__PALOS[i]  = paloDeCarta[i];
    }

    int comodines[5];
    for (int i = 0; i < 5; i++) {
        Entrada >> comodines[i];
    }

    int ciegas[5]; // Soporta hasta un máximo de 5 ciegas
    int totalCiegas = 0;

    while (totalCiegas < 5 && Entrada >> ciegas[totalCiegas]) {
        totalCiegas++;
    }

    Entrada.close();

    int valorBaseMano = selectorDeJugada(mazoSecundario_ROBO__CARTAS, mazoSecundario_ROBO__PALOS,
                                         manoAJugar_CARTAS, manoAJugar_PALOS);

    int sumaCartas = calcularValordelasCartas(manoAJugar_CARTAS);

    bool hayFlush = esFlush(manoAJugar_PALOS);

    int frecuencias[16];
    contarCoincidencias(manoAJugar_CARTAS, frecuencias);
    int numPares = 0;
    for (int i = 0; i < 16; i++) {
        if (frecuencias[i] == 2) numPares++;
    }
    bool hayDosPares = (numPares == 2);

    int multiplicadorFinal = calcularMultiplicador(comodines, manoAJugar_CARTAS, hayFlush, hayDosPares);
    int puntajeFinal = calcularpuntaje(valorBaseMano, sumaCartas, multiplicadorFinal);

   
    archivoSalida << puntajeFinal << std::endl;


    for (int i = 0; i < 5; i++) {
        archivoSav << obtenerCaracterValor(manoAJugar_CARTAS[i]) << manoAJugar_PALOS[i] << " ";
    }
    archivoSav << std::endl;

    archivoSalida.close();
    archivoSav.close();

    return 0;
}
