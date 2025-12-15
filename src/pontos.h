#ifndef PONTOS_H
#define PONTOS_H

typedef void* Ponto;

Ponto ponto_create(int id, double x, double y);



int ponto_getID(Ponto p);

double ponto_getCoordX(Ponto p);

double ponto_getCoordY(Ponto p);



void ponto_setID(Ponto p, int novo_id);

void ponto_setCoordX(Ponto p, double x);

void ponto_setCoordY(Ponto p, double y);



Ponto ponto_clone(Ponto p);

void ponto_destroy(Ponto p);


#endif