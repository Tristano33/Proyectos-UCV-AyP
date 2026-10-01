#!/usr/bin/env bash
# ============================================================
#  Pruebas automaticas - Proyecto #2: Calatro, El Juego Oculto
#
#  Uso:
#     make test
#  o
#     bash test.sh
#
#  Cada caso vive en la carpeta casos/ con tres archivos:
#     <caso>.in            entrada
#     <caso>.expected.out  salida esperada
#     <caso>.expected.sav  partida.sav esperado
# ============================================================

set -u

RAIZ="$(pwd)"
DIR_CASOS="$RAIZ/casos"
BINARIO="$RAIZ/bin/output/programa"
TEMPORAL="$RAIZ/.pruebas"

APROBADOS=0
FALLADOS=0

# ------------------------------------------------------------
# Compilacion
# ------------------------------------------------------------
if ! make compile > /dev/null 2>&1; then
    echo "ERROR: la compilacion fallo. Ejecuta 'make' para ver el detalle."
    exit 1
fi

if [ ! -x "$BINARIO" ]; then
    echo "ERROR: no se encontro el ejecutable $BINARIO"
    exit 1
fi

rm -rf "$TEMPORAL"
mkdir -p "$TEMPORAL"

# ------------------------------------------------------------
# probar_caso <nombre>: ejecuta el caso y compara con lo esperado
# ------------------------------------------------------------
probar_caso() {
    local nombre="$1"
    local entrada="$DIR_CASOS/$nombre.in"
    local esperado_salida="$DIR_CASOS/$nombre.expected.out"
    local esperado_sav="$DIR_CASOS/$nombre.expected.sav"
    local carpeta="$TEMPORAL/$nombre"
    local detalle=""

    mkdir -p "$carpeta"
    # Se ejecuta dentro de la carpeta del caso para que partida.sav
    # no se mezcle entre casos
    ( cd "$carpeta" && "$BINARIO" "$entrada" salida.out > /dev/null 2>&1 )

    if [ ! -f "$carpeta/salida.out" ]; then
        detalle="no se genero salida.out"
    elif ! diff -q "$esperado_salida" "$carpeta/salida.out" > /dev/null 2>&1; then
        detalle="la salida no coincide"
    elif [ -f "$esperado_sav" ] && [ ! -f "$carpeta/partida.sav" ]; then
        detalle="no se genero partida.sav"
    elif [ -f "$esperado_sav" ] && ! diff -q "$esperado_sav" "$carpeta/partida.sav" > /dev/null 2>&1; then
        detalle="partida.sav no coincide"
    fi

    if [ -z "$detalle" ]; then
        echo "PASS  $nombre"
        APROBADOS=$((APROBADOS + 1))
        return
    fi

    echo "FAIL  $nombre ($detalle)"
    FALLADOS=$((FALLADOS + 1))
    if [ -f "$carpeta/salida.out" ] && ! diff -q "$esperado_salida" "$carpeta/salida.out" > /dev/null 2>&1; then
        echo "      --- salida.out (esperado | obtenido) ---"
        diff "$esperado_salida" "$carpeta/salida.out" | sed 's/^/      /'
    fi
    if [ -f "$carpeta/partida.sav" ] && [ -f "$esperado_sav" ] &&
       ! diff -q "$esperado_sav" "$carpeta/partida.sav" > /dev/null 2>&1; then
        echo "      --- partida.sav (esperado | obtenido) ---"
        diff "$esperado_sav" "$carpeta/partida.sav" | sed 's/^/      /'
    fi
}

# ------------------------------------------------------------
# mazoAgotado: con ciegas de valor 1 el mazo alcanza para 9
# rondas (8 + 5 * 8 = 48 <= 52) y luego se agota. Se comprueba
# que la partida se detiene por mazo insuficiente: el ultimo
# bloque de partida.sav deja solo 4 cartas sin robar.
# ------------------------------------------------------------
probar_mazo_agotado() {
    local nombre="mazoAgotado"
    local carpeta="$TEMPORAL/$nombre"
    mkdir -p "$carpeta"
    ( cd "$carpeta" && "$BINARIO" "$DIR_CASOS/$nombre.in" salida.out > /dev/null 2>&1 )

    local rondas
    local bloques
    local sobrantes
    rondas=$(grep -c -E "^(The winner takes it all|We folded like a\.\.\.)$" "$carpeta/salida.out")
    bloques=$(grep -c "^1 2 5 8 11$" "$carpeta/partida.sav")
    # Primer renglon del ultimo bloque (el mazo restante) del sav
    sobrantes=$(awk 'NF' "$carpeta/partida.sav" | tail -n 4 | head -n 1 | wc -w | tr -d ' ')

    if [ "$rondas" = "9" ] && [ "$bloques" = "9" ] && [ "$sobrantes" = "4" ]; then
        echo "PASS  mazoAgotado (9 rondas, 9 bloques y 4 cartas sobrantes)"
        APROBADOS=$((APROBADOS + 1))
    else
        echo "FAIL  mazoAgotado (rondas=$rondas, bloques=$bloques, sobrantes=$sobrantes; se esperaban 9, 9 y 4)"
        FALLADOS=$((FALLADOS + 1))
    fi
}

# ------------------------------------------------------------
# sinArgumentos: sin parametros el programa debe fallar y no
# escribir ningun archivo
# ------------------------------------------------------------
probar_sin_argumentos() {
    local carpeta="$TEMPORAL/sinArgumentos"
    mkdir -p "$carpeta"
    ( cd "$carpeta" && "$BINARIO" > /dev/null 2>&1 )
    local codigo=$?

    if [ "$codigo" -ne 0 ] && [ ! -f "$carpeta/partida.sav" ] && [ ! -f "$carpeta/salida.out" ]; then
        echo "PASS  sinArgumentos (codigo $codigo y sin archivos generados)"
        APROBADOS=$((APROBADOS + 1))
    else
        echo "FAIL  sinArgumentos (codigo $codigo)"
        FALLADOS=$((FALLADOS + 1))
    fi
}

# ------------------------------------------------------------
# Ejecucion de todas las pruebas
# ------------------------------------------------------------
echo "================================================"
echo " Pruebas del Proyecto #2 - Calatro"
echo "================================================"

probar_caso enunciado
probar_caso ciegaFallida
probar_caso royalFlush
probar_caso pares
probar_caso cuatroDedos
probar_caso sinCuatroDedos
probar_caso rueda
probar_caso ruedaColor
probar_caso ciegaEmpate
probar_caso comodinesRepetidos
probar_caso flushVariado
probar_caso unaRonda
probar_caso trio
probar_caso parSimple
probar_caso cartaAlta
probar_caso escaleraAlta
probar_caso joker5
probar_caso joker2
probar_caso joker3
probar_caso joker6
probar_caso joker8
probar_caso joker10
probar_mazo_agotado
probar_sin_argumentos

echo "------------------------------------------------"
echo " Aprobados: $APROBADOS    Fallados: $FALLADOS"
echo "------------------------------------------------"
echo " (los archivos generados quedan en .pruebas/<caso>/)"

if [ "$FALLADOS" -ne 0 ]; then
    exit 1
fi

exit 0
