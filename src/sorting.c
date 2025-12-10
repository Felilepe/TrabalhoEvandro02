#include "sorting.h"
#include <stdlib.h>
#include <string.h>


void generic_insertion_sort(void *base, size_t nmemb, size_t size, CompareFunc cmp) {
    void *key = malloc(size);
    if (key == NULL){
        printf("Erro de alocação de memória na funcao generic_insertion_sort\n");
        exit(1);
    } 

    char *ptr = (char *)base;

    for (size_t i = 1; i < nmemb; i++) {
        memcpy(key, ptr + (i * size), size);

        size_t j = i; 

        while (j > 0 && cmp(ptr + ((j - 1) * size), key) > 0) {
            memmove(ptr + (j * size), ptr + ((j - 1) * size), size);
            j--;
        }
        
        memcpy(ptr + (j * size), key, size);
    }

    free(key);
}



static void merge(char *base, size_t left, size_t mid, size_t right, size_t size, CompareFunc cmp, char *temp_array) {
    size_t i = left;    
    size_t j = mid + 1; 
    size_t k = left;    

    while (i <= mid && j <= right) {
        if (cmp(base + (i * size), base + (j * size)) <= 0) {
            memcpy(temp_array + (k * size), base + (i * size), size);
            i++;
        } else {
            memcpy(temp_array + (k * size), base + (j * size), size);
            j++;
        }
        k++;
    }

    while (i <= mid) {
        memcpy(temp_array + (k * size), base + (i * size), size);
        i++;
        k++;
    }

    while (j <= right) {
        memcpy(temp_array + (k * size), base + (j * size), size);
        j++;
        k++;
    }


    size_t num_elements = right - left + 1;
    memcpy(base + (left * size), temp_array + (left * size), num_elements * size);
}

static void merge_sort_recursive(char *base, size_t left, size_t right, size_t size, CompareFunc cmp, char *temp_array) {
    if (left < right) {
        size_t mid = left + (right - left) / 2;

        merge_sort_recursive(base, left, mid, size, cmp, temp_array);
        merge_sort_recursive(base, mid + 1, right, size, cmp, temp_array);

        merge(base, left, mid, right, size, cmp, temp_array);
    }
}



void generic_merge_sort(void *base, size_t nmemb, size_t size, CompareFunc cmp) {
    if (nmemb < 2) return;

    void *temp_array = malloc(nmemb * size);
    if (temp_array == NULL){
        printf("Erro de alocação de memória na funcao generic_merge_sort\n");
        exit(1);
    }

    merge_sort_recursive((char *)base, 0, nmemb - 1, size, cmp, (char *)temp_array);

    free(temp_array);
}