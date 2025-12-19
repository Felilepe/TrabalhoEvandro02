#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pontos.h"

typedef struct stPonto {
    double x;
    double y;
} ponto;

Ponto ponto_create(double x, double y) 
{
    ponto *p = (ponto *)malloc(sizeof(ponto));
    if(p == NULL) {
        printf("Erro ao alocar memória para o ponto em ponto_create\n");
        exit(1);
    }
    p->x = x;
    p->y = y;
    return (Ponto)p;
}



double ponto_getCoordX(Ponto p) 
{
    ponto *pt = (ponto *)p;
    return pt->x;
}

double ponto_getCoordY(Ponto p) 
{
    ponto *pt = (ponto *)p;
    return pt->y;
}



void ponto_setCoordX(Ponto p, double x) 
{
    ponto *pt = (ponto *)p;
    pt->x = x;
}



void ponto_setCoordY(Ponto p, double y) 
{
    ponto *pt = (ponto *)p;
    pt->y = y;
}



Ponto ponto_clone(Ponto p) 
{
    ponto *pt = (ponto *)p;
    return ponto_create(pt->x, pt->y);
}



void ponto_destroy(Ponto p) 
{
    free(p);
}
