#ifndef PONTOS_H
#define PONTOS_H

typedef void* Ponto;

/****************************************************************************************
* @brief Cria e aloca memória para um novo ponto 2D.
* @param x Coordenada X do ponto.
* @param y Coordenada Y do ponto.
* @return Retorna um ponteiro do tipo Ponto para a estrutura criada.
****************************************************************************************/
Ponto ponto_create(double x, double y);


/****************************************************************************************
* @brief Obtém a coordenada X de um ponto.
* @param p O ponto.
* @return Retorna o valor de X (double).
****************************************************************************************/
double ponto_getCoordX(Ponto p);

/****************************************************************************************
* @brief Obtém a coordenada Y de um ponto.
* @param p O ponto.
* @return Retorna o valor de Y (double).
****************************************************************************************/
double ponto_getCoordY(Ponto p);


/****************************************************************************************
* @brief Define/Altera a coordenada X de um ponto.
* @param p O ponto a ser modificado.
* @param x O novo valor para a coordenada X.
****************************************************************************************/
void ponto_setCoordX(Ponto p, double x);

/****************************************************************************************
* @brief Define/Altera a coordenada Y de um ponto.
* @param p O ponto a ser modificado.
* @param y O novo valor para a coordenada Y.
****************************************************************************************/
void ponto_setCoordY(Ponto p, double y);


/****************************************************************************************
* @brief Cria uma cópia (clone) independente de um ponto existente.
* @param p O ponto original a ser copiado.
* @return Retorna um novo objeto Ponto com as mesmas coordenadas.
****************************************************************************************/
Ponto ponto_clone(Ponto p);

/****************************************************************************************
* @brief Libera a memória alocada para um ponto.
* @param p O ponto a ser destruído.
****************************************************************************************/
void ponto_destroy(Ponto p);

#endif