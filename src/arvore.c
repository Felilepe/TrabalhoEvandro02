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
    if (node == NULL) {
        printf("Erro: Falha na alocacao de memoria para novo no (AVL).\n");
        exit(1);
    }
    
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
        node->esq = insert_recursive(node->esq, data, a, inserted);
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

static ArvNode* search_recursive(ArvNode* n, item key, int (*cmp)(const void*, const void*)) 
{
    if (n == NULL) return NULL;

    int res = cmp(key, n->data);
    
    if (res == 0) return n;
    if (res < 0) return search_recursive(n->esq, key, cmp);
    return search_recursive(n->dir, key, cmp);
}

static void print_recursive(ArvNode *n, void (*print_fn)(void*)) 
{
    if (n != NULL) {
        print_recursive(n->esq, print_fn);
        print_fn(n->data); 
        print_recursive(n->dir, print_fn);
    }
}

static void destroy_recursive(ArvNode *n, void (*free_data)(void*)) 
{
    if (n == NULL) return;
    
    destroy_recursive(n->esq, free_data);
    destroy_recursive(n->dir, free_data);
    
    if (free_data != NULL) {
        free_data(n->data);
    }
    free(n);
}

static ArvNode* get_min_node(ArvNode* n) 
{
    ArvNode* current = n;
    while (current->esq != NULL)
        current = current->esq;
    return current;
}

static ArvNode* delete_node_avl(ArvNode* root, item key, Arvore *a, bool *decremented) 
{
    if (root == NULL) return root;

    int cmp = a->cmp_data(key, root->data);

    if (cmp < 0)
        root->esq = delete_node_avl(root->esq, key, a, decremented);
    else if (cmp > 0)
        root->dir = delete_node_avl(root->dir, key, a, decremented);
    else {
        // Encontrou o nó a remover
        
        // Casos com 0 ou 1 filho
        if ((root->esq == NULL) || (root->dir == NULL)) {
            ArvNode *temp = root->esq ? root->esq : root->dir;

            // Caso sem filhos
            if (temp == NULL) {
                if(a->free_data) a->free_data(root->data);
                free(root);
                root = NULL;
            } 
            // Caso com 1 filho
            else {
                if (a->free_data) a->free_data(root->data);

                *root = *temp; // Agora sim, sobrescrevemos com segurança
                free(temp);
            }
            if(decremented) *decremented = true;
        } 
        else {
            // Caso com 2 filhos: Pega o sucessor (menor da direita)
            ArvNode* temp = get_min_node(root->dir);

            // Trocamos os DADOS de lugar. O dado a ser removido vai para a folha.
            // O dado do sucessor sobe para cá.
            item aux = root->data;
            root->data = temp->data;
            temp->data = aux;

            // Agora chamamos recursivamente para remover o nó que desceu (que agora está numa posição fácil de remover)
            // Note: Passamos temp->data (o dado antigo que queremos remover)
            root->dir = delete_node_avl(root->dir, aux, a, decremented);
        }
    }

    if (root == NULL) return root;

    // Rebalanceamento AVL
    root->height = 1 + max(node_height(root->esq), node_height(root->dir));
    int balance = get_balance(root);

    // Left Left
    if (balance > 1 && get_balance(root->esq) >= 0)
        return rotate_right(root);

    // Left Right
    if (balance > 1 && get_balance(root->esq) < 0) {
        root->esq = rotate_left(root->esq);
        return rotate_right(root);
    }

    // Right Right
    if (balance < -1 && get_balance(root->dir) <= 0)
        return rotate_left(root);

    // Right Left
    if (balance < -1 && get_balance(root->dir) > 0) {
        root->dir = rotate_right(root->dir);
        return rotate_left(root);
    }

    return root;
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

void arvore_destroy(Arvore *a) 
{
    if (a == NULL) return;
    destroy_recursive(a->root, a->free_data);
    free(a);
}



void arvore_insert(Arvore *a, item data) 
{
    if (a == NULL) return;

    bool inserted = false;
    
    ArvNode *raiz = (ArvNode*)a->root;
    
    a->root = (ArvNode*)insert_recursive(raiz, data, a, &inserted);
    
    if (inserted) {
        a->size++;
    }
}

void arvore_remove(Arvore *a, item key) 
{
    if (a == NULL || a->root == NULL) return;
    bool decremented = false;
    a->root = delete_node_avl(a->root, key, a, &decremented);
    if(decremented) a->size--;
}



item arvore_search(Arvore *a, item key) 
{
    if (a == NULL) return NULL;
    ArvNode *res = search_recursive(a->root, key, a->cmp_data);
    return (res != NULL) ? res->data : NULL;
}

void arvore_print(Arvore *a) 
{
    if (a == NULL || a->print_data == NULL) return;
    print_recursive(a->root, a->print_data);
}

bool arvore_isEmpty(Arvore *a) 
{
    if (a == NULL) return true;
    return a->size == 0;
}



int arvore_getHeight(Arvore *a) {

    if (a == NULL) return 0;
    return node_height(a->root);
}

int arvore_getSize(Arvore *a) 
{
    if (a == NULL) return 0;
    return a->size;
}

item arvore_getNodeData(ArvNode *n) 
{
    if (n == NULL) return NULL;
    return n->data;
}

ArvNode* arvore_getRootNode(Arvore *a) 
{
    if (a == NULL) return NULL;
    return a->root;
}

ArvNode* arvore_getRightNode(ArvNode *n) 
{
    if (n == NULL) return NULL;
    return n->dir;
}

ArvNode* arvore_getLeftNode(ArvNode *n) 
{
    if (n == NULL) return NULL;
    return n->esq;
}