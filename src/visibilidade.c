#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "visibilidade.h"
#include "arvore.h"
#include "formas.h"
#include "poligono.h"
#include "retangulo.h"
#include "anteparo.h"

#define EPSILON 1e-10
#define PI 3.14159265358

typedef struct stEvento
{
    int id;
    double angulo;
    double distancia_estatica;
    int tipo_evento; //0 = INICIO, 1 = FIM
    Anteparo *segmento;

} evento;

static double calc_angle(double x_obs, double y_obs,  double x_principal, double y_principal) 
{
    double angulo = atan2(y_principal - y_obs, x_principal - x_obs);
    if (angulo < 0) {
        angulo += 2.0 * PI;
    }

    return angulo;
}

evento visibilidade_prepare(double x_bomba, double y_bomba, Lista *anteparos, int *num_eventos);
