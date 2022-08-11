#include "la_sombra_de_mac.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "utiles.h"
#define ARRIBA 'W'
#define IZQUIERDA 'A'
#define ABAJO 'S'
#define DERECHA 'D'
#define NO_GANADA "No"
#define MAX_NUMERO_LEVEL 3
#define MAX_NUMERO_COINS 700
#define MAX_NUMERO_LIVE 3
#define SI "Si"
#define NO "No"
#define MAX_NOMBRE 100
#define FORMATO_LECTURA "%[^;];%i;%i;%i;%[^\n]\n"
#define FORMATO_ESCRITURA "%s;%i;%i;%i;%s\n"
#define MAX_RESPUESTA 10
#define AGREGAR_PARTIDA "agregar_partida"
#define ELIMINAR_PARTIDA "eliminar_partida"
#define ORDENAR_PARTIDA "ordenar_partida"
#define CONFIGURAR_JUEGO "config_juego"
#define ARCHIVO_CONFIG "config.txt"
#define NO_ABRIO -1
#define VELAS "VELAS" 		
#define POZOS "POZOS" 
#define INTERRUPTORES "INTERRUPTORES" 
#define PORTALES "PORTALES" 
#define ESCALERAS "ESCALERAS" 
#define MONEDAS "MONEDAS" 
#define LLAVE "LLAVE" 
#define PUERTA "PUERTA" 
#define LEIDO 1
const int MAX_CARACTERES = 50;
const int CAMPOS_LEIDOS = 5;
const int MAX_PARTIDAS = 1000;
const char YES = 'Y';
const int PRIMER_NIVEL = 0; 
const int SEGUNDO_NIVEL = 1; 
const int TERCER_NIVEL = 2; 
const int CANTIDAD_NIVELES = 3;

typedef struct partida{
	char jugador[MAX_NOMBRE];
	int nivel_llegado;
	int puntos;
	int vidas_restantes;
	bool gano;
}partida_t;

/*
 *POST-CONDICIONES: Informacion sobre el juego.
 */
void presentar_juego(bool* jugar, char* argv[]) {
	if (strcmp(argv[1], CONFIGURAR_JUEGO) == 0) {
		system("clear"); 
		char respuesta;
		printf("\n                                                    Bienvenido al juego la sombra de mac\n");
		printf("\nMac es un niño de ocho años con gran inteligencia y creatividad. Él es una persona sensata y moral, y puede ser a través de Bloo, su amigo imaginario, \nque hace y dice todas las cosas que quiere pero no puede. Por lo tanto, Bloo existe como un desafío a la moralidad de Mac. Pasa una gran porción de sus días\ninvolucrándose en sus travesuras dentro y fuera de Foster’s. La Mansión Foster para amigos imaginarios es un inmenso orfanato con estilo victoriano al que los \namigos imaginarios se van a vivir cuando ya no pueden pertenecerle a sus creadores. Ahí se muda Bloo, y Mac lo visita todos los días para asegurarse de que Bloo \nno sea adoptado.\n");
		printf("\nDebemos ayudar a Mac y a Bloo a volver de la mansión luego de haber pasado una tarde explorando la ciudad. Deberás guiarlos en su camino ingresando una dirección\nen la que caminar, para poder encontrar la llave que abre la puerta de la mansión. Pero cuidado! Bloo, para hacer la vuelta de forma divertida, va a\ncaminar en espejo con respecto a Mac. Es decir, si Mac se dirige en algun sentido horizontal (izquierda o derecha), Bloo irá en el camino contrario.\n");	
		printf("\nComo el camino es largo, y no todo el camino es igual, deberán pasar por 3 niveles distintos, donde cada nivel estará delimitado por los bordes del terreno,\ny donde habrá paredes, obstáculos y herramientas, que podrán ayudarlos, o no, a volver. En caso de que Bloo o Mac, se choque con algun borde del terreno,\no con alguna pared, el mismo no deberá moverse, pero el otro si. Podrás ayudarlos a volver a la mansión?\n");
		printf("\nQuieres jugar presiona yes:(Y), de lo contrario cualquier otra letra: ");
		scanf(" %c", &respuesta);
		if (respuesta == YES) {
			(*jugar) = true;
		}
	}
}	

/*
 *POST-CONDICIONES: Informa las reglas que se deben seguir a lo largo del juego.
 */
void relgas_juego() {
	printf("                                                                            REGLAS\n");
	printf("\n                       Para guiarlos por cada nivel deberan tener en cuenta los obstaculos y herramientas que se pueden conseguir en el camino:\n\n");
	printf("Velas(V): Matan a Bloo si pasa cerca de una, por lo que Mac(M) deberá ir a apagar la vela, y revivir a Bloo(B), Descontarán 50 puntos a Mac, cada vez que reviva\n");
	printf("Pozos(W): Quitarán una vida al personaje en caso de que éste pise uno.\n");
	printf("Interruptor(O): Si Mac pisa un interruptor, lo activará e intercambiará la forma en que se mueve su sombra\n");
	printf("Portales(P): Si Bloo pisa un portal, las posiciones de los personajes serán intercambiadas, por lo que Bloo quedará en la posición de Mac, y Mac en la de Bloo.\n");
	printf("Escaleras(E): Permitirán a Mac saltarse una posición.\n");
	printf("Monedas(C): Al agarrar una moneda, le dará puntos al personaje. Los puntos seran un número aleatorio entre 10 y 20\n");
	printf("Vidas: El Mac puede elegir intercambiar 200 puntos por una vida al apretar la letra ’V’. Como máximo, el personaje puede contar con 3 vidas al mismo tiempo\n");
	printf("Llave(L): Sólo puede ser agarrada por Mac. Le permitirá a ambos personajes terminar el nivel, al pasar por la puerta de la mansión\n");
	printf("Puerta(D): Tienen que pasar los dos personajes por la puerta para cambiar de nivel\n\n");
}

/*
 *POST-CONDICIONES: Informa las opciones disponibles para abrir correctamente el juego con sus respectivos comandos.
 */
void verificar_comandos() {
	printf("Para acceder al juego o alguna de sus opciones, tiene que ingresar alguno de los siguientes comandos:\n\n");
	printf("./juego agregar_partida <nombre_archivo> ------> PARA AGREGAR PARTIDA.\n");
	printf("./juego eliminar_partida <nombre_archivo> <nombre a eliminar> -----> PARA ELIMINAR PARTIDA.\n");
	printf("./juego ordenar_partida <nombre_archivo> ------> PARA ORDENAR PARTIDA.\n");
	printf("./juego config_juego --------------------------> PARA CONFIGURAR JUEGO.\n\n");

} 

/*
 *PRE-CONDICIONES: Recibe el juego con informacion previamente cargada sobre el estado del juego.
 *POST-CONDICIONES: Devuelve diferentes tipos de mensajes dependiendo si el juego se gano o no.
 */
void finalizar(juego_t juego, bool aceptado) {
	if (estado_juego(juego) == 1) {
		printf("Haz cumplido la mision, FELICITACIONES\n");
	}else if(estado_juego(juego) == -1 && aceptado) {
		printf("HASTA LA VISTA BABY.\n");
	}
}

/*
 *PRE-CONDICIONES: Recibe un vector previamente cargado con informacion y su tope.
 *POST-CONDICIONES: Ordena el vector de menor a mayor segun el nombre del elemento.
 */
void ordenar_partida(partida_t partida[MAX_PARTIDAS], int tope) {
 	partida_t aux;
 	for (int i = 0; i < (tope); i++) {
 		for (int j = 0; j < (tope - i); j++) {
 			if (strcmp(partida[j].jugador, partida[j + 1].jugador) > 0) {
 				aux = partida[j];
 				partida[j] = partida[j + 1];
 				partida[j + 1] = aux;
 			}
 		}
	}
}

/*
 *PRE-CONDICIONES: Recibe previamente cargado con informacion un booleano y una matriz para modificar segun el booleano.
 *POST-CONDICIONES: Asigna al string el valor correspondiente segun la condicion.
 */
void asignar_ganador(partida_t nueva_partida, char gano_partida[MAX_RESPUESTA]) {
		if (nueva_partida.gano) {
			strcpy(gano_partida, SI);
		} else {
			strcpy(gano_partida, NO);
		}
}

/*
 *PRE-CONDICIONES: Recibe un archivo correctamente abierto con informacion ordenada, y un nuevo elemento para ordenar.
 *POST-CONDICIONES: Agrega un nuevo elemento en orden.
 */
void agregar_partida_archivo(FILE* arch, FILE* draft, partida_t nueva_partida) {
	char gano_partida[MAX_RESPUESTA];
	bool insertado = false;
	partida_t partida;
	int leido = fscanf(arch, FORMATO_LECTURA, partida.jugador, &partida.nivel_llegado, &partida.puntos, &partida.vidas_restantes, gano_partida);
	partida.gano = strcmp(gano_partida, NO_GANADA);
	while(leido == CAMPOS_LEIDOS) {
		if (strcmp(nueva_partida.jugador, partida.jugador) <= 0 && !insertado) {
			asignar_ganador(nueva_partida, gano_partida);
			fprintf(draft, FORMATO_ESCRITURA, nueva_partida.jugador, nueva_partida.nivel_llegado, nueva_partida.puntos, nueva_partida.vidas_restantes, gano_partida);
			insertado = true;
		}
		fprintf(draft, FORMATO_ESCRITURA, partida.jugador, partida.nivel_llegado, partida.puntos, partida.vidas_restantes,gano_partida);
		leido = fscanf(arch, FORMATO_LECTURA, partida.jugador, &(partida.nivel_llegado), &(partida.puntos), &(partida.vidas_restantes), gano_partida);
		partida.gano = strcmp(gano_partida, NO_GANADA);
	}
	if (!insertado) {
		asignar_ganador(nueva_partida, gano_partida);
		fprintf(draft, FORMATO_ESCRITURA, nueva_partida.jugador, nueva_partida.nivel_llegado, nueva_partida.puntos, nueva_partida.vidas_restantes, gano_partida);
	}
}

/*
 *PRE-CONDICIONES: Recibe un elemento para ser cargado con informacion sobre la partida que se quiere agregar.
 *POST-CONDICIONES: Carga con informacion el elemento deseado, verificando que dicha informacion sea valida.
 */
void agregar_partida_nueva(partida_t* nueva_partida) {
	char gano_partida[MAX_RESPUESTA];
	printf("----De no ingresar correctamente los datos requeridos, se les solicitaran repetidamente hasta que los mismos sean validos.----\n");
	printf("Ingrese su nombre:");
	scanf("%[^\n]", (*nueva_partida).jugador);

	do {
		printf("----MAXIMA CANTIDAD POSIBLE DE NIVELES HASTA %i----\n", MAX_NUMERO_LEVEL);
		printf("Ingrese hasta el nivel llegado:");
		scanf("%i", &(*nueva_partida).nivel_llegado);
	}while((*nueva_partida).nivel_llegado > MAX_NUMERO_LEVEL);

	do {
		printf("----MAXIMO PUNTOS POSIBLE GANADOS %i----\n",MAX_NUMERO_COINS);
		printf("Ingrese puntos ganados:");
		scanf("%i", &(*nueva_partida).puntos);
	}while(!((*nueva_partida).puntos <= MAX_NUMERO_COINS));

	do {
		printf("----MAXIMO DE VIDA POSIBLE HASTA %i----\n", MAX_NUMERO_LIVE);
		printf("Ingrese cuantas vidas te quedaron:");
		scanf("%i", &(*nueva_partida).vidas_restantes);
	}while(!((*nueva_partida).vidas_restantes <= MAX_NUMERO_LIVE));

	do{
		printf("----SOLO PUEDE INGRESAR: Si o No (MAYUSCULAS INCLUSIVE)----\n");
		printf("Ingrese si gano:");
		scanf("%s",gano_partida);
	}while(!((strcmp(gano_partida, SI) == 0)||(strcmp(gano_partida, NO) == 0)));

	if(strcmp(gano_partida, "Si") == 0) {
		(*nueva_partida).gano = true;
	}else {
		(*nueva_partida).gano = false;
	}
}

/*
 *PRE-CONDICIONES: Recibe un archivo abierto correctamente para lectura, otro archivo que servira como borrador para escritura, y un string con la informacion requerida para eliminar
 *POST-CONDICIONES: Elimina el elemento buscado en el archivo.
 */
void eliminar_partida_archivo(FILE* arch, FILE* draft, char partida_buscada[MAX_NOMBRE]) {
	char gano_partida[MAX_RESPUESTA];
	bool elimine = false;
	partida_t partida;
	int leido = fscanf(arch, FORMATO_LECTURA, partida.jugador, &partida.nivel_llegado, &partida.puntos, &partida.vidas_restantes, gano_partida);
	partida.gano = strcmp(gano_partida, NO_GANADA);
	while(leido == CAMPOS_LEIDOS) {
		if (strcmp(partida_buscada, partida.jugador) != 0 || (strcmp(partida_buscada, partida.jugador) == 0 && elimine)) {
			fprintf(draft, FORMATO_ESCRITURA, partida.jugador, partida.nivel_llegado, partida.puntos, partida.vidas_restantes,gano_partida);
		}else {
			elimine = true;
		}
		leido = fscanf(arch, FORMATO_LECTURA, partida.jugador, &(partida.nivel_llegado), &(partida.puntos), &(partida.vidas_restantes), gano_partida);
		partida.gano = strcmp(gano_partida, NO_GANADA);
	}
}

/*
 *PRE-CONDICIONES: Recibe un archivo abierto correctamente para lectura, otro archivo que servira como borrador para escritura, un vector que que luego sera ordenado.
 *POST-CONDICIONES: Se ordena el archivo segun el nombre de menor a mayor.
 */
void ordenar_partida_archivo(FILE* arch, FILE* draft, partida_t partida[MAX_PARTIDAS], int* tope) {
	char gano_partida[MAX_RESPUESTA];
	char partida_gano[MAX_RESPUESTA] = NO;
	int i = 1;
	int leido = fscanf(arch, FORMATO_LECTURA, partida[(*tope)].jugador, &(partida[(*tope)].nivel_llegado), &(partida[(*tope)].puntos), &(partida[(*tope)].vidas_restantes), gano_partida);
	partida[(*tope)].gano = strcmp(gano_partida, NO_GANADA);
	while(leido == CAMPOS_LEIDOS) {
		(*tope)++;
		leido = fscanf(arch, FORMATO_LECTURA, partida[(*tope)].jugador, &(partida[(*tope)].nivel_llegado), &(partida[(*tope)].puntos), &(partida[(*tope)].vidas_restantes), gano_partida);
		partida[(*tope)].gano = strcmp(gano_partida, NO_GANADA);

	}
	ordenar_partida(partida, (*tope));
	while (i <= (*tope)) {
		if (partida[i].gano) {
			strcpy(partida_gano, SI);	
		}
		fprintf(draft, FORMATO_ESCRITURA, partida[i].jugador, (partida[i].nivel_llegado), (partida[i].puntos), (partida[i].vidas_restantes), partida_gano);
		partida[(*tope)].gano = strcmp(partida_gano, NO_GANADA);
		i++;
	}
}

/*
 *PRE-CONDICIONES: Recibe el archivo que se quiere verificar.
 *POST-CONDICIONES: Verifica si el archivo abrio correctamente.
 */
int correcta_apertura(FILE* arch) {
	int abrio = 0;
	if (!arch) {
		abrio = -1;
	}
	return abrio; 
}

/*
 *PRE-CONDICIONES: Recibe el archivo un archivo borrador y el argumento que se introduce al iniciar el juego. 
 *POST-CONDICIONES: Si la accion que se quiere realizar corresponde a agregar partida, agrega la partida, sino no.
 */
void agregar(char* argv[], FILE* arch, FILE* draft) {
	if (strcmp(argv[1], AGREGAR_PARTIDA) == 0) {
		partida_t nueva_partida;
		agregar_partida_nueva(&nueva_partida);
		agregar_partida_archivo(arch, draft, nueva_partida);
		printf("Se ha agregado exitosamente la partida.\n");
		rename("draft.csv", argv[2]);
	} 
}

/*
 *PRE-CONDICIONES: Recibe el archivo un archivo borrador y el argumento que se introduce al iniciar el juego. 
 *POST-CONDICIONES: Si la accion que se quiere realizar corresponde a agregar partida, eliminar la partida, sino no.
 */
void eliminar(char* argv[], FILE* arch, FILE* draft) {
	if(strcmp(argv[1], ELIMINAR_PARTIDA) == 0) {
		char partida_buscada[MAX_NOMBRE];
		strcpy(partida_buscada, argv[3]);
		eliminar_partida_archivo(arch, draft, partida_buscada);
		printf("Se ha eliminado exitosamente la partida.\n");
		rename("draft.csv", argv[2]);
	}
}

/*
 *PRE-CONDICIONES: Recibe el archivo un archivo borrador y el argumento que se introduce al iniciar el juego. 
 *POST-CONDICIONES: Si la accion que se quiere realizar corresponde a agregar partida, ordenar la partida, sino no.
 */
void ordenar(char* argv[], FILE* arch, FILE* draft) {
	if(strcmp(argv[1], ORDENAR_PARTIDA) == 0) {
		partida_t partida[MAX_PARTIDAS];
		int tope = 0; 
		ordenar_partida_archivo(arch, draft, partida, &tope);
		printf("Se ha ordenado exitosamente la partida.\n");
		rename("draft.csv", argv[2]);
	}
}

/*
 *PRE-CONDICIONES: Recibe la accion que quiere realizar el usuario 
 *POST-CONDICIONES: Hace alguna de las opciones de partida segun lo que quiera el usuario. 
 */
int opciones_partida(char* argv[]) {
	if (strcmp(argv[1], AGREGAR_PARTIDA) == 0 || strcmp(argv[1], ELIMINAR_PARTIDA) == 0 || strcmp(argv[1], ORDENAR_PARTIDA) == 0) {
		FILE* arch = fopen(argv[2], "r");
		FILE* draft = fopen("draft.csv", "w");
		int apertura_arch = correcta_apertura(arch);
		int apertura_draft = correcta_apertura(draft);
		if (apertura_arch == NO_ABRIO || apertura_draft == NO_ABRIO) {
			return NO_ABRIO;
		}
		agregar(argv, arch, draft);
		eliminar(argv, arch, draft);
		ordenar(argv, arch, draft);
		fclose(draft);
		fclose(arch);
	}
	return 0;
}

/*
 *PRE-CONDICIONES: Recibe informacion sobre los valores y etiquetas de los campos que van a los que seran igualados previamente cargada y validada
 *POST-CONDICIONES: Inicializa segun su etiqueta y campo el valor correspondiente.
 */	
void asignar_valor_obstaculos(char* etiqueta, char* valor, int numero_nivel, obstaculos_nivel_t obstaculos_config[MAX_ELEMENTOS]) {
	if (strcmp(etiqueta, VELAS) == 0) {
		obstaculos_config[numero_nivel - 1].velas = atoi(valor);
	} else if (strcmp(etiqueta, POZOS) == 0) {
		obstaculos_config[numero_nivel - 1].pozos = atoi(valor);
	}  else if (strcmp(etiqueta, INTERRUPTORES) == 0) {
		obstaculos_config[numero_nivel - 1].interruptores = atoi(valor);
	}  else if (strcmp(etiqueta, PORTALES) == 0) {
		obstaculos_config[numero_nivel - 1].portales = atoi(valor);
	}
}

/*
 *PRE-CONDICIONES:  Recibe informacion sobre los valores y etiquetas de los campos que van a los que seran igualados previamente cargada y validada
 *POST-CONDICIONES: Inicializa segun su etiqueta si corresponde al campo el valor correspondiente.
 */
void asignar_valor_herramientas(char* etiqueta, char* valor, int numero_nivel, herramientas_nivel_t herramientas_config[MAX_ELEMENTOS]) {
	if (strcmp(etiqueta, ESCALERAS) == 0) {
		herramientas_config[numero_nivel - 1].escaleras = atoi(valor);
	} else if (strcmp(etiqueta, MONEDAS) == 0) {
		herramientas_config[numero_nivel - 1].monedas = atoi(valor);
	}  else if (strcmp(etiqueta, LLAVE) == 0) {
		herramientas_config[numero_nivel - 1].llave = atoi(valor);
	}  else if (strcmp(etiqueta, PUERTA) == 0) {
		herramientas_config[numero_nivel - 1].puerta = atoi(valor);
	} 
}

/*
 *PRE-CONDICIONES: Recibe informacion previamente inicializada, los archivos.
 *POST-CONDICIONES: Inicializa todos los campos que son necesarios para inicializar el juego.
 */
void leer_configuraciones(FILE* arch, obstaculos_nivel_t obstaculos_config[MAX_ELEMENTOS], herramientas_nivel_t herramientas_config[MAX_ELEMENTOS]) {
	char linea[MAX_CARACTERES];
	int leido = fscanf(arch, "%[^\n]\n", linea);
	int numero_nivel;
	while(leido == LEIDO) {
		strtok(linea, "_");
		char* etiqueta = strtok(NULL, "=");
		char* valor = strtok(NULL, "\0");
		numero_nivel =(int)linea[1] - 48;
		if (strcmp(etiqueta, MONEDAS) == 0 || strcmp(etiqueta, ESCALERAS) == 0 || strcmp(etiqueta, LLAVE) == 0 || strcmp(etiqueta, PUERTA) == 0) {
			asignar_valor_herramientas(etiqueta, valor, numero_nivel, herramientas_config);
		}else {
			asignar_valor_obstaculos(etiqueta, valor, numero_nivel, obstaculos_config);
		}
		leido = fscanf(arch, "%[^\n]\n", linea);
	}
}

/*
 *PRE-CONDICIONES: Recibe el archivo que se quiere abrir y los elementos que se quieren cargar.
 *POST-CONDICIONES: Verifica que el archivo se abra correctamente, lee el archivo e inicializa el struct de configuraciones del juego 
 */
int configurar_juego(char* argv[], obstaculos_nivel_t obstaculos_config[MAX_ELEMENTOS], herramientas_nivel_t herramientas_config[MAX_ELEMENTOS], int* tope_herramientas_config, int* tope_obstaculos_config) {
	if (strcmp(argv[1], CONFIGURAR_JUEGO) == 0) {
		FILE* arch = fopen(ARCHIVO_CONFIG, "r");
		int apertura_arch = correcta_apertura(arch);
		if (apertura_arch == NO_ABRIO) {
			return NO_ABRIO;
		}
		(*tope_obstaculos_config) = CANTIDAD_NIVELES;
		(*tope_herramientas_config) = CANTIDAD_NIVELES;
		leer_configuraciones(arch, obstaculos_config, herramientas_config);
		fclose(arch);
	}
	return 0;
}

/*
 *PRE-CONDICIONES: Recibe el juego que es un struct el cual va a recibir toda la informacion para ejecutar y jugar el juego.
 *POST-CONDICIONES: Evalua, ejecuta y verifica todas las condiciones necesarias para la ejecucion del juego.
 */
void jugar(juego_t* juego, bool quiere_jugar, obstaculos_nivel_t obstaculos_config[MAX_ELEMENTOS], herramientas_nivel_t herramientas_config[MAX_ELEMENTOS], int tope_herramientas_config, int tope_obstaculos_config, int apertura_configs) {
	if (quiere_jugar) {
		system("clear"); 
		relgas_juego();
		srand ((unsigned)time(NULL));
		inicializar_juego(&(*juego), obstaculos_config, herramientas_config, tope_obstaculos_config, tope_herramientas_config, apertura_configs);
		while(estado_juego((*juego)) == 0) {
			imprimir_terreno((*juego));
			realizar_jugada(&(*juego));
			system("clear"); 
			imprimir_terreno((*juego)); 	
		}
	}
}

/*
 *POST-CONDICIONES: Verifica que sean correctos los comandos ingresados en la terminal.
 */
bool comandos_correctos(char* argv[]) {
	bool es_correcto = false;
	if (strcmp(argv[1],AGREGAR_PARTIDA) == 0 || strcmp(argv[1],ORDENAR_PARTIDA) == 0 || strcmp(argv[1],CONFIGURAR_JUEGO) == 0) {
		es_correcto = true;
	}
	return es_correcto;
}

/*
 *POST-CONDICIONES: Si los comandos ingresados son incorrectos informa al usuario.
 */
void entrar_correctamente(bool aceptado) {
	if (!aceptado) {
		verificar_comandos();
	}
}

int main(int argc, char* argv[]) {
	obstaculos_nivel_t obstaculos_config[MAX_ELEMENTOS];
	herramientas_nivel_t herramientas_config[MAX_ELEMENTOS];
	bool aceptado = comandos_correctos(argv);
	bool quiere_jugar = false;
	int tope_obstaculos_config;
	int tope_herramientas_config;
 	int apertura_opciones = opciones_partida(argv);
	int apertura_configs = configurar_juego(argv, obstaculos_config, herramientas_config, &tope_herramientas_config, &tope_obstaculos_config);
	if (apertura_opciones == NO_ABRIO) {
		perror("No se pudo abrir el archivo:");
		return NO_ABRIO;
	}
	juego_t juego;
	presentar_juego(&quiere_jugar, argv);
	jugar(&juego, quiere_jugar, obstaculos_config, herramientas_config, tope_obstaculos_config, tope_herramientas_config, apertura_configs);
	entrar_correctamente(aceptado);
	finalizar(juego, aceptado);
	return 0;
}


