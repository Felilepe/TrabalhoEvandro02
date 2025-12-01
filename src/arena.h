#ifndef ARENA_H
#define ARENA_H

#include <stdio.h>
#include <stdbool.h>

#include "fila.h"
#include "formas.h"
#include "lerGeo.h" // onde fica o definição de chão (typedef Fila Chao)
#include "lista.h"

typedef struct arena Arena;

/******************************************************************
*@brief Cria e inicializa uma nova Arena (fila de combate).
*@return Um ponteiro para a Arena recém-criada.
******************************************************************/
Arena *arena_create();

/******************************************************************
*@brief Adiciona uma forma à fila da Arena.
*@param a Ponteiro para a Arena.
*@param f Ponteiro para a forma a ser adicionada (tipo 'forma *f').
*@return Retorna um ponteiro para a forma adicionada, ou NULL se falhar.
******************************************************************/
forma arena_add(Arena *a, forma f);

/******************************************************************
*@brief Remove e retorna a próxima forma da fila da Arena.
*@param a Ponteiro para a Arena.
*@return Retorna a forma (void*) removida.
******************************************************************/
forma arena_remove(Arena *a);

/******************************************************************
*@brief Verifica se a Arena está vazia.
*@param a Ponteiro para a Arena.
*@return True se a Arena estiver vazia, caso contrário False.
******************************************************************/
bool arena_isEmpty(Arena *a);

/******************************************************************
*@brief Libera toda a memória associada à Arena.
*@param a Ponteiro para o ponteiro da Arena (Arena **a) para zerá-lo.
******************************************************************/
void arena_destroy(Arena **a);

/******************************************************************
*@brief Obtém o número de formas atualmente na Arena.
*@param a Ponteiro para a Arena.
*@return O número de formas (int) na Arena.
******************************************************************/
int arena_getSize(Arena *a);

/******************************************************************
*@brief Processa um "round" de combate na Arena.
*@param a Ponteiro para a Arena.
*@param c Ponteiro para o Chao (onde as formas terminam).
*@param pontuacao_total Ponteiro para a pontuação global do jogo.
*@param anotacoes_svg Fila para registrar anotações visuais (SVG).
*@param arquivo_txt Handle do arquivo de log (.txt).
*@param formas_clonadas Ponteiro para o contador de formas clonadas.
*@param formas_destruidas Ponteiro para o contador de formas destruídas.
******************************************************************/
void processaArena(Arena *a, Chao *c, double *pontuacao_total, Fila *anotacoes_svg, FILE *arquivo_txt, int *formas_clonadas, int *formas_destruidas);

/*****************************************************************************
*@brief Percorre a arena e executa uma função 'acao' para cada item.
* (Modelado a partir de fila_passthrough)
*@param l Ponteiro para a lista.
*@param acao A função de callback a ser executada em cada item.
*@param aux_data Um ponteiro auxiliar para ser passado para a função 'acao'.
*****************************************************************************/
void arena_passthrough(Arena *a, void (*acao)(item i, item aux_data), item aux_data);

#endif