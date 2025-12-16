#ifndef POLIGONO_H
#define POLIGONO_H
#include "pontos.h"
#include "lista.h"
#include <stdbool.h>

typedef void* Poligono;

Poligono poligono_create();

void poligono_insertVertice(Poligono p, Ponto v);

Ponto poligono_copyPasteVertice(Poligono p, Ponto v);

bool poligono_isEmpty(Poligono p);

void poligono_calcBBox(Poligono p);

bool poligono_isInside(Poligono p, double xp, double yp);




void poigono_getBBox(Poligono, double *xMin, double *xMax, double *yMin, double *yMax);

Ponto poligono_getVertice(Poligono p);

Ponto poligono_getLastVertice(Poligono p);

Lista poligono_getVerticeList(Poligono p);

Lista poligono_getSegments(Poligono p);




#endif