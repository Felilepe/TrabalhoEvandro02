#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "pontos.h"
#include "anteparo.h"
#include "geometria.h"
#include <float.h>
#include <stddef.h>

#define EPSILON 1e-10
#define PI 3.14159265358


double geometria_calcAngle(Ponto ob, Ponto main)
{
    double x_ob = ponto_getCoordX(ob), y_ob = ponto_getCoordY(ob);
    double x_main = ponto_getCoordX(main), y_main = ponto_getCoordY(main);

    double angle = atan2((y_main - y_ob), (x_main - x_ob));

    if(angle < 0){
        angle += 2 * PI;
    }

    return angle;
}

double geometria_distSqrd(Ponto a, Ponto b)
{
    double x1 = ponto_getCoordX(a), y1 = ponto_getCoordY(a);
    double x2 = ponto_getCoordX(b), y2 = ponto_getCoordY(b);
    return (((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)));
}

int geometria_prodVet(Ponto p, Ponto q, Ponto r)
{
    double px = ponto_getCoordX(p), py = ponto_getCoordY(p);
    double qx = ponto_getCoordX(q), qy = ponto_getCoordY(q);
    double rx = ponto_getCoordX(r), ry = ponto_getCoordY(r);

    double val = (qx - px) * (ry - py) - (qy - py) * (rx - px);

    if(fabs(val) < EPSILON) return 0;
    return (fabs(val) > 0) ? 1 : 2;
}

bool geometria_isPointOnSeg(Ponto p, Ponto a, Ponto b) 
{
    double pv = geometria_prodVet(p, a, b); 
    
    if (fabs(pv) > EPSILON) return false; 

    double px = ponto_getCoordX(p);
    double py = ponto_getCoordY(p);
    double ax = ponto_getCoordX(a);
    double ay = ponto_getCoordY(a);
    double bx = ponto_getCoordX(b);
    double by = ponto_getCoordY(b);

    bool x_in = (px >= fmin(ax, bx) - EPSILON) && (px <= fmax(ax, bx) + EPSILON);
    bool y_in = (py >= fmin(ay, by) - EPSILON) && (py <= fmax(ay, by) + EPSILON);

    return x_in && y_in;
}

double geometria_distPointSeg(Ponto p, Anteparo s) 
{
    if (p == NULL || s == NULL) return 0;

    Ponto p1 = anteparo_getP1(s);
    Ponto p2 = anteparo_getP2(s);

    double x_p = ponto_getCoordX(p);
    double y_p = ponto_getCoordY(p);

    double x1_a = ponto_getCoordX(p1);
    double y1_a = ponto_getCoordY(p1);
    double x2_a = ponto_getCoordX(p2);
    double y2_a = ponto_getCoordY(p2);

    double dx = x2_a - x1_a;
    double dy = y2_a - y1_a;

    if (dx == 0 && dy == 0) {
        return sqrt(pow(x_p - x1_a, 2) + pow(y_p - y1_a, 2));
    }

    double t = ((x_p - x1_a) * dx + (y_p - y1_a) * dy) / (dx * dx + dy * dy);

    if (t < 0) t = 0;
    if (t > 1) t = 1;

    double proj_x = x1_a + t * dx;
    double proj_y = y1_a + t * dy;

    double dist_x = x_p - proj_x;
    double dist_y = y_p - proj_y;
    
    return sqrt(dist_x * dist_x + dist_y * dist_y);
}

double geometria_distAnteBomb(Anteparo a, Ponto bomb, double angle, double max_radius)
{
    if (a == NULL || bomb == NULL) return DBL_MAX;

    double bx = ponto_getCoordX(bomb);
    double by = ponto_getCoordY(bomb);

    double rx = bx + max_radius * cos(angle);
    double ry = by + max_radius * sin(angle);

    Ponto p1 = anteparo_getP1(a);
    Ponto p2 = anteparo_getP2(a);
    if (p1 == NULL || p2 == NULL) return DBL_MAX;

    double s1x = ponto_getCoordX(p1), s1y = ponto_getCoordY(p1);
    double s2x = ponto_getCoordX(p2), s2y = ponto_getCoordY(p2);

    double denom = (s2y - s1y) * (rx - bx) - (s2x - s1x) * (ry - by);

    if (fabs(denom) < EPSILON) return DBL_MAX;

    double ua = ((s2x - s1x) * (by - s1y) - (s2y - s1y) * (bx - s1x)) / denom;
    double ub = ((rx - bx) * (by - s1y) - (ry - by) * (bx - s1x)) / denom;

    if (ua >= -EPSILON && ua <= 1.0 + EPSILON && ub >= -EPSILON && ub <= 1.0 + EPSILON) {
        double dist = ua * max_radius;

        if (dist >= -EPSILON && dist <= max_radius + EPSILON) {
            return (dist < 0) ? 0 : dist;
        }
    }
    return DBL_MAX;
}

double geometria_calcDistPoints(Ponto a, Ponto b)
{
    return sqrt(geometria_distSqrd(a, b));
}

