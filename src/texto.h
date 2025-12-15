#ifndef TEXTO_H
#define TEXTO_H
#include "anteparo.h"
#include "linha.h"

typedef void* Texto;

/******************************************************************
*@brief Cria e aloca a memória para um novo objeto de texto.
*@param id Identificador único para o texto.
*@param x Coordenada X da âncora do texto.
*@param y Coordenada Y da âncora do texto.
*@param corborda String que representa a cor da borda (contorno).
*@param corpreench String que representa a cor de preenchimento.
*@param a Caractere da âncora ('i', 'm', 'f').
*@param txto O conteúdo do texto (string).
*@return Retorna um ponteiro do tipo Texto para o texto recém-criado.
******************************************************************/
Texto texto_create(int id, double x, double y, char *corborda, char *corpreench, char a, char* txto);

Linha conversaoTxtoLinha(Texto t);

/******************************************************************
*@brief Libera a memória alocada para o objeto de texto.
*@param t O texto a ser destruído.
******************************************************************/
void texto_destroy(Texto t);

/******************************************************************
*@brief Calcula a "área" (comprimento * altura base) de um texto.
*@param t O texto.
*@return A área (double) do texto.
******************************************************************/
double texto_calcArea(Texto t);


Anteparo texto_toAnteparo(Texto t, int *id_next);


// --- Getters ---

/******************************************************************
*@brief Obtém o ID de um texto.
*@param t O texto.
*@return O ID (int) do texto.
******************************************************************/
int texto_getID(Texto t);

/******************************************************************
*@brief Obtém a coordenada X da âncora de um texto.
*@param t O texto.
*@return A coordenada X (double).
******************************************************************/
double texto_getCoordX(Texto t);

/******************************************************************
*@brief Obtém a coordenada Y da âncora de um texto.
*@param t O texto.
*@return A coordenada Y (double).
******************************************************************/
double texto_getCoordY(Texto t);

/******************************************************************
*@brief Obtém o conteúdo de um texto.
*@param t O texto.
*@return A Conteúdo (string).
******************************************************************/
char *texto_getTexto(Texto t);

/******************************************************************
*@brief Obtém a cor de borda de um texto.
*@param t O texto.
*@return A Cor de Borda (string).
******************************************************************/
char *texto_getCorBorda(Texto t);

/******************************************************************
*@brief Obtém a cor de preenchimento de um texto.
*@param t O texto.
*@return A Cor de Preenchimento (string).
******************************************************************/
char *texto_getCorPreench(Texto t);

/******************************************************************
*@brief Obtém a família da fonte de um texto.
*@param t O texto.
*@return A Família (string).
******************************************************************/
char *texto_getFamily(Texto t);

/******************************************************************
*@brief Obtém o peso da fonte de um texto.
*@param t O texto.
*@return O Peso (string).
******************************************************************/
char *texto_getWeight(Texto t);

/******************************************************************
*@brief Obtém o tamanho da fonte de um texto.
*@param t O texto.
*@return O Tamanho (string).
******************************************************************/
char *texto_getSize(Texto t);

/******************************************************************
*@brief Obtém a âncora de um texto.
*@param t O texto.
*@return A Âncora (string).
******************************************************************/
char texto_getAnchor(Texto t);

/******************************************************************
*@brief Obtém a contagem de caracteres do texto.
*@param t O texto.
*@return O número de caracteres (int).
******************************************************************/
int texto_getCharCount(Texto t);

/****************************************************************************************
 * @brief Obtém o Tipo de texto.                                                    
 * @param t O retângulo do qual o Tipo será obtido.                                        
 * @return Retorna o valor do Tipo (um int).                                               
 ****************************************************************************************/
int texto_getType(Texto t);



// --- Setters ---

/******************************************************************
*@brief Define o estilo de fonte padrão para novos textos.
*@param fFamily A família da fonte (ex: "Arial").
*@param fWeight O peso da fonte (ex: "bold").
*@param fSize O tamanho da fonte (ex: "12").
******************************************************************/
void texto_setNewStyle(char* fFamily, char* fWeight, char* fSize);

/******************************************************************
*@brief Define a coordenada X da âncora de um texto.
*@param t O texto.
*@param x A nova coordenada X.
******************************************************************/
void texto_setCoordX(Texto t, double x);

/******************************************************************
*@brief Define a coordenada Y da âncora de um texto.
*@param t O texto.
*@param y A nova coordenada Y.
******************************************************************/
void texto_setCoordY(Texto t, double y); 

/******************************************************************
*@brief Define a cor de preenchimento de um texto.
*@param t O texto.
*@param corpreench A nova Cor de Preenchimento.
******************************************************************/
void texto_setCorPreench(Texto t, char* corpreench);

/******************************************************************
*@brief Define a cor de borda de um texto.
*@param t O texto.
*@param corborda A nova Cor de Borda.
******************************************************************/
void texto_setCorBorda(Texto t, char* corborda);

/******************************************************************
*@brief Define o conteúdo de um texto.
*@param t O texto.
*@param txto O novo Conteúdo.
******************************************************************/
void texto_setTexto(Texto t, char* txto);

/******************************************************************
*@brief Define a família da fonte de um texto.
*@param t O texto.
*@param fFamily A nova Família.
******************************************************************/
void texto_setFamily(Texto t, char* fFamily);

/******************************************************************
*@brief Define o peso da fonte de um texto.
*@param t O texto.
*@param fWeight O novo Peso.
******************************************************************/
void texto_setWeight(Texto t, char* fWeight);

/******************************************************************
*@brief Define o tamanho da fonte de um texto.
*@param t O texto.
*@param fSize O novo Tamanho.
******************************************************************/
void texto_setSize(Texto t, char* fSize);

/******************************************************************
*@brief Define a âncora de um texto.
*@param t O texto.
*@param a A nova Âncora.
******************************************************************/
void texto_setAnchor(Texto t, char a);


#endif