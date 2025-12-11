#ifndef ARVORE_H
#define ARVORE_H
#include <stdbool.h>

/******************************************************************
*@brief TAD ARVORE - Árvore AVL balanceada para busca rápida.
*
*Implementação de uma árvore binária de busca auto-balanceada (AVL)
*que mantém a altura mínima para operações eficientes de busca,
*inserção e remoção em O(log n).
******************************************************************/

typedef struct stNode node_AVL;
typedef struct stArvore arvore;

/******************************************************************
*@brief Cria uma nova árvore AVL.
*@param cmp_data Função para comparar elementos.
*@param free_data Função para liberar elementos.
*@param print_data Função para imprimir elementos.
*@return Ponteiro para a árvore criada.
******************************************************************/
arvore *arvore_create(int (*cmp_data)(const void *a, const void *b), void (*free_data)(void *data), void (*print_data)(void *data));

/******************************************************************
*@brief Obtém o nó raiz da árvore.
*@param t Ponteiro para a árvore.
*@return Ponteiro para o nó raiz.
******************************************************************/
node_AVL *arvore_getRoot(arvore *t);

/******************************************************************
*@brief Obtém a altura máxima da árvore.
*@param t Ponteiro para a árvore.
*@return Altura da árvore.
******************************************************************/
int get_altura_arvore(arvore *t);

/******************************************************************
*@brief Obtém o número total de nós na árvore.
*@param t Ponteiro para a árvore.
*@return Tamanho da árvore.
******************************************************************/
int get_tam_AVL(arvore *t);

/******************************************************************
*@brief Obtém o filho esquerdo de um nó.
*@param n Ponteiro para o nó.
*@return Ponteiro para o filho esquerdo ou NULL.
******************************************************************/
node_AVL *get_esquerda_node(node_AVL *n);

/******************************************************************
*@brief Obtém o filho direito de um nó.
*@param n Ponteiro para o nó.
*@return Ponteiro para o filho direito ou NULL.
******************************************************************/
node_AVL *get_direita_node(node_AVL *n);

/******************************************************************
*@brief Obtém os dados armazenados em um nó.
*@param n Ponteiro para o nó.
*@return Dados do nó.
******************************************************************/
void *get_node_dataAVL(node_AVL *n);

/******************************************************************
*@brief Verifica se a árvore está vazia.
*@param avl Ponteiro para a árvore.
*@return true se vazia, false caso contrário.
******************************************************************/
bool is_empty_avl(arvore *avl);

/******************************************************************
*@brief Insere um novo elemento na árvore.
*@param t Ponteiro para a árvore.
*@param data Dado a ser inserido.
*@note Duplicatas não são inseridas.
******************************************************************/
void arvore_insere(arvore *t, void *data);

/******************************************************************
*@brief Busca um elemento na árvore.
*@param t Ponteiro para a árvore.
*@param key Elemento a procurar.
*@return Os dados encontrados ou NULL.
******************************************************************/
void *binary_search(arvore *t, void *key);

/******************************************************************
*@brief Remove um elemento da árvore.
*@param t Ponteiro para a árvore.
*@param key Elemento a remover.
*@return Os dados removidos ou NULL.
******************************************************************/
void *remove_node(arvore *t, void *key);

/******************************************************************
*@brief Imprime a árvore em ordem (in-order).
*@param t Ponteiro para a árvore.
*@details Ordem: esquerda, raiz, direita.
******************************************************************/
void in_order(arvore *t);

/******************************************************************
*@brief Libera toda a memória da árvore.
*@param t Ponteiro para a árvore.
******************************************************************/
void free_arvore(arvore *t);

#endif // ARVORE_H
