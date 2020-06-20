SECCIÓN 1: COMPILACIÓN Y EJECUCIÓN

  Lo entregado es un  tipo  de  dato  abstracto  (TDA).  En particular, un árbol
binario  de  búsqueda  (ABB).  Para  compilarlo  debemos  abrir  una  terminal y
posicionarnos  en  el  directorio  en  que  el  que  se encuentran los archivos:
abb_test.c abb.c  abb.h  ansicolor.h  tester.c tester.h e introducir el comando:

  $ gcc abb.c abb_test.c tester.c -O0 -g -Wtype-limits -Wall -std=c99 -Wconversion -pedantic -Werror -o < nombre_ejecutable >

  Donde  < nombre_ejecutable >  es  el  nombre  que  el usuario le quiera dar al
binario ejecutable. Luego de la compilación,  podemos ejecutar el programa. Para
ello basta con correr en la terminal (en el directorio en el que se encuentra el
ejecutable) el comando

  $ ./< nombre_ejecutable >

  Sin  embargo  también  lo podemos correr con Valgrind, un programa que realiza
chequeos de memoria para revisar  que  el  programa  no  pierda memoria ni tenga
errores al manejar esta. Para ello, en el directorio que se encuentra nuestro
ejecutable debemos correr el comando

  $ valgrind --track-origins=yes --leak-check=full --show-reachable=yes ./< nombre_ejecutable >



SECCIÓN 2: ÁRBOLES BINARIOS VS ABB


  ¿Qué es un ABB?
    Un  ABB  es  una estructura  de  datos  pensada  en  reducir  la complejidad
  algorítmica de operaciones tales como la inserción  de  nueva  información, su
  borrado y su búsqueda. Se organiza de  modo  tal  que  cada elemento del árbol
  tiene la información que guarda y a  su vez dos referencias: una a un elemento
  de clave menor (generalmente a su  izquierda, aunque no es obligatorio que sea
  de esta forma) y otro de  clave  mayor. Esto se cumple  para cada elemento del
  árbol excepto para las hojas,  que son  los últimos  elementos de cada "rama".
  Otra forma de verlo es  pensando que cada elemento del árbol es la información
  que este contiene  y la  referencia a dos sub-árboles: uno en el que todas las
  claves  de sus elementos son menores a la del elemento que tiene su referencia
  y otro  sub-árbol  en  el que son mayores. Cabe destacar que hay elementos con
  una sola  referencia.  Esto cumple  con  la definición árbol binario ya que la
  restricción  de  referencias (o hijos)  es  de una cantidad máxima (2). Por lo
  tanto cualquier cantidad de hijos igual o menor cumple con la definición.


  ¿Cuál es la diferencia con un árbol binario?

    La diferencia entre un ABB  y un árbol  binario es que más allá de que en un
  ABB los elementos con clave menor pueden  ir a la izquierda o la derecha según
  lo  defina  quien  lo implemente  (siempre y cuando  mayores vayan a un lado y
  menores a  otro), no da igual la relación entre las claves. Existe un criterio
  que nos indica cómo debemos operar con cada elemento. En un árbol binario este
  tipo  de reglas  no   existe. La  única  restricción  que  tenemos es que cada
  elemento puede  tener hasta 2 hijos. En  síntesis,  los  árboles  binarios  de
  búsqueda son un  sub-grupo  de  árboles  binarios que además de cumplir con la
  restricción de la cantidad de hijos que cada  elemento puede tener, cumple con
  otra restricción: todas las claves de cada  sub-árbol al que un elemento tiene
  referencia deben ser o todas mayores o todas menores y no se puede repetir. Es
  decir,  no  puede  tener  dos  referencias  distintas de sub-árboles con todas
  claves menores o todas claves mayores.


  Complejidad algorítmica

    Fundamentalmente  tenemos  que  hablar  de  la  inserción,  el  borrado y la
  búsqueda. El caso de los recorridos es trivial ya  que  necesariamente el peor
  de los casos siempre es  recorrer todos  los elementos  por lo tanto son O(n).
  Acceder a la raiz también es trivial ya que es O(1), pues tenemos acceso directo.
  En los iteradores,  el  peor  caso  es  recorrer  todo  el árbol, por lo tanto
  estamos en el mismo caso que los recorridos.
    Sin embargo, a la hora de pensar en la  inserción, el  borrado y la búsqueda
  no podemos perder de vista que no solo es  importante el algoritmo que vayamos
  a utilizar, sino que también  lo es  el orden  en el que nos llegan los datos.
  Cabe  la  posibilidad  de  que en un  árbol  nos  lleguen los  datos de  forma
  ordenada. Este es el peor de los casos, ya que  todos los datos se insertarían
  de modo  que todos  los elementos  tengan solo  un  hijo. En este caso estamos
   hablando de  un árbol  que  degeneró en  lista, pero   sin la  posibilidad de
  tener un puntero al último elemento. Por lo tanto, las operaciones de insertar,
  borrar y buscar son O(n). Esto  se debe  a que  para insertar siempre  debemos
  hacerlo como hoja, para ello debemos recorrer los n elementos. Para borrar, el
  peor  caso  es  querer  borrar  el  último elemento. Para  llegar a él debemos
  recorrer los n elementos anteriores. Para buscar, también  el peor caso es que
  el elemento buscado sea el  último  y  debamos recorrer los  n  elementos para
  hallarlo.
    Si bien, por  lo explicado  arriba,  los algoritmos  son O(n), es importante
  notar  que el  caso de los  elementos insertados  en orden  no  es el promedio
  y por  lo tanto es  interesante estudiar  un  "caso feliz", en el que seguimos
  considerando  el  peor caso  de nuestro  algoritmo pero  en un  árbol que está
  equilibrado.  En  este  caso, siempre  hablamos de  O(log n). ¿Por qué?  vamos
  a analizarlo con el teorema maestro.

    T(n) = a(n/b) + f(n) -> forma general

    a es la cantidad de llamados recursivos. b es una proporción del problema de
  tamañon n y f(n) es el costo de todo  lo que  no son  llamados recursivos.  Si
  analizamos  el  algoritmo de  inserción, si bien en la estructura se ve más de
  un llamado recursivo,  siempre se va a ejecutar  uno o ninguno.  No existe  el
  caso en el que en una inserción cada contexto  ejecute más de un llamado.  Por
  lo tanto,  a = 1. Cada  vez que  hacemos el  llamado recursivo, descartamos un
  sub-árbol,  así que de  dos sub-árboles  que teníamos  nos quedamos   con uno.
  Dividimos el problema a la mitad. Entonces, b = 2. Además del llamado recursivo,
  ejecutamos comparaciones para saber qué sub-árbol descartar o la  inserción en
  sí  misma. La  inserción  en sí  es O(1)  y se  ejecuta una  única vez  en  el
  algoritmo,  ya que al  insertar el  algoritmo termina. Por lo tanto, para f(n)
  debemos considerar la comparación.  Esto  no es  un caso trivial para un árbol
  genérico  como  el presentado. Esto  se  debe a que la  función de comparación
  es provista  por  el  usuario y  no por quien  implementa  el árbol. Entonces,
  nuestra ecuación queda de la forma:

    T(n) = (n/2) + f(n)

    Entonces, procedemos a comparar  f(n) contra  n ^ log2(1) = 1. Por lo tanto,
  la  comparación  queda  f(n)  contra 1.  Si f(n), es  decir,  la   función  de
  comparación es constante, entonces  estamos ante  el segundo caso  del teorema
  maestro. Esto nos devuelve que

      T(n) = O(n^logb(a)) log(n)) => T(n) = log(n).

    Cabe  la   posibilidad  de  que f(n) = n².  De  este modo,  nos  encontramos
  frente al caso 3. Ya que Big-Omega(f(n)) = n^logb(a)+e, con e = 1 y af(n/b) =
  f(n/b) = (n/2)² = cf(n) para c = 1/4. Entonces, tenemos que

    T(n) = O(f(n)) = O(n²)

    Por lo tanto, concluimos que la inserción en un arbol binario de búsqueda(en
  un árbol equilibrado)  depende  de la  función de  comparación provista por el
  usuario.  Si  esta no  es constante,  entonces T(n)  será igual  a f(n). Si es
  constante, T(n) es igual a log(n).

    El razonamiento para la  búsqueda  es  el mismo, ya que a, b y f(n) valen lo
  mismo. Por lo tanto, el resultado al que llegamos no cambia.
    Donde sí tenemos que hacer una salvedad es en el borrado. Esto se debe a que
  el peor caso del  borrado tiene  instrucciones extra además de la búsqueda del
  elemento  a  borrar y su eliminación  en sí. Si el elemento que buscamos tiene
  dos hijos,  debemos buscar al predecesor  inorden  (es lo que elegimos en esta
  implementación,  también  podría ser  el sucesor  inorden,  no cambia  nada en
  términos  de complejidad algorítmica) en  el árbol, y esto tiene un coste.
    Determinar  el  coste  de  esta operación  a  partir de la cantidad de nodos
  requiere seguir  una  cadena  de  razonamientos  para  conseguir la altura del
  árbol. Dado un  árbol de n nodos, equilibrado y  en el  que  todas  las  hojas
  tienen dos hijos o ninguno, la cantidad de  hojas  es  igual  a la cantidad de
  nodos que no son hoja + 1. De este modo, dados n elementos en un  árbol de las
  características que se  describieron sabemos  que su  altura es  (n+1)/2. Para
  obtener la altura  a  partir de la cantidad de hojas, debemos aplicarle a esta
  cantidad el logaritmo en base 2.
    Entonces, ya  sabiendo la altura en función de la cantidad de nodos, podemos
  hablar  de la complejidad  algorítmica de buscar el predecesor inorden. Buscar
  el predecesor inorden es desplazarse  a la izquierda del nodo a borrar y luego
  a la derecha hasta que ya no se pueda, bajando por log(n) - 1 nodos.
    En un árbol de las características mencionadas, tenemos 2 posibilidades para
  el peor caso: borrar la raíz y  borrar un nodo de altura 0. Al borrar la raíz,
  la búsqueda  del  nodo  a  borrar es  O(1) ya  que tenemos acceso directo, sin
  embargo, hay que buscar el predecesor inorden, realizando una operación O(log n).
  El otro peor caso es borrar un nodo hoja,  en este caso no es necesario buscar
  el  predecesor  inorden, sin  embargo  nos  encontramos  ante  el peor caso de
  búsqueda en un  ABB, por lo  que  nuevamente  la  complejidad es O(n). Cabe la
  de que en un  árbol  de  estas  características  borrar cualquier nodo con dos
  hijos nos  significa reccorrer  log(n) nodos, por lo tanto cualquiera de estos
  puede tomarse como peor caso.
