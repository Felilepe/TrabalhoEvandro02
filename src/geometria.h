#ifndef GEOMETRIA_H
#define GEOMETRIA_H
#include "anteparo.h"
#include "pontos.h"
#include <stdbool.h>

double geometria_calcAngle(Ponto ob, Ponto main);

double geometria_distSqrd(double x1, double y1, double x2, double y2);

double geometria_prodVet(double px, double py, double qx, double qy, double rx, double ry);

bool geometria_isPointOnSeg(double px, double py, double qx, double qy, double rx, double ry);

double geometria_distPointSeg(Ponto p, Anteparo s);

double geometria_distAnteBomb(Anteparo a, Ponto bomb, double angle, double max_radius);

double geometria_calcDistPoints(Ponto a, Ponto b);

#endif