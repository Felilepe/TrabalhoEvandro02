#include "../arvore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int cmp_int(const void *a, const void *b) {
    int *ia = (int *)a;
    int *ib = (int *)b;
    return (*ia - *ib);
}

void free_int(void *data) {
    free(data);
}

void print_int(void *data) {
    printf("%d ", *(int *)data);
}

int *create_int(int value) {
    int *p = malloc(sizeof(int));
    if (p) *p = value;
    return p;
}

int cmp_string(const void *a, const void *b) {
    return strcmp((char *)a, (char *)b);
}

void free_string(void *data) {
    free(data);
}

void print_string(void *data) {
    printf("%s ", (char *)data);
}

char *create_string(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str) + 1;
    char *copy = malloc(len);
    if (copy) {
        memcpy(copy, str, len);
    }
    return copy;
}


void test_init() {
    printf("\n--- teste 1: inicializacao ---\n");

    Arvore *t = arvore_create(cmp_int, free_int, print_int);

    if (t == NULL) {
        printf("erro ao criar arvore\n");
        return;
    }

    if (arvore_isEmpty(t)) {
        printf("arvore criada vazia\n");
    }

    printf("tamanho: %d\n", arvore_getSize(t));
    printf("altura: %d\n", arvore_getHeight(t));

    arvore_destroy(t);
}

void test_insert() {
    printf("\n--- teste 2: insercao ---\n");

    Arvore *t = arvore_create(cmp_int, free_int, print_int);

    int valores[] = {10, 5, 15, 3, 7, 12, 17};
    int n = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < n; i++) {
        arvore_insert(t, create_int(valores[i]));
    }

    printf("inseridos %d elementos\n", n);
    printf("tamanho: %d\n", arvore_getSize(t));
    printf("altura: %d\n", arvore_getHeight(t));

    printf("in-order: ");
    arvore_print(t);
    printf("\n");

    for (int i = 0; i < n; i++) {
        int key = valores[i];
        arvore_remove(t, &key);

    }
    arvore_destroy(t);
}

void test_duplicado() {
    printf("\n--- teste 3: insercao duplicada ---\n");

    Arvore *t = arvore_create(cmp_int, free_int, print_int);

    arvore_insert(t, create_int(10));
    int tam_antes = arvore_getSize(t);

    arvore_insert(t, create_int(10));
    int tam_depois = arvore_getSize(t);

    printf("tamanho antes: %d\n", tam_antes);
    printf("tamanho depois: %d\n", tam_depois);

    if (tam_antes == tam_depois) {
        printf("duplicado nao inserido\n");
    }

    int key = 10;
    arvore_remove(t, &key);

    arvore_destroy(t);
}

void test_busca() {
    printf("\n--- teste 4: busca ---\n");

    Arvore *t = arvore_create(cmp_int, free_int, print_int);

    int valores[] = {50, 30, 70, 20, 40, 60, 80};
    int n = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < n; i++) {
        arvore_insert(t, create_int(valores[i]));
    }

    for (int i = 0; i < n; i++) {
        int *result = (int *)arvore_search(t, &valores[i]);
        if (result != NULL) {
            printf("encontrado: %d\n", valores[i]);
        }
    }


    int inexistente = 999;
    void *result = arvore_search(t, &inexistente);
    if (result == NULL) {
        printf("999 nao encontrado\n");
    }

    for (int i = 0; i < n; i++) {
        arvore_remove(t, &valores[i]);
    }
    arvore_destroy(t);
}

void test_remocao_folha() {
    printf("\n--- teste 5: remocao de folha ---\n");

    Arvore *t = arvore_create(cmp_int, free_int, print_int);

    arvore_insert(t, create_int(10));
    arvore_insert(t, create_int(5));
    arvore_insert(t, create_int(15));

    printf("antes: ");
    arvore_print(t);
    printf("\n");

    int key = 5;
    void *data = arvore_search(t, &key);
    if (data) {
        printf("removido: %d\n", *(int *)data);
        arvore_remove(t, &key);
    }

    printf("depois: ");
    arvore_print(t);
    printf("\n");
    printf("tamanho: %d\n", arvore_getSize(t));


    key = 10;
    arvore_remove(t, &key);
    key = 15;
    arvore_remove(t, &key);

    arvore_destroy(t);
}

void test_remocao_um_filho() {
    printf("\n--- teste 6: remocao com 1 filho ---\n");

    Arvore *t = arvore_create(cmp_int, free_int, print_int);

    arvore_insert(t, create_int(10));
    arvore_insert(t, create_int(5));
    arvore_insert(t, create_int(3));

    printf("antes: ");
    arvore_print(t);
    printf("\n");

    int key = 5;
    void *data = arvore_search(t, &key);
    if (data) {
        printf("removido: %d\n", *(int *)data);
        arvore_remove(t, &key);
    }

    printf("depois: ");
    arvore_print(t);
    printf("\n");


    key = 10;
    arvore_remove(t, &key);
    key = 3;
    arvore_remove(t, &key);

    arvore_destroy(t);
}

void test_remocao_dois_filhos() {
    printf("\n--- teste 7: remocao com 2 filhos ---\n");

    Arvore *t = arvore_create(cmp_int, free_int, print_int);

    int valores[] = {50, 30, 70, 20, 40, 60, 80};
    int n = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < n; i++) {
        arvore_insert(t, create_int(valores[i]));
    }

    printf("antes: ");
    arvore_print(t);
    printf("\n");

    int key = 50;
    void *data = arvore_search(t, &key);
    if (data) {
        printf("removido: %d\n", *(int *)data);
        arvore_remove(t, &key);
    }

    printf("depois: ");
    arvore_print(t);
    printf("\n");


    for (int i = 0; i < n; i++) {
        if (valores[i] == 50) continue;
        arvore_remove(t, &valores[i]);
    }


    arvore_destroy(t);
}

void test_remocao_inexistente() {
    printf("\n--- teste 8: remocao de elemento inexistente ---\n");

    Arvore *t = arvore_create(cmp_int, free_int, print_int);

    arvore_insert(t, create_int(10));
    arvore_insert(t, create_int(5));

    int tam_antes = arvore_getSize(t);

    int key = 999;
    void *data = arvore_search(t, &key);

    int tam_depois = arvore_getSize(t);

    if (data == NULL) {
        printf("elemento nao encontrado\n");
    }
    arvore_remove(t, &key);

    printf("tamanho antes: %d, depois: %d\n", tam_antes, tam_depois);

    key = 10;
    arvore_remove(t, &key);
    key = 5;
    arvore_remove(t, &key);

    arvore_destroy(t);
}

void test_balanceamento() {
    printf("\n--- teste 9: balanceamento ---\n");

    Arvore *t = arvore_create(cmp_int, free_int, print_int);

    printf("inserindo 1, 2, 3\n");
    arvore_insert(t, create_int(1));
    arvore_insert(t, create_int(2));
    arvore_insert(t, create_int(3));

    printf("in-order: ");
    arvore_print(t);
    printf("\n");
    printf("altura: %d\n", arvore_getHeight(t));

    for (int i = 1; i <= 3; i++) {
        arvore_remove(t, &i);
    }

    printf("\ninserindo 3, 2, 1\n");
    arvore_insert(t, create_int(3));
    arvore_insert(t, create_int(2));
    arvore_insert(t, create_int(1));

    printf("in-order: ");
    arvore_print(t);
    printf("\n");
    printf("altura: %d\n", arvore_getHeight(t));


    for (int i = 1; i <= 3; i++) {
        arvore_remove(t, &i);
    }

    printf("\ninserindo 3, 1, 2\n");
    arvore_insert(t, create_int(3));
    arvore_insert(t, create_int(1));
    arvore_insert(t, create_int(2));

    printf("in-order: ");
    arvore_print(t);
    printf("\n");
    printf("altura: %d\n", arvore_getHeight(t));


    for (int i = 1; i <= 3; i++) {
        arvore_remove(t, &i);
    }

    arvore_destroy(t);
}

void test_string() {
    printf("\n--- teste 10: arvore de strings ---\n");

    Arvore *t = arvore_create(cmp_string, free_string, print_string);

    char *palavras[] = {"dog", "cat", "elephant", "ant", "bear"};
    int n = sizeof(palavras) / sizeof(palavras[0]);

    for (int i = 0; i < n; i++) {
        arvore_insert(t, create_string(palavras[i]));
    }

    printf("palavras inseridas: %d\n", arvore_getSize(t));
    printf("altura: %d\n", arvore_getHeight(t));
    printf("in-order: ");
    arvore_print(t);
    printf("\n");

    char *busca = "cat";
    char *result = (char *)arvore_search(t, busca);
    if (result != NULL) {
        printf("encontrado: %s\n", result);
    }

    char *inexistente = "zebra";
    result = (char *)arvore_search(t, inexistente);
    if (result == NULL) {
        printf("zebra nao encontrada\n");
    }


    for (int i = 0; i < n; i++) {
        arvore_remove(t, palavras[i]);
    }

    arvore_destroy(t);
}

void test_estresse() {
    printf("\n--- teste 11: estresse ---\n");

    Arvore *t = arvore_create(cmp_int, free_int, print_int);

    int n = 100;
    printf("inserindo %d elementos\n", n);

    for (int i = 0; i < n; i++) {
        arvore_insert(t, create_int(i));
    }

    printf("tamanho: %d\n", arvore_getSize(t));
    printf("altura: %d\n", arvore_getHeight(t));

    int encontrados = 0;
    for (int i = 0; i < n; i++) {
        int *result = (int *)arvore_search(t, &i);
        if (result != NULL) {
            encontrados++;
        }
    }
    printf("buscas bem sucedidas: %d/%d\n", encontrados, n);


    for (int i = 0; i < n/2; i++) {
        arvore_remove(t, &i);
    }

    printf("tamanho apos remover metade: %d\n", arvore_getSize(t));


    for (int i = n/2; i < n; i++) {
        arvore_remove(t, &i);
    }

    printf("tamanho final: %d\n", arvore_getSize(t));
    printf("altura final: %d\n", arvore_getHeight(t));

    arvore_destroy(t);
}

void test_tamanho() {
    printf("\n--- teste 12: consistencia do tamanho ---\n");

    Arvore *t = arvore_create(cmp_int, free_int, print_int);

    for (int i = 0; i < 5; i++) {
        arvore_insert(t, create_int(i * 10));
        printf("inseriu %d, tamanho = %d\n", i * 10, arvore_getSize(t));
    }

    for (int i = 0; i < 5; i++) {
        int key = i * 10;
        arvore_remove(t, &key);
        printf("removeu %d, tamanho = %d\n", key, arvore_getSize(t));
    }

    arvore_destroy(t);
}

int main() {
    printf("testando arvore AVL\n");

    test_init();
    test_insert();
    test_duplicado();
    test_busca();
    test_remocao_folha();
    test_remocao_um_filho();
    test_remocao_dois_filhos();
    test_remocao_inexistente();
    test_balanceamento();
    test_string();
    test_estresse();
    test_tamanho();

    printf("\ntestes concluidos\n");

    return 0;
}