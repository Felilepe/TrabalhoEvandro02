#include "arvore.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct stNode 
{
    void *data;
    struct stNode *esquerda;
    struct stNode *direita;
    int altura;
} node_AVL;

typedef struct stArvore 
{
    node_AVL *root;
    int tamanho;
    int (*cmp_data)(const void *a, const void *b);
    void (*free_data)(void *data);
    void (*print_data)(void *data);
} arvore;

arvore *arvore_create(int (*cmp_data)(const void *a, const void *b), void (*free_data)(void *data), void (*print_data)(void *data)) {
    arvore *a = malloc (sizeof(arvore));
    if (a == NULL) {
        printf("ÁRVORE: Erro ao criar a estrutura árvore!\n");
        return NULL;
    }

    a -> root = NULL;
    a -> tamanho = 0;
    a -> cmp_data = cmp_data;
    a -> free_data = free_data;
    a -> print_data = print_data;

    return a;
}

node_AVL *arvore_getRoot(arvore *t) {
    return t -> root;
}

static int get_altura_node(node_AVL *n) {
    if (n == NULL) return -1;
    return n -> altura;
}

int arvore_getHeight(arvore *t) {
    if (t == NULL || t -> root == NULL) return -1;
    return get_altura_node(t -> root);
}

arvore_getSize(arvore *t) {
    return t -> tamanho;
}

static int get_fb(node_AVL *n) {
    if (n == NULL) return 0;
    return get_altura_node(n -> esquerda) - get_altura_node(n -> direita);
}

node_AVL *get_esquerda_node(node_AVL *n) {
    if (n == NULL) return NULL;
    return n -> esquerda;
}

node_AVL *get_direita_node(node_AVL *n) {
    if (n == NULL) return NULL;
    return n -> direita;
}

void *get_node_dataAVL(node_AVL *n) {
    if (n == NULL) return NULL;
    return n -> data;
}

bool is_empty_avl(arvore *avl) {
    if (avl -> tamanho == 0) {
        return true;
    }

    return false;
}

static int maior_altura(int a, int b) {
    return (a > b)? a : b;
}

static node_AVL *rotate_left(node_AVL *r) {
    node_AVL *y, *f;

    y = r -> direita;
    f = y -> esquerda;

    y -> esquerda = r;
    r -> direita = f;

    r -> altura = maior_altura(get_altura_node(r -> esquerda), get_altura_node(r -> direita)) + 1;
    y -> altura = maior_altura(get_altura_node(y -> esquerda), get_altura_node(y -> direita)) + 1;

    return y;
}

static node_AVL *rotate_right(node_AVL *r) {
    node_AVL *y, *f;

    y = r -> esquerda;
    f = y -> direita;

    y -> direita = r;
    r -> esquerda = f;

    r -> altura = maior_altura(get_altura_node(r -> esquerda), get_altura_node(r -> direita)) + 1;
    y -> altura = maior_altura(get_altura_node(y -> esquerda), get_altura_node(y -> direita)) + 1;

    return y;
}

static node_AVL *rotate_right_left(node_AVL *r) {
    r -> direita = rotate_right(r -> direita);
    return rotate_left(r);
}

static node_AVL *rotate_left_right(node_AVL *r) {
    r -> esquerda = rotate_left(r -> esquerda);
    return rotate_right(r);
}

static node_AVL *create_node(void *data) {
    node_AVL *node = malloc (sizeof(node_AVL));
    if (node == NULL) {
        printf("ÁRVORE: Erro ao criar nó para a árvore!\n");
        return NULL;
    }

    node -> data = data;
    node -> esquerda = NULL;
    node -> direita = NULL;
    node -> altura = 0;

    return node;
}

static int recalc_altura(node_AVL *node) {

    return maior_altura(get_altura_node(node -> esquerda), get_altura_node(node -> direita)) + 1;
}

static node_AVL *balancear(node_AVL *r) {
    int fb = get_fb(r);

    if (fb < -1 && get_fb(r -> direita) <= 0) {
        r = rotate_left(r);
    }

    else if (fb > 1 && get_fb(r -> esquerda) >= 0) {
        r = rotate_right(r);
    }

    else if (fb > 1 && get_fb(r -> esquerda) < 0) {
        r = rotate_left_right(r);
    }

    else if (fb < -1 && get_fb(r -> direita) > 0) {
        r = rotate_right_left(r);
    }

    return r;
}

static node_AVL *insert_bst(arvore *t, node_AVL *node, void *data) {
    if (node == NULL) {
        node_AVL *new_node = create_node(data);
        if (new_node != NULL) t -> tamanho++;
        return new_node;
    }

    int c = t -> cmp_data(data, node -> data);

    if (c < 0) {
        node -> esquerda = insert_bst(t, node -> esquerda, data);
    }

    else if (c > 0) {
        node -> direita = insert_bst(t, node -> direita, data);
    }

    else {
        if (t -> free_data != NULL) {
            t -> free_data(data);
        }
    }

    node -> altura = recalc_altura(node);
    node = balancear(node);

    return node;

}

void arvore_insere(arvore *t, void *data) {
    t -> root = insert_bst(t, t -> root, data);
}

void *binary_search(arvore *t, void *key) {
    if (t == NULL || t -> root == NULL) {
        return NULL;
    }

    node_AVL *current = t -> root;

    while (current != NULL) {
        int comparacao = t -> cmp_data(key, current -> data);

        if (comparacao == 0) {
            return current -> data;
        }

        if (comparacao < 0) {
            current = current -> esquerda;
        }

        else {
            current = current -> direita;
        }
    }

    return NULL;
}

static node_AVL *find_min(node_AVL *node) {
    if (node == NULL) return NULL;

    if (node -> esquerda == NULL) return node;
    return find_min(node -> esquerda);
}

static node_AVL *remove_node_recursivo(arvore *t, node_AVL *root, void *key, void **removed_data) {
    if (root == NULL) {
        *removed_data = NULL;
        return NULL;
    }

    if (key == NULL) return NULL;

    int cmp = t -> cmp_data(key, root -> data);

    if (cmp < 0) {
        root -> esquerda = remove_node_recursivo(t, root -> esquerda, key, removed_data);
    } else if (cmp > 0) {
        root -> direita = remove_node_recursivo(t, root -> direita, key, removed_data);
    } else {
        *removed_data = root -> data;

        if (root -> esquerda == NULL) {
            node_AVL *temp = root -> direita;
            free(root);
            t -> tamanho--;
            return temp;
        }
        if (root -> direita == NULL) {
            node_AVL *temp = root -> esquerda;
            free(root);
            t -> tamanho--;
            return temp;
        }

        void *original_data = root -> data;
        node_AVL *sucessor = find_min(root -> direita);
        root -> data = sucessor -> data;

        void *r = NULL;
        root -> direita = remove_node_recursivo(t, root -> direita, sucessor -> data, &r);

        *removed_data = original_data;
    }


    root -> altura = recalc_altura(root);
    root = balancear(root);

    return root;
}

void *remove_node(arvore *t, void *key) {
    if (t == NULL || key == NULL) {
        return NULL;
    }

    void *removed_data = NULL;

    t -> root = remove_node_recursivo(t, t -> root, key, &removed_data);


    return removed_data;
}

static void in_order_recursivo(arvore *t, node_AVL *root) {
    if (root == NULL) return;

    in_order_recursivo(t, root -> esquerda);
    t -> print_data(root -> data);
    in_order_recursivo(t, root -> direita);
}

void in_order(arvore *t) {
    in_order_recursivo(t, t -> root);
}

static void free_nodes_recursivo(arvore *t, node_AVL *node) {
    if (node == NULL) {
        return;
    }

    free_nodes_recursivo(t, node -> esquerda);
    free_nodes_recursivo(t, node -> direita);

    if (t -> free_data != NULL) {
        t -> free_data(node -> data);
    }

    free(node);
}

void free_arvore(arvore *t) {
    if (t == NULL) {
        return;
    }

    free_nodes_recursivo(t, t -> root);

    free(t);
}
