#ifndef PONTOS_H
#define PONTOS_H

typedef void* Ponto;

Ponto ponto_create(double x, double y);




double ponto_getCoordX(Ponto p);

double ponto_getCoordY(Ponto p);



void ponto_setCoordX(Ponto p, double x);

void ponto_setCoordY(Ponto p, double y);



Ponto ponto_clone(Ponto p);

void ponto_destroy(Ponto p);


#endif