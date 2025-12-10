#ifndef ARVORE_H
#define ARVORE_H

#include <stdbool.h>

typedef struct stArvore Arvore;

typedef int (*CompareFunc)(const void *a, const void *b);
typedef void (*ActionFunc)(void *data); 



Arvore* arvore_create(CompareFunc cmp, ActionFunc free_data);
void arvore_destroy(Arvore *arvore);



void arvore_insert(Arvore *arvore, void *data);
void* arvore_search(Arvore *arvore, void *key);
void* arvore_remove(Arvore *arvore, void *key); 



int arvore_getHeight(Arvore *arvore);
int arvore_getSize(Arvore *arvore);
bool arvore_isEmpty(Arvore *arvore);

void arvore_em_ordem(Arvore *arvore, ActionFunc action);
void arvore_pre_ordem(Arvore *arvore, ActionFunc action);
void arvore_pos_ordem(Arvore *arvore, ActionFunc action);

#endif