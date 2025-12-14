#ifndef ANTEPARO_H
#define ANTEPARO_H

typedef void *Anteparo;

Anteparo anteparo_create(int id, double x1, double y1, double x2, double y2, const char *cor);


int anteparo_getId(Anteparo a);

double anteparo_getX1(Anteparo a);

double anteparo_getY1(Anteparo a); 

double anteparo_getX2(Anteparo a);

double anteparo_getY2(Anteparo a);

char *anteparo_getCor(Anteparo a);



void anteparo_setId(Anteparo a, int novo_id);

void anteparo_setX1(Anteparo a, double x1);

void anteparo_setY1(Anteparo a, double y1);

void anteparo_setX2(Anteparo a, double x2);

void anteparo_setY2(Anteparo a, double y2);

void anteparo_setCor(Anteparo a, const char *cor);



void anteparo_destroy(Anteparo a);

#endif