#include <stdio.h>
#include <stdlib.h>
#include "../lista.h"

int compare_ints(void* a, void* b) {
    int int_a = *(int*)a;
    int int_b = *(int*)b;

    return (int_a - int_b);
}

void print_array(int* arr, size_t num) {
    for (size_t i = 0; i < num; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void print_int_data(void *data) {
    printf("%d ", *(int*)data);
}

void free_int_ptr(void *data) {
    free(data);
}

void teste_merge_sort() {
    printf("\nTeste: merge_sort_lista (TAD lista)\n");

    Lista *minha_lista  = lista_create();

    int valores[] = {8, 2, 5, 1, 10, 9, 3, 20, 39, 348, 10, 348, 47, 138, 1374, 1993, 0, 88, 38, -4, -3, -1};
    size_t num_lista = sizeof(valores) / sizeof(valores[0]);

    for (size_t i = 0; i < num_lista; i++) {
        int* dado = (int*)malloc(sizeof(int));
        *dado = valores[i];
        insert_tail(minha_lista, dado);
    }

    printf("Lista ANTES:  ");
    print_lista(minha_lista, print_int_data);
    printf("\n");

    merge_sort_lista(minha_lista, compare_ints);

    printf("Lista DEPOIS: ");
    print_lista(minha_lista, print_int_data);
    printf("\n");


    free_lista(minha_lista, free_int_ptr);
}

void teste_insertion_sort() {
    printf("Teste: insertion_sort_lista (TAD lista)\n");

    lista* minha_lista = lista_create();

    int valores[] = {8, 2, 5, 1, 10, 9, 3};
    size_t num_lista = sizeof(valores) / sizeof(valores[0]);

    for (size_t i = 0; i < num_lista; i++) {
        int* dado = (int*)malloc(sizeof(int));
        *dado = valores[i];
        insert_tail(minha_lista, dado);
    }

    printf("Lista ANTES:  ");
    print_lista(minha_lista, print_int_data);
    printf("\n");

    insertion_sort_lista(minha_lista, compare_ints);

    printf("Lista DEPOIS: ");
    print_lista(minha_lista, print_int_data);
    printf("\n");

    free_lista(minha_lista, free_int_ptr);


}

int main() {

    teste_insertion_sort();
    teste_merge_sort();

    printf("\nTestes concluidos.\n");

    return 0;
}