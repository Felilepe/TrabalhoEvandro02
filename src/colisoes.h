#ifndef COLISOES_H
#define COLISOES_H

#include <stdbool.h>
#include "formas.h"
#include "linha.h"
#include "circulo.h"
#include "retangulo.h"
#include "texto.h"  

/******************************************************************
*@brief Verifica a colisão entre duas formas genéricas.
*@param f1 A primeira forma.
*@param f2 A segunda forma.
*@return True se as formas colidem, caso contrário False.
******************************************************************/
bool checarColisao(forma f1, forma f2);

/******************************************************************
*@brief Verifica a colisão entre dois círculos.
*@param c1 O primeiro Círculo.
*@param c2 O segundo Círculo.
*@return True se colidem, caso contrário False.
******************************************************************/
bool colisaoCircCirc(Circulo c1, Circulo c2);

/******************************************************************
*@brief Verifica a colisão entre um círculo e uma linha.
*@param c O Círculo.
*@param l A Linha.
*@return True se colidem, caso contrário False.
******************************************************************/
bool colisaoCircLinha(Circulo c, Linha l);

/******************************************************************
*@brief Verifica a colisão entre um círculo e um retângulo.
*@param c O Círculo.
*@param r O Retângulo.
*@return True se colidem, caso contrário False.
******************************************************************/
bool colisaoCircRet(Circulo c, Retangulo r);

/******************************************************************
*@brief Verifica a colisão entre um círculo e um texto.
*@param c O Círculo.
*@param t O Texto.
*@return True se colidem, caso contrário False.
******************************************************************/
bool colisaoCircTxto(Circulo c, Texto t);

/******************************************************************
*@brief Verifica a colisão entre duas linhas.
*@param l1 A primeira Linha.
*@param l2 A segunda Linha.
*@return True se colidem, caso contrário False.
******************************************************************/
bool colisaoLinhaLinha(Linha l1, Linha l2);

/******************************************************************
*@brief Verifica a colisão entre uma linha e um retângulo.
*@param l A Linha.
*@param r O Retângulo.
*@return True se colidem, caso contrário False.
******************************************************************/
bool colisaoLinhaRet(Linha l, Retangulo r);

/******************************************************************
*@brief Verifica a colisão entre uma linha e um texto.
*@param l A Linha.
*@param t O Texto.
*@return True se colidem, caso contrário False.
******************************************************************/
bool colisaoLinhaTxto(Linha l, Texto t);

/******************************************************************
*@brief Verifica a colisão entre dois retângulos.
*@param r1 O primeiro Retângulo.
*@param r2 O segundo Retângulo.
*@return True se colidem, caso contrário False.
******************************************************************/
bool colisaoRetRet(Retangulo r1, Retangulo r2);

/******************************************************************
*@brief Verifica a colisão entre um retângulo e um texto.
*@param r O Retângulo.
*@param t O Texto.
*@return True se colidem, caso contrário False.
******************************************************************/
bool colisaoRetTxto(Retangulo r, Texto t);

/******************************************************************
*@brief Verifica a colisão entre dois textos.
*@param t1 O primeiro Texto.
*@param t2 O segundo Texto.
*@return True se colidem, caso contrário False.
******************************************************************/
bool colisaoTxtoTxto(Texto t1, Texto t2);

#endif