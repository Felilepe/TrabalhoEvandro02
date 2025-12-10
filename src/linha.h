#ifndef LINHA_H
#define LINHA_H
#include <stdbool.h>
#include "anteparo.h"

typedef void* Linha;

/******************************************************************
*@brief Cria e aloca a memória para uma nova linha.
*@param id Identificador único para a linha.
*@param x1 Coordenada X do ponto inicial.
*@param y1 Coordenada Y do ponto inicial.
*@param x2 Coordenada X do ponto final.
*@param y2 Coordenada Y do ponto final.
*@param cor String que representa a cor da linha.
*@param isDotted Flag booleano se a linha é pontilhada.
*@return Retorna um ponteiro do tipo Linha para a linha recém-criada.
******************************************************************/
Linha linha_create(int id, double x1, double y1, double x2, double y2, char* cor, bool isDotted);


/******************************************************************
*@brief Obtém o ID de uma linha.
*@param l A linha.
*@return O ID (int) da linha.
******************************************************************/
int linha_getID(Linha l);

/******************************************************************
*@brief Obtém a coordenada X1 (início) de uma linha.
*@param l A linha.
*@return A coordenada X1 (double).
******************************************************************/
double linha_getCoordX1(Linha l);

/******************************************************************
*@brief Obtém a coordenada X2 (fim) de uma linha.
*@param l A linha.
*@return A coordenada X2 (double).
******************************************************************/
double linha_getCoordX2(Linha l);

/******************************************************************
*@brief Obtém a coordenada Y1 (início) de uma linha.
*@param l A linha.
*@return A coordenada Y1 (double).
******************************************************************/
double linha_getCoordY1(Linha l);

/******************************************************************
*@brief Obtém a coordenada Y2 (fim) de uma linha.
*@param l A linha.
*@return A coordenada Y2 (double).
******************************************************************/
double linha_getCoordY2(Linha l);

/******************************************************************
*@brief Obtém a Cor de uma linha.
*@param l A linha.
*@return A Cor (string).
******************************************************************/
char* linha_getCor(Linha l);

/******************************************************************
*@brief Obtém a Flag boolena de uma linha.
*@param l A linha.
*@return A Flag (bool).
******************************************************************/
bool linha_getIsDotted(Linha l);

/******************************************************************
*@brief Obtém o tipo de uma linha.
*@param l A linha.
*@return O Tipo (int).
******************************************************************/
int linha_getType(Linha l);


/******************************************************************
*@brief Calcula a "área" (comprimento) de uma linha.
*@param l A linha.
*@return O comprimento (double) da linha.
******************************************************************/
double linha_calcArea(Linha l);

// ... (Setters) ...

/******************************************************************
*@brief Obtém a coordenada X1 (início) de uma linha.
*@param l A linha.
*@param x1 Coordenada X1 nova
*@return A coordenada X1 (double).
******************************************************************/
void linha_setCoordX1(Linha l, double x1);

/******************************************************************
*@brief Obtém a coordenada X2 (fim) de uma linha.
*@param l A linha.
*@param x2 Coordenada X2 nova
******************************************************************/
void linha_setCoordX2(Linha l, double x2);

/******************************************************************
*@brief Obtém a coordenada Y1 (início) de uma linha.
*@param l A linha.
*@param y1 Coordenada Y1 nova
******************************************************************/
void linha_setCoordY1(Linha l, double y1);

/******************************************************************
*@brief Obtém a coordenada Y2 (fim) de uma linha.
*@param l A linha.
*@param y2 Coordenada Y2 nova
******************************************************************/
void linha_setCoordY2(Linha l, double y2);

/******************************************************************
*@brief Obtém a cor de uma linha.
*@param l A linha.
*@param cor Cor nova
******************************************************************/
void linha_setCor(Linha l, char *cor);


/******************************************************************
*@brief Libera a memória alocada para a linha.
*@param l A linha a ser destruída.
******************************************************************/
void linha_destroy(Linha l);


/****************************************************************************************
* @brief Converte uma linha em um anteparo.
* @param l A linha a ser convertido.
* @param ant_id Ponteiro para o ID do anteparo.
* @return Retorna o anteparo criado.
****************************************************************************************/
Anteparo linha_anteparo(Linha l, int *ant_id);


#endif