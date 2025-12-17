#include <stdio.h>
#include "sort.h"
#include <stdlib.h>
#include <string.h>

// --- Funções Auxiliares ---

static void merge(char *base, size_t left, size_t mid, size_t right, size_t element_size, 
                  int (*cmp)(const void* a, const void* b))  // Função interna para mesclar dois sub-arrays ordenados
{
    size_t i = left;
    size_t j = mid + 1;
    size_t k = 0;
    size_t size = right - left + 1;

    // Buffer temporário para armazenar a mistura ordenada
    char *temp = (char *)malloc(size * element_size);
    if (!temp) return;

    while (i <= mid && j <= right) {
        // Compara base[i] com base[j]
        if (cmp(base + i * element_size, base + j * element_size) <= 0) {
            memcpy(temp + k * element_size, base + i * element_size, element_size);
            i++;
        } else {
            memcpy(temp + k * element_size, base + j * element_size, element_size);
            j++;
        }
        k++;
    }

    // Copia os restantes da esquerda, se houver
    while (i <= mid) {
        memcpy(temp + k * element_size, base + i * element_size, element_size);
        i++; k++;
    }

    // Copia os restantes da direita, se houver
    while (j <= right) {
        memcpy(temp + k * element_size, base + j * element_size, element_size);
        j++; k++;
    }

    // Copia do temp de volta para o array original
    memcpy(base + left * element_size, temp, size * element_size);

    free(temp);
}

static void mergeSortRecursive(char *base, size_t left, size_t right, size_t element_size, 
                               int (*cmp)(const void* a, const void* b), int threshold) 
                               {
    if (left >= right) return;

    // Otimização: Se o tamanho for pequeno, usa Insertion Sort
    // (right - left + 1) é o tamanho atual do subarray
    if ((right - left + 1) <= (size_t)threshold) {
        // Chama o insertionSort passando o endereço do início deste bloco
        insertionSort(base + left * element_size, right - left + 1, element_size, cmp);
        return;
    }

    size_t mid = left + (right - left) / 2;

    mergeSortRecursive(base, left, mid, element_size, cmp, threshold);
    mergeSortRecursive(base, mid + 1, right, element_size, cmp, threshold);

    merge(base, left, mid, right, element_size, cmp);
}

// --- Funções Públicas ---

int sort_doubleCompare(const void* a, const void* b) 
{
    const double *x = (const double *)a;
    const double *y = (const double *)b;

    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}



void mergeSort(void* base, size_t element_count, size_t element_size, 
               int (*cmp)(const void* a, const void* b), int threshold) 
{
    if (element_count <= 1) return;
    
    mergeSortRecursive((char *)base, 0, element_count - 1, element_size, cmp, threshold);
}



void insertionSort(void* base, size_t element_count, size_t element_size, 
                   int (*cmp)(const void* a, const void* b)) 
{
    char *ptr = (char *)base;
    
    void *key = malloc(element_size);
    if (!key) return; 

    for (size_t i = 1; i < element_count; i++) {
        memcpy(key, ptr + i * element_size, element_size);

        long j = i - 1;

        while (j >= 0 && cmp(ptr + j * element_size, key) > 0) {
            memcpy(ptr + (j + 1) * element_size, ptr + j * element_size, element_size);
            j--;
        }
        
        memcpy(ptr + (j + 1) * element_size, key, element_size);
    }

    free(key);
}