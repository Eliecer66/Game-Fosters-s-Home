#include <stdio.h>
#include <stdbool.h>
#include "la_sombra_de_mac.h"
#include <stdlib.h>
#include <time.h>
#include "utiles.h"
#define ARRIBA 'W'
#define IZQUIERDA 'A'
#define ABAJO 'S'
#define DERECHA 'D'
const char VIDA = 'V';
const char PAREDES = 'X';
const char MAC = 'M';
const char BLOO = 'B';
const char PUERTA = 'D'; 
const char LLAVE = 'L'; 
const char MONEDAS = 'C'; 
const char POZOS = 'W' ; 
const char INTERRUPTORES = 'O'; 
const char PORTALES = 'P'; 
const char VELAS = 'V'; 
const char ESCALERAS = 'E'; 
const int COSTO_VIDA = 50;
const int MIN_PUNTOS = 200;
const int MAC_VIDAS = 3;
const int UNFINDED = -1;
const int NIVEL_UNO = 1;
const int NIVEL_DOS = 2;
const int NIVEL_TRES = 3;
const int INDEX_N1 = 0;
const int INDEX_N2 = 1;
const int INDEX_N3 = 2;
const int VELAS_NI = 5;
const int VELAS_NII = 10;
const int VELAS_NIII = 12;
const int POZOS_NI = 15;
const int POZOS_NII = 20;
const int POZOS_NIII = 30;
const int INTERRUPTORES_NI = 1;
const int INTERRUPTORES_NII = 2;
const int INTERRUPTORES_NIII = 4;
const int PORTALES_NII = 2; 
const int PORTALES_NIII = 4; 
const int ESCALERAS_NI = 10;
const int ESCALERAS_NII = 15;
const int ESCALERAS_NIII = 15;
const int MONEDAS_NI = 10;
const int MONEDAS_NII = 15;
const int MONEDAS_NIII = 15;
const int LLAVE_NI = 0;
const int LLAVE_NII = 1;
const int LLAVE_NIII = 1;
const int PUERTA_NIVEL = 1;
const int NO_ENCONTRADO = -1;
const int JUGANDO = 0;
const int PERDIDO = -1;
const int GANADO = 1;
const int DISTANCIA_MANHATTAN = 1;
const int MAX_VIDAS = 3;
const int V_INICIO = 1;
const int V_TOPE= 2;


/*
 *PRE-CONDICIONES: Recibe por parametro, dos enteros usados como la amplitud maxima para calcular numeros aleatorios.
 *POST-CONDICIONES: Retornar una estructura formada por dos variables de tipo entero cargadas con numeros aleatorios.
 */
coordenada_t coordenada_aleatoria(int max_alto, int max_ancho) {
	coordenada_t coordenada_aleatoria;
	coordenada_aleatoria.fila = rand() % max_alto;
	coordenada_aleatoria.col = rand() % max_ancho;
	return coordenada_aleatoria;
}

/*
 *PRE-CONDICIONES: Recibe dos estructuras por parametro previamente cargadas.
 *POST-CONDICIONES: Devuelve true si las estructuras son iguales y false de lo contrario.
 */
bool coordenadas_iguales(coordenada_t a, coordenada_t b) {
	return (a.fila == b.fila && a.col == b.col);
}

/*
 *PRE-CONDICIONES: Recibe un vector vacio y su tope.
 *POST-CONDICIONES: LLena el vector con todas las posiciones posibles que puede tener el terreno.
 */
void inicializar_posiciones(coordenada_t random_list[MAX_ELEMENTOS], int tope_elementos) {
	int i = 0;
	for (int j = 0; j < MAX_FILAS; j++) {
		for (int k = 0; k < MAX_COLUMNAS; k++) {
			if (i < tope_elementos) {
				random_list[i].fila = j; 
				random_list[i].col = k;
				i++;
			}
		}
	}
}

/*
 *PRE-CONDICIONES: Recibe un vector de coordenadas el cual contiene coordenadas disponibles, recibe el tope del vector, y la coordenada que se esta buscando 
 *POST-CONDICIONES: Devuelve el indice del vector donde se encuentra la coordenada buscada. de lo contrario devuelve -1. 
 */
int index_personajes(coordenada_t random_list[MAX_ELEMENTOS], int tope_elementos, coordenada_t posicion) {
	int index_buscado = NO_ENCONTRADO;
	for (int i = 0; i < tope_elementos; i++) {
		if (coordenadas_iguales(random_list[i], posicion)) {
			index_buscado = i;
		}
	}
	return index_buscado;
}
/*
 *PRE-CONDICIONES: Recibe por parametro un vector de coordenadas previamente una posicion de vector y el tope del vector.
 *POST-CONDICIONES: Elimina el e elemento que se encuentra en la posicion de vector que se recivio y resta uno al tope del vector.
 */
void eliminar_elemento(coordenada_t random_list[MAX_ELEMENTOS], int index, int* tope_elementos) {
	for (int i = index; i < (*tope_elementos); i++) {
		random_list[i] = random_list[i + 1];
	}
	(*tope_elementos)--;
}

/*
 *PRE-CONDICIONES: Recibe por referencia un struct y por valor dos enteros.
 *POST-CONDICIONES: Inicializa el parametro pasado por referencia. 
 */
void generar_coordenada_personaje(coordenada_t* arranque_personaje, int max_alto, int max_ancho, coordenada_t random_list[MAX_ELEMENTOS], int* tope_elementos) {
	int index = 0;
	coordenada_t auxiliar;
	int index_aux = 0;
	do {
		(*arranque_personaje) = coordenada_aleatoria(max_alto, max_ancho);
		index = index_personajes(random_list, (*tope_elementos), (*arranque_personaje));
		auxiliar.fila = (*arranque_personaje).fila;
		auxiliar.col = (max_ancho - ((*arranque_personaje).col) - 1);
		index_aux = index_personajes(random_list, (*tope_elementos), auxiliar);
	}while(index == NO_ENCONTRADO || index_aux == NO_ENCONTRADO);
	eliminar_elemento(random_list, index, &(*tope_elementos));  
}

/*
 *PRE-CONDICIONES: Recibe un structs pasado por referencia y uno por valor y un entero
 *POST-CONDICIONES: Inicializa el struct pasado por referencia con la informacion que tenemos en el struct llamado arranque personaje.
 */
void generar_coordenada_sombra(coordenada_t* arranque_sombra, coordenada_t arranque_personaje, int max_ancho, coordenada_t random_list[MAX_ELEMENTOS], int* tope_elementos) {
	(*arranque_sombra).fila = arranque_personaje.fila;
	(*arranque_sombra).col = (max_ancho - arranque_personaje.col - 1);
	int index = index_personajes(random_list, (*tope_elementos),(*arranque_sombra));
	eliminar_elemento(random_list, index, &(*tope_elementos));
}

/*
 *PRE-CONDICIONES: Recibe por parametro un struct pasado por referencia y uno pasado por valor el cual tiene informacion para inicializar la posicion del personaje.
 *POST-CONDICIONES: Inicializa el struct(personaeje) pasado por referencia.
 */
void inicializar_personaje(personaje_t* ref_personaje, coordenada_t arranque_personaje) {
	(*ref_personaje).vida = MAC_VIDAS;
	(*ref_personaje).puntos = 0;
	(*ref_personaje).tiene_llave = false;
	(*ref_personaje).interruptor_apretado = false;
	(*ref_personaje).posicion.fila = arranque_personaje.fila;
	(*ref_personaje).posicion.col = arranque_personaje.col;
}

/*
 *PRE-CONDICIONES: Recibe por parametro un struct pasado por referencia y uno pasado por valor el cual tiene informacion para inicializar la posicion de la sombra.
 *POST-CONDICIONES: Inicializa el struct(sombra) pasado por referencia.
 */
void inicializar_sombra(sombra_t* ref_sombra, coordenada_t arranque_sombra) {
	(*ref_sombra).esta_viva = true;
	(*ref_sombra).posicion.fila = arranque_sombra.fila;
	(*ref_sombra).posicion.col = arranque_sombra.col;
}

/*
 *PRE-CONDICIONES: Recibe un vector de tipo struct y su tope vacios.
 *POST-CONDICIONES: Inicializa el vector con la informacion de cantidad de obstaculos por nivel.
 */
void inicializar_valores_obstaculos(obstaculos_nivel_t valores_obstaculos[MAX_ELEMENTOS], int* tope_valores) {
	for (int i = 0; i < (*tope_valores); i++) {
		if (i == INDEX_N1) {
			valores_obstaculos[i].velas = VELAS_NI;
			valores_obstaculos[i].pozos = POZOS_NI;
			valores_obstaculos[i].interruptores = INTERRUPTORES_NI;
			valores_obstaculos[i].portales = 0;
			(*tope_valores)++;
		}
		if (i == INDEX_N2) {
			valores_obstaculos[i].velas = VELAS_NII;
			valores_obstaculos[i].pozos = POZOS_NII;
			valores_obstaculos[i].interruptores = INTERRUPTORES_NII;
			valores_obstaculos[i].portales = PORTALES_NII;
			(*tope_valores)++;
		}
		if (i == INDEX_N3) {
			valores_obstaculos[i].velas = VELAS_NIII;
			valores_obstaculos[i].pozos = POZOS_NIII;
			valores_obstaculos[i].interruptores = INTERRUPTORES_NIII;
			valores_obstaculos[i].portales = PORTALES_NIII;
		}
	}
}

/*
 *PRE-CONDICIONES:Recibe un vector de tipo struct y su tope vacios.
 *POST-CONDICIONES:Inicializa el vector con la informacion de cantidad de herramientas por nivel.
 */
void inicializar_valores_herramientas(herramientas_nivel_t valores_herramientas[MAX_ELEMENTOS], int* tope_valor_herramientas) {
	for (int i = 0; i < (*tope_valor_herramientas); i++) {
		if (i == INDEX_N1) {
			valores_herramientas[i].escaleras = ESCALERAS_NI;
			valores_herramientas[i].monedas = MONEDAS_NI;
			valores_herramientas[i].llave = LLAVE_NI;
			valores_herramientas[i].puerta = PUERTA_NIVEL;
			(*tope_valor_herramientas)++;
		}
		if (i == INDEX_N2) {
			valores_herramientas[i].escaleras = ESCALERAS_NII;
			valores_herramientas[i].monedas = MONEDAS_NII;
			valores_herramientas[i].llave = LLAVE_NII;
			valores_herramientas[i].puerta = PUERTA_NIVEL;
			(*tope_valor_herramientas)++;
		}
		if (i == INDEX_N3) {
			valores_herramientas[i].escaleras = ESCALERAS_NIII;
			valores_herramientas[i].monedas = MONEDAS_NIII;
			valores_herramientas[i].llave = LLAVE_NIII;
			valores_herramientas[i].puerta = PUERTA_NIVEL;
		}
	}
}

/*
 *PRE-CONDICIONES: Recibe por parametro un vector de elementos un vector de coordenadas su tope y el indice de la posicion del vecroe de elemento.
 *POST-CONDICIONES: Asigna una coordenada valida a elemento, y elimina del vector de coordenadas la coordenada que se le asigno al elemento.
 */
void asignar_coordenadas(elemento_t elemento[MAX_ELEMENTOS], coordenada_t random_list[MAX_ELEMENTOS], int* tope_elementos, int i) {
	int index = rand() % *tope_elementos;
	elemento[i].coordenada = random_list[index];
	eliminar_elemento(random_list, index, &(*tope_elementos));
}

/*
 *PRE-CONDICIONES: Recibe un struct del nivel que se quiere inicializar, el vector que contiene la cantidad de obstaculos correspondientes al nivel, su tope, el vector que contiene la informacion de las coordenadas disponibles restantes y su tope. 
 *POST-CONDICIONES: Inicializa los obstaculos del nivel correspondiente, con todas sus estructuras validas.
 */
void inicializar_obstaculos(nivel_t* niveles, int tope_obstaculos, obstaculos_nivel_t valores_obstaculos[MAX_ELEMENTOS], int* tope_elementos, coordenada_t random_list[MAX_ELEMENTOS]) {
	int level = (*niveles).numero_nivel - 1;
	for (int i = 0; i < tope_obstaculos; i++) {
		if (i < valores_obstaculos[level].velas) {
			(*niveles).obstaculos[i].tipo = VELAS;
			asignar_coordenadas((*niveles).obstaculos, random_list, &(*tope_elementos), i);
		}else if( i >= valores_obstaculos[level].velas && i < valores_obstaculos[level].velas + valores_obstaculos[level].pozos) {
			(*niveles).obstaculos[i].tipo = POZOS;
			asignar_coordenadas((*niveles).obstaculos, random_list, &(*tope_elementos), i);
		}else if(i >= valores_obstaculos[level].velas + valores_obstaculos[level].pozos && i < valores_obstaculos[level].velas + valores_obstaculos[level].pozos + valores_obstaculos[level].interruptores) {
			(*niveles).obstaculos[i].tipo = INTERRUPTORES;
			asignar_coordenadas((*niveles).obstaculos, random_list, &(*tope_elementos), i);
		}else if (i >= valores_obstaculos[level].velas + valores_obstaculos[level].pozos + valores_obstaculos[level].interruptores && i < tope_obstaculos) {
			(*niveles).obstaculos[i].tipo = PORTALES;
			asignar_coordenadas((*niveles).obstaculos, random_list, &(*tope_elementos), i);
		}
	} 
}

/*
 *PRE-CONDICIONES: Recibe un struct de coordenadas un vector de coordenadas y su tope cargados con informacion para generar posiciones validas.
 *POST-CONDICIONES: Inicializa al elemento, en este caso la escalera sobre una pared aleatoriamente.
 */
void set_stairs(coordenada_t* coordenada, coordenada_t walls[MAX_ELEMENTOS], int* tope_walls) {
	int index = rand() % (*tope_walls);
	(*coordenada) = walls[index];
	eliminar_elemento(walls, index, &(*tope_walls));
}

/*
 *PRE-CONDICIONES: Recibe un struct del nivel que se quiere inicializar, el vector que contiene la cantidad de herramientas correspondientes al nivel, su tope, el vector que contiene la informacion de las coordenadas disponibles restantes y su tope. 
 *POST-CONDICIONES: Inicializa las herramientas del nivel correspondiente, con todas sus estructuras validas.
 */
void inicializar_herramientas(nivel_t* niveles, int tope_herramientas, herramientas_nivel_t valores_herramientas[MAX_ELEMENTOS], int* tope_elementos, coordenada_t random_list[MAX_ELEMENTOS], coordenada_t walls[MAX_ELEMENTOS], int* tope_walls) {
	int level = (*niveles).numero_nivel - 1;
	for (int i = 0; i < tope_herramientas; i++) {
		if (i < valores_herramientas[level].escaleras) {
			(*niveles).herramientas[i].tipo = ESCALERAS;
			set_stairs(&((*niveles).herramientas[i].coordenada), walls, &(*tope_walls));
		}else if( i >= valores_herramientas[level].escaleras && i < valores_herramientas[level].escaleras + valores_herramientas[level].monedas) {
			(*niveles).herramientas[i].tipo = MONEDAS;
			asignar_coordenadas((*niveles).herramientas, random_list, &(*tope_elementos), i);
		}else if(i >= valores_herramientas[level].escaleras + valores_herramientas[level].monedas && i < valores_herramientas[level].escaleras + valores_herramientas[level].monedas +  valores_herramientas[level].llave) {
			(*niveles).herramientas[i].tipo = LLAVE;
			asignar_coordenadas((*niveles).herramientas, random_list, &(*tope_elementos), i);
		}else if(i >= valores_herramientas[level].escaleras + valores_herramientas[level].monedas +  valores_herramientas[level].llave && i < tope_herramientas) {
			(*niveles).herramientas[i].tipo = PUERTA;
			asignar_coordenadas((*niveles).herramientas, random_list, &(*tope_elementos), i);
		}
	}
}

/*
 *PRE-CONDICIONES: Recibe la informacion previamente cargada con los datos del nivel que se esta inicializando.
 *POST-CONDICIONES: Retorna el indice del elmento que se esta buscando si lo encuentra.
 */
int buscar_elemento(coordenada_t paredes, coordenada_t random_list[MAX_ELEMENTOS], int tope_elementos) {
	int index = UNFINDED;
	for (int j = 0; j < tope_elementos; j++) {
		if (coordenadas_iguales(paredes, random_list[j])) {
			index = j;
		}
	}
	return index;
}

/*
 *PRE-CONDICIONES: Recibe un vector de coordenadas de paredes previamente inicializado y otro para inicializar  con su tope vacio.
 *POST-CONDICIONES: Inicializa el vector walls con la informacion que recivimos de paredes.
 */
void cargar_coordenadas_walls(coordenada_t paredes[MAX_PAREDES], int tope_paredes, coordenada_t walls[MAX_PAREDES], int* tope_walls) {
	for (int i = 0; i < tope_paredes; i++) {
		walls[i] = paredes[i];
		(*tope_walls)++;
	}
}

/*
 *PRE-CONDICIONES: Recibe un vector de paredes de tipo coordenada y su tope previamente inicializados y el vector de posiciones disponibles previamente cargado.
 *POST-CONDICIONES: Se elimina del vector de posiciones disponibles todas las coordenadas de las paredes que se generaron previamente.
 */
void eliminar_coordenadas_paredes(coordenada_t paredes[MAX_PAREDES],int tope_paredes, coordenada_t random_list[MAX_PAREDES], int* tope_elementos) {
	for (int i = 0; i < tope_paredes; i++) {
		int buscado = buscar_elemento(paredes[i], random_list, (*tope_elementos));
		eliminar_elemento(random_list, buscado, &(*tope_elementos));
	}
}

/*
 *PRE-CONDICIONES: Recibe el struct de obstaculos cargado con informacion del juego.
 *POST-CONDICIONES: Suma la totalidad de los elementos que contiene el struct ya que todos los campos son enteros.
 */
int max_obstaculos(obstaculos_nivel_t valores_obstaculos) {
	int total_obstaculos = (valores_obstaculos.velas + valores_obstaculos.pozos + valores_obstaculos.interruptores + valores_obstaculos.portales);
	return total_obstaculos;
}

/*
 *PRE-CONDICIONES:Recibe el struct de herramientas cargado con informacion del juego. 
 *POST-CONDICIONES:Suma la totalidad de los elementos que contiene el struct ya que todos los campos son enteros.
 */
int max_herramientas(herramientas_nivel_t valores_herramientas) {
	int total_herramientas = (valores_herramientas.escaleras + valores_herramientas.monedas + valores_herramientas.llave + valores_herramientas.puerta);
	return total_herramientas;
}

/*
 *PRE-CONDICIONES: Recibe el nivel 1 para inicializar con la informacion de obstaculos herramientas y posiciones disponibles.
 *POST-CONDICIONES: Inicializa la estructura de nivel 1 con todas las estructuras cargadas.
 */
void inicializar_nivel_i(nivel_t* niveles, obstaculos_nivel_t valores_obstaculos[MAX_ELEMENTOS], int* tope_elementos, coordenada_t random_list[MAX_ELEMENTOS], herramientas_nivel_t valores_herramientas[MAX_ELEMENTOS]) {
	coordenada_t walls[MAX_ELEMENTOS];
	int tope_walls = 0;
	(*niveles).numero_nivel = NIVEL_UNO;
	(*niveles).tope_paredes = INDEX_N1;
	(*niveles).tope_obstaculos = max_obstaculos(valores_obstaculos[NIVEL_UNO - 1]);
	(*niveles).tope_herramientas = max_herramientas(valores_herramientas[NIVEL_UNO - 1]);
	obtener_mapa((*niveles).paredes, &(*niveles).tope_paredes);
	cargar_coordenadas_walls((*niveles).paredes, (*niveles).tope_paredes, walls, &tope_walls);
	eliminar_coordenadas_paredes((*niveles).paredes, (*niveles).tope_paredes, random_list, &(*tope_elementos));
	inicializar_obstaculos(&(*niveles), (*niveles).tope_obstaculos, valores_obstaculos, &(*tope_elementos), random_list);
	inicializar_herramientas(&(*niveles), (*niveles).tope_herramientas, valores_herramientas, &(*tope_elementos), random_list, walls, &tope_walls);
}

/*
 *PRE-CONDICIONES: Recibe el nivel 2 para inicializar con la informacion de obstaculos herramientas y posiciones disponibles.
 *POST-CONDICIONES: Inicializa la estructura de nivel 2 con todas las estructuras cargadas.
 */
void inicializar_nivel_ii(nivel_t* niveles, obstaculos_nivel_t valores_obstaculos[MAX_ELEMENTOS], int* tope_elementos, coordenada_t random_list[MAX_ELEMENTOS], herramientas_nivel_t valores_herramientas[MAX_ELEMENTOS]) {
	coordenada_t walls[MAX_ELEMENTOS];
	int tope_walls = 0;
	(*niveles).numero_nivel = NIVEL_DOS;
	(*niveles).tope_paredes = INDEX_N1;
	(*niveles).tope_obstaculos = max_obstaculos(valores_obstaculos[NIVEL_DOS - 1]);
	(*niveles).tope_herramientas = max_herramientas(valores_herramientas[NIVEL_DOS - 1]);
	obtener_mapa((*niveles).paredes, &(*niveles).tope_paredes);
	cargar_coordenadas_walls((*niveles).paredes, (*niveles).tope_paredes, walls, &tope_walls);
	eliminar_coordenadas_paredes((*niveles).paredes, (*niveles).tope_paredes, random_list, &(*tope_elementos));
	inicializar_obstaculos(&(*niveles), (*niveles).tope_obstaculos, valores_obstaculos, &(*tope_elementos), random_list);
	inicializar_herramientas(&(*niveles), (*niveles).tope_herramientas, valores_herramientas, &(*tope_elementos), random_list, walls, &tope_walls);
}

/*
 *PRE-CONDICIONES: Recibe el nivel 3 para inicializar con la informacion de obstaculos herramientas y posiciones disponibles.
 *POST-CONDICIONES: Inicializa la estructura de nivel 3 con todas las estructuras cargadas.
 */
void inicializar_nivel_iii(nivel_t* niveles, obstaculos_nivel_t valores_obstaculos[MAX_ELEMENTOS], int* tope_elementos, coordenada_t random_list[MAX_ELEMENTOS], herramientas_nivel_t valores_herramientas[MAX_ELEMENTOS]) {
	coordenada_t walls[MAX_ELEMENTOS];
	int tope_walls = 0;
	(*niveles).numero_nivel = NIVEL_TRES;
	(*niveles).tope_paredes = INDEX_N1;
	(*niveles).tope_obstaculos = max_obstaculos(valores_obstaculos[NIVEL_TRES - 1]);
	(*niveles).tope_herramientas = max_herramientas(valores_herramientas[NIVEL_TRES - 1]);
	obtener_mapa((*niveles).paredes, &(*niveles).tope_paredes);
	cargar_coordenadas_walls((*niveles).paredes, (*niveles).tope_paredes, walls, &tope_walls);
	eliminar_coordenadas_paredes((*niveles).paredes, (*niveles).tope_paredes, random_list, &(*tope_elementos));
	inicializar_obstaculos(&((*niveles)), (*niveles).tope_obstaculos, valores_obstaculos, &(*tope_elementos), random_list);
	inicializar_herramientas(&((*niveles)), (*niveles).tope_herramientas, valores_herramientas, &(*tope_elementos), random_list, walls, &tope_walls);
}

/*
 *PRE-CONDICIONES: Recibe al juego con todas sus estructuras, y el vector de estructura de niveles sin inicializar.
 *POST-CONDICIONES: Inicializa todos los niveles con toda su informacion validada.
 */
void inicializar_niveles(juego_t* juego, obstaculos_nivel_t valores_obstaculos[MAX_ELEMENTOS], int* tope_elementos, coordenada_t random_list[MAX_ELEMENTOS], herramientas_nivel_t valores_herramientas[MAX_ELEMENTOS]) {
	cargar_mapas();
	inicializar_nivel_i(&(*juego).niveles[0], valores_obstaculos, &(*tope_elementos), random_list, valores_herramientas);
	(*tope_elementos) = MAX_ELEMENTOS;
	inicializar_posiciones(random_list, (*tope_elementos));
	inicializar_nivel_ii(&(*juego).niveles[1], valores_obstaculos, &(*tope_elementos), random_list, valores_herramientas);
	(*tope_elementos) = MAX_ELEMENTOS;
	inicializar_posiciones(random_list, (*tope_elementos));
	inicializar_nivel_iii(&(*juego).niveles[2], valores_obstaculos, &(*tope_elementos), random_list, valores_herramientas);
}

/*
 *PRE-CONDICIONES: Si se recibe un archivo de configuraciones correctamente.
 *POST-CONDICIONES: Inicializa los campos de herramientas y obstaculos con la informacion recibida de el archivo sino, inicia el juego con la configuracion predeterminada.
 */
void configurar_elementos(obstaculos_nivel_t obstaculos_config[MAX_ELEMENTOS], herramientas_nivel_t herramientas_config[MAX_ELEMENTOS], int tope_herramientas_config, int tope_obstaculos_config, int apertura_configs, obstaculos_nivel_t valores_obstaculos[MAX_ELEMENTOS], int* tope_valores, herramientas_nivel_t valores_herramientas[MAX_ELEMENTOS], int* tope_valor_herramientas) {
	if (apertura_configs != NO_ENCONTRADO ) {
		(*valores_obstaculos).velas =(*obstaculos_config).velas;
		(*valores_obstaculos).pozos =(*obstaculos_config).pozos;
		(*valores_obstaculos).interruptores =(*obstaculos_config).interruptores;
		(*valores_obstaculos).portales =(*obstaculos_config).portales;
		(*valores_herramientas).escaleras = (*herramientas_config).escaleras;
		(*valores_herramientas).monedas = (*herramientas_config).monedas;
		(*valores_herramientas).llave = (*herramientas_config).llave;
		(*valores_herramientas).puerta = (*herramientas_config).puerta;
		(*tope_valores) = tope_obstaculos_config;
		(*tope_valor_herramientas) = tope_herramientas_config;
	}
}

/*
 *PRE-CONDICIONES: Recibe al juego completo sin inicializar con todas sus estructuras.
 *POST-CONDICIONES: Inicializa el juego completo y sus estructuras.
 */
void inicializar_juego(juego_t* juego, obstaculos_nivel_t obstaculos_config[MAX_ELEMENTOS], herramientas_nivel_t herramientas_config[MAX_ELEMENTOS], int tope_herramientas_config, int tope_obstaculos_config, int apertura_configs) {
	(*juego).nivel_actual = 1;
	int max_alto = MAX_FILAS;
	int max_ancho = MAX_COLUMNAS;
	int tope_elementos = MAX_ELEMENTOS;
	int tope_valor_herramientas = 1;
	int tope_valores = 1;
	coordenada_t random_list[MAX_ELEMENTOS] = {0};
	coordenada_t arranque_sombra;
	coordenada_t arranque_personaje;
	obstaculos_nivel_t valores_obstaculos[MAX_ELEMENTOS];
	herramientas_nivel_t valores_herramientas[MAX_ELEMENTOS];
	printf("%i\n", obstaculos_config[0].velas);
	inicializar_posiciones(random_list, tope_elementos);
	inicializar_valores_obstaculos(valores_obstaculos, &tope_valores);
	inicializar_valores_herramientas(valores_herramientas, &tope_valor_herramientas);
	configurar_elementos(obstaculos_config, herramientas_config, tope_herramientas_config, tope_obstaculos_config, apertura_configs, valores_obstaculos, &tope_valores, valores_herramientas, &tope_valor_herramientas);
	inicializar_niveles(&(*juego), valores_obstaculos, &(tope_elementos), random_list, valores_herramientas);
	generar_coordenada_personaje(&arranque_personaje, max_alto, max_ancho, random_list, &tope_elementos );
	generar_coordenada_sombra(&arranque_sombra, arranque_personaje, max_ancho, random_list, &tope_elementos);
	inicializar_personaje(&(*juego).personaje, arranque_personaje);
	inicializar_sombra(&(*juego).sombra, arranque_sombra);
}

/*
 *PRE-CONDICIONES: Recibe una variable de tipo char.
 *POST-CONDICIONES: Retorna verdadero si la letra recibida esta dentro de la condicion y falso de lo contrario.
 */
bool es_movimiento_valido(char movimiento) {
	return (movimiento == ARRIBA || movimiento == ABAJO || movimiento == IZQUIERDA || movimiento == DERECHA || movimiento == VIDA);
}

/*
 *PRE-CONDICIONES: Recibe una variable tipo char vacia.
 *POST-CONDICIONES: Pide un caracter que sera ingresado por teclado, valida dicho caracter e inicializa la variable que recibimos por parametro con dicho caracter.
 */
void pedir_movimiento(char* ref_movimiento) {
	printf("Ingrese un movimiento para mover al personaje\n");
	printf("Los caracteres permitidos ingresados en mayuscula son los siguientes: W:(arriba) A:(izquierda) S:(abajo) D:(derecha)\n");
	scanf(" %c", ref_movimiento);
	while (!es_movimiento_valido(*ref_movimiento)) {
		printf("CARACTER INVALIDO!, por favor ingresar un caracter valido: W:(arriba) A:(izquierda) S:(abajo) D:(derecha)\n");
		scanf(" %c", ref_movimiento);
	}
}

/*
 *PRE-CONDICIONES: Recibe por parametro struct(posicion) y dos numeros enteros.
 *POST-CONDICIONES: Retorna true cuando las posiciones dentro del struct se encuentran dentro de los rangos que define los numeros enteros y false de lo contrario.
 */
bool esta_dentro_rango(coordenada_t posicion, int max_alto, int max_ancho) {
	return ((posicion.fila <= max_alto && posicion.col <= max_ancho) && (posicion.fila >= 0 && posicion.col >= 0));
}

/*
 *PRE-CONDICIONES: Recibe por parametro personaje, sombra y movimiento.
 *POST-CONDICIONES: Cambia la posicion segun el movimiento que recibe.
 */
void mover_personaje_sombra(personaje_t* ref_personaje, sombra_t* ref_sombra, char movimiento, bool interruptor) {
	if (!interruptor) {
		switch (movimiento) {
			case (ARRIBA):
				(*ref_personaje).posicion.fila--;
				break;
	
			case (IZQUIERDA):
				(*ref_personaje).posicion.col--;
				break;
	
			case (ABAJO):
				(*ref_personaje).posicion.fila++;
				break;
	
			case (DERECHA):
				(*ref_personaje).posicion.col++;
				break;
		}
		if ((*ref_sombra).esta_viva) {
			switch (movimiento) {
				case (ARRIBA):
					(*ref_sombra).posicion.fila--;
					break;
		
				case (IZQUIERDA):
					(*ref_sombra).posicion.col++;
					break;
		
				case (ABAJO):
					(*ref_sombra).posicion.fila++;
					break;
		
				case (DERECHA):
					(*ref_sombra).posicion.col--;
					break;
			}
		}
		
	} else if (interruptor) {
		switch (movimiento) {
			case (ARRIBA):
				(*ref_personaje).posicion.fila--;
				break;
	
			case (IZQUIERDA):
				(*ref_personaje).posicion.col--;
				break;
	
			case (ABAJO):
				(*ref_personaje).posicion.fila++;
				break;
	
			case (DERECHA):
				(*ref_personaje).posicion.col++;
				break;
		}
		if ((*ref_sombra).esta_viva) {
			switch (movimiento) {
				case (ARRIBA):
					(*ref_sombra).posicion.fila--;
					break;
		
				case (IZQUIERDA):
					(*ref_sombra).posicion.col--;
					break;
		
				case (ABAJO):
					(*ref_sombra).posicion.fila++;
					break;
		
				case (DERECHA):
					(*ref_sombra).posicion.col++;
					break;
			}
		}
	}
}

/*
 *PRE-CONDICIONES: Necesita los valores de posicion personaje previamente inicializados y cargados con el movimiento que se quiere verificar.
 *POST-CONDICIONES: Si no esta dentro del rango devuelve a la ultima posicion valida al personaje de lo contrario no hace nada.
 */
void verificacion_posicion_mac(personaje_t* personaje, int max_alto, int max_ancho, char movimiento) {
	if (!esta_dentro_rango((*personaje).posicion, max_alto, max_ancho)) {
		if (movimiento == ARRIBA) {
			(*personaje).posicion.fila = 0;
		}else if (movimiento == IZQUIERDA) {
 			(*personaje).posicion.col = 0;
 		}else if (movimiento == ABAJO) {
 			(*personaje).posicion.fila = max_alto;
		}else if (movimiento == DERECHA) {
			(*personaje).posicion.col = max_ancho;
		}
	}
}
/*
 *PRE-CONDICIONES: Necesita los valores de posicion bloo previamente inicializados y cargados con el movimiento que se quiere verificar.
 *POST-CONDICIONES: Si no esta dentro del rango devuelve a la ultima posicion valida al sombra de lo contrario no hace nada.
 */
void verificacion_posicion_bloo(sombra_t* sombra, personaje_t personaje,  int max_alto, int max_ancho, char movimiento) {
	if (!esta_dentro_rango((*sombra).posicion, max_alto, max_ancho)) {
		if (!personaje.interruptor_apretado) {
			if (movimiento == ARRIBA) {
				(*sombra).posicion.fila = 0;
			}else if (movimiento == IZQUIERDA) {
 				(*sombra).posicion.col = max_ancho;
 			}else if (movimiento == ABAJO) {
 				(*sombra).posicion.fila = max_alto;
			}else if (movimiento == DERECHA) {
				(*sombra).posicion.col = 0;
			}	
		}else if (personaje.interruptor_apretado) {
			if (movimiento == ARRIBA) {
				(*sombra).posicion.fila = 0;
			}else if (movimiento == IZQUIERDA) {
 				(*sombra).posicion.col = 0;
 			}else if (movimiento == ABAJO) {
 				(*sombra).posicion.fila = max_alto;
			}else if (movimiento == DERECHA) {
				(*sombra).posicion.col = max_ancho;
			}
		}
	}
}

/*
 *PRE-CONDICIONES: Recibe dos structs por referencia dos enteros por valor, y un caracter por valor.
 *POST-CONDICIONES: Evalua si el movimiento que se quiere hacer sale de los limites del terreno, si se pasa entonces se devuelve a la posicion valida anterior.
 */
void movimiento_valido(personaje_t* personaje, sombra_t* sombra, int max_alto, int max_ancho, char movimiento) {
	verificacion_posicion_mac(&(*personaje), max_alto, max_ancho, movimiento);
	verificacion_posicion_bloo(&(*sombra), (*personaje), max_alto, max_ancho, movimiento);
}

/*
 *PRE-CONDICIONES: Recibe dos coordenadas previamente inicializadas con informacion.
 *POST-CONDICIONES: Devuelve la distancia mahattan entre dos coordenadas.
 */
int distancia_manhattan(coordenada_t personaje, coordenada_t puerta) {
	int distancia = 0;
	int a = ((personaje.fila - puerta.fila));
	int b = ((personaje.col - puerta.col)); 
	if (a < 0) {
		a *= -1;
	}
	if (b < 0) {
		b *= -1;
	}
	distancia = a + b;
	return distancia;
}

/*
 *PRE-CONDICIONES: Recibe un vector con la informacion de elementos.
 *POST-CONDICIONES: Devueleve el indice del vector donde esta el elemento buscado.
 */
int buscar_elemento_nivel(int tope_herramientas, elemento_t elemento[MAX_ELEMENTOS], char elemento_buscado) {
	int buscado = UNFINDED;
	bool encontrado = false;
	for (int i = 0; i < tope_herramientas; i++) {
		if (elemento[i].tipo == elemento_buscado && !encontrado) {
			buscado = i;
			encontrado = true;
		}
	}
	return buscado;
}

/*
 *PRE-CONDICIONES: Recibe el juego con todas sus estructuras cargadas.
 *POST-CONDICIONES: Devuelve el estado del nivel dependiendo de los parametros que definen el estado.
 */
int estado_nivel(juego_t juego) {
	int estado = 0;
	int nivel = (juego.nivel_actual - 1);
	int index = buscar_elemento_nivel(juego.niveles[nivel].tope_herramientas, juego.niveles[nivel].herramientas, PUERTA);
	coordenada_t puerta = juego.niveles[nivel].herramientas[index].coordenada;
	coordenada_t personaje = juego.personaje.posicion;
	coordenada_t sombra = juego.sombra.posicion;
	int distancia_puerta_mac = distancia_manhattan(personaje, puerta);
	int distancia_puerta_bloo = distancia_manhattan(sombra, puerta);
	
	if (nivel == INDEX_N1) {
		if ( distancia_puerta_mac <= DISTANCIA_MANHATTAN && distancia_puerta_bloo <= DISTANCIA_MANHATTAN && juego.sombra.esta_viva) {
			estado = GANADO;
		}else {
			estado = JUGANDO;
		}
	}else if (nivel == INDEX_N2) {
		if ( distancia_puerta_mac <= DISTANCIA_MANHATTAN && distancia_puerta_bloo <= DISTANCIA_MANHATTAN && juego.personaje.tiene_llave && juego.sombra.esta_viva) {
			estado = GANADO;
		}else {
			estado = JUGANDO;
		}
	}else if (nivel == INDEX_N3) {
		if (distancia_puerta_mac <= DISTANCIA_MANHATTAN && distancia_puerta_bloo <= DISTANCIA_MANHATTAN && juego.personaje.tiene_llave && juego.sombra.esta_viva) {
			estado = GANADO;
		}else {
			estado = JUGANDO;
		}
	}		
	return estado;
}

/*
 *PRE-CONDICIONES: Recibe el juego previamente cargado.
 *POST-CONDICIONES: Cambia el campo de nivel actual que esta en juego, si se gana el nivel.
 */
void actualizar_nivel(juego_t* juego) {
	int level_state = estado_nivel((*juego));
	if (level_state == GANADO && (*juego).nivel_actual < NIVEL_TRES) {
		((*juego).nivel_actual)++;
	} 
}

/*
 *PRE-CONDICIONES: Recibe el movimiento hacia donde quiere moverse el personaje.
 *POST-CONDICIONES: Devuelve al personaje a su posicion anterior si hay una pared, sino no hace nada.
 */
void colisionar_paredes_mac(personaje_t* personaje, coordenada_t paredes[MAX_PAREDES], int tope_paredes, char movimiento) {
	for (int i = 0; i < tope_paredes; i++) {
		if (coordenadas_iguales((*personaje).posicion, paredes[i])) {
			if (movimiento == ARRIBA) {
				(*personaje).posicion.fila++;
			}else if (movimiento == IZQUIERDA) {
 				(*personaje).posicion.col++;
 			}else if (movimiento == ABAJO) {
 				(*personaje).posicion.fila--;
			}else if (movimiento == DERECHA) {
				(*personaje).posicion.col--;
			}
		}
	}
}

/*
 *PRE-CONDICIONES: Recibe el movimiento hacia donde quiere moverse el bloo y el interruptor que esta en la estructura del personaje.
 *POST-CONDICIONES: Devuelve al personaje a su posicion anterior si hay una pared, sino no hace nada.
 */
void colisionar_pared_bloo(sombra_t* sombra, personaje_t personaje, coordenada_t paredes[MAX_PAREDES], int tope_paredes, char movimiento) {
	for (int i = 0; i < tope_paredes; i++) {
		if (coordenadas_iguales((*sombra).posicion, paredes[i])) {
			if (personaje.interruptor_apretado) {
				if (movimiento == ARRIBA) {
					(*sombra).posicion.fila++;
				}else if (movimiento == IZQUIERDA) {
 					(*sombra).posicion.col++;
 				}else if (movimiento == ABAJO) {
 					(*sombra).posicion.fila--;
				}else if (movimiento == DERECHA) {
					(*sombra).posicion.col--;
				}
			}else {
				if (movimiento == ARRIBA) {
					(*sombra).posicion.fila++;
				}else if (movimiento == IZQUIERDA) {
 					(*sombra).posicion.col--;
 				}else if (movimiento == ABAJO) {
 					(*sombra).posicion.fila--;
				}else if (movimiento == DERECHA) {
					(*sombra).posicion.col++;
				}
			}
		}
	}
}

/*
 *PRE-CONDICIONES: Recibe un vector de tipo estructura que contiene todos los elementos que estan en el mapa
 *POST-CONDICIONES: Elimina del vector el elemento que se desea eliminar.
 */
void eliminar_elemento_map(elemento_t herramientas[MAX_ELEMENTOS], int index, int* tope_herramientas) {
	for (int i = index; i < (*tope_herramientas); i++) {
		herramientas[i] = herramientas[i + 1];
	}
	(*tope_herramientas)--;
}

/*
 *PRE-CONDICIONES: Recibe la posicion que se quiere mover el personaje y el vector de herramientas, con el tope y el movimiento previamente cargados.
 *POST-CONDICIONES: Salta una posicion si hay una escalera en la posicion donde se quiere mover el personaje.
 */
void doblar_escalera(personaje_t* personaje, elemento_t herramientas[MAX_ELEMENTOS], int* tope_herramientas, char movimiento) {
	for (int i = 0; i < (*tope_herramientas); i++) {
		if (coordenadas_iguales((*personaje).posicion, herramientas[i].coordenada) && herramientas[i].tipo == ESCALERAS) {
			if (movimiento == ARRIBA) {
				(*personaje).posicion.fila--;
				eliminar_elemento_map(herramientas, i, &(*tope_herramientas));
			}else if (movimiento == IZQUIERDA) {
 				(*personaje).posicion.col--;
 				eliminar_elemento_map(herramientas, i, &(*tope_herramientas));
 			}else if (movimiento == ABAJO) {
 				(*personaje).posicion.fila++;
 				eliminar_elemento_map(herramientas, i, &(*tope_herramientas));
			}else if (movimiento == DERECHA) {
				(*personaje).posicion.col++;
				eliminar_elemento_map(herramientas, i, &(*tope_herramientas));
			}
		}
	}
}

/*
 *PRE-CONDICIONES: Recibe la posicion que se quiere mover el personaje y el vector de herramientas, con el tope y el movimiento previamente cargados.
 *POST-CONDICIONES: Salta una posicion si hay una escalera en la posicion donde se quiere mover el personaje.
 */
void escalar_escalera_mac(personaje_t* personaje, elemento_t herramientas[MAX_PAREDES], int* tope_herramientas, char movimiento,  coordenada_t paredes[MAX_ELEMENTOS], int tope_paredes) {
	for (int i = 0; i < (*tope_herramientas); i++) {
		if (coordenadas_iguales((*personaje).posicion, herramientas[i].coordenada) && herramientas[i].tipo == ESCALERAS) {
			if (movimiento == ARRIBA) {
				(*personaje).posicion.fila--;
				doblar_escalera(personaje, herramientas, tope_herramientas, movimiento);
				if (buscar_elemento((*personaje).posicion, paredes, tope_paredes) != -1) {
					(*personaje).posicion.fila++;
				}else {
					eliminar_elemento_map(herramientas, i, &(*tope_herramientas));
				}
			}else if (movimiento == IZQUIERDA) {
 				(*personaje).posicion.col--;
				doblar_escalera(personaje, herramientas, tope_herramientas, movimiento);
 				if (buscar_elemento((*personaje).posicion, paredes, tope_paredes) != -1) {
					(*personaje).posicion.col++;
				}else {
					eliminar_elemento_map(herramientas, i, &(*tope_herramientas));
				}
 			}else if (movimiento == ABAJO) {
 				(*personaje).posicion.fila++;
				doblar_escalera(personaje, herramientas, tope_herramientas, movimiento);
 				if (buscar_elemento((*personaje).posicion, paredes, tope_paredes) != -1) {
					(*personaje).posicion.fila--;
				}else {
					eliminar_elemento_map(herramientas, i, &(*tope_herramientas));
				}
			}else if (movimiento == DERECHA) {
				(*personaje).posicion.col++;
				doblar_escalera(personaje, herramientas, tope_herramientas, movimiento);
				if (buscar_elemento((*personaje).posicion, paredes, tope_paredes) != -1) {
					(*personaje).posicion.col--;
				}else {
					eliminar_elemento_map(herramientas, i, &(*tope_herramientas));
				}
			}
		}
	}
}

/*
 *PRE-CONDICIONES: Recibe la posicion que se quiere mover el personaje y el vector de herramientas, con el tope y el movimiento previamente cargados.
 *POST-CONDICIONES: Verifica si hay una moneda en la posicion en la que se quiere mover el personaje, si hay una la agarra y la elimina del vector de herramientas.
 */
void recolectar_monedas_mac(personaje_t* personaje, elemento_t herramientas[MAX_ELEMENTOS], int* tope_herramientas) {
	for (int i = 0; i < (*tope_herramientas); i++) {
		if (coordenadas_iguales((*personaje).posicion, herramientas[i].coordenada) && herramientas[i].tipo == MONEDAS) {
			(*personaje).puntos += rand() % 11 + 10;
			eliminar_elemento_map(herramientas, i, &(*tope_herramientas));
		}
	}
}

/*
 *PRE-CONDICIONES: Recibe la posicion que se quiere mover el bloo y el vector de herramientas, con el tope y el movimiento previamente cargados.
 *POST-CONDICIONES: Verifica si hay una moneda en la posicion en la que se quiere mover el bloo, si hay una la agarra y la elimina del vector de herramientas.
 */
void recolectar_monedas_bloo(personaje_t* personaje, sombra_t sombra, elemento_t herramientas[MAX_ELEMENTOS], int* tope_herramientas) {
	for (int i = 0; i < (*tope_herramientas); i++) {
		if (coordenadas_iguales(sombra.posicion, herramientas[i].coordenada) && herramientas[i].tipo == MONEDAS) {
			(*personaje).puntos += rand() % 11 + 10;
			eliminar_elemento_map(herramientas, i, &(*tope_herramientas));
		}
	}
}

/*
 *PRE-CONDICIONES: Recibe a personaje y el movimiento que dio el usuario.
 *POST-CONDICIONES: Si recibio el movimiento para canjear las monedas entonces lo hace y resta los puntos y suma vida, sino no hace nada.
 */
void canjear_monedas(personaje_t* personaje, char movimiento) {
	if ((*personaje).vida < MAX_VIDAS && (*personaje).vida != 0 && movimiento == VIDA &&(*personaje).puntos >= MIN_PUNTOS) {
		((*personaje).vida)++;
		(*personaje).puntos -= MIN_PUNTOS;
	}
}

/*
 *PRE-CONDICIONES: Recibe la posicion del personaje, dos enteros para la generacion de numeros ramdon, el vector con las posiciones disponibles y su tope.
 *POST-CONDICIONES: Genera una posicion valida aleatoria para el persoje.
 */
void generar_coordenada_mac(coordenada_t* arranque_personaje, int max_alto, int max_ancho, coordenada_t random_list[MAX_ELEMENTOS], int* tope_elementos) {
	int index = 0;
	coordenada_t auxiliar;
	int index_aux = 0;
	do {
		(*arranque_personaje) = coordenada_aleatoria(max_alto, max_ancho);
		index = index_personajes(random_list, (*tope_elementos), (*arranque_personaje));
		auxiliar.fila = (*arranque_personaje).fila;
		auxiliar.col = (max_ancho - ((*arranque_personaje).col) - 1);
		index_aux = index_personajes(random_list, (*tope_elementos), auxiliar);
	}while(index != NO_ENCONTRADO || index_aux != NO_ENCONTRADO);
	eliminar_elemento(random_list, index, &(*tope_elementos)); 
 }

/*
 *PRE-CONDICIONES: Recibe el personaje y la sombra, previamente cargados, y el vector con las coordenadas disponibles y su tope.
 *POST-CONDICIONES: Cambia las posiciones de los personajes, a una aleatoria valida.
 */
void posicionar_aleatoriamente(personaje_t* personaje, sombra_t* sombra, coordenada_t elementos_map[MAX_ELEMENTOS], int* tope_elementos_map) {
	int max_alto = MAX_FILAS;
	int max_ancho = MAX_COLUMNAS;
	generar_coordenada_mac(&(*personaje).posicion, max_alto, max_ancho, elementos_map, &(*tope_elementos_map));
	generar_coordenada_sombra(&(*sombra).posicion,(*personaje).posicion, max_ancho, elementos_map, &(*tope_elementos_map));
}

/*
 *PRE-CONDICIONES: Recibe la posicion donde se movio el personaje, el vector de obstaculos, las posiciones ocupadas por los demas elementos y su tope. 
 *POST-CONDICIONES: Si cayo en un pozo entonces se le restara una vida y se posicionara aleatoriamente los dos personajes nuevamente en el terreno.
 */
void caer_pozo(personaje_t* personaje, sombra_t* sombra, elemento_t obstaculos[MAX_ELEMENTOS], int* tope_obstaculos, coordenada_t elementos_map[MAX_ELEMENTOS], int* tope_elementos_map) {
	for (int i = 0; i < (*tope_obstaculos); i++) {
		if (coordenadas_iguales((*personaje).posicion, obstaculos[i].coordenada) && obstaculos[i].tipo == POZOS) {
			((*personaje).vida)--;
			posicionar_aleatoriamente(&(*personaje), &(*sombra), elementos_map, &(*tope_elementos_map));
		}
	}
}

/*
 *PRE-CONDICIONES: Recibe la posicion donde se movio el personaje, el vector de herramientas y su tope. 
 *POST-CONDICIONES: Si en la posicion donde se movio hay una llave entonces la agarra y se elimina del vector de herramientas.
 */
void recoger_llave(personaje_t* personaje, elemento_t herramientas[MAX_ELEMENTOS], int* tope_herramientas) {
	for (int i = 0; i < (*tope_herramientas); i++) {
		if (coordenadas_iguales((*personaje).posicion, herramientas[i].coordenada) && herramientas[i].tipo == LLAVE) {
			(*personaje).tiene_llave = true;
			eliminar_elemento_map(herramientas, i, &(*tope_herramientas));
		}
	}
}

/*
 *PRE-CONDICIONES: Recibe la posicion donde se movio el personaje, el personaje completo y el vector de obstaculos y su tope previamente cargados.
 *POST-CONDICIONES: Si en la posicion donde se movio hay un interruptor entonces cambia el estado de interruptor del estruct de personaje.
 */
void apretar_interruptor(personaje_t* personaje, elemento_t obstaculos[MAX_ELEMENTOS], int* tope_obstaculos, bool* interruptor) {
	for (int i = 0; i < (*tope_obstaculos); i++) {
		if (coordenadas_iguales((*personaje).posicion, obstaculos[i].coordenada) && obstaculos[i].tipo == INTERRUPTORES) {
			if (!(*interruptor) ) {
				(*interruptor) = true;
			} else if ((*interruptor)) {
				(*interruptor) = false;
			}
		}
	}
}

/*
 *PRE-CONDICIONES: Recibe el vector de obstaculos con su tope y la posicion donde se movio el personaje.
 *POST-CONDICIONES: Si en la posicion donde se movio hay una vela entonces se elimina del vector de obstaculos.
 */
void apagar_vela(personaje_t* personaje, elemento_t obstaculos[MAX_ELEMENTOS], int* tope_obstaculos) {
	for (int i = 0; i < (*tope_obstaculos); i++) {
		if (coordenadas_iguales((*personaje).posicion, obstaculos[i].coordenada) && obstaculos[i].tipo == VELAS) {
			eliminar_elemento_map(obstaculos, i, &(*tope_obstaculos));
		}
	}
}

/*
 *PRE-CONDICIONES: Recibe el vector de obstaculos con su tope y la posicion donde se movio el bloo y el personaje cargados.
 *POST-CONDICIONES: Si en la posicion donde se movio hay un portal entonces se intercambian las posiciones de los personajes.
 */
void teletransportar_bloo_portales(sombra_t* sombra, personaje_t* personaje, elemento_t obstaculos[MAX_ELEMENTOS], int tope_obstaculos) {
	coordenada_t auxiliar;
	for (int i = 0; i < (tope_obstaculos); i++) {
		if (coordenadas_iguales((*sombra).posicion, obstaculos[i].coordenada) && obstaculos[i].tipo == PORTALES) {
				auxiliar = (*personaje).posicion;
				(*personaje).posicion = (*sombra).posicion;
				(*sombra).posicion = auxiliar;
		}
	}
}

/*
 *PRE-CONDICIONES: Recibe una coordenada de inicio una coordenada de fin un vector auxiliar y su tope, previamente cargados.
 *POST-CONDICIONES: Llena el vector auxiliar con todas las posiciones generadas a partir de las coordenadas recibidas y el tope, haciendo el efecto 3x3 alrededor de cada vela.
 */
void proyectar_luz_vela(coordenada_t inicio, coordenada_t fin, coordenada_t auxiliar_velas[MAX_ELEMENTOS], int* tope_auxiliar) {
	for (int i = inicio.fila; i < fin.fila; i++) {
		for (int j = inicio.col; j < fin.col; j++) {
			auxiliar_velas[(*tope_auxiliar)].fila = i;
			auxiliar_velas[(*tope_auxiliar)].col = j;
			(*tope_auxiliar)++;
		}
	}
}

/*
 *PRE-CONDICIONES: Recibe el vector de obstaculos su tope previamente cargados y un vector auxiliar con su tope. 
 *POST-CONDICIONES: Llena todo el vector auxiliar con las posiciones de las velas y su efecto iluminador es decir el efecto matriz 3x3 con la vela en el centro.
 */
void resplandor_velas(elemento_t obstaculos[MAX_ELEMENTOS], int tope_obstaculos, coordenada_t auxiliar_velas[MAX_ELEMENTOS], int* tope_auxiliar) {
	coordenada_t inicio;
	coordenada_t fin;
	int cantidad_velas = 0;
	for (int i = 0; i < (tope_obstaculos); i++) {
		if (obstaculos[i].tipo == VELAS) {
			auxiliar_velas[(*tope_auxiliar)] = obstaculos[i].coordenada;
			(*tope_auxiliar)++;
		}
	}
	cantidad_velas = (*tope_auxiliar);
	for (int i = 0; i < cantidad_velas; i++) {
		inicio.fila = auxiliar_velas[i].fila - V_INICIO;
		inicio.col = auxiliar_velas[i].col - V_INICIO;
		fin.fila =  auxiliar_velas[i].fila + V_TOPE;
		fin.col =  auxiliar_velas[i].col + V_TOPE;
		proyectar_luz_vela(inicio, fin, auxiliar_velas, &(*tope_auxiliar));
	}
}

/*
 *PRE-CONDICIONES: Recibe la sombra y el vector con las coordenadas de las velas y su resplandor previamente cargadas.
 *POST-CONDICIONES: Muere la sombra si su posicion coincide con la vela o su resplandor.
 */
void iluminar_sombra(sombra_t* sombra, coordenada_t auxiliar_velas[MAX_ELEMENTOS], int tope_auxiliar) {
	for (int i = 0; i < tope_auxiliar; i++) {
		if (coordenadas_iguales((*sombra).posicion, auxiliar_velas[i])) {
			(*sombra).esta_viva = false;
		} 
	}
}

/*
 *PRE-CONDICIONES: Recibe la posicion de mac y bloo y el tope de herramientas cargados previamente.
 *POST-CONDICIONES: Revive a bloo si se cumplen las condiciones.
 */
void revivir_mac_bloo(sombra_t* sombra, personaje_t* personaje, int tope_herramientas) {
	if (coordenadas_iguales((*sombra).posicion, (*personaje).posicion) && !(*sombra).esta_viva) {
		(*sombra).esta_viva = true;
		(*personaje).puntos -= COSTO_VIDA;	
	} 
}

/*
 *PRE-CONDICIONES: Recibe el nivel actual un vector de elementos y su tope.
 *POST-CONDICIONES: Carga las posiciones de todos los elementos que estan en dicho nivel en el vector de elementos. 
 */
void cargar_elementos_map(nivel_t niveles, coordenada_t elementos_map[MAX_ELEMENTOS], int tope_elementos_map) {
	int contador = 0;
	bool paredes = false;
	bool obstaculos = false;
	bool herramientas = false;
	for (int i = 0; i < tope_elementos_map; i++) {
		if (contador < niveles.tope_paredes && !paredes) {
			elementos_map[i] = niveles.paredes[contador];
			contador++; 
			if (contador == niveles.tope_paredes) {
				paredes = true;
				contador = 0;
			}
		}else if (contador < niveles.tope_obstaculos && !obstaculos) {
			elementos_map[i] = niveles.obstaculos[contador].coordenada;
			contador++; 
			if (contador == niveles.tope_obstaculos) {
				obstaculos = true;
				contador = 0;
			}
		}else if (contador < niveles.tope_herramientas && !herramientas) {
			elementos_map[i] = niveles.obstaculos[contador].coordenada;
			contador++; 
			if (contador == niveles.tope_herramientas) {
				obstaculos = true;
			}
		}
	}
}

/*
 *PRE-CONDICIONES: Recibe el personaje previamente cargado
 *POST-CONDICIONES: Apaga el interruptor.
 */
void apagar_interruptor(personaje_t* personaje) {
	if((*personaje).interruptor_apretado) {
		(*personaje).interruptor_apretado = false;
	}
}
/*
 *PRE-CONDICIONES: Recibe el juego con todas sus estructuras validas y previamente cargadas.
 *POST-CONDICIONES: Cambia la posicion del personaje y sombra segun el caracter movimiento que indica la direccion,y realiza las acciones correspondientes a cada posicion en la que se mueve, si es valido el movimiento.
 */
void realizar_jugada(juego_t* juego) {
	coordenada_t auxiliar_velas[MAX_ELEMENTOS];
	coordenada_t elementos_map[MAX_ELEMENTOS];
	char movimiento = 0;
	int tope_auxiliar = 0;
	int estado = 0;
	int max_alto = MAX_FILAS - 1;
	int max_ancho = MAX_COLUMNAS - 1;
	int level = ((*juego).nivel_actual - 1);
	int tope_elementos_map = ((*juego).niveles[level].tope_obstaculos + (*juego).niveles[level].tope_herramientas + (*juego).niveles[level].tope_paredes);
	
	cargar_elementos_map((*juego).niveles[level], elementos_map, tope_elementos_map);
	pedir_movimiento(&movimiento);
	mover_personaje_sombra(&(*juego).personaje, &(*juego).sombra, movimiento, (*juego).personaje.interruptor_apretado);
	movimiento_valido(&(*juego).personaje, &(*juego).sombra, max_alto, max_ancho, movimiento);
	estado = estado_nivel(*juego);
	if (estado == GANADO && (*juego).nivel_actual < 3) {
		actualizar_nivel(&(*juego));
		level = ((*juego).nivel_actual - 1);
		tope_elementos_map = ((*juego).niveles[level].tope_obstaculos + (*juego).niveles[level].tope_herramientas + (*juego).niveles[level].tope_paredes);
		posicionar_aleatoriamente(&(*juego).personaje, &(*juego).sombra, elementos_map, &tope_elementos_map);
		apagar_interruptor(&(*juego).personaje);
	}else if (estado == JUGANDO && (*juego).nivel_actual <= 3) {
		escalar_escalera_mac(&(*juego).personaje, (*juego).niveles[(*juego).nivel_actual - 1].herramientas, &(*juego).niveles[(*juego).nivel_actual - 1].tope_herramientas, movimiento, (*juego).niveles[(*juego).nivel_actual - 1].paredes,  (*juego).niveles[(*juego).nivel_actual - 1].tope_paredes);
		colisionar_paredes_mac(&(*juego).personaje, (*juego).niveles[(*juego).nivel_actual - 1].paredes, (*juego).niveles[(*juego).nivel_actual - 1].tope_paredes, movimiento);
		colisionar_pared_bloo(&(*juego).sombra, (*juego).personaje , (*juego).niveles[(*juego).nivel_actual - 1].paredes, (*juego).niveles[(*juego).nivel_actual - 1].tope_paredes, movimiento);
		recolectar_monedas_mac(&(*juego).personaje, (*juego).niveles[(*juego).nivel_actual - 1].herramientas, &(*juego).niveles[(*juego).nivel_actual - 1].tope_herramientas);
		recolectar_monedas_bloo(&(*juego).personaje, (*juego).sombra, (*juego).niveles[(*juego).nivel_actual - 1].herramientas, &(*juego).niveles[(*juego).nivel_actual - 1].tope_herramientas);
		apretar_interruptor(&(*juego).personaje, (*juego).niveles[(*juego).nivel_actual - 1].obstaculos,  &(*juego).niveles[(*juego).nivel_actual - 1].tope_obstaculos, &(*juego).personaje.interruptor_apretado);
		apagar_vela(&(*juego).personaje, (*juego).niveles[(*juego).nivel_actual - 1].obstaculos,  &(*juego).niveles[(*juego).nivel_actual - 1].tope_obstaculos);
		canjear_monedas(&(*juego).personaje, movimiento);
		revivir_mac_bloo(&(*juego).sombra, &(*juego).personaje,(*juego).niveles[(*juego).nivel_actual - 1].tope_herramientas );
		recoger_llave(&(*juego).personaje, (*juego).niveles[(*juego).nivel_actual - 1].herramientas, &(*juego).niveles[(*juego).nivel_actual - 1].tope_herramientas);
		teletransportar_bloo_portales(&(*juego).sombra, &(*juego).personaje, (*juego).niveles[(*juego).nivel_actual - 1].obstaculos, (*juego).niveles[(*juego).nivel_actual - 1].tope_obstaculos);
		caer_pozo(&(*juego).personaje, &(*juego).sombra, (*juego).niveles[(*juego).nivel_actual - 1].obstaculos,  &(*juego).niveles[(*juego).nivel_actual - 1].tope_obstaculos, elementos_map, &tope_elementos_map);
		resplandor_velas((*juego).niveles[(*juego).nivel_actual - 1].obstaculos, (*juego).niveles[(*juego).nivel_actual - 1].tope_obstaculos, auxiliar_velas, &tope_auxiliar);
		iluminar_sombra(&(*juego).sombra,auxiliar_velas, tope_auxiliar);
	}
}

/*
 *PRE-CONDICIONES: Recibe el jego con todas sus estructuras validas y previamente cargadas.
 *POST-CONDICIONES: Devuelve 1 si el estado es ganado 0 si es jugando y - 1 si es perdido.
 */
int estado_juego(juego_t juego) {
	int estado = 0;
	bool esta_perdido = (juego.personaje.vida == 0);
	if (esta_perdido) {
		estado = PERDIDO;
	} else if (juego.nivel_actual == NIVEL_TRES && estado_nivel(juego) == GANADO) {
		estado = GANADO;
	}else {
		estado = JUGANDO;
	}
	return estado;
}

/*
 *PRE-CONDICIONES: Recibe un struct juego que contiene toda la informacion que se necesita para imprimir por pantalla el juego.
 *POST-CONDICIONES: Imprime por pantalla el terreno y los elementos que fueron cargados en las coordenadas del terreno.
 */
void imprimir_terreno(juego_t juego) {
	char terreno[MAX_FILAS][MAX_COLUMNAS] = {0};
	int nivel_corriente = juego.nivel_actual - 1; 

	for (int i = 0; i < juego.niveles[nivel_corriente].tope_obstaculos; i++) {
		terreno[juego.niveles[nivel_corriente].obstaculos[i].coordenada.fila][juego.niveles[nivel_corriente].obstaculos[i].coordenada.col] = juego.niveles[nivel_corriente].obstaculos[i].tipo;
	}
	for (int i = 0; i < juego.niveles[nivel_corriente].tope_herramientas; i++) {
		terreno[juego.niveles[nivel_corriente].herramientas[i].coordenada.fila][juego.niveles[nivel_corriente].herramientas[i].coordenada.col] = juego.niveles[nivel_corriente].herramientas[i].tipo;
	}
	for (int i = 0; i < juego.niveles[nivel_corriente].tope_paredes; i++) {
		if (juego.niveles[nivel_corriente].tope_paredes > 0 && terreno[juego.niveles[nivel_corriente].paredes[i].fila][juego.niveles[nivel_corriente].paredes[i].col] != ESCALERAS) {
			terreno[juego.niveles[nivel_corriente].paredes[i].fila][juego.niveles[nivel_corriente].paredes[i].col] = PAREDES;
		}
	}
	terreno[juego.sombra.posicion.fila][juego.sombra.posicion.col] = BLOO;
	terreno[juego.personaje.posicion.fila][juego.personaje.posicion.col]  = MAC;

	printf("VIDAS: %i", juego.personaje.vida);
	printf("       PUNTO: %i", juego.personaje.puntos);
	printf("       LLAVE: %i\n", juego.personaje.tiene_llave);

	for (int i = 0; i < MAX_FILAS; i++) {
		for (int j = 0; j < MAX_COLUMNAS; j++) {
			if ((terreno[i][j] != MAC && terreno[i][j] != BLOO &&  terreno[i][j] != MONEDAS  &&  terreno[i][j] != ESCALERAS &&  terreno[i][j] != PUERTA &&  terreno[i][j] != LLAVE && terreno[i][j] != PAREDES && terreno[i][j] != VELAS && terreno[i][j] != PORTALES && terreno[i][j] != INTERRUPTORES && terreno[i][j] != POZOS)) {
				terreno[i][j] = '.';
			}
			printf("  %c", terreno[i][j]);
		}
		printf(" \n");
	}
	printf("\n\n");
}




