#ifndef GEOMETRIA_H
#define GEOMETRIA_H
#include "anteparo.h"
#include "pontos.h"
#include <stdbool.h>

double geometria_calcAngle(Ponto ob, Ponto main);

double geometria_distSqrd(Ponto a, Ponto b);

int geometria_prodVet(Ponto p, Ponto q, Ponto r);

bool geometria_isPointOnSeg(Ponto p, Ponto q, Ponto r);

double geometria_distPointSeg(Ponto p, Anteparo s);

double geometria_distAnteBomb(Anteparo a, Ponto bomb, double angle, double max_radius);

double geometria_calcDistPoints(Ponto a, Ponto b);

#endif