#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "circulo.h"

#define TIPO_C 1
#define PI 3.14159265359

typedef struct circulo 
{
    int id, type;
    double x, y, r;
    char *corborda, *corpreench;

}circulo;


Circulo circulo_create(int id, double x, double y, double r, char *corborda, char *corpreench)
{
    circulo *circ= malloc(sizeof(circulo));
    if(circ == NULL){
        printf("Erro na alocação de memória ao criar circulo");
        exit(1);
    }
    
    circ -> id = id;
    circ -> type = TIPO_C;
    circ -> x = x;
    circ -> y = y;
    circ -> r = r;
    
    circ -> corborda = (char*)malloc(strlen(corborda) + 1);
    if(circ -> corborda == NULL){
        printf("Erro na alocação da memorio ao atribuir cor da borda");
        exit(1);
    }
    strcpy(circ -> corborda, corborda);

    circ -> corpreench = (char*)malloc(strlen(corpreench) + 1);
    if(circ -> corpreench == NULL){
        printf("Erro na alocacao de memoria ao atribuir cor de preenchimento");
        exit(1);
    }
    strcpy(circ -> corpreench, corpreench);

    return circ;
}



double circulo_getCoordX(Circulo c) {return ((circulo*)c) -> x;}

double circulo_getCoordY(Circulo c) {return ((circulo*)c) -> y;}

int circulo_getID(Circulo c) {return((circulo*)c) -> id;}

double circulo_getRaio(Circulo c) {return ((circulo*)c) -> r;}

char* circulo_getCorBorda(Circulo c) {return ((circulo*)c) -> corborda;}

char* circulo_getCorPreench(Circulo c) {return ((circulo*)c) -> corpreench;}

int circulo_getType(Circulo c) {return ((circulo*)c) -> type;}



double circulo_calcArea(Circulo c) 
{
    double raio = ((circulo*)c) -> r; 
    return (PI * (raio*raio));
}



void circulo_setCoordX(Circulo c, double x) {((circulo*)c) -> x = x;}

void circulo_setCoordY(Circulo c, double y) {((circulo*)c) -> y = y;}

void circulo_setRaio(Circulo c, double r) {((circulo*)c) -> r = r;}

void circulo_setCorBorda(Circulo c, char* corborda)
{
    free(((circulo*)c) -> corborda);
    ((circulo*)c) -> corborda = (char*)malloc(strlen(corborda) + 1);
    strcpy(((circulo*)c) -> corborda, corborda); 
}

void circulo_setCorPreench(Circulo c, char* corpreench)
{
    free(((circulo*)c) -> corpreench);
    ((circulo*)c) -> corpreench = (char*)malloc(strlen(corpreench) + 1);
    strcpy(((circulo*)c) -> corpreench, corpreench); 
}



void circulo_destroy(Circulo c)
{
    circulo* temp = (circulo*)c;

    free(temp ->corborda);
    free(temp ->corpreench);
    free(temp);
}



Anteparo circulo_anteparo(Circulo c, char orientacao, int ant_id)
{
    circulo* circ = (circulo*) c;
    
    if(circ == NULL || ant_id == NULL){
        printf("Erro: circulo ou ant_id nulo na funcao circulo_anteparo\n");
        exit(1);
    }

    switch(orientacao){
        case 'H': case 'h': {
            return anteparo_create(++(ant_id), (circ -> x - circ -> r), circ -> y, 
            (circ -> x + circ -> r), circ -> y, circulo_getCorBorda(c));
            break;
        }
        case 'V': case 'v': {
            return anteparo_create(++(ant_id), circ -> x, (circ -> y - circ -> r), 
            circ -> x, (circ -> y + circ -> r), circulo_getCorBorda(c));
            break;
        }
        default: {
            printf("Erro: orientacao invalida na funcao circulo_anteparo\n");
            exit(1);
            break;
        }
    }
    return NULL;
}