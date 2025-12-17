#ifndef SORT_H
#define SORT_H
#include <stddef.h>

#ifndef ITEM_T_DEFINED
typedef void *item;
#define ITEM_T_DEFINED
#endif

int sort_doubleCompare(const item a, const item b);

void insertionSort(item base, size_t element_count, size_t element_size, int (*cmp)(const item a, const item b));

void mergeSort(item base, size_t element_count, size_t element_size, int (*cmp)(const item a, const item b), int threshold);


#endif