#ifndef ANTEPARO_H
#define ANTEPARO_H
#include "pontos.h"

typedef void *Anteparo;

/****************************************************************************************
* @brief Cria e aloca memória para um novo anteparo (linha definida por dois pontos).
* @param id Identificador único do anteparo.
* @param x1 Coordenada X do primeiro ponto.
* @param y1 Coordenada Y do primeiro ponto.
* @param x2 Coordenada X do segundo ponto.
* @param y2 Coordenada Y do segundo ponto.
* @param cor String que representa a cor do anteparo.
* @return Retorna um ponteiro do tipo Anteparo para o objeto recém-criado.
****************************************************************************************/
Anteparo anteparo_create(int id, double x1, double y1, double x2, double y2, const char *cor);


/****************************************************************************************
* @brief Obtém o ID de um anteparo.
* @param a O anteparo.
* @return Retorna o ID (int).
****************************************************************************************/
int anteparo_getID(Anteparo a);

/****************************************************************************************
* @brief Obtém o tipo do anteparo.
* @param a O anteparo.
* @return Retorna o tipo (int).
****************************************************************************************/
int anteparo_getType(Anteparo a);

/****************************************************************************************
* @brief Obtém a coordenada X do primeiro ponto do anteparo.
* @param a O anteparo.
* @return Retorna a coordenada X (double).
****************************************************************************************/
double anteparo_getCoordX1(Anteparo a);

/****************************************************************************************
* @brief Obtém a coordenada Y do primeiro ponto do anteparo.
* @param a O anteparo.
* @return Retorna a coordenada Y (double).
****************************************************************************************/
double anteparo_getCoordY1(Anteparo a); 

/****************************************************************************************
* @brief Obtém a coordenada X do segundo ponto do anteparo.
* @param a O anteparo.
* @return Retorna a coordenada X (double).
****************************************************************************************/
double anteparo_getCoordX2(Anteparo a);

/****************************************************************************************
* @brief Obtém a coordenada Y do segundo ponto do anteparo.
* @param a O anteparo.
* @return Retorna a coordenada Y (double).
****************************************************************************************/
double anteparo_getCoordY2(Anteparo a);

/****************************************************************************************
* @brief Obtém a cor do anteparo.
* @param a O anteparo.
* @return Retorna um ponteiro para a string da cor (char*).
****************************************************************************************/
char *anteparo_getCor(Anteparo a);

/****************************************************************************************
* @brief Obtém o primeiro ponto (P1) que define o anteparo.
* @param a O anteparo.
* @return Retorna o objeto Ponto correspondente a P1.
****************************************************************************************/
Ponto anteparo_getP1(Anteparo a);

/****************************************************************************************
* @brief Obtém o segundo ponto (P2) que define o anteparo.
* @param a O anteparo.
* @return Retorna o objeto Ponto correspondente a P2.
****************************************************************************************/
Ponto anteparo_getP2(Anteparo a);



// ... (Setters) ...

/****************************************************************************************
* @brief Define o identificador do anteparo.
* @param a O anteparo.
* @param novo_id O novo valor para o ID.
****************************************************************************************/
void anteparo_setId(Anteparo a, int novo_id);

/****************************************************************************************
* @brief Define a coordenada X do primeiro ponto do anteparo.
* @param a O anteparo.
* @param x1 A nova coordenada X para o ponto P1.
****************************************************************************************/
void anteparo_setCoordX1(Anteparo a, double x1);

/****************************************************************************************
* @brief Define a coordenada Y do primeiro ponto do anteparo.
* @param a O anteparo.
* @param y1 A nova coordenada Y para o ponto P1.
****************************************************************************************/
void anteparo_setCoordY1(Anteparo a, double y1);

/****************************************************************************************
* @brief Define a coordenada X do segundo ponto do anteparo.
* @param a O anteparo.
* @param x2 A nova coordenada X para o ponto P2.
****************************************************************************************/
void anteparo_setCoordX2(Anteparo a, double x2);

/****************************************************************************************
* @brief Define a coordenada Y do segundo ponto do anteparo.
* @param a O anteparo.
* @param y2 A nova coordenada Y para o ponto P2.
****************************************************************************************/
void anteparo_setCoordY2(Anteparo a, double y2);

/****************************************************************************************
* @brief Define a cor do anteparo.
* @param a O anteparo.
* @param cor String representando a nova cor.
****************************************************************************************/
void anteparo_setCor(Anteparo a, const char *cor);


/****************************************************************************************
* @brief Libera a memória alocada para o anteparo.
* @param a O anteparo a ser destruído.
****************************************************************************************/
void anteparo_destroy(Anteparo a);

#endif