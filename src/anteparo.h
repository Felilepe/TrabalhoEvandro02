#ifndef ANTEPARO_H
#define ANTEPARO_H
#include "pontos.h"

typedef void *Anteparo;

Anteparo anteparo_create(int id, double x1, double y1, double x2, double y2, const char *cor);


int anteparo_getID(Anteparo a);

int anteparo_getType(Anteparo a);

double anteparo_getCoordX1(Anteparo a);

double anteparo_getCoordY1(Anteparo a); 

double anteparo_getCoordX2(Anteparo a);

double anteparo_getCoordY2(Anteparo a);

char *anteparo_getCor(Anteparo a);

Ponto anteparo_getP1(Anteparo a);

Ponto anteparo_getP2(Anteparo a);



void anteparo_setId(Anteparo a, int novo_id);

void anteparo_setCoordX1(Anteparo a, double x1);

void anteparo_setCoordY1(Anteparo a, double y1);

void anteparo_setCoordX2(Anteparo a, double x2);

void anteparo_setCoordY2(Anteparo a, double y2);

void anteparo_setCor(Anteparo a, const char *cor);



void anteparo_destroy(Anteparo a);

#endif