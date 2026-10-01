#include <iostream>
#include <fstream>
#include <cmath>

// ================================================================
//  Proyecto #2 - Calatro: El Juego Oculto
//  Algoritmos y Programacion - UCV
//
//  Restricciones respetadas:
//   - Solo se incluyen <iostream>, <cmath> y <fstream>.
//   - No se usa "enum" (palos y tipos de mano son constantes).
//   - No se declaran punteros ni se usa memoria dinamica.
//     La unica excepcion es "char* argv[]" del main, que el
//     enunciado exige para recibir los parametros.
//   - El mazo, la mano, la jugada y los descartes son arreglos
//     estaticos.
// ================================================================

// ================================================================
//  CONSTANTES
// ================================================================

// Tamanos
const int TOTAL_CARTAS_MAZO = 52;
const int CARTAS_POR_JUGADA = 5;
const int CARTAS_POR_MANO = 8;
const int TOTAL_COMODINES = 5;
// Con 52 cartas el maximo de rondas posibles es 9:
// robo inicial 8 + un refill de 5 por cada ronda previa -> 8 + 5 * 8 = 48 <= 52
const int MAX_RONDAS = 9;

// Rango del As para la escalera: la tabla le da 15 como valor de
// carta, pero para formar escalera ocupa el lugar del 14 (A K Q J T)
const int RANGO_AS = 14;

// IDs de los comodines
const int JOKER = 1;
const int JOKER_ALEGRE = 2;
const int JOKER_DEMENTE = 3;
const int JOKER_GRACIOSO = 4;
const int JOKER_HABILIDOSO = 5;
const int JOKER_TAIMADO = 6;
const int CUATRO_DEDOS = 7;
const int PUNO_ELEVADO = 8;
const int FIBONACCI = 9;
const int NEGATIVO = 10;
const int REFLECTANTE = 11;

// ================================================================
//  ESTRUCTURAS
// ================================================================

struct Carta {
    int valor;
    int palo;
};

struct TipoMano {
    char nombre[20];
    int valor;
};

// Tabla Mano-Valor del enunciado
const TipoMano TABLA_MANOS[10] = {
    {"Royal Flush", 100},
    {"Straight Flush", 80},
    {"Four of a Kind", 75},
    {"Full House", 60},
    {"Flush", 50},
    {"Straight", 40},
    {"Three of a Kind", 30},
    {"Two Pair", 20},
    {"One Pair", 10},
    {"High Card", 0}
};

struct Jugada {
    Carta cartas[CARTAS_POR_JUGADA];
};

struct ConteoValores {
    int frecuencias[16];
};

struct RangoEscalera {
    int valores[CARTAS_POR_JUGADA];
    int cantidad;
};

struct Comodines {
    int ids[TOTAL_COMODINES];
    int total;
    int capacidad;
};

struct ResultadoJugada {
    int indices[CARTAS_POR_JUGADA];
    Jugada jugada;
    int valorBase;
    int sumaCartas;
    int bonusValorMano;
    int multiplicador;
    int puntaje;
};

// ================================================================
//  FUNCIONES BASICAS DE CARTA
// ================================================================

Carta armarCarta(int valor, int palo) {
    Carta nueva;
    nueva.valor = valor;
    nueva.palo = palo;
    return nueva;
}

Jugada armarJugada(Carta a, Carta b, Carta c, Carta d, Carta e) {
    Jugada jugada;
    jugada.cartas[0] = a;
    jugada.cartas[1] = b;
    jugada.cartas[2] = c;
    jugada.cartas[3] = d;
    jugada.cartas[4] = e;
    return jugada;
}

// Tabla Carta-Valor: 'A' -> 15, 'K' -> 13, ... '1'..'9' -> 1..9
int obtenerValorNumerico(char c) {
    if (c >= '1' && c <= '9') return c - '0';
    if (c == 'T') return 10;
    if (c == 'J') return 11;
    if (c == 'Q') return 12;
    if (c == 'K') return 13;
    if (c == 'A') return 15;
    return 0;
}

char obtenerCaracterValor(int valor) {
    if (valor >= 1 && valor <= 9) return valor + '0';
    if (valor == 10) return 'T';
    if (valor == 11) return 'J';
    if (valor == 12) return 'Q';
    if (valor == 13) return 'K';
    if (valor == 15) return 'A';
    return '0';
}

int obtenerPaloNumerico(char c) {
    return c - '0';
}

// ================================================================
//  CLASE MAZO (arreglo estatico, sin memoria dinamica)
// ================================================================

class Mazo {
private:
    Carta cartas[TOTAL_CARTAS_MAZO];
    int total;
    int indiceRobo;

public:
    Mazo() {
        total = 0;
        indiceRobo = 0;
    }

    void agregarCarta(Carta nueva) {
        if (total < TOTAL_CARTAS_MAZO) {
            cartas[total] = nueva;
            total++;
        }
    }

    int getTotal() { return total; }
    int getIndiceRobo() { return indiceRobo; }
    bool haySiguiente() { return indiceRobo < total; }
    Carta getCarta(int i) { return cartas[i]; }

    Carta robar() {
        if (!haySiguiente()) return armarCarta(0, 0);
        Carta robada = cartas[indiceRobo];
        indiceRobo++;
        return robada;
    }
};

// ================================================================
//  CLASE MANO (arreglo estatico, sin memoria dinamica)
// ================================================================

class Mano {
private:
    Carta cartas[CARTAS_POR_MANO];
    int tamano;

public:
    Mano() { tamano = 0; }

    int getTamano() { return tamano; }
    Carta getCarta(int i) { return cartas[i]; }

    void agregar(Carta nueva) {
        if (tamano < CARTAS_POR_MANO) {
            cartas[tamano] = nueva;
            tamano++;
        }
    }

    void quitarEn(int i) {
        if (i < 0 || i >= tamano) return;
        for (int j = i; j < tamano - 1; j++) {
            cartas[j] = cartas[j + 1];
        }
        tamano--;
    }
};

// ================================================================
//  ORDEN Y RANGOS
// ================================================================

// Ordena una jugada por valor de carta (menor a mayor), sincronizando palos
Jugada ordenarJugada(Jugada jugada) {
    for (int i = 0; i < CARTAS_POR_JUGADA - 1; i++) {
        for (int j = i + 1; j < CARTAS_POR_JUGADA; j++) {
            if (jugada.cartas[i].valor > jugada.cartas[j].valor) {
                Carta auxiliar = jugada.cartas[i];
                jugada.cartas[i] = jugada.cartas[j];
                jugada.cartas[j] = auxiliar;
            }
        }
    }
    return jugada;
}

int rangoDeCarta(int valor) {
    if (valor == 15) return RANGO_AS;
    return valor;
}

// Arma los rangos de la jugada omitiendo la carta en la posicion "omitir".
// Con omitir = -1 se usan las 5 cartas.
RangoEscalera armarRango(Jugada jugada, int omitir) {
    RangoEscalera rango;
    rango.cantidad = 0;
    for (int i = 0; i < CARTAS_POR_JUGADA; i++) {
        if (i != omitir) {
            rango.valores[rango.cantidad] = rangoDeCarta(jugada.cartas[i].valor);
            rango.cantidad++;
        }
    }
    return rango;
}

RangoEscalera ordenarRango(RangoEscalera rango) {
    for (int i = 0; i < rango.cantidad - 1; i++) {
        for (int j = i + 1; j < rango.cantidad; j++) {
            if (rango.valores[i] > rango.valores[j]) {
                int auxiliar = rango.valores[i];
                rango.valores[i] = rango.valores[j];
                rango.valores[j] = auxiliar;
            }
        }
    }
    return rango;
}

bool esConsecutivo(RangoEscalera rango) {
    for (int i = 0; i < rango.cantidad - 1; i++) {
        if (rango.valores[i + 1] != rango.valores[i] + 1) return false;
    }
    return true;
}

// Escalera "rueda" A-2-3-4-5: aqui el As hace de 1.
// Se remapea el rango del As (14) a 1 y se comprueba la consecutividad.
bool esRueda(RangoEscalera rango) {
    RangoEscalera copia = rango;
    for (int i = 0; i < copia.cantidad; i++) {
        if (copia.valores[i] == RANGO_AS) copia.valores[i] = 1;
    }
    copia = ordenarRango(copia);
    return esConsecutivo(copia);
}

// El Royal Flush exige exactamente T J Q K A (rangos 10..14).
// Se comprueba el rango explicito para no confundirlo con la rueda
// de color A-2-3-4-5, que tambien contiene un As.
// Recibe el rango ya ordenado de las 5 cartas.
bool esRangoReal(RangoEscalera rango) {
    if (rango.cantidad != CARTAS_POR_JUGADA) return false;
    for (int i = 0; i < rango.cantidad; i++) {
        if (rango.valores[i] != 10 + i) return false;
    }
    return true;
}

// ================================================================
//  TABLA MANO-VALOR
// ================================================================

// Con Cuatro Dedos (7) el color y la escalera valen con 4 cartas
bool esFlush(Jugada jugada, bool cuatroDedos) {
    int maximoRepetidos = 0;
    for (int i = 0; i < CARTAS_POR_JUGADA; i++) {
        int repetidos = 0;
        for (int j = 0; j < CARTAS_POR_JUGADA; j++) {
            if (jugada.cartas[j].palo == jugada.cartas[i].palo) repetidos++;
        }
        if (repetidos > maximoRepetidos) maximoRepetidos = repetidos;
    }
    if (cuatroDedos) return maximoRepetidos >= 4;
    return maximoRepetidos == CARTAS_POR_JUGADA;
}

bool esStraight(Jugada jugada, bool cuatroDedos) {
    if (!cuatroDedos) {
        RangoEscalera rango = ordenarRango(armarRango(jugada, -1));
        return esConsecutivo(rango) || esRueda(rango);
    }
    // Se prueba cada subconjunto de 4 cartas
    for (int omitir = 0; omitir < CARTAS_POR_JUGADA; omitir++) {
        RangoEscalera rango = ordenarRango(armarRango(jugada, omitir));
        if (esConsecutivo(rango) || esRueda(rango)) return true;
    }
    return false;
}

ConteoValores contarValores(Jugada jugada) {
    ConteoValores conteo;
    for (int i = 0; i < 16; i++) conteo.frecuencias[i] = 0;
    for (int i = 0; i < CARTAS_POR_JUGADA; i++) {
        int valor = jugada.cartas[i].valor;
        if (valor >= 0 && valor < 16) conteo.frecuencias[valor]++;
    }
    return conteo;
}

// "Al menos un par": una mano con trio, poker o full house tambien lo cumple
bool tieneAlMenosUnPar(ConteoValores conteo) {
    for (int i = 0; i < 16; i++) {
        if (conteo.frecuencias[i] >= 2) return true;
    }
    return false;
}

// "Dos pares": exactamente dos valores repetidos dos veces
bool tieneDosPares(ConteoValores conteo) {
    int pares = 0;
    for (int i = 0; i < 16; i++) {
        if (conteo.frecuencias[i] == 2) pares++;
    }
    return pares == 2;
}

int evaluarTipoMano(Jugada jugada, bool cuatroDedos) {
    ConteoValores conteo = contarValores(jugada);

    // El Royal Flush exige las 5 cartas: A K Q J T del mismo palo
    bool royalFlush = esFlush(jugada, false) &&
                      esRangoReal(ordenarRango(armarRango(jugada, -1)));

    bool hayFlush = esFlush(jugada, cuatroDedos);
    bool hayStraight = esStraight(jugada, cuatroDedos);

    int maximoCoincidencias = 0;
    int pares = 0;
    bool hayTrio = false;

    for (int i = 0; i < 16; i++) {
        if (conteo.frecuencias[i] > maximoCoincidencias) {
            maximoCoincidencias = conteo.frecuencias[i];
        }
        if (conteo.frecuencias[i] == 2) pares++;
        if (conteo.frecuencias[i] == 3) hayTrio = true;
    }

    if (royalFlush) return 100;              // Royal Flush
    if (hayFlush && hayStraight) return 80;  // Straight Flush
    if (maximoCoincidencias == 4) return 75; // Four of a Kind
    if (hayTrio && pares == 1) return 60;    // Full House
    if (hayFlush) return 50;                 // Flush
    if (hayStraight) return 40;              // Straight
    if (hayTrio) return 30;                  // Three of a Kind
    if (pares == 2) return 20;               // Two Pair
    if (pares == 1) return 10;               // One Pair
    return 0;                                // High Card
}

TipoMano buscarTipoMano(int valorBase) {
    for (int i = 0; i < 10; i++) {
        if (TABLA_MANOS[i].valor == valorBase) return TABLA_MANOS[i];
    }
    return TABLA_MANOS[9];
}

// ================================================================
//  VALORES DE LA MANO
// ================================================================

int sumarValores(Jugada jugada) {
    int suma = 0;
    for (int i = 0; i < CARTAS_POR_JUGADA; i++) suma += jugada.cartas[i].valor;
    return suma;
}

int valorMenor(Jugada jugada) {
    int menor = jugada.cartas[0].valor;
    for (int i = 1; i < CARTAS_POR_JUGADA; i++) {
        if (jugada.cartas[i].valor < menor) menor = jugada.cartas[i].valor;
    }
    return menor;
}

bool tieneComodin(Comodines comodines, int id) {
    for (int i = 0; i < comodines.total; i++) {
        if (comodines.ids[i] == id) return true;
    }
    return false;
}

// Comodines que suman puntos al valor de la mano: 5, 6 y 11
int calcularBonusValorMano(Comodines comodines, bool hayPar, bool hayDosPares) {
    int bonus = 0;
    for (int i = 0; i < comodines.total; i++) {
        if (comodines.ids[i] == JOKER_HABILIDOSO && hayPar) bonus += 50;
        else if (comodines.ids[i] == JOKER_TAIMADO && hayDosPares) bonus += 100;
        else if (comodines.ids[i] == REFLECTANTE) bonus += 50;
    }
    return bonus;
}

// Comodines que afectan el multiplicador: 1, 2, 3, 4, 8 y 9
int calcularMultiplicador(Jugada jugada, Comodines comodines,
                          bool hayFlush, bool hayPar, bool hayDosPares) {
    int multiplicador = 1;
    for (int i = 0; i < comodines.total; i++) {
        int id = comodines.ids[i];

        if (id == JOKER) {
            multiplicador += 4;
        } else if (id == JOKER_ALEGRE) {
            if (hayPar) multiplicador += 8;
        } else if (id == JOKER_DEMENTE) {
            if (hayDosPares) multiplicador += 10;
        } else if (id == JOKER_GRACIOSO) {
            if (hayFlush) multiplicador += 10;
        } else if (id == PUNO_ELEVADO) {
            multiplicador += valorMenor(jugada);
        } else if (id == FIBONACCI) {
            for (int j = 0; j < CARTAS_POR_JUGADA; j++) {
                int valor = jugada.cartas[j].valor;
                if (valor == 15 || valor == 2 || valor == 3 || valor == 5 || valor == 8) {
                    multiplicador += 2;
                }
            }
        }
    }
    return multiplicador;
}

// Formula: (valor de la mano + total de puntos de las cartas) * Mult
int calcularPuntaje(int valordelaMano, int totalCartas, int multiplicador) {
    return (valordelaMano + totalCartas) * multiplicador;
}

// Evalua una jugada de 5 cartas aplicando todos los comodines
ResultadoJugada evaluarJugada(Jugada jugada, Comodines comodines) {
    ResultadoJugada resultado;
    for (int i = 0; i < CARTAS_POR_JUGADA; i++) resultado.indices[i] = i;

    resultado.jugada = ordenarJugada(jugada);

    bool cuatroDedos = tieneComodin(comodines, CUATRO_DEDOS);
    ConteoValores conteo = contarValores(resultado.jugada);
    bool hayPar = tieneAlMenosUnPar(conteo);
    bool hayDosPares = tieneDosPares(conteo);
    bool hayFlush = esFlush(resultado.jugada, cuatroDedos);

    resultado.valorBase = evaluarTipoMano(resultado.jugada, cuatroDedos);
    resultado.sumaCartas = sumarValores(resultado.jugada);
    resultado.bonusValorMano = calcularBonusValorMano(comodines, hayPar, hayDosPares);
    resultado.multiplicador = calcularMultiplicador(resultado.jugada, comodines,
                                                    hayFlush, hayPar, hayDosPares);
    resultado.puntaje = calcularPuntaje(resultado.valorBase + resultado.bonusValorMano,
                                        resultado.sumaCartas,
                                        resultado.multiplicador);
    return resultado;
}

// ================================================================
//  SELECTOR DE LA MEJOR JUGADA (las 56 combinaciones de 8 tomadas de 5 en 5)
// ================================================================

ResultadoJugada seleccionarMejorJugada(Mano mano, Comodines comodines) {
    ResultadoJugada mejor;
    mejor.puntaje = -1;
    mejor.sumaCartas = -1;
    mejor.valorBase = -1;
    mejor.bonusValorMano = 0;
    mejor.multiplicador = 0;
    for (int i = 0; i < CARTAS_POR_JUGADA; i++) mejor.indices[i] = i;

    for (int a = 0; a < 4; a++) {
        for (int b = a + 1; b < 5; b++) {
            for (int c = b + 1; c < 6; c++) {
                for (int d = c + 1; d < 7; d++) {
                    for (int e = d + 1; e < 8; e++) {

                        Jugada jugada = armarJugada(mano.getCarta(a), mano.getCarta(b),
                                                    mano.getCarta(c), mano.getCarta(d),
                                                    mano.getCarta(e));

                        ResultadoJugada actual = evaluarJugada(jugada, comodines);
                        actual.indices[0] = a;
                        actual.indices[1] = b;
                        actual.indices[2] = c;
                        actual.indices[3] = d;
                        actual.indices[4] = e;

                        // Criterio: el mayor puntaje real de la ronda;
                        // el desempate es la mayor suma de cartas
                        if (actual.puntaje > mejor.puntaje ||
                            (actual.puntaje == mejor.puntaje &&
                             actual.sumaCartas > mejor.sumaCartas)) {
                            mejor = actual;
                        }
                    }
                }
            }
        }
    }
    return mejor;
}

// ================================================================
//  MAIN
// ================================================================

int main(int argc, char* argv[]) {
    // Los parametros son obligatorios
    if (argc < 3) {
        std::cout << "Uso correcto: " << argv[0]
                  << " <archivo_entrada.in> <archivo_salida.out>" << std::endl;
        return 1;
    }

    std::ifstream entrada(argv[1]);
    if (!entrada.is_open()) {
        std::cout << "Error al abrir el archivo de entrada: " << argv[1] << std::endl;
        return 1;
    }

    std::ofstream archivoSalida(argv[2]);
    if (!archivoSalida.is_open()) {
        std::cout << "Error al crear/abrir el archivo de salida: " << argv[2] << std::endl;
        entrada.close();
        return 1;
    }

    std::ofstream archivoSav("partida.sav");
    if (!archivoSav.is_open()) {
        std::cout << "Error al crear el archivo de persistencia: partida.sav" << std::endl;
        entrada.close();
        archivoSalida.close();
        return 1;
    }

    // ------------------ LECTURA DEL MAZO ------------------
    Mazo mazo;
    for (int i = 0; i < TOTAL_CARTAS_MAZO; i++) {
        char ficha[3];
        entrada >> ficha;
        Carta nueva = armarCarta(obtenerValorNumerico(ficha[0]),
                                 obtenerPaloNumerico(ficha[1]));
        mazo.agregarCarta(nueva);
    }

    // ------------------ LECTURA DE COMODINES ------------------
    Comodines comodines;
    comodines.total = TOTAL_COMODINES;
    for (int i = 0; i < TOTAL_COMODINES; i++) {
        entrada >> comodines.ids[i];
    }
    // El comodin Negativo (10) suma un espacio a la capacidad de
    // comodines, es decir, cede su lugar a otro comodin. Como la entrada
    // fija los mismos 5 comodines, este espacio extra no altera el puntaje
    // de la partida: queda como dato informativo (no-op).
    comodines.capacidad = TOTAL_COMODINES;
    for (int i = 0; i < comodines.total; i++) {
        if (comodines.ids[i] == NEGATIVO) comodines.capacidad++;
    }

    // ------------------ ROBO INICIAL (8 cartas) ------------------
    Mano mano;
    for (int i = 0; i < CARTAS_POR_MANO; i++) {
        if (mazo.haySiguiente()) mano.agregar(mazo.robar());
    }

    // ------------------ LECTURA DE CIEGAS ------------------
    int ciegas[MAX_RONDAS];
    int totalCiegas = 0;
    while (totalCiegas < MAX_RONDAS && (entrada >> ciegas[totalCiegas])) {
        totalCiegas++;
    }
    entrada.close();

    // ------------------ PILA DE DESCARTES ------------------
    Carta descartes[TOTAL_CARTAS_MAZO];
    int cantDescartes = 0;

    // ------------------ RONDAS ------------------
    bool partidaActiva = true;
    bool primeraRonda = true;
    int ronda = 0;

    while (partidaActiva && ronda < totalCiegas) {

        // Sin 5 cartas en la mano no se puede completar una jugada
        if (mano.getTamano() < CARTAS_POR_JUGADA) {
            partidaActiva = false;
            break;
        }

        ResultadoJugada resultado = seleccionarMejorJugada(mano, comodines);
        TipoMano tipo = buscarTipoMano(resultado.valorBase);
        bool ciegaSuperada = (resultado.puntaje >= ciegas[ronda]);

        // ---------- SALIDA DE LA RONDA ----------
        if (!primeraRonda) archivoSalida << std::endl;
        archivoSalida << resultado.puntaje << std::endl;
        archivoSalida << tipo.nombre << std::endl;
        if (ciegaSuperada) {
            archivoSalida << "The winner takes it all" << std::endl;
        } else {
            archivoSalida << "We folded like a..." << std::endl;
        }
        primeraRonda = false;

        // ---------- DESCARTE PERMANENTE DE LAS 5 JUGADAS ----------
        for (int k = 0; k < CARTAS_POR_JUGADA; k++) {
            Carta jugada = mano.getCarta(resultado.indices[k]);
            if (cantDescartes < TOTAL_CARTAS_MAZO) {
                descartes[cantDescartes] = jugada;
                cantDescartes++;
            }
        }
        // Se quitan de la mano desde el indice mayor al menor
        for (int k = CARTAS_POR_JUGADA - 1; k >= 0; k--) {
            mano.quitarEn(resultado.indices[k]);
        }

        // ---------- PERSISTENCIA (partida.sav) ----------
        // Cada ronda queda separada por un salto de linea
        if (ronda > 0) archivoSav << std::endl;

        for (int i = mazo.getIndiceRobo(); i < mazo.getTotal(); i++) {
            if (i > mazo.getIndiceRobo()) archivoSav << " ";
            Carta carta = mazo.getCarta(i);
            archivoSav << obtenerCaracterValor(carta.valor) << carta.palo;
        }
        archivoSav << std::endl;

        for (int i = 0; i < mano.getTamano(); i++) {
            if (i > 0) archivoSav << " ";
            Carta carta = mano.getCarta(i);
            archivoSav << obtenerCaracterValor(carta.valor) << carta.palo;
        }
        archivoSav << std::endl;

        for (int i = 0; i < comodines.total; i++) {
            if (i > 0) archivoSav << " ";
            archivoSav << comodines.ids[i];
        }
        archivoSav << std::endl;

        for (int i = 0; i < cantDescartes; i++) {
            if (i > 0) archivoSav << " ";
            archivoSav << obtenerCaracterValor(descartes[i].valor) << descartes[i].palo;
        }
        archivoSav << std::endl;

        // ---------- FIN DE LA RONDA ----------
        ronda++;

        if (!ciegaSuperada) {
            // No se supero la ciega del jefe: termina la partida
            partidaActiva = false;
        } else {
            // Reposicion: 5 cartas nuevas para la mano
            int robadas = 0;
            while (robadas < CARTAS_POR_JUGADA && mazo.haySiguiente()) {
                mano.agregar(mazo.robar());
                robadas++;
            }
            // El mazo se vacio sin poder completar una jugada de 5 cartas
            if (robadas < CARTAS_POR_JUGADA) partidaActiva = false;
        }
    }

    archivoSalida.close();
    archivoSav.close();

    return 0;
}
