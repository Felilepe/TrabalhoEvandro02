#include "arvore.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct Node 
{
    void *data;
    struct Node *left;
    struct Node *right;
    int height; 
} Node;

struct stArvore 
{
    Node *root;
    int tamanho;
    CompareFunc cmp;
    ActionFunc free_data; 
};


static int altura_no(Node *n) {return (n == NULL) ? -1 : n->height;}

static int maior(int a, int b) {return (a > b) ? a : b;}

static void atualizar_altura(Node *n) 
{
    if (n) {
        n->height = 1 + maior(altura_no(n->left), altura_no(n->right));
    }
}

static int fator_balanceamento(Node *n) 
{
    if (n == NULL) return 0;
    return altura_no(n->left) - altura_no(n->right);
}

static Node* rotar_direita(Node *y) 
{
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    atualizar_altura(y);
    atualizar_altura(x);

    return x;
}

static Node* rotar_esquerda(Node *x) 
{
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    atualizar_altura(x);
    atualizar_altura(y);

    return y;
}

static Node* balancear(Node *no) 
{
    if (no == NULL) return NULL;

    atualizar_altura(no);
    int fb = fator_balanceamento(no);

    if (fb > 1 && fator_balanceamento(no->left) >= 0)
        return rotar_direita(no);

    if (fb > 1 && fator_balanceamento(no->left) < 0) {
        no->left = rotar_esquerda(no->left);
        return rotar_direita(no);
    }

    if (fb < -1 && fator_balanceamento(no->right) <= 0)
        return rotar_esquerda(no);

    if (fb < -1 && fator_balanceamento(no->right) > 0) {
        no->right = rotar_direita(no->right);
        return rotar_esquerda(no);
    }

    return no;
}


static Node* inserir_rec(Node* no, void* data, CompareFunc cmp, int *incrementou) 
{
    if (no == NULL) {
        Node* novo = (Node*) malloc(sizeof(Node));
        if (!novo) return NULL;
        novo->data = data;
        novo->left = NULL;
        novo->right = NULL;
        novo->height = 0;
        *incrementou = 1; 
        return novo;
    }

    int comp = cmp(data, no->data);

    if (comp < 0)
        no->left = inserir_rec(no->left, data, cmp, incrementou);
    else if (comp > 0)
        no->right = inserir_rec(no->right, data, cmp, incrementou);
    else {
        return no; 
    }

    return balancear(no);
}

static Node* menor_no(Node* no) 
{
    Node* atual = no;
    while (atual->left != NULL)
        atual = atual->left;
    return atual;
}

static Node* remover_rec(Node* no, void* key, CompareFunc cmp, void** dado_removido, int *decrementou) 
{
    if (no == NULL) return NULL;

    int comp = cmp(key, no->data);

    if (comp < 0) {
        no->left = remover_rec(no->left, key, cmp, dado_removido, decrementou);
    } else if (comp > 0) {
        no->right = remover_rec(no->right, key, cmp, dado_removido, decrementou);
    } else {
        *dado_removido = no->data; 

        if ((no->left == NULL) || (no->right == NULL)) {
            Node *temp = (no->left) ? no->left : no->right;
            
            if (temp == NULL) { 
                temp = no;
                no = NULL;
            } else { 
                *no = *temp; 
                free(temp); 
            }
            
            if (no == NULL) { 
                free(temp);
                *decrementou = 1;
                return NULL;
            }
            *decrementou = 1;
        } else {
            Node* temp = menor_no(no->right);
            no->data = temp->data; 
            void *lixo; 
            int dec_temp = 0;
            no->right = remover_rec(no->right, temp->data, cmp, &lixo, &dec_temp);
            if(dec_temp) *decrementou = 1; 
        }
    }

    if (no == NULL) return NULL;
    return balancear(no);
}

static void* buscar_rec(Node *no, void *key, CompareFunc cmp) 
{
    if (no == NULL) return NULL;
    int res = cmp(key, no->data);
    if (res == 0) return no->data;
    if (res < 0) return buscar_rec(no->left, key, cmp);
    return buscar_rec(no->right, key, cmp);
}

static void destruir_rec(Node *no, ActionFunc free_data) 
{
    if (no != NULL) {
        destruir_rec(no->left, free_data);
        destruir_rec(no->right, free_data);
        if (free_data) free_data(no->data);
        free(no);
    }
}

static void em_ordem_rec(Node *no, ActionFunc action) 
{
    if (no) {
        em_ordem_rec(no->left, action);
        action(no->data);
        em_ordem_rec(no->right, action);
    }
}
static void pre_ordem_rec(Node *no, ActionFunc action) 
{
    if (no) {
        action(no->data);
        pre_ordem_rec(no->left, action);
        pre_ordem_rec(no->right, action);
    }
}
static void pos_ordem_rec(Node *no, ActionFunc action) 
{
    if (no) {
        pos_ordem_rec(no->left, action);
        pos_ordem_rec(no->right, action);
        action(no->data);
    }
}


Arvore* arvore_create(CompareFunc cmp, ActionFunc free_data) 
{
    Arvore *arv = (Arvore*) malloc(sizeof(Arvore));
    if (arv) {
        arv->root = NULL;
        arv->tamanho = 0;
        arv->cmp = cmp;
        arv->free_data = free_data;
    }
    return arv;
}

void arvore_destroy(Arvore *arvore) 
{
    if (arvore) {
        destruir_rec(arvore->root, arvore->free_data);
        free(arvore);
    }
}

void arvore_insert(Arvore *arvore, void *data) 
{
    if (arvore) {
        int incrementou = 0;
        arvore->root = inserir_rec(arvore->root, data, arvore->cmp, &incrementou);
        if (incrementou) arvore->tamanho++;
    }
}

void* arvore_remove(Arvore *arvore, void *key) 
{
    if (!arvore || !arvore->root) return NULL;
    
    void *dado_removido = NULL;
    int decrementou = 0;
    
    arvore->root = remover_rec(arvore->root, key, arvore->cmp, &dado_removido, &decrementou);
    
    if (decrementou) arvore->tamanho--;
    
    return dado_removido;
}

void* arvore_search(Arvore *arvore, void *key) 
{
    if (arvore) return buscar_rec(arvore->root, key, arvore->cmp);
    return NULL;
}

int arvore_getHeight(Arvore *arvore) {return arvore ? altura_no(arvore->root) : -1;}

int arvore_getSize(Arvore *arvore) {return arvore ? arvore->tamanho : 0;}

bool arvore_isEmpty(Arvore *arvore) {return arvore ? (arvore->tamanho == 0) : true;}

void avl_em_ordem(Arvore *arvore, ActionFunc action) {if (arvore) em_ordem_rec(arvore->root, action);}

void avl_pre_ordem(Arvore *arvore, ActionFunc action) {if (arvore) pre_ordem_rec(arvore->root, action);}

void avl_pos_ordem(Arvore *arvore, ActionFunc action) {if (arvore) pos_ordem_rec(arvore->root, action);}