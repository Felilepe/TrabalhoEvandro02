#ifndef VISIBILIDADE_H
#define VISIBILIDADE_H

typedef void* Evento;

#include "anteparo.h"
#include "lista.h"
#include "arvore.h"
#include "poligono.h"

Evento visibilidade_prepare(double x_bomba, double y_bomba, Lista *anteparos, int *num_eventos);

double visibilidade_getXRay(Arvore *segmentos_visiveis, double x_origem, double y_origem, double angulo, double max_dist);

double visibilidade_getYRay(Arvore *segmentos_visiveis, double x_origem, double y_origem, double angulo, double max_dist);

Poligono visibilidade_calc(double x_bomba, double y_bomba, Lista *anteparos, char tipo_ord, double max_dist, int insert_limit);



#endif