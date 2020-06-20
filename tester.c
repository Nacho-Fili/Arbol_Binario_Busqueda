#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ansicolor.h"

typedef struct tester{
  size_t pruebas_totales;
  size_t pruebas_pasadas;
  size_t avisos;
} tester_t;


tester_t* tester_crear(){
  tester_t* tester = calloc(1, sizeof(tester_t));

  return tester;
}

void tester_afirmar(tester_t* tester, char* descripcion, bool afirmacion){
  if(afirmacion) (tester -> pruebas_pasadas)++;
  (tester -> pruebas_totales)++;

  printf("\tPrueba %s: %s%s%s\n", descripcion, afirmacion ? ANSI_COLOR_GREEN:ANSI_COLOR_RED, afirmacion ? "EXITOSA":"FALLADA", ANSI_RESET);
}

void tester_avisar(tester_t* tester, char* descripcion, bool afirmacion){
  if(afirmacion){
    printf("\t%sAVISO:%s %s\n", ANSI_COLOR_YELLOW, ANSI_RESET, descripcion);
    (tester -> avisos)++;
  }
}

void tester_mostrar_estadisticas(tester_t* tester){
  printf("PRUEBAS PASADAS: %li/%li\n", tester -> pruebas_pasadas, tester -> pruebas_totales);
  printf("AVISOS: %li\n", tester -> avisos);
  if(tester -> pruebas_pasadas == tester -> pruebas_totales)
    printf("Todo en orden :) \n");
  else
     printf("Hay %li %s :(\n", tester -> pruebas_totales - tester -> pruebas_pasadas, (tester -> pruebas_totales - tester -> pruebas_pasadas) == 1 ? "error" : "errores");
}

void tester_destruir(tester_t* tester){
  free(tester);
}
