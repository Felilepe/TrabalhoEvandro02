#ifndef FILA_H
#define FILA_H

#include <stdbool.h>

#ifndef ITEM_T_DEFINED
typedef void *item;
#define ITEM_T_DEFINED
#endif
typedef struct fila Fila;
typedef struct node NodeF;


/******************************************************************
*@brief Cria e inicializa uma nova Fila vazia.
*@return Um ponteiro para a Fila recém-criada.
******************************************************************/
Fila *fila_create();

/******************************************************************
*@brief Verifica se a Fila está vazia.
*@param f Ponteiro para a Fila.
*@return True se a fila estiver vazia, caso contrário False.
******************************************************************/
bool fila_isEmpty(Fila *f);

/******************************************************************
*@brief Adiciona um item ao final da Fila (enfileirar).
*@param f Ponteiro para a Fila.
*@param i O item (void*) a ser adicionado.
******************************************************************/
void fila_queue(Fila *f, item i);

/******************************************************************
*@brief Remove e retorna o item do início da Fila (desenfileirar).
*@param f Ponteiro para a Fila.
*@return O item (void*) removido do início.
*@note Causa 'exit(1)' se a fila estiver vazia.
******************************************************************/
item fila_dequeue(Fila *f);

/******************************************************************
*@brief Obtém o primeiro nó (NodeF*) da Fila.
*@param f Ponteiro para a Fila.
*@return Um ponteiro para o nó da cabeça (NodeF*).
******************************************************************/
NodeF *fila_getHead(Fila *f);

/******************************************************************
*@brief Obtém o último nó (NodeF*) da Fila.
*@param f Ponteiro para a Fila.
*@return Um ponteiro para o nó da cauda (NodeF*).
******************************************************************/
NodeF *fila_getTail(Fila *f);

/******************************************************************
*@brief Obtém o item (dado) de um nó específico da Fila.
*@param n O nó (NodeF*) do qual se deseja obter o item.
*@return O item (void*) armazenado no nó.
******************************************************************/
item fila_getItem(NodeF *n);

/******************************************************************
*@brief Obtém o próximo nó na Fila.
*@param n O nó (NodeF*) atual.
*@return O próximo nó (NodeF*), ou NULL se for o último.
******************************************************************/
NodeF *fila_getNext(NodeF *n);

/******************************************************************
*@brief Libera a memória de todos os nós da Fila e da própria Fila.
*@param f Ponteiro para a Fila a ser destruída.
*@note NÃO libera a memória dos 'itens' (dados) armazenados.
******************************************************************/
void fila_destroy(Fila* f);

/******************************************************************
*@brief Obtém o número de itens atualmente na Fila.
*@param f Ponteiro para a Fila.
*@return O número de itens (int) na Fila.
******************************************************************/
int fila_getSize(Fila *f);


/******************************************************************
*@brief Percorre a Fila e executa uma função 'acao' para cada item.
*@param f Ponteiro para a Fila.
*@param acao A função de callback a ser executada em cada item.
*@param aux_data Um ponteiro auxiliar para ser passado para a função 'acao'.
******************************************************************/
void fila_passthrough(Fila *f, void (*acao)(item i, item aux_data), item aux_data);

#endif