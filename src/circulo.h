#ifndef CIRCULO_H
#define CIRCULO_H
#include "anteparo.h"

typedef void* Circulo;

/****************************************************************************************
* @brief Cria e aloca a memória para um novo círculo.
* @param id Identificador único para o círculo.
* @param x Coordenada X do centro do círculo.
* @param y Coordenada Y do centro do círculo.
* @param r Raio do círculo.
* @param corborda String que representa a cor da borda.
* @param corpreench String que representa a cor de preenchimento.
* @return Retorna um ponteiro do tipo Circulo para o círculo recém-criado.
****************************************************************************************/
Circulo circulo_create(int id, double x, double  y, double r, char *corborda, char *corpreench);

/****************************************************************************************
* @brief Obtém a área de um círculo.
* @param c O círculo do qual a área será obtida.
* @return Retorna o valor da área (um double).
****************************************************************************************/
double circulo_calcArea(Circulo c);

/****************************************************************************************
* @brief Converte um círculo em um anteparo.
* @param c O círculo a ser convertido/utilizado.
* @param orientacao Caractere que define a orientação do anteparo.
* @param id_next Ponteiro para o próximo ID disponível (para geração de novos elementos).
* @return Retorna um objeto do tipo Anteparo.
****************************************************************************************/
Anteparo circulo_toAnteparo(Circulo c, char orientacao, int *id_next);

/****************************************************************************************
 * @brief Libera a memória alocada para o círculo.
* @param c O círculo a ser destruído.
****************************************************************************************/
void circulo_destroy(Circulo c);



// ... (Getters) ...

/****************************************************************************************
* @brief Obtém a coordenada X do centro de um círculo.
* @param c O círculo do qual a coordenada será obtida.
* @return Retorna o valor da coordenada X (um double).
****************************************************************************************/
double circulo_getCoordX(Circulo c);

/****************************************************************************************
* @brief Obtém a coordenada Y do centro de um círculo.
* @param c O círculo do qual a coordenada será obtida.
* @return Retorna o valor da coordenada Y (um double).
****************************************************************************************/
double circulo_getCoordY(Circulo c);

/****************************************************************************************
* @brief Obtém o ID de um círculo.
* @param c O círculo do qual o ID será obtido.
* @return Retorna o valor do ID (um int).
****************************************************************************************/
int circulo_getID(Circulo c);

/****************************************************************************************
* @brief Obtém o raio de um círculo.
* @param c O círculo do qual o raio será obtido.
* @return Retorna o valor do raio (um double).
****************************************************************************************/
 double circulo_getRaio(Circulo c);

/****************************************************************************************
* @brief Obtém a cor da borda de um círculo.
* @param c O círculo.
* @return Retorna um ponteiro para a string da cor da borda (char*).
****************************************************************************************/
char* circulo_getCorBorda(Circulo c);

/****************************************************************************************
* @brief Obtém a cor de preenchimento de um círculo.
* @param c O círculo.
* @return Retorna um ponteiro para a string da cor de preenchimento (char*).
****************************************************************************************/
char* circulo_getCorPreench(Circulo c);

/****************************************************************************************
* @brief Obtém o tipo do círculo.
* @param c O círculo.
* @return Retorna o tipo(int).
****************************************************************************************/
int circulo_getType(Circulo c);



// ... (Setters) ...

/****************************************************************************************
 * @brief Define a coordenada X do centro de um círculo.
 * @param c O círculo.
 * @param x A nova coordenada X.
 ****************************************************************************************/
void circulo_setCoordX(Circulo c, double x);

/****************************************************************************************
 * @brief Define a coordenada X do centro de um círculo.
 * @param c O círculo.
 * @param x A nova coordenada X.
 ****************************************************************************************/
void circulo_setCoordY(Circulo c, double y);

/****************************************************************************************
 * @brief Define a coordenada X do centro de um círculo.
 * @param c O círculo.
 * @param x A nova coordenada X.
 ****************************************************************************************/
void circulo_setRaio(Circulo c, double r);

/****************************************************************************************
 * @brief Define a coordenada X do centro de um círculo.
 * @param c O círculo.
 * @param x A nova coordenada X.
 ****************************************************************************************/
void circulo_setCorBorda(Circulo c, char *corborda);

/****************************************************************************************
 * @brief Define a coordenada X do centro de um círculo.
 * @param c O círculo.
 * @param x A nova coordenada X.
 ****************************************************************************************/
void circulo_setCorPreench(Circulo c, char *corpreench);


#endif
