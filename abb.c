#include <stdlib.h>
#include <stdbool.h>

#define ERROR -1
#define EXITO 0
#define ELEMENTOS_IGUALES 0
#define SEGUNDO_MAYOR -1
#define SEGUNDO_MENOR 1
#define ABB_RECORRER_INORDEN   0
#define ABB_RECORRER_PREORDEN  1
#define ABB_RECORRER_POSTORDEN 2


typedef int (*abb_comparador)(void*, void*);

typedef void (*abb_liberar_elemento)(void*);


typedef struct nodo_abb {
	void* elemento;
	struct nodo_abb* izquierda;
	struct nodo_abb* derecha;
} nodo_abb_t;

typedef struct abb{
	nodo_abb_t* nodo_raiz;
	abb_comparador comparador;
	abb_liberar_elemento destructor;
} abb_t;

/*******************************************************************************/
/************************   FUNCIONES PRIVADAS   *******************************/
/*******************************************************************************/

/*
 *	Recibe un nodo y devuelve su cantidad de hijos
 */
int cantidad_hijos(nodo_abb_t* nodo){
	int contador = 0;

	if(nodo -> derecha)
		contador++;

	if(nodo -> izquierda)
		contador++;

	return contador;
}

/*
 *	Recibe un nodo raíz e inserta de forma recursiva el elemento en la posición
 * correspondiente, valiéndose del comparador. Devuelve el nodo raíz.
 */
nodo_abb_t* nodo_insertar(nodo_abb_t* nodo_raiz, abb_comparador comparador, void* elemento){

  if(!nodo_raiz){
    nodo_raiz = calloc(1, sizeof(nodo_abb_t));
    if(!nodo_raiz)
      return NULL;

    nodo_raiz -> elemento = elemento;
    nodo_raiz -> izquierda = NULL;
    nodo_raiz -> derecha = NULL;
  }

  else if(comparador(nodo_raiz -> elemento, elemento) == SEGUNDO_MENOR)
    nodo_raiz -> izquierda = nodo_insertar(nodo_raiz -> izquierda, comparador, elemento);

  else
    nodo_raiz -> derecha = nodo_insertar(nodo_raiz -> derecha, comparador, elemento);

  return nodo_raiz;
}


/*
 *	Recibe un nodo raiz,  carga en array el elemento al que apunta el nodo raiz
 * y aumenta en uno la variable elementos_cargados y la posición que representa *array.
 */
void cargar_elemento(nodo_abb_t* nodo_raiz, void*** array, int* elementos_cargados){
	if(!nodo_raiz)
		return;

	**array = nodo_raiz -> elemento;
	(*array)++;
	(*elementos_cargados)++;
}

/*
 *	El parámetro recorrido solo admite las constantes ABB_RECORRER_INORDEN, ABB_RECORRER_PREORDEN
 * ABB_RECORRER_POSTORDEN. Si se pasa otro número, el compartamiento es indeterminado.
 * Carga en *array la cantidad de elementos tamanio_array del árbol que tiene la referencia a
 * nodo_raiz en el orden introducido en recorrido.
 */
void recorrer_nodos(nodo_abb_t* nodo_raiz, int recorrido, int* elementos_cargados, void*** array, int tamanio_array){

	if(nodo_raiz == NULL)
		return;

	if(*elementos_cargados >= tamanio_array)
		return;

	nodo_abb_t* raiz_e_hijos[3];

	if(recorrido == ABB_RECORRER_INORDEN){
		raiz_e_hijos[0] = nodo_raiz -> izquierda;
		raiz_e_hijos[1] = nodo_raiz;
		raiz_e_hijos[2] = nodo_raiz -> derecha;
	}

	else if(recorrido == ABB_RECORRER_PREORDEN){
		raiz_e_hijos[0] = nodo_raiz;
		raiz_e_hijos[1] = nodo_raiz -> izquierda;
		raiz_e_hijos[2] = nodo_raiz -> derecha;
	}

	else if(recorrido == ABB_RECORRER_POSTORDEN){
		raiz_e_hijos[0] = nodo_raiz -> izquierda;
		raiz_e_hijos[1] = nodo_raiz -> derecha;
		raiz_e_hijos[2] = nodo_raiz;
	}

	/*
	 *En este punto raiz_e_hijos tiene guardado al nodo raiz y sus hijos en el
	 *orden que se deben recorrer según el recorrido pedido
	 */

	(raiz_e_hijos[0] != nodo_raiz) ?
		recorrer_nodos(raiz_e_hijos[0], recorrido, elementos_cargados, array, tamanio_array) :
		cargar_elemento(raiz_e_hijos[0], array, elementos_cargados);

	if(*elementos_cargados >= tamanio_array) return;

	(raiz_e_hijos[1] != nodo_raiz) ?
			recorrer_nodos(raiz_e_hijos[1], recorrido, elementos_cargados, array, tamanio_array) :
			cargar_elemento(raiz_e_hijos[1], array, elementos_cargados);

	if(*elementos_cargados >= tamanio_array) return;

	(raiz_e_hijos[2] != nodo_raiz) ?
		recorrer_nodos(raiz_e_hijos[2], recorrido, elementos_cargados, array, tamanio_array) :
		cargar_elemento(raiz_e_hijos[2], array, elementos_cargados);
}

/*
 *	Compara a nodo_raiz con elemento valiéndose del comparador. Si son iguales,
 * devuelve el elemento al que apunta nodo_raiz, sino se llama a sí misma con
 * nodo_raiz -> izquierda o nodo_raiz -> derecha según corresponda.
 */
nodo_abb_t* arbol_buscar_nodo(nodo_abb_t* nodo_raiz, void* elemento, abb_comparador comparador){

	void* ret = NULL;

	if(!nodo_raiz)
		ret = NULL;

	else if(comparador(nodo_raiz -> elemento, elemento) == SEGUNDO_MAYOR){
		ret = arbol_buscar_nodo(nodo_raiz -> derecha, elemento, comparador);
	}

	else if(comparador(nodo_raiz -> elemento, elemento) == SEGUNDO_MENOR){
		ret = arbol_buscar_nodo(nodo_raiz -> izquierda, elemento, comparador);
	}
	else if(comparador(nodo_raiz -> elemento, elemento) == ELEMENTOS_IGUALES)
		ret = nodo_raiz;

	return ret;
}

/*
 *	Recibe el hijo izquierdo del nodo al que se le quiere encontrar el predecesor
 * inorden. El padre del predecesor inorden queda apuntando al hijo del predecesor_inorden
 * o a NULL según corresponda. Devuelve al predecesor inorden
 */
nodo_abb_t* buscar_predecesor_inorden(nodo_abb_t* nodo_raiz){
	nodo_abb_t* predecesor_inorden = NULL;
	nodo_abb_t* proximo_nodo;

	if(!(nodo_raiz -> derecha))
		return nodo_raiz;

	else if(!(nodo_raiz -> derecha -> derecha)){
		//Guardo la referencia al predecesor inorden
		predecesor_inorden = nodo_raiz -> derecha;
		//Salvo la referencia del hijo del predecesor_inorden
		nodo_raiz -> derecha = predecesor_inorden -> izquierda;
	}

	proximo_nodo = predecesor_inorden ? predecesor_inorden : nodo_raiz -> derecha;

	return buscar_predecesor_inorden(proximo_nodo);
}

/*
 *	El nodo raíz recibido debe pertenecer al árbol recibido. Busca a elemento
 * a partir del nodo_raiz mediante la función arbol -> comparador y llamados recursivos.
 * Si el elemento al que apunta nodo_raiz es ditinto de elemento, devuelve el nodo_raiz,
 * si son iguales, borra al nodo_raiz del árbol y devuelve al nodo que lo debe reemplazar
 * en la estructura.
 */
nodo_abb_t* arbol_borrar_rec(nodo_abb_t* nodo_raiz, void* elemento, abb_t* arbol, bool* pudo_borrar){
	if(!nodo_raiz){
		*pudo_borrar = false;
		return NULL;
	}
	nodo_abb_t* hijo = NULL;

	if(arbol -> comparador(nodo_raiz -> elemento, elemento) == ELEMENTOS_IGUALES && cantidad_hijos(nodo_raiz) != 2){

		if(nodo_raiz -> izquierda)
			hijo = nodo_raiz -> izquierda;

		else if(nodo_raiz -> derecha)
			hijo = nodo_raiz -> derecha;

		if(arbol -> destructor)
			arbol -> destructor(nodo_raiz -> elemento);
		free(nodo_raiz);

		*pudo_borrar = true;
		return hijo;
	}

	else if(arbol -> comparador(nodo_raiz -> elemento, elemento) == ELEMENTOS_IGUALES && cantidad_hijos(nodo_raiz) == 2){
		nodo_abb_t* predecesor_inorden = buscar_predecesor_inorden(nodo_raiz -> izquierda);

		predecesor_inorden -> derecha = nodo_raiz -> derecha;
		if(predecesor_inorden != nodo_raiz -> izquierda)
			predecesor_inorden -> izquierda = nodo_raiz -> izquierda;

		if(arbol -> destructor)
			arbol -> destructor(nodo_raiz -> elemento);
		free(nodo_raiz);

		*pudo_borrar = true;
		return predecesor_inorden;
	}

	else if(arbol -> comparador(nodo_raiz -> elemento, elemento) == SEGUNDO_MENOR){
		nodo_raiz -> izquierda = arbol_borrar_rec(nodo_raiz -> izquierda, elemento, arbol, pudo_borrar);
		return nodo_raiz;
	}

	else if(arbol -> comparador(nodo_raiz -> elemento, elemento) == SEGUNDO_MAYOR){
		nodo_raiz -> derecha = arbol_borrar_rec(nodo_raiz -> derecha, elemento, arbol, pudo_borrar);
		return nodo_raiz;
	}
	return nodo_raiz;
}


/*
 *	Los únicos recorridos posibles son ABB_RECORRER_INORDEN, ABB_RECORRER_PREORDEN
 * y ABB_RECORRER_POSTORDEN. Recorre el árbol cuya raíz es nodo_raiz y aplica la
 * funcion con cada nodo, pasándo nodo_raiz -> elemento como primer parámetro y extra
 * como segundo.
 */
bool abb_con_cada_nodo(nodo_abb_t* nodo_raiz, int recorrido, bool (*funcion)(void*, void*), void* extra){
	bool hay_que_parar = false;
	if(!nodo_raiz)
	return false;

	if(recorrido == ABB_RECORRER_INORDEN){
		if(!hay_que_parar)
			hay_que_parar = abb_con_cada_nodo(nodo_raiz -> izquierda, recorrido, funcion, extra);

		if(!hay_que_parar)
			hay_que_parar = funcion(nodo_raiz -> elemento, extra);

		if(!hay_que_parar)
			hay_que_parar = abb_con_cada_nodo(nodo_raiz -> derecha, recorrido, funcion, extra);

	}
	else if(recorrido == ABB_RECORRER_PREORDEN){
		if(!hay_que_parar)
			hay_que_parar = funcion(nodo_raiz -> elemento, extra);

		if(!hay_que_parar)
			hay_que_parar = abb_con_cada_nodo(nodo_raiz -> izquierda, recorrido, funcion, extra);

		if(!hay_que_parar)
			hay_que_parar = abb_con_cada_nodo(nodo_raiz -> derecha, recorrido, funcion, extra);

	}
	else if(recorrido == ABB_RECORRER_POSTORDEN){
		if(!hay_que_parar)
			hay_que_parar = abb_con_cada_nodo(nodo_raiz -> izquierda, recorrido, funcion, extra);

		if(!hay_que_parar)
			hay_que_parar = abb_con_cada_nodo(nodo_raiz -> derecha, recorrido, funcion, extra);

		if(!hay_que_parar)
			hay_que_parar = funcion(nodo_raiz -> elemento, extra);

	}
	else
	return false;

	return hay_que_parar;
}

/*
 *	Libera la memoria reservada para el árbol cuya raíz es nodo_raiz. Adicionalmente
 * invoca al destructor para cada elemento almacenado en el árbol si es que existe.
 */
void arbol_destruir_rec(nodo_abb_t* nodo_raiz, abb_liberar_elemento destructor){

	if(nodo_raiz -> izquierda)
		arbol_destruir_rec(nodo_raiz -> izquierda, destructor);

	if(nodo_raiz -> derecha)
		arbol_destruir_rec(nodo_raiz -> derecha, destructor);

	if(destructor)
		destructor(nodo_raiz -> elemento);
	free(nodo_raiz);
}

/*******************************************************************************/
/************************   FUNCIONES PÚBLICAS   *******************************/
/*******************************************************************************/

bool arbol_vacio(abb_t* arbol){
  if(!arbol)
    return true;

  if(!(arbol -> nodo_raiz))
    return true;

  return false;
}

abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){

  if(!comparador)
    return NULL;

  abb_t* abb = calloc(1, sizeof(abb_t));

  if(!abb)
    return NULL;

  abb -> destructor =  destructor;
  abb -> comparador = comparador;

  return abb;
}

int arbol_insertar(abb_t* arbol, void* elemento){
  if(!arbol)
    return ERROR;

	arbol -> nodo_raiz = nodo_insertar(arbol -> nodo_raiz, arbol -> comparador, elemento);

  return EXITO;
}

int arbol_borrar(abb_t* arbol, void* elemento){
	if(!arbol)
		return ERROR;
	bool pudo_borrar = true;
	arbol -> nodo_raiz = arbol_borrar_rec(arbol -> nodo_raiz, elemento, arbol, &pudo_borrar);

	return pudo_borrar ? EXITO : ERROR;
}

void* arbol_buscar(abb_t* arbol, void* elemento){
  if(arbol_vacio(arbol))
    return NULL;

	nodo_abb_t* nodo_a_ret = NULL;
	nodo_a_ret = arbol_buscar_nodo(arbol -> nodo_raiz, elemento, arbol -> comparador);

  return nodo_a_ret ? nodo_a_ret -> elemento : NULL;
}

void* arbol_raiz(abb_t* arbol){
	void* elemento = NULL;

	if(!arbol)
		return NULL;

	else if(arbol -> nodo_raiz)
		elemento = arbol -> nodo_raiz -> elemento;

	return elemento;
}

int arbol_recorrido_inorden(abb_t* arbol, void** array, int tamanio_array){
  if(arbol_vacio(arbol))
    return 0;

	int elementos_cargados = 0;

	recorrer_nodos(arbol -> nodo_raiz, ABB_RECORRER_INORDEN, &elementos_cargados, &array, tamanio_array);

	return elementos_cargados;
}

int arbol_recorrido_preorden(abb_t* arbol, void** array, int tamanio_array){
	if(arbol_vacio(arbol))
		return 0;

	int elementos_cargados = 0;

	recorrer_nodos(arbol -> nodo_raiz, ABB_RECORRER_PREORDEN, &elementos_cargados, &array, tamanio_array);

	return elementos_cargados;
}

int arbol_recorrido_postorden(abb_t* arbol, void** array, int tamanio_array){
	if(arbol_vacio(arbol))
		return 0;

	int elementos_cargados = 0;

	recorrer_nodos(arbol -> nodo_raiz, ABB_RECORRER_POSTORDEN, &elementos_cargados, &array, tamanio_array);

	return elementos_cargados;
}

void arbol_destruir(abb_t* arbol){
  if(!arbol)
    return;

  if(!arbol_vacio(arbol))
		arbol_destruir_rec(arbol -> nodo_raiz, arbol -> destructor);

  free(arbol);
}

void abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){
	if(!arbol || !funcion)
		return;

	abb_con_cada_nodo(arbol -> nodo_raiz, recorrido, funcion, extra);
}
