#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "linha.h"
#include <math.h>

#define TIPO_L 3

typedef struct linha
{
    int id, type;
    double x1, x2, y1, y2;
    char* cor;
    bool isDotted;
}linha;

Linha linha_create(int id, double x1, double y1, double x2, double y2, char* cor, bool isDotted)
{
    linha *l = malloc(sizeof(linha));
    if(l == NULL){
        printf("Erro na alocacao de memoria ao criar linha");
        exit(1);
    }
    
    l -> id = id;
    l -> type = TIPO_L;
    l -> x1 = x1;
    l -> x2 = x2;
    l -> y1 = y1;
    l -> y2 = y2;
    l -> isDotted = isDotted;
    l -> cor = (char*)malloc(strlen(cor) + 1);
    if(l-> cor == NULL){
        printf("Erro na alocacao de memoria ao atribuir cor a linha");
        exit(1);
    }
    strcpy(l->cor, cor);

    return l;
}



int linha_getID(Linha l){return ((linha*)l) -> id;}

double linha_getCoordX1(Linha l) {return ((linha*)l) -> x1;}

double linha_getCoordX2(Linha l) {return ((linha*)l) -> x2;}

double linha_getCoordY1(Linha l) {return ((linha*)l) -> y1;}

double linha_getCoordY2(Linha l) {return ((linha*)l) -> y2;}

char *linha_getCor(Linha l) {return ((linha*)l) -> cor;}

bool linha_getIsDotted(Linha l) {return ((linha*)l) -> isDotted;}

int linha_getType(Linha l) {return ((linha*)l) -> type;}


double linha_calcArea(Linha l)
{
    double x1 = ((linha*)l) -> x1;
    double x2 = ((linha*)l) -> x2;
    double y1 = ((linha*)l) -> y1;
    double y2 = ((linha*)l) -> y2;

    return sqrt(pow((x2-x1), 2) + pow((y2-y1), 2)) * 2;

}



void linha_setCoordX1(Linha l, double x1) {((linha*)l) -> x1 = x1;}

void linha_setCoordX2(Linha l, double x2) {((linha*)l) -> x2 = x2;}

void linha_setCoordY1(Linha l, double y1) {((linha*)l) -> y1 = y1;}

void linha_setCoordY2(Linha l, double y2) {((linha*)l) -> y2 = y2;}

void linha_setCor(Linha l, char* cor)
{
    free(((linha*)l) -> cor);
    ((linha*)l) -> cor = (char*)malloc(strlen(cor) + 1);
    strcpy(((linha*)l) -> cor, cor);
}



void linha_destroy(Linha l)
{
    linha *temp = (linha*)l;
    
    free(temp -> cor);
    free(temp);
}


