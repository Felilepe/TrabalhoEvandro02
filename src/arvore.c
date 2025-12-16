#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

struct NodeSt
{
    item data;
    struct NodeSt *esq;
    struct NodeSt *dir;
    int height;
};

struct ArvoreSt
{
    ArvNode *root;
    int size;
    int (*cmp_data)(const void *a, const void *b);
    void (*free_data)(void *data);
    void (*print_data)(void *data);
};



// --- Funções Auxiliares Privadas ---

static int max(int a, int b) {return (a > b) ? a : b;}

static int node_height(ArvNode *node) 
{
    if (node == NULL) return 0;
    return node->height;
}

static int get_balance(ArvNode *node) 
{
    if (node == NULL) return 0;
    return node_height(node-> esq) - node_height(node-> dir);
}

static ArvNode* new_node(item data) 
{
    struct NodeSt *node = (ArvNode*)malloc(sizeof(ArvNode));
    node->data = data;
    node->esq = NULL;
    node->dir = NULL;
    node->height = 1; // Novo nó começa com altura 1
    return node;
}

static ArvNode *rotate_right(ArvNode *y) 
{
    ArvNode *x = y->esq;
    ArvNode *T2 = x->dir;

    // Realiza a rotação
    x->dir = y;
    y->esq = T2;

    // Atualiza alturas (Importante: Y primeiro, pois ele agora é filho de X)
    y->height = max(node_height(y->esq), node_height(y->dir)) + 1;
    x->height = max(node_height(x->esq), node_height(x->dir)) + 1;

    return x; // Nova raiz
}

static ArvNode* rotate_left(ArvNode *x) {
    ArvNode *y = x->dir;
    ArvNode *T2 = y->esq;

    // Realiza a rotação
    y->esq = x;
    x->dir = T2;

    // Atualiza alturas
    x->height = max(node_height(x->esq), node_height(x->dir)) + 1;
    y->height = max(node_height(y->esq), node_height(y->dir)) + 1;

    return y; // Nova raiz
}

static ArvNode* insert_recursive(ArvNode* node, item data, Arvore *a, bool *inserted) 
{
    // 1. Inserção normal de BST
    if (node == NULL) {
        *inserted = true; // Marca que inseriu com sucesso
        return new_node(data);
    }

    // Usa a função de comparação do usuário
    int cmp = a->cmp_data(data, node->data);

    if (cmp < 0)
        node->esq = insert_recursive(node->dir, data, a, inserted);
    else if (cmp > 0)
        node->dir = insert_recursive(node->dir, data, a, inserted);
    else 
        return node; // Chaves duplicadas não são permitidas na AVL padrão

    // 2. Atualiza a altura do ancestral
    node->height = 1 + max(node_height(node->esq), node_height(node->dir));

    // 3. Obtém o fator de balanceamento
    int balance = get_balance(node);

    // 4. Se o nó ficou desbalanceado, temos 4 casos:

    // Caso Esquerda-Esquerda
    if (balance > 1 && a->cmp_data(data, node->esq->data) < 0)
        return rotate_right(node);

    // Caso Direita-Direita
    if (balance < -1 && a->cmp_data(data, node->dir->data) > 0)
        return rotate_left(node);

    // Caso Esquerda-Direita
    if (balance > 1 && a->cmp_data(data, node->esq->data) > 0) {
        node->esq = rotate_left(node->esq);
        return rotate_right(node);
    }

    // Caso Direita-Esquerda
    if (balance < -1 && a->cmp_data(data, node->dir->data) < 0) {
        node->dir = rotate_right(node->dir);
        return rotate_left(node);
    }

    return node; // Retorna o ponteiro (inalterado ou rotacionado)
}


// --- Funções Públicas ---

Arvore *arvore_create(int (*cmp_data)(const void *a, const void *b), void (*free_data)(void *data), void (*print_data)(void *data))
{
    if (cmp_data == NULL) {
        printf("Erro: Funcao de comparacao obrigatoria em arvore_create.\n");
        exit(1);
    }
    
    Arvore *a = malloc(sizeof(Arvore));
    if (a == NULL) {
        printf("Erro: Arvore nula em arvore_create\n");
        return NULL;
    }

    a -> root = NULL;
    a -> size = 0;
    a -> cmp_data = cmp_data;
    a -> free_data = free_data;
    a -> print_data = print_data;

    return a;
}



void arvore_insert(Arvore *a, item data) 
{
    if (a == NULL) return;

    bool inserted = false;
    
    // Convertendo ArvNode* para struct NodeSt* internamente
    ArvNode *raiz = (ArvNode*)a->root;
    
    a->root = (ArvNode*)insert_recursive(raiz, data, a, &inserted);
    
    if (inserted) {
        a->size++;
    }
}