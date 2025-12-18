#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "anteparo.h"
#include "pontos.h"

#define TIPO_A 5

typedef struct AnteparoSt
{
    int id, type;
    Ponto p1, p2;
    char *cor;
} anteparo;



Anteparo anteparo_create(int id, double x1, double y1, double x2, double y2, const char *cor) 
{
    anteparo* a = (anteparo*) malloc(sizeof(anteparo));
    if (a == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o anteparo em anteparo_create.\n");
        exit(EXIT_FAILURE);
    }
    a->id = id;
    a->type = TIPO_A;
    a -> p1 = NULL;
    a -> p2 = NULL;
    ponto_setCoordX(a -> p1, x1);
    ponto_setCoordY(a -> p1, y1);
    ponto_setCoordX(a -> p2, x2);
    ponto_setCoordY(a -> p2, y2);
    a->cor = (char*) malloc((strlen(cor) + 1) * sizeof(char));
    if (a->cor == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a cor do anteparo em anteparo_create.\n");
        free(a);
        exit(EXIT_FAILURE);
    }

    strcpy(a->cor, cor);


    return a;
}



int anteparo_getID(Anteparo a) {return ((anteparo*)a)->id;}

int anteparo_getType(Anteparo a) {return ((anteparo*)a)->type;}

double anteparo_getCoordX1(Anteparo a) {return ponto_getCoordX(((anteparo*)a) -> p1);}

double anteparo_getCoordY1(Anteparo a) {return ponto_getCoordY(((anteparo*)a) -> p1);}

double anteparo_getCoordX2(Anteparo a) {return ponto_getCoordX(((anteparo*)a) -> p2);}

double anteparo_getCoordY2(Anteparo a) {return ponto_getCoordY(((anteparo*)a) -> p2);}

char *anteparo_getCor(Anteparo a) {return ((anteparo*)a)->cor;}

Ponto anteparo_getP1(Anteparo a) {return ((anteparo*)a) -> p1;}

Ponto anteparo_getP2(Anteparo a) {return ((anteparo*)a) -> p2;}



void anteparo_setId(Anteparo a, int novo_id) {((anteparo*)a)->id = novo_id;}

void anteparo_setCoordX1(Anteparo a, double x1) {ponto_setCoordX(((anteparo*)a) -> p1, x1);}

void anteparo_setCoordY1(Anteparo a, double y1) {ponto_setCoordY(((anteparo*)a) -> p1, y1);}

void anteparo_setCoordX2(Anteparo a, double x2) {ponto_setCoordX(((anteparo*)a) -> p2, x2);}

void anteparo_setCoordY2(Anteparo a, double y2) {ponto_setCoordY(((anteparo*)a) -> p2, y2);}

void anteparo_setCor(Anteparo a, const char *cor) 
{
    anteparo* ap = (anteparo*) a;
    free(ap->cor);
    ap->cor = (char*) malloc((strlen(cor) + 1) * sizeof(char));
    if (ap->cor == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a cor do anteparo em anteparo_seCor.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(ap->cor, cor);
}



void anteparo_destroy(Anteparo a) 
{
    anteparo* ap = (anteparo*) a;
    free(ap->cor);
    ponto_destroy(ap -> p1);
    ponto_destroy(ap -> p2);
    free(ap);
}


