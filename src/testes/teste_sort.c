#include "../sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int cmp_int(const void *a, const void *b) {
    return (*(const int *)a > *(const int *)b) - (*(const int *)a < *(const int *)b);
}

void teste_logica_threshold() {
    printf("--- Teste 1: Lógica do Threshold (-i) (threshold = 10) ---\n");
    int threshold_i = 10;


    printf("  Sub-teste A: n=9 (n < 10). Deve usar insertion_sort.\n");
    int arr_a[] = {9, 1, 8, 2, 7, 3, 6, 4, 5};
    size_t n_a = sizeof(arr_a) / sizeof(int);

    mergeSort(arr_a, n_a, sizeof(int), cmp_int, threshold_i);

    for (int i = 0; i < n_a - 1; i++) {
        if (arr_a[i] > arr_a[i+1]) {
            printf("    >> DEBUG: ERRO! Teste A (n=9) falhou na ordenação.\n");
            return;
        }
    }
    printf("    >> OK! Teste A (n=9) ordenado com sucesso.\n");


    printf("  Sub-teste B: n=11 (n > 10). Deve usar recursão + merge.\n");
    int arr_b[] = {11, 1, 10, 2, 9, 3, 8, 4, 7, 5, 6};
    size_t n_b = sizeof(arr_b) / sizeof(int);

    mergeSort(arr_b, n_b, sizeof(int), cmp_int, threshold_i);

    for (int i = 0; i < n_b - 1; i++) {
        if (arr_b[i] > arr_b[i+1]) {
            printf("    >> DEBUG: ERRO! Teste B (n=11) falhou na ordenação.\n");
            return;
        }
    }
    printf("    >> OK! Teste B (n=11) ordenado com sucesso.\n\n");
}

void teste_mergesort_doubles_reverso() {
    printf("--- Teste 2: Mergesort com Doubles (Array Reverso) ---\n");
    double arr[] = {10.5, 9.4, 8.3, 7.2, 6.1, 5.9, 4.8, 3.7, 2.6, 1.5, 0.4, -1.3};
    size_t n = sizeof(arr) / sizeof(double);
    int threshold_teste = 3;

    printf("  Antes : [");
    for(int i=0; i<n; i++) printf("%.1f ", arr[i]);
    printf("]\n");

    mergeSort(arr, n, sizeof(double), sort_doubleCompare, threshold_teste);

    printf("  Depois: [");
    for(int i=0; i<n; i++) printf("%.1f ", arr[i]);
    printf("]\n");

    for(int i=0; i < n - 1; i++) {
        if (arr[i] > arr[i+1] + 1e-9) {
            printf("  >> DEBUG: ERRO! Array de doubles não foi ordenado.\n");
            return;
        }
    }
    printf("  >> OK! Array de doubles ordenado com sucesso.\n\n");
}

void teste_insertion_direto() {
    printf("--- Teste 3: Insertion Sort (Chamada Direta) ---\n");
    int arr[] = {5, 1, 4, 2, 3};
    size_t n = sizeof(arr) / sizeof(int);

    insertionSort(arr, n, sizeof(int), cmp_int);

    if (arr[0] != 1 || arr[1] != 2 || arr[2] != 3 || arr[3] != 4 || arr[4] != 5) {
         printf("  >> DEBUG: ERRO! Insertion sort direto falhou.\n");
         return;
    }
    printf("  >> OK! Insertion sort direto ordenado com sucesso.\n\n");
}

void teste_casos_borda() {
    printf("--- Teste 4: Casos de Borda (n=0 e n=1) ---\n");
    int arr_um[] = {100};

    mergeSort(NULL, 0, sizeof(int), cmp_int, 10);
    printf("  Teste n=0 (NULL) ... OK (não crashou)\n");

    mergeSort(arr_um, 1, sizeof(int), cmp_int, 10);
    if (arr_um[0] == 100) {
        printf("  Teste n=1 ... OK (não crashou e não modificou)\n");
    } else {
        printf("  >> DEBUG: ERRO! Teste n=1 modificou o dado.\n");
    }
    printf("\n");
}

int main() {
    printf("  INICIANDO TESTE UNITÁRIO DO MÓDULO SORT\n");
    printf("  (sort.c e sort.h)\n");

    teste_logica_threshold();
    teste_mergesort_doubles_reverso();
    teste_insertion_direto();
    teste_casos_borda();

    printf("--- TESTES DO MÓDULO SORT CONCLUÍDOS ---\n");
    return 0;
}