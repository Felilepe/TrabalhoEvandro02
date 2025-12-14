#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "anteparo.h"

#define TIPO_A 5

typedef struct AnteparoSt
{
    int id, type;
    double x1, y1, x2, y2;
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
    a->x1 = x1;
    a->y1 = y1;
    a->x2 = x2;
    a->y2 = y2;
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


double anteparo_getX1(Anteparo a) {return ((anteparo*)a)->x1;}

double anteparo_getY1(Anteparo a) {return ((anteparo*)a)->y1;}

double anteparo_getX2(Anteparo a) {return ((anteparo*)a)->x2;}

double anteparo_getY2(Anteparo a) {return ((anteparo*)a)->y2;}

char *anteparo_getCor(Anteparo a) {return ((anteparo*)a)->cor;}



void anteparo_setId(Anteparo a, int novo_id) {((anteparo*)a)->id = novo_id;}

void anteparo_setX1(Anteparo a, double x1) {((anteparo*)a)->x1 = x1;}

void anteparo_setY1(Anteparo a, double y1) {((anteparo*)a)->y1 = y1;}

void anteparo_setX2(Anteparo a, double x2) {((anteparo*)a)->x2 = x2;}

void anteparo_setY2(Anteparo a, double y2) {((anteparo*)a)->y2 = y2;}

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
    free(ap);
}


