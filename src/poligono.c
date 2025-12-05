#include <stdio.h>
#include <stdlib.h>
#include "poligono.h"
#include "lista.h"
#include <stdbool.h>
#include "linha.h"

typedef struct stPonto //struct auxiliar para representar pontos
{
    double x;
    double y;
    bool isLast; // Indica se é o último ponto inserido   
} ponto;


typedef struct stPoligono 
{
    Lista* vertices; // Lista de pontos (vértices)
    Lista* segmentos; // Lista de segmentos (arestas)
    double xMin, yMin, xMax, yMax; // Bounding Box
} poligono;

Poligono poligono_create() 
{
    poligono* p = (poligono*)malloc(sizeof(poligono));
    if (p == NULL) {
        printf("Erro na alocacao de memoria ao criar poligono.");
        exit(1);
    }

    p->vertices = lista_create();
    p->segmentos = lista_create();
    p->xMin = p->yMin = p->xMax = p->yMax = 0.0;

    return (Poligono)p;
}

void poligono_insertPoint(Poligono p, double xp, double yp)
{
    poligono* poly = (poligono*)p;

    ponto* novoPonto = (ponto*)malloc(sizeof(ponto));
    if (novoPonto == NULL) {
        printf("Erro na alocacao de memoria ao criar ponto.");
        exit(1);
    }
    novoPonto->x = xp;
    novoPonto->y = yp;
    novoPonto->isLast = false;

    lista_insere_fim(poly->vertices, (item)novoPonto);
}

