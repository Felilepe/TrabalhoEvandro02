#ifndef SORTING_H
#define SORTING_H

int double_compare(const void *x, const void *y);

void sort_insertionGeneric(void *start, int num_elements, int element_size, int (*compare_func)(const void *a, const void *b));

void sort_mergeGeneric(void *start, int num_elements, int element_size, int (*compare_func)(const void *a, const void *b), int threshold);

#endif