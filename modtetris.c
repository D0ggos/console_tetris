#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>

#define FILAS 16
#define COLUMNAS 10

char grilla[FILAS][COLUMNAS] = {0};
int puntaje = 0;
int linea = 0;
char inicio = true; // bool true y false esta en ncurses
suseconds_t velocidad = 400000;
int siguiente;
int Puntaje_Mayor = 0;
char Top[100];
int aux = 8;
int nivel = 1;
char Jugador_Maximo[100];
char Jugador[100];

typedef struct {
	char **array;
	int ancho, cols, filas;
} Piezas;
Piezas actualizar;

const Piezas PiezasArray[7] = {
	// L1
	{(char *[]){
		(char []){0,0,1,0},
		(char []){1,1,1,0},
		(char []){0,0,0,0},
		(char []){0,0,0,0}}, 3},
	// Z1
	{(char *[]){
		(char []){1,1,0,0},
		(char []){0,1,1,0},
		(char []){0,0,0,0},
		(char []){0,0,0,0}}, 3},
	// I
	{(char *[]){
		(char []){1,1,1,1},
		(char []){0,0,0,0},
		(char []){0,0,0,0},
		(char []){0,0,0,0}}, 4},
	// L2
	{(char *[]){
		(char []){1,0,0,0},
		(char []){1,1,1,0},
		(char []){0,0,0,0},
		(char []){0,0,0,0}}, 3},
	// Cuadrado
	{(char *[]){
		(char []){1,1,0,0},
		(char []){1,1,0,0},
		(char []){0,0,0,0},
		(char []){0,0,0,0}}, 2},
	// Z2
	{(char *[]){
		(char []){0,1,1,0},
		(char []){1,1,0,0},
		(char []){0,0,0,0},
		(char []){0,0,0,0}}, 3},
	// T
	{(char *[]){
		(char []){0,1,0,0},
		(char []){1,1,1,0},
		(char []){0,0,0,0},
		(char []){0,0,0,0}}, 3},
};

Piezas NuevaPieza(Piezas piezas) {

	Piezas nuepieza = piezas;
	char **copypieza = piezas.array;
	nuepieza.array = (char**)malloc(nuepieza.ancho * sizeof(char*));
	for (int i = 0; i < nuepieza.ancho; i++) {
		nuepieza.array[i] = (char*)malloc(nuepieza.ancho * sizeof(char));
		for (int j = 0; j < nuepieza.ancho; j++) {
			nuepieza.array[i][j] = copypieza[i][j];
		}
	}
	return nuepieza;
}

void EliminarPieza(Piezas piezas) {

	for (int i = 0; i < piezas.ancho; i++) {
		free(piezas.array[i]);
	}
	free(piezas.array);
}

int RevisarPosicion(Piezas piezas) {

	char **array = piezas.array;
	for (int i = 0; i < piezas.ancho; i++) {
		for (int j = 0; j < piezas.ancho; j++) {
			if ((piezas.cols + j < 0 || piezas.cols + j >= COLUMNAS || piezas.filas + i >= FILAS)) {
				if (array[i][j]) return false;
			} else if (grilla[piezas.filas + i][piezas.cols + j] && array[i][j]) {
				return false;
			}
		}
	}

	return true;
}

void NuePiezasRandom() {

	int actual;
	actual = siguiente;
	siguiente = rand()%7;
	Piezas nuepieza = NuevaPieza(PiezasArray[actual]);
	nuepieza.cols = rand()%(COLUMNAS - nuepieza.ancho + 1);
	nuepieza.filas = 0;
	EliminarPieza(actualizar);
	actualizar = nuepieza;
	if (!RevisarPosicion(actualizar)) {
		inicio = false;
	}
}

void SiguientePieza() {

	switch(siguiente)
	{
		case 0:
			printw("                                          @\n");
            printw("                                      @ @ @\n");
			return;

		case 1:
			printw("                                      @ @\n");
            printw("                                        @ @");
			return;

		case 2:
			printw("                                      @ @ @ @\n");
            printw("                                               ");
			return;
		case 3:
			printw("                                      @\n");
            printw("                                      @ @ @");
			return;
		case 4:
			printw("                                      @ @\n");
            printw("                                      @ @");
			return;
		case 5:
			printw("                                        @ @\n");
            printw("                                      @ @");
			return;
		case 6:
			printw("                                        @\n");
            printw("                                      @ @ @");
			return;		
	}
}

void RotarPiezas(Piezas piezas) {

	Piezas temp = NuevaPieza(piezas);
	int k;
	int ancho = piezas.ancho;
	for (int i = 0; i < ancho; i++) {
		for (int j = 0, k = ancho - 1; j < ancho; j++, k--) {
			piezas.array[i][j] = temp.array[k][i];
		}
	}
	EliminarPieza(temp);
}


void DefGrilla(){

	for(int i = 0; i < actualizar.ancho; i++) {
		for (int j = 0; j < actualizar.ancho; j++) {
			if (actualizar.array[i][j]) {
				grilla[actualizar.filas + i][actualizar.cols + j] = actualizar.array[i][j];
			}
		}
	}
}
void PuntajeLineasyBorrado() {

	int cont = 0;
	for (int i = 0; i < FILAS; i++) {
		int sum = 0;
		for (int j = 0; j < COLUMNAS; j++) {
			sum += grilla [i][j];
		}

		if (sum == COLUMNAS) {
			cont++;
			int l,k;
			for (k = i; k >= 1; k--) {
				for (l = 0; l < COLUMNAS; l++) {
					grilla[k][l] = grilla[k - 1][l];
				}
			}
		}
	}
	
	puntaje += 100 * cont;
	linea += 1 * cont;
	velocidad = velocidad - 2000 * nivel;

	if (linea >= aux) {
		aux = aux + 8;
		nivel++;
	}
}

void ObtenerPuntajeMayor() {

    FILE* txt;
    txt = fopen("TOP.txt", "rt");
    fgets (Top, 100, txt);
    fclose(txt);
    Puntaje_Mayor = atoi(Top);
}

void EscribirPuntajeMayor() {

	if (!puntaje || puntaje < Puntaje_Mayor) return;
	FILE* txt;
	txt = fopen("TOP.txt","wt");
	fprintf(txt,"%d",puntaje);
	fclose(txt);
    return;
}

void IngresarJugadorConMayorPuntaje() {

	if (!puntaje || puntaje<Puntaje_Mayor) return;
	FILE* txt2;
	txt2 = fopen("Jugador.txt","wt");
	fprintf(txt2,"%s",Jugador);
	fclose(txt2);
    return;
}

void ObtenerJugadorConMayorPuntaje() {

	FILE* txt2;
	txt2 = fopen("Jugador.txt","rt");
	fgets (Jugador_Maximo, 100, txt2);
	fclose(txt2);
}

void ImpimirGrilla() {

	char memoria[FILAS][COLUMNAS] = {0};
	int i, j;
	for (i = 0; i < actualizar.ancho; i++) {
		for (j = 0; j < actualizar.ancho; j++) {
			if (actualizar.array[i][j]) {
				memoria[actualizar.filas + i][actualizar.cols + j] = actualizar.array[i][j];
			}
		}
	}

	clear();
	for (i = 0; i < COLUMNAS - 9; i++)
	printw("	  _______   _        _              _    _     _           \n" );
	printw("	 |__   __| | |      (_)            | |  | |   | |          \n" ); 
	printw("	    | | ___| |_ _ __ _ ___         | |  | | __| | ___  ___ \n" );
	printw("	    | |/ _   __| '__| / __|        | |  | |/ _` |/ _ / __| \n" );
	printw("	    | |  __/ |_| |  |  __          | |__| | (_| |  __/ (__ \n" ); 
	printw("	    |_| ___| __|_|  |_|___/          ____/  __,_| ___|___| \n\n" );

	for (i = 0; i < FILAS; i++) {
		printw("                       <! ");   
		for (j = 0; j < COLUMNAS; j++) {
			printw("%c ", (grilla[i][j] + memoria[i][j])? 64 : '.');
		}
		printw("!>\n");
	}
	printw("                       <!=====================!>"); 
	printw("\n\n                         Mejor Jugador:%s %d\n",Jugador_Maximo, Puntaje_Mayor);
	printw("\n                         Puntaje: %d", puntaje);
	printw("\n                         Lineas: %d", linea);
	printw("\n                         Nivel: %d \n", nivel);
	printw("\n                         Siguiente:\n");
	SiguientePieza();
}

void MoverPiezas (int accion) {

	Piezas temp = NuevaPieza(actualizar);

	switch(accion){
        case 'a':
        		temp.cols--;
        		if(RevisarPosicion(temp))
        			actualizar.cols--;
        		break;

		case 'd':
				temp.cols++;
				if(RevisarPosicion(temp))
					actualizar.cols++;
				break;

		case 's':
				temp.filas++;
				if (RevisarPosicion(temp))
					actualizar.filas++;
				else {
					DefGrilla();
					PuntajeLineasyBorrado();
					NuePiezasRandom();
					}
				break;

		case 'w':
				RotarPiezas(temp);
					if(RevisarPosicion(temp))
					RotarPiezas(actualizar);
				break;

	}	
	EliminarPieza(temp);
	ImpimirGrilla();
}

struct timeval antes, ahora; //timeval struct de <sys/time.h>

int ParaActualizar() {

	return(((suseconds_t)(ahora.tv_sec*1000000 + ahora.tv_usec) - ((suseconds_t) antes.tv_sec*1000000 + antes.tv_usec)) > velocidad);
}

int main(){
	system("clear");
	printf("Ingresa nombre de jugador:");
	scanf("%s",Jugador);
	srand(time(0));
	puntaje = 0;
	int c;
	initscr();// ncurses
	gettimeofday(&antes, NULL);
	timeout(1);
	NuePiezasRandom();
	ImpimirGrilla();
	ObtenerPuntajeMayor();
	ObtenerJugadorConMayorPuntaje();

	while(inicio){
		if ((c = getch ()) != ERR) {
			MoverPiezas(c);
		}
		gettimeofday(&ahora, NULL);
		if (ParaActualizar()) {
			MoverPiezas('s');
			gettimeofday(&antes, NULL);
		}
		EscribirPuntajeMayor();
		IngresarJugadorConMayorPuntaje();
	}

	EliminarPieza(actualizar);
	endwin();//ncurses
	for (int i = 0; i < FILAS; i++) {
		for (int j = 0; j < COLUMNAS; j++) {
			printf("%c ", grilla[i][j]? 64 : '.');
		}
		printf("\n");
	}
	printf("\n                         Perdiste\n");
	printf("\n                         Puntaje: %d\n", puntaje);
	printf("\n                         Lineas: %d\n", linea);
	system("clear");

    return 0;
}







	









	
