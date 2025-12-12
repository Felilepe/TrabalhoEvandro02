#include "geometria.h"

#include <float.h>

#include "linha.h"
#include <math.h>
#include <stddef.h>

#define EPSILON 1e-10
#define PI 3.14159265358

double geometria_calcAngulo(double x_obs, double y_obs, double x_p, double y_p) 
{

    double angulo = atan2(y_p - y_obs, x_p - x_obs);
    if (angulo < 0) {
        angulo += 2.0 * PI;
    }

    return angulo;
}

double geometria_distSqrd(double x1, double y1, double x2, double y2) 
{
    double deltaX = x1 - x2;
    double deltaY = y1 - y2;
    return (deltaX * deltaX) + (deltaY * deltaY);
}

int geometria_prodVet(double px, double py, double qx, double qy, double rx, double ry) 
{
    double val = (qx - px) * (ry - py) - (qy - py) * (rx - px);
    if (fabs(val) < 1e-10) return 0;
    return (val > 0) ? 1 : 2;
}

bool geometria_isPointInSeg(double px, double py, double qx, double qy, double rx, double ry) {
    return (qx <= fmax(px, rx) && qx >= fmin(px, rx) && qy <= fmax(py, ry) && qy >= fmin(py, ry));
}

double geometria_calcDistPointSeg(double px, double py, Anteparo a) 
{
    double x1 = anteparo_getX1(a);
    double y1 = anteparo_getY1(a);
    double x2 = anteparo_getX2(a);
    double y2 = anteparo_getY2(a);

    double dx = x2 - x1;
    double dy = y2 - y1;

    double len_sq = dx * dx + dy * dy;
    if (fabs(len_sq) < EPSILON) {
        return sqrt(geometria_distSqrd(px, py, x1, y1));
    }

    double t = ((px - x1) * dx + (py - y1) * dy) / len_sq;

    t = fmax(0.0, fmin(1.0, t));

    double proj_x = x1 + t * dx;
    double proj_y = y1 + t * dy;

    return sqrt(geometria_distSqrd(px, py, proj_x, proj_y));
}

double geometria_calcDistAnteBmb(Anteparo a, double x_bomba, double y_bomba, double angulo, double raio_max) 
{
    if (a == NULL) return DBL_MAX;


    double rx = x_bomba + raio_max * cos(angulo);
    double ry = y_bomba + raio_max * sin(angulo);


    double s1x = anteparo_getX1(a);
    double s1y = anteparo_getY1(a);
    double s2x = anteparo_getX2(a);
    double s2y = anteparo_getY2(a);

    double denom = (s2y - s1y) * (rx - x_bomba) - (s2x - s1x) * (ry - y_bomba);

    if (fabs(denom) < EPSILON) return DBL_MAX;

    double ua = ((s2x - s1x) * (y_bomba - s1y) - (s2y - s1y) * (x_bomba - s1x)) / denom;
    double ub = ((rx - x_bomba) * (y_bomba - s1y) - (ry - y_bomba) * (x_bomba - s1x)) / denom;

    if (ua >= -EPSILON && ua <= 1.0 + EPSILON && ub >= -EPSILON && ub <= 1.0 + EPSILON) {
        double dist = ua * raio_max;

        if (dist >= -EPSILON && dist <= raio_max + EPSILON) {
            return (dist < 0) ? 0 : dist;
        }
    }
    return DBL_MAX;
}

double geometria_calcDistPoint(double xp1, double yp1, double xp2, double yp2) {return sqrt(geometria_distSqrd(xp1, yp1, xp2, yp2));}
