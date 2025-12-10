#ifndef SORTING_H
#define SORTING_H

#include <stddef.h> /* para size_t */

// Definição do tipo para a função de comparação
// Retorna < 0 se a < b, 0 se a == b, > 0 se a > b
typedef int (*CompareFunc)(const void *a, const void *b);

// Insertion Sort Genérico
void generic_insertion_sort(void *base, size_t nmemb, size_t size, CompareFunc cmp);

// Merge Sort Genérico
void generic_merge_sort(void *base, size_t nmemb, size_t size, CompareFunc cmp);

#endif