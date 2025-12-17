#ifndef SORT_H
#define SORT_H
#include <stddef.h>



int sort_doubleCompare(const void* a, const void* b);

void insertionSort(void *base, size_t element_count, size_t element_size, int (*cmp)(const void* a, const void* b));

void mergeSort(void *base, size_t element_count, size_t element_size, int (*cmp)(const void* a, const void* b), int threshold);


#endif