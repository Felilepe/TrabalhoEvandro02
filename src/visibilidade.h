#ifndef VISIBILIDADE_H
#define VISIBILIDADE_H
#include "pontos.h"
#include "arvore.h"
#include "lista.h"
#include "poligono.h"

typedef struct stEvent Evento;

Evento *visibilidade_prepSegments(Lista *anteparos, Ponto bomb, int *event_count);

Ponto visibilidade_rayCollision(Arvore *active_seg, Ponto bomb, double angle, double max_radius);

Poligono visibilidade_calc(Lista *anteparos, Ponto bomb, char sort_type, double max_radius, int max_inserts);

#endif