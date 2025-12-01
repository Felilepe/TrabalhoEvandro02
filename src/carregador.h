#ifndef CARREGADOR_H
#define CARREGADOR_H

#include "fila.h"
#include "formas.h"
#include <stdbool.h>
#include "lerGeo.h"

typedef void* Carregador;


/******************************************************************
*@brief Cria um novo Carregador (magazine) com um ID.
*@param id O ID único para este carregador.
*@return Um ponteiro para o Carregador (void*).
*@note O tipo de retorno correto é 'Carregador' (void*), não 'Carregador*' (void**).
******************************************************************/
Carregador carregador_create(int id);

/******************************************************************
*@brief Obtém o ID de um Carregador.
*@param c O Carregador (void*).
*@return O ID (int) do Carregador.
******************************************************************/
int carregador_getID(Carregador c);

/******************************************************************
*@brief Insere uma forma diretamente na pilha do Carregador.
*@param c O Carregador (void*).
*@param f A forma (void*) a ser inserida.
*@return A forma (void*) que foi inserida.
******************************************************************/
forma carregador_loadForma(Carregador c, forma f);

/******************************************************************
*@brief Remove uma forma do Chão e a insere no Carregador.
*@param c O Carregador (void*).
*@param h O Chão (Fila*) de onde a forma será retirada.
*@return A forma (void*) que foi movida do Chão para o Carregador.
******************************************************************/
forma carregador_loadFromChao(Carregador c, Chao *h);

/******************************************************************
*@brief Carrega 'n' formas do Chão para o Carregador.
*@param h O Chão (Fila*).
*@param c O Carregador (void*).
*@param n O número de formas a carregar.
*@return Retorna uma Fila (Chao*) contendo o histórico de formas carregadas.
******************************************************************/
Chao *carregador_loadAmount(Chao *h, Carregador c, int n);

/******************************************************************
*@brief Verifica se o Carregador está vazio (sem formas).
*@param c O Carregador (void*).
*@return True se o Carregador estiver vazio, caso contrário False.
******************************************************************/
bool carregador_isEmpty(Carregador c);

/******************************************************************
*@brief Libera a memória do Carregador e sua pilha interna.
*@param c_point Ponteiro para o Carregador (void**) para zerá-lo.
******************************************************************/
void carregador_destroy(Carregador c);

/******************************************************************
*@brief Remove e retorna a forma do topo da pilha do Carregador.
*@param c O Carregador (void*).
*@return A forma (void*) removida do topo.
******************************************************************/
forma carregador_remove(Carregador c);

/**
 * Retorna (sem remover) a forma no topo do carregador.
 * Retorna NULL se o carregador estiver vazio.
 */
forma carregador_peek(Carregador c);

/**
 * Retorna o número de formas atualmente no carregador.
 */
int carregador_getSize(Carregador c);

#endif