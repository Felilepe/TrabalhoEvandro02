#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "poligono.h"
#include "lista.h"
#include "linha.h"

typedef struct 
{
    double x, y;
} Vertice;

typedef struct 
{
    Lista* vertices;    
    double minX, minY;  
    double maxX, maxY;
} poligono;



Poligono poligono_create() 
{
    poligono* p = (poligono*) malloc(sizeof(poligono));
    if(p == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o polígono.\n");
        exit(EXIT_FAILURE);
    }
    p->vertices = lista_create();
    
    p->minX = DBL_MAX;
    p->maxX = -DBL_MAX;
    p->minY = DBL_MAX;
    p->maxY = -DBL_MAX;
    
    return (Poligono) p;
}

void poligono_insertPoint(Poligono p, double xp, double yp) 
{
    poligono* poly = (poligono*) p;
    
    Vertice* v = (Vertice*) malloc(sizeof(Vertice));
    v->x = xp;
    v->y = yp;
    

    lista_insere_inicio(poly->vertices, v); 

    if (xp < poly->minX) poly->minX = xp;
    if (xp > poly->maxX) poly->maxX = xp;
    if (yp < poly->minY) poly->minY = yp;
    if (yp > poly->maxY) poly->maxY = yp;
}

static Vertice** listaParaArray(Lista* l, int* tamanho) 
{
    int n = lista_getSize(l);
    *tamanho = n;
    if (n == 0) return NULL;

    Vertice** array = (Vertice**) malloc(n * sizeof(Vertice*));
    

    for(int i = 0; i < n; i++) {
        array[i] = (Vertice*) lista_remove_inicio(l);
    }
    
    for(int i = n-1; i >= 0; i--) {
        lista_insere_inicio(l, array[i]);
    }

    return array;
}

bool poligono_isInside(Poligono p, double xp, double yp) 
{
    poligono* poly = (poligono*) p;

    if (xp < poly->minX || xp > poly->maxX || yp < poly->minY || yp > poly->maxY) {
        return false;
    }

    int n = 0;
    Vertice** v = listaParaArray(poly->vertices, &n);
    if (v == NULL || n < 3) return false; 

    bool c = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        if (((v[i]->y > yp) != (v[j]->y > yp)) &&
            (xp < (v[j]->x - v[i]->x) * (yp - v[i]->y) / (v[j]->y - v[i]->y) + v[i]->x)) {
            c = !c;
        }
    }

    free(v); 
    return c;
}

void poligono_getBoundingBox(Poligono p, double *xMin, double *yMin, double *xMax, double *yMax) 
{
    poligono* poly = (poligono*) p;
    *xMin = poly->minX;
    *yMin = poly->minY;
    *xMax = poly->maxX;
    *yMax = poly->maxY;
}

Lista* poligono_getVertices(Poligono p) 
{
    return ((poligono*)p)->vertices;
}



Lista* poligono_getSegmentos(Poligono p) 
{
    poligono* poly = (poligono*) p;
    Lista* listaSegs = lista_create();
    
    int n = 0;
    Vertice** v = listaParaArray(poly->vertices, &n); 
    
    if (n < 2) {
        free(v);
        return listaSegs; // Retorna vazia
    }

    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n; 

        Linha l = linha_create(-1, v[i]->x, v[i]->y, v[j]->x, v[j]->y, "BLACK", false);
        
        lista_insere_inicio(listaSegs, l);
    }

    free(v); 
    return listaSegs;
}

void poligono_destroy(Poligono p) 
{
    poligono* poly = (poligono*) p;
    if (poly == NULL) return;

    if (poly->vertices != NULL) {
        while (!lista_isEmpty(poly->vertices)) {
            void* pt = lista_remove_inicio(poly->vertices); 
            free(pt); 
        }

        lista_destroy(poly->vertices);
    }

    free(poly);
}