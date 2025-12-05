#ifndef POLIGONO_H
#define POLIGONO_H

#include "lista.h"
#include <stdbool.h>


typedef void* Poligono;

Poligono poligono_create();

void poligono_insertPoint(Poligono p, double xp, double yp);

void poligono_getBoundingBox(Poligono p, double *xMin, double *yMin, double *xMax, double *yMax);

bool pligono_isInside(Poligono p, double xp, double yp);

Lista *poligono_getVertices(Poligono p);

Lista *poligono_getSegments(Poligono p);




#endif