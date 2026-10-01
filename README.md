# Proyecto UCV Calatro V2

Proyecto hecho basandonos en el 2do proyecto de Algoritmos y Programacion

## Compilacion

Para compilar el proyecto se tendra que usar el siguiente comando:

````
$ make
````
O
````
$ make compile
````

## Ejecucion

El programa recibe por parametros el archivo de entrada y el archivo de salida:

````
$ ./bin/output/programa input.txt salida.out
````

Ademas genera el archivo de persistencia `partida.sav` en la carpeta donde se ejecute.

## Test

Las pruebas son automaticas y se corren con:

````
$ make test
````

O directamente:

````
$ bash test.sh
````

El script compila el proyecto, ejecuta cada caso de la carpeta `casos/` y
compara la salida y el `partida.sav` generados contra los archivos esperados
(`<caso>.expected.out` y `<caso>.expected.sav`). Al final imprime cuantos casos
pasaron y devuelve un codigo de error si alguno falla. Los archivos generados
quedan en `.pruebas/<caso>/` para poder revisarlos.

Casos incluidos:

- `enunciado`: el caso de prueba del enunciado (es el unico dato oficial).
- `ciegaFallida`: la primera ciega no se supera, la partida termina ahi.
- `royalFlush`: escalera con As alto del mismo palo (A K Q J T).
- `pares`: mano de dos pares, prueba los comodines 3, 5, 6 y 9.
- `cuatroDedos`: con el comodin 7 el color se forma con 4 cartas.
- `sinCuatroDedos`: el mismo mazo sin el comodin 7, ese color ya no cuenta.
- `rueda`: escalera "rueda" A-2-3-4-5 (el As vale como 1).
- `ruedaColor`: rueda del mismo palo; debe ser Straight Flush, nunca Royal Flush.
- `ciegaEmpate`: el puntaje es exactamente igual a la ciega (se supera con >=).
- `comodinesRepetidos`: cinco comodines iguales apilan su multiplicador.
- `flushVariado`: color con los comodines 1, 4, 8, 2 y 10.
- `unaRonda`: una sola ciega produce exactamente una ronda y un bloque en el sav.
- `trio`: Three of a Kind (tres nueves con el comodin 11).
- `parSimple`: One Pair (con el comodin 11).
- `cartaAlta`: High Card (con el comodin 11).
- `escaleraAlta`: escalera con As alto A K Q J T de palos distintos (no es color).
- `joker5`: el comodin Habilidoso suma 50 al valor de la mano con un par.
- `joker2`: el comodin Alegre suma 8 al multiplicador con un par.
- `joker3`: el comodin Demente suma 10 al multiplicador con dos pares.
- `joker6`: el comodin Taimado suma 100 al valor de la mano con dos pares.
- `joker8`: el comodin Puno Elevado suma la carta menor al multiplicador.
- `joker10`: el comodin Negativo no altera el puntaje (es nulo).
- `mazoAgotado`: 10 ciegas faciles; el mazo alcanza para 9 rondas y luego se agota
  (el ultimo bloque de `partida.sav` deja 4 cartas sin robar).
- `sinArgumentos`: sin parametros el programa falla y no escribe archivos.

## Descripcion

El proyecto usa Programacion Orientada a Objetos con `struct` y clases
(`Carta`, `Mazo`, `Mano`, `Jugada`), y arreglos estaticos para el mazo, la mano,
la jugada y los descartes. Como no se permite `enum`, los palos y los tipos de
mano se manejan con constantes enteras y con la tabla `TABLA_MANOS`.

La bateria de pruebas cubre los 10 tipos de mano de la tabla y los 11 comodines
(varios de ellos aislados en su propio caso). Cada ronda se juega asi:

1. Se evaluan las 56 combinaciones posibles de 5 cartas dentro de la mano de 8.
2. Se elige la de mayor puntaje, aplicando la formula (valor de la mano + total
   de puntos de las cartas) * Mult.
3. Se compara contra la ciega del jefe: se supera con `>=` y se escribe el resultado.
4. Las 5 cartas jugadas se descartan y se roban 5 nuevas para reponer la mano.
5. La partida termina si no se supera la ciega o si el mazo no alcanza para
   completar una jugada de 5 cartas (con 52 cartas caben 9 rondas).

En la escalera el As es alto (A K Q J T) pero tambien puede ser bajo (A 2 3 4 5,
la "rueda"). El Royal Flush solo se reconoce con los rangos T J Q K A, para no
confundir una rueda de color con una escalera real.
