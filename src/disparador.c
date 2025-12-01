#include <stdio.h>
#include <stdlib.h>
#include "disparador.h"
#include "formas.h"
#include "carregador.h"
#include "pilha.h"

struct disparador
{
    Carregador cesq, cdir;
    forma pos_lanc;
    double x, y;
    int id;
};

Disparador *disparador_create(int id, double x, double y, Carregador cesq, Carregador cdir) 
{
    Disparador *d = (Disparador*)malloc(sizeof(Disparador));
    if(d == NULL){
        printf("Erro na alocacao de memoria ao criar disparador.");
        exit(1);
    }

    d -> id = id;
    d -> x = x;
    d -> y = y;

    d -> cdir = cdir;
    d -> cesq = cesq;
    d ->pos_lanc = NULL;

    return d;
}



int disparador_getID(Disparador *d)
{
    if(d == NULL){
        printf("Erro: Disparador nulo.");
        return -1;
    }

    return d -> id;
}

double disparador_getCoordX(Disparador *d)
{
    if(d == NULL){
        printf("Erro: disparador nulo.");
        exit(1);
    }

    return d -> x;
}

double disparador_getCoordY(Disparador *d)
{
    if(d == NULL){
        printf("Erro: disparador nulo.");
        exit(1);
    }

    return d -> y;
}

Carregador disparador_getCEsq(Disparador *d)
{
    if(d == NULL){
        printf("Erro: disparador nulo.");
        exit(1);
    }

    return d -> cesq;
}

Carregador disparador_getCDir(Disparador *d)
{
    if(d == NULL){
        printf("Erro: disparador nulo.");
        exit(1);
    }

    return d -> cdir;
}

forma disparador_getFormaDisparo(Disparador *d)
{
    if(d == NULL){
        printf("Erro: disparador nulo.");
        exit(1);
    }

    return d -> pos_lanc;
}



void disparador_posicionarDisp(Disparador *d, double x, double y)
{
    if(d == NULL){
        printf("Erro: Disparador nulo.");
        return;
    }
    d -> x = x;
    d -> y = y;

}

void disparador_attachCarregador(Disparador *d, Carregador cesq, Carregador cdir) //atch
{
    d -> cesq = cesq;
    d -> cdir = cdir;

}

item disparador_shift(Disparador *d, char b, int n) //shft
{
    switch (b){
    case 'e': 
        for(int i = 0; i < n; i++){
            if(d -> pos_lanc != NULL && d->cdir != NULL){
                carregador_loadForma(d -> cdir, d -> pos_lanc);
                printf("[DIAG] disparador %d: carregou pos_lanc id=%d into carregador %d\n", d->id, forma_getID(d->pos_lanc), carregador_getID(d->cdir));
            }
            if (d->cesq != NULL && !carregador_isEmpty(d->cesq)) {
                d -> pos_lanc = carregador_remove(d -> cesq);
                if (d->pos_lanc) printf("[DIAG] disparador %d: pos_lanc set to id=%d (from cesq)\n", d->id, forma_getID(d->pos_lanc));
            } else {
                d -> pos_lanc = NULL;
            }
        } break;
    case 'd':
        for(int i = 0; i < n; i++){
            if(d -> pos_lanc != NULL && d->cesq != NULL){
                carregador_loadForma(d -> cesq, d ->pos_lanc);
                printf("[DIAG] disparador %d: carregou pos_lanc id=%d into carregador %d\n", d->id, forma_getID(d->pos_lanc), carregador_getID(d->cesq));
            }
            if (d->cdir != NULL && !carregador_isEmpty(d->cdir)) {
                d -> pos_lanc = carregador_remove(d -> cdir);
                if (d->pos_lanc) printf("[DIAG] disparador %d: pos_lanc set to id=%d (from cdir)\n", d->id, forma_getID(d->pos_lanc));
            } else {
                d -> pos_lanc = NULL;
            }
        } break;
    
    default: printf("Erro: Botao %c nao e uma opcao aceita.", b);
    exit(1);


    }
    
    return d -> pos_lanc;
}

void disparador_move(Disparador *d, double dx, double dy)
{
    if(d == NULL){
        printf("Erro: Disparador nulo.");
        return;
    }

    d -> x = dx;
    d -> y = dy;
}


forma disparador_disparar(Disparador *d, double dx, double dy)
{

    if (d == NULL) {
		printf("Erro: disparador nulo.\n");
		return NULL;
	}


	if (d -> pos_lanc == NULL) {
		printf("Nenhuma forma na posição de disparo.\n");
		return NULL;
	}

	forma disparada = d -> pos_lanc;
	d -> pos_lanc = NULL;

	double dispx = disparador_getCoordX(d);
	double dispy = disparador_getCoordY(d);

	double xfinal = dx + dispx;
	double yfinal = dy + dispy;

	forma_setCoordX(disparada, xfinal);
    forma_setCoordY(disparada, yfinal);


	return disparada;

}

Fila *disparador_rajada(Disparador *d, char botao, double dx, double dy, double ix, double iy, Arena *a) {
	if (d == NULL || a == NULL) {
		return NULL;
	}

	double x_original = disparador_getCoordX(d);
	double y_original = disparador_getCoordY(d);

	Fila *fila_disparos = fila_create();
	int formas_disparadas = 0;


    for (int i = 0; ; i++) {

        forma formaAtual = (forma)disparador_shift(d, botao, 1);
        if (formaAtual == NULL) {
            break;
        }

        double dx_atual = dx + (i * ix);
        double dy_atual = dy + (i * iy);

        forma formaDisparada = disparador_disparar(d, dx_atual, dy_atual);

        if (formaDisparada != NULL) {

            arena_add(a, formaDisparada);
            fila_queue(fila_disparos, formaDisparada);
            formas_disparadas++;
        }
    }

	disparador_move(d, x_original, y_original);

	return fila_disparos;
}


void disparador_removeForma(Disparador *d, forma f)
{
    if (d == NULL || f == NULL) {
		return;
	}

	if (d -> pos_lanc == f) {
		d -> pos_lanc = NULL;
	}
}



void disparador_destroy(Disparador **ptr_disparador) 
{
	if (ptr_disparador == NULL || *ptr_disparador == NULL) return;

	Disparador* d = *ptr_disparador;

	free(d);
	*ptr_disparador = NULL;
}