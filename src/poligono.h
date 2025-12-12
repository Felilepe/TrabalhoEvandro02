#ifndef POLIGONO_H
#define POLIGONO_H

#include "lista.h"
#include <stdbool.h>


typedef void* Poligono;

Poligono poligono_create();



void poligono_insertPoint(Poligono p, double xp, double yp);



bool poligono_isInside(Poligono p, double xp, double yp);



void poligono_calcBoundingBox(Poligono p);


void poligono_getBoundingBox(Poligono p, double *xMin, double *yMin, double *xMax, double *yMax);

int poligono_getNumVertices(Poligono p);

Lista *poligono_getVertices(Poligono p);

Lista *poligono_getSegmentos(Poligono p);

double poligono_getXVertice(Poligono p, int index);

double poligono_getYVertice(Poligono p, int index); 



void poligono_destroy(Poligono p);





#endif