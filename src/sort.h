#ifndef SORT_H
#define SORT_H
#include <stddef.h>

/****************************************************************************************
* @brief Função de comparação padrão para valores do tipo double.
* @param a Ponteiro para o primeiro valor (const void*).
* @param b Ponteiro para o segundo valor (const void*).
* @return Retorna -1 se *a < *b, 1 se *a > *b, e 0 se forem iguais.
****************************************************************************************/
int sort_doubleCompare(const void* a, const void* b);

/****************************************************************************************
* @brief Ordena um vetor genérico utilizando o algoritmo Insertion Sort.
* @param base Ponteiro para o início do vetor a ser ordenado.
* @param element_count O número total de elementos no vetor.
* @param element_size O tamanho em bytes de cada elemento.
* @param cmp Ponteiro para a função de comparação entre dois elementos.
****************************************************************************************/
void insertionSort(void *base, size_t element_count, size_t element_size, int (*cmp)(const void* a, const void* b));

/****************************************************************************************
* @brief Ordena um vetor genérico utilizando o algoritmo Merge Sort.
* @param base Ponteiro para o início do vetor a ser ordenado.
* @param element_count O número total de elementos no vetor.
* @param element_size O tamanho em bytes de cada elemento.
* @param cmp Ponteiro para a função de comparação.
* @param threshold Limite de tamanho do subvetor para alternar para Insertion Sort (otimização).
****************************************************************************************/
void mergeSort(void *base, size_t element_count, size_t element_size, int (*cmp)(const void* a, const void* b), int threshold);

#endif