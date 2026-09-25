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

int calcularMultiplicador(int comodines[5]) {
    int Multiplicador = 1;
    for (int i = 0; i < 5; i++) {
        Multiplicador += 0;
        }
    }
    return Multiplicador;


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

    for (int i = 0; i < 52; i++) {
        char extractorDeValores[3];
        Entrada >> extractorDeValores; // Lee el texto como "K1", "71", "24"

        mazoInicial[i][0] = extractorDeValores[0];
        mazoInicial[i][1] = extractorDeValores[1];

        numeroDeCarta[i] = obtenerValorNumerico(mazoInicial[i][0]);
        paloDeCarta[i]   = obtenerPaloNumerico(mazoInicial[i][1]);
    }

    int comodines[5];
    for (int i = 0; i < 5; i++) {
        Entrada >> comodines[i];
    }


    int ciegas[5]; // recordatorio que son maximo 5 etapas/jefes
    int totalCiegas = 0;

    // Lee las ciegas del archivo de entrada
    while (totalCiegas < 5 && Entrada >> ciegas[totalCiegas]) {
        totalCiegas++;
    }

    Entrada.close();

    // -------------------------------------------------------------
    // AQUÍ IRA LA LÓGICA DE JUEGO (Evaluación de manos, rondas y guardado)
    // -------------------------------------------------------------

    // Cierre de los archivos de salida al finalizar la ejecución
    archivoSalida.close();
    archivoSav.close();

    return 0;
}

    //Se cierra el ifstream de input
    input.close();

}
