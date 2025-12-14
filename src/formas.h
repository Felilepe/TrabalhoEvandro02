#ifndef FORMAS_H
#define FORMAS_H

#define TIPO_C 1
#define TIPO_R 2
#define TIPO_L 3
#define TIPO_T 4
#define TIPO_A 5

#include <stdio.h>

typedef void* forma;

/******************************************************************
*@brief Libera a memória de uma forma (chamada polimórfica).
*@param f A forma genérica a ser destruída.
******************************************************************/
void forma_destroy(forma f);

/******************************************************************
*@brief Calcula a área de uma forma (chamada polimórfica).
*@param f A forma genérica.
*@return A área (double) da forma.
******************************************************************/
double forma_calcArea(forma f);

/******************************************************************
*@brief Troca a cor da borda e preenchimento de uma forma.
*@param f A forma genérica a ser modificada.
******************************************************************/
void forma_trocarCores(forma f);

/******************************************************************
*@brief Troca as cores entre duas formas.
*@param f1 A primeira forma genérica.
*@param f2 A segunda forma genérica.
******************************************************************/
void forma_trocarCoresEntreFormas(forma f1, forma f2);

/******************************************************************
*@brief Cria e retorna um clone (cópia profunda) de uma forma.
*@param f A forma genérica a ser clonada.
*@return Uma nova forma genérica que é um clone da original.
******************************************************************/
forma forma_clonar(forma f);

/******************************************************************
*@brief Exporta dados de uma forma para o arquivo de log (obsoleto?).
*@param f A forma genérica.
*@param file_name O handle do arquivo de log.
*@param report_QRY O nome do arquivo QRY (?).
******************************************************************/
void forma_exportarDados(forma f, FILE *file_name, char* report_QRY);

// --- Getters Polimórficos ---

/******************************************************************
*@brief Obtém o ID de uma forma (chamada polimórfica).
*@param f A forma genérica.
*@return O ID (int) da forma.
******************************************************************/
int forma_getID(forma f);

/******************************************************************
*@brief Obtém o tipo (TIPO_C, TIPO_R, etc.) de uma forma.
*@param f A forma genérica.
*@return O tipo (int) da forma.
******************************************************************/
int forma_getType(forma f);

/******************************************************************
*@brief Obtém a coordenada X de uma forma.
*@param f A forma genérica.
*@return A coordenada X (double) da forma.
******************************************************************/
double forma_getCoordX(forma f);

/******************************************************************
*@brief Obtém a coordenada Y de uma forma.
*@param f A forma genérica.
*@return A coordenada Y (double) da forma.
******************************************************************/
double forma_getCoordY(forma f);

/******************************************************************
*@brief Obtém a Cor de Borda de uma forma.
*@param f A forma genérica.
*@return a Cor de Borda (string) da forma.
******************************************************************/
char* forma_getCorBorda(forma f);

/******************************************************************
*@brief Obtém a Cor de Preenchimento de uma forma.
*@param f A forma genérica.
*@return a Cor de Preenchimento (string) da forma.
******************************************************************/
char* forma_getCorPreench(forma f);

/******************************************************************
*@brief Calcula a cor complementar de uma cor (ex: "blue" -> "red").
*@param cor A string da cor original.
*@return Uma nova string (alocada) com a cor complementar.
******************************************************************/
char *forma_getCorComp(char *cor);

// --- Setters Polimórficos ---

/******************************************************************
*@brief Define a coordenada X de uma forma (chamada polimórfica).
*@param f A forma genérica.
*@param x A nova coordenada X.
******************************************************************/
void forma_setCoordX(forma f, double x);

/******************************************************************
*@brief Define a coordenada Y de uma forma.
*@param f A forma genérica.
*@param y A nova coordenada Y.
******************************************************************/
void forma_setCoordY(forma f, double y);

/******************************************************************
*@brief Define a Cor de Borda de uma forma.
*@param f A forma genérica.
*@param corb A nova Cor de Borda.
******************************************************************/
void forma_setCorBorda(forma f, char* corb);

/******************************************************************
*@brief Define a Cor de Preenchimento.
*@param f A forma genérica.
*@param corp A nova Cor de Preenchimento.
******************************************************************/
void forma_setCorPreench(forma f, char* corp);


#endif