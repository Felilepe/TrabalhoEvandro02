#ifndef GEOMETRIA_H
#define GEOMETRIA_H
#include "anteparo.h"
#include "pontos.h"
#include <stdbool.h>

/****************************************************************************************
* @brief Calcula o ângulo entre um ponto principal e um observador.
* @param ob O ponto do observador (origem relativa).
* @param main O ponto alvo.
* @return Retorna o ângulo em radianos (entre 0 e 2*PI).
****************************************************************************************/
double geometria_calcAngle(Ponto ob, Ponto main);

/****************************************************************************************
* @brief Calcula a distância ao quadrado entre dois pontos.
* @param a O primeiro ponto.
* @param b O segundo ponto.
* @return Retorna a distância ao quadrado (evita o custo da raiz quadrada).
****************************************************************************************/
double geometria_distSqrd(Ponto a, Ponto b);

/****************************************************************************************
* @brief Calcula a orientação relativa (produto vetorial 2D) de três pontos ordenados.
* @param p O primeiro ponto.
* @param q O segundo ponto.
* @param r O terceiro ponto.
* @return Retorna 0 se colineares, 1 se sentido positivo/anti-horário, 2 se negativo/horário.
****************************************************************************************/
int geometria_prodVet(Ponto p, Ponto q, Ponto r);

/****************************************************************************************
* @brief Verifica se um ponto pertence a um segmento de reta definido por dois pontos.
* @param p O ponto a ser verificado.
* @param a O ponto inicial do segmento.
* @param b O ponto final do segmento.
* @return Retorna true se o ponto estiver no segmento, false caso contrário.
****************************************************************************************/
bool geometria_isPointOnSeg(Ponto p, Ponto a, Ponto b);

/****************************************************************************************
* @brief Calcula a menor distância entre um ponto e um anteparo (segmento).
* @param p O ponto.
* @param s O anteparo (segmento).
* @return Retorna a distância (double).
****************************************************************************************/
double geometria_distPointSeg(Ponto p, Anteparo s);

/****************************************************************************************
* @brief Calcula a distância de intersecção entre uma "bomba" (raio) e um anteparo.
* @param a O anteparo alvo.
* @param bomb O ponto de origem da bomba/raio.
* @param angle O ângulo de disparo/trajetória.
* @param max_radius O raio máximo de alcance.
* @return Retorna a distância até o impacto ou DBL_MAX se não houver intersecção.
****************************************************************************************/
double geometria_distAnteBomb(Anteparo a, Ponto bomb, double angle, double max_radius);

/****************************************************************************************
* @brief Calcula a distância euclidiana entre dois pontos.
* @param a O primeiro ponto.
* @param b O segundo ponto.
* @return Retorna a distância (double).
****************************************************************************************/
double geometria_calcDistPoints(Ponto a, Ponto b);

#endif