#ifndef POLIGONO_H
#define POLIGONO_H
#include "pontos.h"
#include "lista.h"
#include <stdbool.h>

typedef void* Poligono;

/****************************************************************************************
* @brief Cria e aloca memória para um novo polígono vazio.
* @return Retorna um ponteiro do tipo Poligono recém-criado.
****************************************************************************************/
Poligono poligono_create();

/****************************************************************************************
* @brief Insere um vértice (ponto) existente no polígono.
* @param p O polígono.
* @param v O objeto Ponto a ser inserido como vértice.
****************************************************************************************/
void poligono_insertVertice(Poligono p, Ponto v);

/****************************************************************************************
* @brief Cria uma cópia de um ponto e o insere como vértice no polígono.
* @param p O polígono.
* @param v O ponto original cujos dados serão copiados.
****************************************************************************************/
void poligono_copyPasteVertice(Poligono p, Ponto v);

/****************************************************************************************
* @brief Verifica se o polígono não possui vértices.
* @param p O polígono.
* @return Retorna true se estiver vazio, false caso contrário.
****************************************************************************************/
bool poligono_isEmpty(Poligono p);

/****************************************************************************************
* @brief Calcula ou atualiza as coordenadas da Bounding Box (retângulo envolvente) do polígono.
* @param p O polígono.
****************************************************************************************/
void poligono_calcBBox(Poligono p);

/****************************************************************************************
* @brief Verifica se um ponto está dentro do polígono (algoritmo Ray Casting).
* @param p O polígono.
* @param pp O ponto a ser testado.
* @return Retorna true se o ponto estiver dentro, false caso contrário.
****************************************************************************************/
bool poligono_isInside(Poligono p, Ponto pp);

/****************************************************************************************
* @brief Libera a memória alocada para a estrutura do polígono e sua lista de vértices.
* @param p O polígono a ser destruído.
****************************************************************************************/
void poligono_destroy(Poligono p);


/****************************************************************************************
* @brief Obtém os limites (min/max) da Bounding Box do polígono.
* @param p O polígono.
* @param xMin Ponteiro para armazenar o X mínimo.
* @param xMax Ponteiro para armazenar o X máximo.
* @param yMin Ponteiro para armazenar o Y mínimo.
* @param yMax Ponteiro para armazenar o Y máximo.
****************************************************************************************/
void poligono_getBBox(Poligono p, double *xMin, double *xMax, double *yMin, double *yMax);

/****************************************************************************************
* @brief Obtém um vértice do polígono pelo índice.
* @param p O polígono.
* @param index O índice do vértice na lista.
* @return Retorna o objeto Ponto correspondente.
****************************************************************************************/
Ponto poligono_getVertice(Poligono p, int index);

/****************************************************************************************
* @brief Obtém o último vértice adicionado ao polígono.
* @param p O polígono.
* @return Retorna o objeto Ponto.
****************************************************************************************/
Ponto poligono_getLastVertice(Poligono p);

/****************************************************************************************
* @brief Obtém a lista interna de vértices do polígono.
* @param p O polígono.
* @return Retorna o ponteiro para a Lista de vértices.
****************************************************************************************/
Lista *poligono_getVerticeList(Poligono p);

/****************************************************************************************
* @brief Gera uma lista de segmentos (linhas/anteparos) representando as arestas do polígono.
* @param p O polígono.
* @return Retorna uma nova Lista contendo os segmentos gerados.
****************************************************************************************/
Lista *poligono_getSegments(Poligono p);

/****************************************************************************************
* @brief Obtém a contagem total de vértices do polígono.
* @param p O polígono.
* @return Retorna o número de vértices (int).
****************************************************************************************/
int poligono_getVerticeCount(Poligono p);

#endif