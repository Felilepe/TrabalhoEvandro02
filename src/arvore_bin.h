#ifndef ARVORE_BIN_H
#define ARVORE_BIN_H

typedef struct ArvoreBin ArvoreBin;


typedef int (*CompareFunc)(const void *a, const void *b);
typedef void (*PrintFunc)(void *data);
typedef void (*FreeFunc)(void *data);


ArvoreBin* arvore_criar(CompareFunc cmp);

void arvore_destruir(ArvoreBin *arvore, FreeFunc free_data);



void arvore_inserir(ArvoreBin *arvore, void *data);

void *arvore_buscar(ArvoreBin *arvore, void *key);



void arvore_em_ordem(ArvoreBin *arvore, PrintFunc print);

void arvore_pre_ordem(ArvoreBin *arvore, PrintFunc print);

void arvore_pos_ordem(ArvoreBin *arvore, PrintFunc print);

#endif