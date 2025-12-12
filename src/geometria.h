#ifndef GEOMETRIA_H
#define GEOMETRIA_H
#include <stdbool.h>

#include "anteparo.h"

/******************************************************************
*@brief TAD GEOMETRIA - Algoritmos de computação geométrica.
*
*Implementa operações geométricas fundamentais para computação gráfica,
*incluindo cálculos de ângulos, distâncias, orientações vetoriais,
*intersecções de raios com segmentos e testes de posicionamento.
*Suporta varredura angular para visibilidade e rastreamento de raios.
******************************************************************/

/******************************************************************
*@brief Calcula o ângulo de um ponto em relação a um observador.
*@param obs Ponto de observação usado como referência angular.
*@param principal Ponto que está sendo observado para o cálculo.
*@return O ângulo calculado em radianos.
*@details Utiliza atan2 para calcular o ângulo polar do vetor
*entre obs e principal, com resultado entre -π e π.
******************************************************************/
double geometria_calcAngulo(double x_obs, double y_obs, double x_p, double y_p); 

/******************************************************************
*@brief Calcula a distância ao quadrado entre dois pontos.
*@param x1 Coordenada X do primeiro ponto.
*@param y1 Coordenada Y do primeiro ponto.
*@param x2 Coordenada X do ponto de destino.
*@param y2 Coordenada Y do ponto de destino.
*@return A distância ao quadrado entre os dois pontos.
*@details Evita a operação sqrt() para economia computacional,
*mantendo a ordem relativa entre distâncias.
******************************************************************/
double geometria_distSqrd(double x1, double y1, double x2, double y2);

/******************************************************************
*@brief Determina a orientação geométrica de três pontos no plano.
*@param px Coordenada X do ponto p (origem).
*@param py Coordenada Y do ponto p (origem).
*@param qx Coordenada X do ponto q.
*@param qy Coordenada Y do ponto q.
*@param rx Coordenada X do ponto r.
*@param ry Coordenada Y do ponto r.
*@return 0 se colineares, 1 se virada à esquerda, 2 se à direita.
*@details Utiliza o produto vetorial dos vetores PQ e PR para
*identificar a orientação no sentido horário ou anti-horário.
*Essencial para algoritmos de convexidade e interseção de segmentos.
*@warning Implementa determinação via produto vetorial 2D cruzado.
******************************************************************/
int geometria_prodVet(double px, double py, double qx, double qy, double rx, double ry);

/******************************************************************
*@brief Verifica se um ponto está dentro do retângulo definido por dois pontos.
*@param px Coordenada X do ponto p (extremo do segmento).
*@param py Coordenada Y do ponto p (extremo do segmento).
*@param qx Coordenada X do ponto q (ponto a ser testado).
*@param qy Coordenada Y do ponto q (ponto a ser testado).
*@param rx Coordenada X do ponto r (outro extremo do segmento).
*@param ry Coordenada Y do ponto r (outro extremo do segmento).
*@return true se q está dentro do bounding box de p e r, false caso contrário.
*@details Checagem auxiliar para testes de colinearidade.
*@warning **NÃO** verifica se o ponto está no segmento.
*Retorna true para qualquer ponto no retângulo formado por p e r.
*Sempre combine com geometria_prodVet para teste de segmento completo.
******************************************************************/
bool geometria_isPointInSeg(double px, double py, double qx, double qy, double rx, double ry);

/******************************************************************
*@brief Calcula a distância entre um ponto e um segmento de reta.
*@param p Ponteiro para o ponto.
*@param a Ponteiro para o segmento (anteparo).
*@return A distância mínima entre ponto e segmento.
*@details Projeta ortogonalmente o ponto no segmento,
*considerando endpoints se a projeção cair fora do intervalo.
******************************************************************/
double geometria_calcDistPointSeg(double px, double py, Anteparo a); 

/******************************************************************
*@brief Calcula a distância de uma bomba até a interseção com anteparo.
*@param a Anteparo a ser testado.
*@param p_bomba Ponto de origem da bomba.
*@param angulo Direção do raio em radianos.
*@param raio_max Alcance máximo do raio.
*@return Distância até interseção, ou DBL_MAX se não houver interseção.
*@details Emite um raio (semi-reta) a partir da bomba no ângulo
*especificado e calcula onde ele intercepta o anteparo.
*Fundamental para cálculos de visibilidade.
******************************************************************/
double geometria_calcDistAnteBmb(Anteparo a, double x_bomba, double y_bomba, double angulo, double raio_max); 

/******************************************************************
*@brief Calcula a distância Euclidiana entre dois pontos.
*@param a Ponto 1.
*@param b Ponto 2.
*@return A distância Euclidiana entre os pontos.
*@details Usa sqrt((xa-xb)² + (ya-yb)²) para calcular a distância.
******************************************************************/
double geometria_calcDistPoint(double xp1, double yp1, double xp2, double yp2);

#endif //PROJETO_02_EDI_GEOMETRIA_H