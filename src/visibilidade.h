#ifndef VISIBILIDADE_H
#define VISIBILIDADE_H

typedef void* evento;

#include "anteparo.h"
#include "lista.h"
#include "arvore.h"
#include "poligono.h"

evento visibilidade_prepare(double x_bomba, double y_bomba, Lista *anteparos, int *num_eventos);

double visibilidade_getXRay(Arvore *segmentos, double x_origem, double y_origem, double angulo, double max_dist);

double visibilidade_getYRay(Arvore *segmentos, double x_origem, double y_origem, double angulo, double max_dist);

Poligono visibilidade_calc(double x_bomba, double y_bomba, Lista *anteparos, char tipo_ord, double max_dist, int insert_limit);



#endif