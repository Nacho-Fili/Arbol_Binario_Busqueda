#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"
#include "tester.h"
#include <time.h>

#define ELEMENTOS_IGUALES 0
#define SEGUNDO_MAYOR -1
#define SEGUNDO_MENOR 1
#define EXITO 0
#define ERROR -1
#define ELEMENTOS_ARBOL_PRUEBA 23
#define ORDEN_INSERCION 0
#define INORDEN 1
#define PREORDEN 2
#define POSTORDEN 3
#define MAX_COSAS 20000
#define MAX_NUMERO 100000
#define MAX_BUSQUEDA 15000
#define MAX_BORRADO 50 // culpa de array_borrar
#define MAX_COMPARACION 5000

typedef struct cosa_a_destruir{
  int clave;
  int borrado;
} cosa_a_destruir_t;


/*******************************************************************************/
/***********************   FUNCIONES AUXILIARES   ******************************/
/*******************************************************************************/
/*
 *  Devuelve un número pseudo aleatorio entre min y max.
 */
int numero_random(int min, int max){
  int numero = rand();
  int ret = (numero % (max - min)) + min;

  return ret;
}

/*
 *  Si n es un número primo, devuelve true. Sino, devuelve false.
 */
bool es_primo(int n){
  for(int i = n/2; i > 1; i--){
    if((n % i) == 0)
      return false;
  }

  return true;
}

/*
 *  Recibe un array, su tope, una función de comparación y el tamaño de los elementos guardados
 * en el array. Devuelve el índice en el que se encuentra elemento_buscado.
 */
int buscar_elemento(void* array, void* elemento_buscado, int tope, int(*comparador)(void*, void*), int tamanio){

    int medio = tope/2;

    int posicion = -1;

    if(tope == 1 && comparador(array, elemento_buscado) != ELEMENTOS_IGUALES)
      return ERROR;

    else if(comparador((char*)array + tamanio*medio, elemento_buscado) == SEGUNDO_MENOR)
      posicion = buscar_elemento(array, elemento_buscado, medio, comparador, tamanio);

    else if(comparador((char*)array + tamanio*medio, elemento_buscado) == SEGUNDO_MAYOR)
      posicion = buscar_elemento((char*)array + tamanio*medio, elemento_buscado, tope, comparador, tamanio);

    else{
      posicion = medio;
      return posicion;
    }
    return posicion;
}

/*
 *  Carga en nuevo_array todos los elementos que se encuentran en array y no se encuentran
 * en elementos_a_borrar. Inicializar tope_nuevo_array con la cantidad de elementos elementos_cargados
 * en nuevo_array.
 */
void diferencia_arrays(int array[ELEMENTOS_ARBOL_PRUEBA], int nuevo_array[ELEMENTOS_ARBOL_PRUEBA], int elementos_a_borrar[ELEMENTOS_ARBOL_PRUEBA], size_t* tope_nuevo_array){
  int i = 0;
  int j = 0;
  int k = 0;

  while(i < ELEMENTOS_ARBOL_PRUEBA){
    if(array[i] != elementos_a_borrar[k]){
      nuevo_array[j] = array[i];
      (*tope_nuevo_array)++;
      i++;
      j++;
    }
    else if(array[i] == elementos_a_borrar[k]){
      k++;
      i++;
    }
  }
}

/*
 *  Recibe un array genérico y un array void**, tope representa la cantidad de elementos
 * que se desean comparar con el comparador. Tamanio representa el tamaño de los datos que
 * almacenan los arrays. Devuelve la cantidad de elementos con los que el comparador devolvió
 * ELEMENTOS_IGUALES. Carga en posiciones_erroneas los índices de array_1 que no resultaron iguales
 * a los de array_2.
 */
size_t comparar_arrays(void* array_1, void** array_2, size_t tope, size_t posiciones_erroneas[], int (*comparador)(void*, void*), size_t  tamanio){
  size_t elementos_iguales = 0;
  size_t errores_tope = 0;

  for(size_t i = 0; i < tope; i++){
    if(comparador((char*)array_1 + i*tamanio, *(array_2 + i) ) == ELEMENTOS_IGUALES){
      elementos_iguales++;
    }
    else{
      posiciones_erroneas[errores_tope] = i;
      errores_tope++;
    }
  }
  return elementos_iguales;
}

void swap(void* elemento_1, void* elemento_2, size_t tamanio){
  cosa_a_destruir_t aux;
  memcpy(&aux, elemento_1, tamanio);
  memcpy(elemento_1, elemento_2, tamanio);
  memcpy(elemento_2, &aux, tamanio);
}

/*
 *  Borra ordenadamente del array el elemento igual al parámetro elemento, valiéndose
 * del comparador. Recibe el tamaño de los elementos almacenados en el array carga en tope
 * el nuevo tope del array.
 */
void array_borrar(void* array, void* elemento, size_t tamanio, size_t* tope, int comparador(void*, void*)){
  void* elemento_1;
  void* elemento_2;
  int elemento_encontrado;
  int indice = -1;
  do{
    indice++;
    elemento_encontrado = comparador((char*)array+(size_t)indice*tamanio, elemento);
  } while(elemento_encontrado != 0);

  for(size_t i = (size_t)indice; i < (*tope - 1); i++){
    elemento_1 = (char*)array + i*tamanio;
    elemento_2 = (char*)array + (i+1)*tamanio;
    swap(elemento_1, elemento_2, tamanio);
  }
  (*tope)--;
}

/*
 *  Recibe un árbol, un array de elementos a insertar y junto con su tope. Inserta
 * los elementos del array en el árbol hasta que haya una inserción incorrecta o
 * no queden más elemetos por insertar.
 */
size_t llenar_arbol(abb_t* arbol, int* elementos_a_insertar, int tope){
  size_t i = 0;
  size_t elementos_insertados = 0;
  int insercion_correcta;

  do{
    insercion_correcta = arbol_insertar(arbol, elementos_a_insertar + i);

    if(insercion_correcta == EXITO)
      elementos_insertados++;

    i++;
  } while(insercion_correcta == EXITO && i < tope);

  return elementos_insertados;
}

/*
 *  Compara dos números y devuelve SEGUNDO MAYOR, SEGUNDO MENOR o ELEMENTOS_IGUALES
 * según corresponda.
 */
int comparador(void* primer_elemento, void* segundo_elemento){
  int primer_numero = *( (int*) primer_elemento);
  int segundo_numero = *( (int*) segundo_elemento);
  int ret;

  if(primer_numero < segundo_numero)
    ret = SEGUNDO_MAYOR;

  else if(primer_numero > segundo_numero)
    ret = SEGUNDO_MENOR;

  else
    ret = ELEMENTOS_IGUALES;

  return ret;
}

/*
 *  Compara dos cosas_a_destruir_t y devuelve SEGUNDO MAYOR, SEGUNDO MENOR o ELEMENTOS_IGUALES
 * según corresponda fijándose en su clave.
 */
int comparador_cosa_a_destruir(void* primer_elemento_generico, void* segundo_elemento_generico){
  cosa_a_destruir_t* primer_elemento = primer_elemento_generico;
  cosa_a_destruir_t* segundo_elemento = segundo_elemento_generico;
  int ret = comparador(&(primer_elemento -> clave), &(segundo_elemento -> clave));

  return ret;
}

/*****************************************/
  /*     GRACIAS PARÁMETROS DE QSORT   */
/*****************************************/
//igual que comparador_cosa_a_destruir
int comparador_const(const void* primer_elemento, const void* segundo_elemento){
  int ret;

  if(((cosa_a_destruir_t*)primer_elemento) -> clave > ((cosa_a_destruir_t*)segundo_elemento) -> clave)
    ret = 1;
  else if(((cosa_a_destruir_t*)primer_elemento) -> clave == ((cosa_a_destruir_t*)segundo_elemento) -> clave)
    ret = 0;
  else
    ret = -1;

  return ret;
}

/*
 * Recibe un cosa_a_destruir_t* y aumenta su campo borrado en uno.
 */
void destructor(void* elemento_generico){
  cosa_a_destruir_t* elemento = elemento_generico;
  (elemento -> borrado)++;
}

/*
 *  Imprime el elemento recibido en el primer parámetro y devuelve true si es un
 * número primo distinto de 5
 */
bool parar_primer_primo_distinto_5(void* elemento_generico, void* ultimo_elemento){

  int* numero = elemento_generico;
  bool es_primo_no_5 = (*numero == 5) ? false : es_primo(*numero);

  if(!es_primo_no_5) printf("%i, ", *numero);
  else printf("%i\n", *numero);

  return es_primo_no_5;
}
/*
 *  Imprime los tope primeros elementos de array_genérico en una misma línea
 * separados por coma.
 */
void imprimir_array(void** array_generico, size_t tope){
  int** array = (int**)array_generico;

  printf("\t\t");
  for(size_t i = 0; i < tope; i++){
    if(i != tope - 1)
      printf("%i, ", *array[i]);
    else
      printf("%i\n", *array[i]);
  }
}

/*******************************************************************************/
/*************************   FUNCIONES TESTING   *******************************/
/*******************************************************************************/

void probar_crear(tester_t* tester, int* elementos_arbol[4]){
  abb_t* arbol = arbol_crear(NULL, NULL);
  tester_afirmar(tester, "Árbol sin destructor ni comparador devuelve NULL", !arbol);
  arbol = arbol_crear(comparador, NULL);
  tester_afirmar(tester, "Puedo crear un arbol sin destructor", arbol);
  arbol_destruir(arbol);
}

void probar_raiz(tester_t* tester, int* elementos_arbol[4]) {
  int a = 1, b = 2;
  int insercion_correcta;
  abb_t* arbol = NULL;
  tester_afirmar(tester, "Pedir la raíz de un árbol NULL devuelve NULL", !arbol_raiz(arbol));

  arbol = arbol_crear(comparador, NULL);
  tester_afirmar(tester, "Pedir la raiz de un árbol vacío devuelve NULL", !arbol_raiz(arbol));

  insercion_correcta = arbol_insertar(arbol, &a);
  tester_avisar(tester, "Falló la inserción", insercion_correcta == ERROR);
  tester_afirmar(tester, "Luego de insertar un elemento en el árbol, la raiz es la esperada", *(int*)arbol_raiz(arbol) == a);

  insercion_correcta = arbol_insertar(arbol, &b);
  tester_avisar(tester, "Falló la inserción", insercion_correcta == ERROR);
  tester_afirmar(tester, "Luego de insertar otro elemento en el árbol, la raiz sigue siendo la misma", *(int*)arbol_raiz(arbol) == a);

  arbol_destruir(arbol);
}

void probar_arbol_vacio(tester_t* tester, int* elementos_arbol[4]){
  int a = 10;
  abb_t* arbol = arbol_crear(comparador, NULL);

  int insercion_correcta;
  int borrado_correcto;

  tester_afirmar(tester, "Arbol se crea vacío", arbol_vacio(arbol));

  insercion_correcta = arbol_insertar(arbol, &a);
  tester_avisar(tester, "Falló la inserción", insercion_correcta == ERROR);
  tester_afirmar(tester, "Arbol no está vacío luego de una inserción", !arbol_vacio(arbol));

  borrado_correcto = arbol_borrar(arbol, &a);
  tester_avisar(tester, "Falló el borrado", borrado_correcto == ERROR);
  tester_afirmar(tester, "Arbol vuelve a estar vacío si borro el elemento insertado", arbol_vacio(arbol));

  arbol_destruir(arbol);
}

void probar_insertar(tester_t* tester, int* elementos_arbol[4]){
  abb_t* arbol = arbol_crear(comparador, NULL);
  size_t elementos_en_posicion_esperada = 0;
  size_t errores[ELEMENTOS_ARBOL_PRUEBA];
  size_t elementos_insertados = 0;

  void* array[ELEMENTOS_ARBOL_PRUEBA];

  tester_afirmar(tester, "Árbol se crea vacío", arbol_vacio(arbol));
  tester_afirmar(tester, "Insertar en ábol NULL devuelve error", arbol_insertar(NULL, elementos_arbol[ORDEN_INSERCION]) == ERROR);
  tester_afirmar(tester, "Puedo insertar un elemento en la raíz", arbol_insertar(arbol, elementos_arbol[ORDEN_INSERCION]) == EXITO);
  tester_afirmar(tester, "Árbol no está vacío luego de una inserción", !arbol_vacio(arbol));
  tester_afirmar(tester, "Puedo insertar un segundo elemento", arbol_insertar(arbol, elementos_arbol[ORDEN_INSERCION] + 1) == EXITO);
  tester_afirmar(tester, "Puedo insertar un tercer elemento", arbol_insertar(arbol, elementos_arbol[ORDEN_INSERCION] + 2) == EXITO);
  tester_afirmar(tester, "El elemento en la raíz coincide con el ingresado", *( (int*) arbol_raiz(arbol)) == *elementos_arbol[ORDEN_INSERCION]);
  tester_afirmar(tester, "El primer elemento ingresado se encuentra en el arbol", *( (int*)arbol_buscar(arbol, elementos_arbol[ORDEN_INSERCION]) ) == elementos_arbol[ORDEN_INSERCION][0]);
  tester_afirmar(tester, "El tercer elemento ingresado se encuentra en el arbol", *( (int*)arbol_buscar(arbol, elementos_arbol[ORDEN_INSERCION] + 2) ) == elementos_arbol[ORDEN_INSERCION][2]);
  elementos_insertados += 3;
  elementos_insertados += llenar_arbol(arbol, elementos_arbol[ORDEN_INSERCION] + 3, ELEMENTOS_ARBOL_PRUEBA - 3);

  tester_afirmar(tester, "Se insertan otros 20 elementos correctamente", elementos_insertados == ELEMENTOS_ARBOL_PRUEBA);
  if(elementos_insertados != ELEMENTOS_ARBOL_PRUEBA)
    printf("Solo se insertaron correctamente %li elementos\n", elementos_insertados);

  arbol_recorrido_inorden(arbol, array, ELEMENTOS_ARBOL_PRUEBA);
  elementos_en_posicion_esperada = comparar_arrays(elementos_arbol[INORDEN], array, elementos_insertados, errores, comparador, sizeof(int));

  tester_afirmar(tester, "Todos los elementos se insertaron en la posicion esperada", elementos_en_posicion_esperada == ELEMENTOS_ARBOL_PRUEBA);

  if(elementos_en_posicion_esperada != ELEMENTOS_ARBOL_PRUEBA){
    for(size_t i = 0; i < ELEMENTOS_ARBOL_PRUEBA - elementos_en_posicion_esperada; i++){
      printf("\tError en la iteración %li, se esperaba un %i y había un %i", errores[i], elementos_arbol[INORDEN][errores[i]], **(int**)(array + errores[i]));
    }
  }

  arbol_destruir(arbol);
}

void probar_borrar(tester_t* tester, int* elementos_arbol[4]){

  abb_t* arbol = arbol_crear(comparador, NULL);
  size_t elementos_insertados = llenar_arbol(arbol, elementos_arbol[INORDEN], ELEMENTOS_ARBOL_PRUEBA);
  tester_avisar(tester, "El árbol no cuenta con la cantidad esperada de elementos", elementos_insertados != ELEMENTOS_ARBOL_PRUEBA);

  void* array[ELEMENTOS_ARBOL_PRUEBA];

  int elementos_a_borrar[4] = { 22, 25, 50, 60 }; //Elección arbitraria
  size_t errores[ELEMENTOS_ARBOL_PRUEBA];
  size_t elementos_en_posicion_esperada = 0;
  int entero_no_insertado  = 100;
  int array_post_borrado[ELEMENTOS_ARBOL_PRUEBA];
  size_t tope_nuevo_array = 0;

  tester_afirmar(tester, "Borrar en un arbol NULL devuelve error", arbol_borrar(NULL, elementos_arbol[INORDEN]) == ERROR);
  tester_afirmar(tester, "Borrar un elemento que no fue insertado devuelve error", arbol_borrar(arbol, &entero_no_insertado) == ERROR);

  tester_afirmar(tester, "Borrar un nodo sin hijos no causa problemas", arbol_borrar(arbol, elementos_a_borrar ) == EXITO);
  tester_afirmar(tester, "Borrar un nodo con un hijo no causa problemas", arbol_borrar(arbol, elementos_a_borrar + 1) == EXITO);
  tester_afirmar(tester, "Borrar un nodo con dos hijos no causa problemas", arbol_borrar(arbol, elementos_a_borrar + 2) == EXITO);
  tester_afirmar(tester, "Borrar un nodo con dos hijos no causa problemas", arbol_borrar(arbol, elementos_a_borrar + 3) == EXITO);

  diferencia_arrays(elementos_arbol[INORDEN], (int*)array_post_borrado, elementos_a_borrar, &tope_nuevo_array);
  arbol_recorrido_inorden(arbol, array, (int)tope_nuevo_array);

  elementos_en_posicion_esperada = comparar_arrays(array_post_borrado, array, tope_nuevo_array, errores, comparador, sizeof(int));

  tester_afirmar(tester, "El orden después de borrar es correcto", elementos_en_posicion_esperada == tope_nuevo_array);
  if(elementos_en_posicion_esperada != tope_nuevo_array){
    for(size_t i = 0; i < tope_nuevo_array - elementos_en_posicion_esperada; i++){
      printf("\tError en la iteración %li, se esperaba un %i y había un %i\n", errores[i], elementos_arbol[INORDEN][errores[i]], **(int**)(array_post_borrado + errores[i]));
    }
  }

  arbol_destruir(arbol);

}

void probar_buscar(tester_t* tester, int* elementos_arbol[4]){
  abb_t* arbol = arbol_crear(comparador, NULL);
  size_t elementos_insertados = llenar_arbol(arbol, elementos_arbol[ORDEN_INSERCION], ELEMENTOS_ARBOL_PRUEBA);
  tester_avisar(tester, "El árbol no cuenta con la cantidad esperada de elementos", elementos_insertados != ELEMENTOS_ARBOL_PRUEBA);
  int elementos_a_buscar[5] = { 50, 80, 57, 42, 100 }; //Eleccion arbitraria

  tester_afirmar(tester, "Buscar en árbol NULL devuelve NULL", !arbol_buscar(NULL, elementos_a_buscar));
  tester_afirmar(tester, "Puedo obtener el elemento en la raíz", *(int*)arbol_buscar(arbol, elementos_a_buscar) == elementos_a_buscar[0]);
  tester_afirmar(tester, "Puedo obtener un elemento con dos hijos", *(int*)arbol_buscar(arbol, elementos_a_buscar + 1) == elementos_a_buscar[1]);
  tester_afirmar(tester, "Puedo obtener un elemento con un hijo", *(int*)arbol_buscar(arbol, elementos_a_buscar + 2) == elementos_a_buscar[2]);
  tester_afirmar(tester, "Puedo obtener un elemento sin hijos", *(int*)arbol_buscar(arbol, elementos_a_buscar + 3) == elementos_a_buscar[3]);
  tester_afirmar(tester, "Buscar un elemento que no existe devuelve NULL", !arbol_buscar(arbol, elementos_a_buscar + 4));

  arbol_destruir(arbol);
}

void probar_recorridos(tester_t* tester, int* elementos_arbol[4], size_t elementos_a_cargar){
  abb_t* arbol = arbol_crear(comparador, NULL);
  size_t elementos_insertados = llenar_arbol(arbol, elementos_arbol[ORDEN_INSERCION], ELEMENTOS_ARBOL_PRUEBA);
  tester_avisar(tester, "El árbol no cuenta con la cantidad esperada de elementos", elementos_insertados != ELEMENTOS_ARBOL_PRUEBA);

  void* array_inorden[elementos_a_cargar];
  size_t elementos_en_posicion_inorden = 0;
  size_t errores_inorden[elementos_a_cargar];

  void* array_preorden[elementos_a_cargar];
  size_t elementos_en_posicion_preorden = 0;
  size_t errores_preorden[elementos_a_cargar];

  void* array_postorden[elementos_a_cargar];
  size_t elementos_en_posicion_postorden = 0;
  size_t errores_postorden[elementos_a_cargar];

  arbol_recorrido_inorden(arbol, array_inorden, (int)elementos_a_cargar);
  arbol_recorrido_postorden(arbol, array_postorden, (int)elementos_a_cargar);
  arbol_recorrido_preorden(arbol, array_preorden, (int)elementos_a_cargar);

  printf("\tÁrbol en inorden:\n");
  imprimir_array(array_inorden, elementos_a_cargar);
  elementos_en_posicion_inorden = comparar_arrays(elementos_arbol[INORDEN], array_inorden, elementos_a_cargar, errores_inorden, comparador, sizeof(int));
  tester_afirmar(tester, "Recorrido inorden con salida esperada", elementos_en_posicion_inorden == elementos_a_cargar);
  if(elementos_en_posicion_inorden != elementos_a_cargar){
    for(size_t i = 0; i < elementos_a_cargar - elementos_en_posicion_inorden; i++){
      printf("Error en la iteración %li: se esperaba un %i y había un %i", i, elementos_arbol[INORDEN][errores_inorden[i]], **(int**)(array_inorden + errores_inorden[i]));
    }
  }

  printf("\n\tÁrbol en preorden:\n");

  imprimir_array(array_preorden, elementos_a_cargar);
  elementos_en_posicion_preorden = comparar_arrays(elementos_arbol[PREORDEN], array_preorden, elementos_a_cargar, errores_preorden, comparador, sizeof(int));
  tester_afirmar(tester, "Recorrido inorden con salida esperada", elementos_en_posicion_preorden == elementos_a_cargar);

  if(elementos_en_posicion_preorden != elementos_a_cargar){
    for(size_t j = 0; j < elementos_a_cargar - elementos_en_posicion_preorden; j++){
      printf("Error en la iteración %li: se esperaba un %i y había un %i", j, elementos_arbol[PREORDEN][errores_preorden[j]], **(int**)(array_preorden + errores_preorden[j]));
    }
  }

  printf("\n\tÁrbol en postorden:\n");

  imprimir_array(array_postorden, elementos_a_cargar);
  elementos_en_posicion_postorden = comparar_arrays(elementos_arbol[POSTORDEN], array_postorden, elementos_a_cargar, errores_postorden, comparador, sizeof(int));
  tester_afirmar(tester, "Recorrido inorden con salida esperada", elementos_en_posicion_postorden == elementos_a_cargar);
  
  if(elementos_en_posicion_postorden != elementos_a_cargar){
    for(size_t k = 0; k < elementos_a_cargar - elementos_en_posicion_postorden; k++){
      printf("Error en la iteración %li: se esperaba un %i y había un %i", k, elementos_arbol[INORDEN][errores_postorden[k]], **(int**)(array_postorden + errores_postorden[k]));
    }
  }

  arbol_destruir(arbol);
}

void probar_iterador(tester_t* tester, int* elementos_arbol[4]){
  abb_t* arbol = arbol_crear(comparador, NULL);
  size_t elementos_insertados = llenar_arbol(arbol, elementos_arbol[ORDEN_INSERCION], ELEMENTOS_ARBOL_PRUEBA);
  tester_avisar(tester, "El árbol no cuenta con la cantidad esperada de elementos", elementos_insertados != ELEMENTOS_ARBOL_PRUEBA);

  int indice_inorden = 1;
  int indice_postorden = 1;
  int indice_preorden = 1;
  bool (*parar_primer_primo_p)(void*, void*) = parar_primer_primo_distinto_5;


  printf("\tLos recorridos deberían detenerse al hallar el primer número primo distinto de 5\n");
  printf("\t\tInorden\n\t\t");
  abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, parar_primer_primo_p, &indice_inorden);
  printf("\t\tPreorden\n\t\t");
  abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, parar_primer_primo_p, &indice_preorden);
  printf("\t\tPostorden\n\t\t");
  abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, parar_primer_primo_p, &indice_postorden);

  arbol_destruir(arbol);
}

void probar_destructor(tester_t* tester, int* elementos_arbol[4]){
  cosa_a_destruir_t numeros[ELEMENTOS_ARBOL_PRUEBA];
  size_t i = 0;
  size_t j = 0;
  bool borrado_correcto;
  int numeros_a_borrar[3] = { 50, 75, 77 };
  int posicion_50 = buscar_elemento(elementos_arbol[INORDEN], numeros_a_borrar, ELEMENTOS_ARBOL_PRUEBA, comparador, sizeof(int));
  int posicion_75 = buscar_elemento(elementos_arbol[INORDEN], numeros_a_borrar+1, ELEMENTOS_ARBOL_PRUEBA, comparador, sizeof(int));
  int posicion_77 = buscar_elemento(elementos_arbol[INORDEN], numeros_a_borrar+2, ELEMENTOS_ARBOL_PRUEBA, comparador, sizeof(int));
  abb_t* arbol = arbol_crear(comparador_cosa_a_destruir, destructor);

  if(!arbol)
    return;

  for(i = 0; i < ELEMENTOS_ARBOL_PRUEBA; i++){
    numeros[i].clave = elementos_arbol[ORDEN_INSERCION][i];
    numeros[i].borrado = 0;
    arbol_insertar(arbol, numeros + i);
  }

  arbol_borrar(arbol, elementos_arbol[INORDEN] + posicion_50); //Borro nodo raiz con dos hijos
  arbol_borrar(arbol, elementos_arbol[INORDEN] + posicion_75); //Borro nodo con un hijo
  arbol_borrar(arbol, elementos_arbol[INORDEN] + posicion_77); //Borro nodo sin hijos

  arbol_destruir(arbol);
  do{
    borrado_correcto = numeros[j].borrado == 1;
    j++;
  } while(j < ELEMENTOS_ARBOL_PRUEBA && borrado_correcto);

  tester_afirmar(tester, "El destructor se invocó una vez por cada elemento", borrado_correcto);
  if(!borrado_correcto)
    printf("El número %i se borró %i veces\n", numeros[j].clave, numeros[j].clave);

}

void probar_integracion(tester_t* tester){
  abb_t* arbol = arbol_crear(comparador_cosa_a_destruir, destructor);

  cosa_a_destruir_t cosas[MAX_COSAS];
  cosa_a_destruir_t cosas_post_borrado[MAX_COSAS];
  cosa_a_destruir_t* cosa_a_comparar;

  srand((unsigned int)time(NULL));

  bool busqueda_correcta;
  int insercion_correcta;
  int borrado_correcto;
  int destructor_correcto;
  size_t l = 0;
  size_t elementos_en_posicion_esperada;
  size_t errores_orden[MAX_COSAS];
  size_t elementos_cargados;
  size_t indice_a_comparar;
  size_t indice_a_borrar;
  size_t tope_borrados = MAX_COSAS;
  size_t tope_cosas = MAX_COSAS;
  size_t errores = 0;
  void* cosas_inorden[MAX_COMPARACION];

  memset(cosas, 0, sizeof(cosa_a_destruir_t)*MAX_COSAS);
  memset(cosas_post_borrado, 0, sizeof(cosa_a_destruir_t)*MAX_COSAS);


  printf("\tSe insertan en el árbol %i elementos...\n", MAX_COSAS);
  for(size_t i = 0; i < tope_cosas; i++){
    cosas[i].clave = numero_random(0, MAX_NUMERO);
    cosas_post_borrado[i].clave = cosas[i].clave;

    insercion_correcta = arbol_insertar(arbol, cosas + i);
    tester_avisar(tester, "No se insertó correctamente", insercion_correcta == ERROR);
    if(insercion_correcta == ERROR)
      errores++;
  }
  tester_afirmar(tester, "Se insertaron sin errores", !errores);
  errores = 0;


  printf("\tSe buscan %i elementos en el árbol...\n", MAX_BUSQUEDA);
  for(size_t j = 0; j < MAX_BUSQUEDA; j++){
    indice_a_comparar = (size_t)numero_random(0, (int)tope_cosas);
    cosa_a_comparar = arbol_buscar(arbol, cosas + indice_a_comparar);

    tester_avisar(tester, "Búsqueda devolvió NULL", !cosa_a_comparar);
    busqueda_correcta = (cosa_a_comparar -> clave) == (cosas[indice_a_comparar].clave);

    if(!busqueda_correcta)
      errores++;
  }
  tester_afirmar(tester, "Se encontraron todos los elementos buscados", !errores);
  errores = 0;


  printf("\tSe borran del árbol %i elementos...\n", MAX_BORRADO);
  for(size_t k = 0; k < MAX_BORRADO; k++){

    indice_a_borrar = (size_t)numero_random(0, (int)tope_borrados);

    borrado_correcto = arbol_borrar(arbol, cosas_post_borrado + indice_a_borrar);
    array_borrar(cosas_post_borrado, cosas_post_borrado + indice_a_borrar, sizeof(cosa_a_destruir_t), &tope_borrados, comparador_cosa_a_destruir);

    tester_avisar(tester, "No se borró correctamente", borrado_correcto == ERROR);

    if(borrado_correcto == ERROR)
      errores++;
  }
  tester_afirmar(tester, "Se borraron sin errores", !errores);

  qsort(cosas_post_borrado, tope_borrados, sizeof(cosa_a_destruir_t), comparador_const);

  elementos_cargados = (size_t)arbol_recorrido_inorden(arbol, cosas_inorden, MAX_COMPARACION);
  elementos_en_posicion_esperada = comparar_arrays(cosas_post_borrado, cosas_inorden, elementos_cargados, errores_orden, comparador_cosa_a_destruir, sizeof(cosa_a_destruir_t));

  tester_afirmar(tester, "Los elementos están en la posición esperada", elementos_en_posicion_esperada == elementos_cargados);

  printf("\tSe destruye el árbol...\n");
  arbol_destruir(arbol);

  printf("\tRecorremos el array de elementos insertados para verificar el uso del destructor...\n");
  do {
    destructor_correcto = cosas[l].borrado;
    l++;
  } while(destructor_correcto == 1 && l < MAX_COSAS);

  tester_afirmar(tester, "Todos los elementos se borraron una sola vez", destructor_correcto == 1);
  if(destructor_correcto != 1)
    printf("Error en la iteración %li\n", l);

}

int main(){
    tester_t* tester = tester_crear();

    int enteros_orden_insercion[ELEMENTOS_ARBOL_PRUEBA] = { 50, 30, 70, 10, 40, 20, 5, 60, 80, 90, 35, 45, 55, 65, 75, 25, 42, 77, 27, 22, 53, 57, 56 };
    //El árbol que se genera al introducir los números de enteros_orden_insercion es el que se ve en la imagen ABB.jpg

    int enteros_inorden[ELEMENTOS_ARBOL_PRUEBA] = { 5, 10, 20, 22, 25, 27, 30, 35, 40, 42, 45, 50, 53, 55, 56, 57, 60, 65, 70, 75, 77, 80, 90 };
    int enteros_preorden[ELEMENTOS_ARBOL_PRUEBA] = { 50, 30, 10, 5, 20, 25, 22, 27, 40, 35, 45, 42, 70, 60, 55, 53, 57, 56, 65, 80, 75, 77, 90 };
    int enteros_postorden[ELEMENTOS_ARBOL_PRUEBA] = { 5, 22, 27, 25, 20, 10, 35, 42, 45, 40, 30, 53, 56, 57, 55, 65, 60, 77, 75, 90, 80, 70, 50 };
    int* elementos_arbol[4] = { enteros_orden_insercion, enteros_inorden, enteros_preorden, enteros_postorden };

    printf("PRUEBAS CREAR\n");
    probar_crear(tester, elementos_arbol);

    printf("\nPROBAR RAIZ\n");
    probar_raiz(tester, elementos_arbol);

    printf("\nPROBAR ÁRBOL VACÍO\n");
    probar_arbol_vacio(tester, elementos_arbol);

    printf("\nPRUEBAS INSERTAR\n");
    probar_insertar(tester, elementos_arbol);

    printf("\nPRUEBAS BORRAR\n");
    probar_borrar(tester, elementos_arbol);

    printf("\nPRUEBAS BUSCAR\n");
    probar_buscar(tester, elementos_arbol);

    printf("\nPROBAR RECORRIDOS COMPLETOS\n");
    probar_recorridos(tester, elementos_arbol, ELEMENTOS_ARBOL_PRUEBA);

    printf("\nPROBAR RECORRIDOS INCOMPLETOS\n");
    probar_recorridos(tester, elementos_arbol, 10);

    printf("\nPRUEBAS ITERADOR\n");
    probar_iterador(tester, elementos_arbol);

    printf("\nPRUEBAS DESTRUCTOR\n");
    probar_destructor(tester, elementos_arbol);

    printf("\nPRUEBAS MIXTAS\n");
    probar_integracion(tester);

    printf("\n\n");

    tester_mostrar_estadisticas(tester);
    tester_destruir(tester);
    return 0;
}
