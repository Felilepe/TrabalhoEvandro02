#ifndef ARVORE_H
#define ARVORE_H
#include <stdbool.h>

#ifndef ITEM_T_DEFINED
typedef void *item;
#define ITEM_T_DEFINED
#endif

typedef struct ArvoreSt Arvore;
typedef struct NodeSt ArvNode;

/****************************************************************************************
* @brief Cria e aloca a memória para uma nova árvore (AVL).
* @param cmp_data Ponteiro para a função de comparação entre dois itens.
* @param free_data Ponteiro para a função que libera a memória de um item (pode ser NULL).
* @param print_data Ponteiro para a função que imprime um item (pode ser NULL).
* @return Retorna um ponteiro do tipo Arvore para a estrutura recém-criada.
****************************************************************************************/
Arvore *arvore_create(int (*cmp_data)(const void *a, const void *b), void (*free_data)(void *data), void (*print_data)(void *data));

/****************************************************************************************
* @brief Insere um novo item na árvore, mantendo o balanceamento AVL.
* @param a A árvore onde o item será inserido.
* @param data O item a ser inserido.
****************************************************************************************/
void arvore_insert(Arvore *a, item data);

/****************************************************************************************
* @brief Remove um item da árvore com base em uma chave, rebalanceando se necessário.
* @param a A árvore da qual o item será removido.
* @param key A chave que identifica o item a ser removido.
****************************************************************************************/
void arvore_remove(Arvore *a, item key);


/****************************************************************************************
* @brief Busca um item na árvore.
* @param a A árvore onde a busca será realizada.
* @param key A chave do item a ser buscado.
* @return Retorna o item encontrado ou NULL se não existir.
****************************************************************************************/
item arvore_search(Arvore *a, item key); 

/****************************************************************************************
* @brief Imprime todos os elementos da árvore (percurso em ordem).
* @param a A árvore a ser impressa.
****************************************************************************************/
void arvore_print(Arvore *a);

/****************************************************************************************
* @brief Libera a memória alocada para a árvore e seus nós.
* @param a A árvore a ser destruída.
****************************************************************************************/
void arvore_destroy(Arvore *a);

/****************************************************************************************
* @brief Verifica se a árvore está vazia.
* @param a A árvore.
* @return Retorna true se estiver vazia, false caso contrário.
****************************************************************************************/
bool arvore_isEmpty(Arvore *a);



/****************************************************************************************
* @brief Obtém o dado armazenado em um nó específico.
* @param n O nó do qual o dado será obtido.
* @return Retorna o item (dado) armazenado no nó.
****************************************************************************************/
item arvore_getNodeData(ArvNode *n);

/****************************************************************************************
* @brief Obtém a altura atual da árvore.
* @param a A árvore.
* @return Retorna a altura (int).
****************************************************************************************/
int arvore_getHeight(Arvore *a);

/****************************************************************************************
* @brief Obtém o número total de elementos (tamanho) da árvore.
* @param a A árvore.
* @return Retorna o tamanho (int).
****************************************************************************************/
int arvore_getSize(Arvore *a);

/****************************************************************************************
* @brief Obtém o nó raiz da árvore.
* @param a A árvore.
* @return Retorna um ponteiro para o nó raiz (ArvNode*).
****************************************************************************************/
ArvNode* arvore_getRootNode(Arvore *a);

/****************************************************************************************
* @brief Obtém o nó filho à direita de um nó dado.
* @param n O nó pai.
* @return Retorna um ponteiro para o nó filho à direita (ArvNode*).
****************************************************************************************/
ArvNode* arvore_getRightNode(ArvNode *n);

/****************************************************************************************
* @brief Obtém o nó filho à esquerda de um nó dado.
* @param n O nó pai.
* @return Retorna um ponteiro para o nó filho à esquerda (ArvNode*).
****************************************************************************************/
ArvNode* arvore_getLeftNode(ArvNode *n);



#endif