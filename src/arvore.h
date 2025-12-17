#ifndef ARVORE_H
#define ARVORE_H
#include <stdbool.h>

#ifndef ITEM_T_DEFINED
typedef void *item;
#define ITEM_T_DEFINED
#endif

typedef struct ArvoreSt Arvore;
typedef struct NodeSt ArvNode;

Arvore *arvore_create(int (*cmp_data)(const void *a, const void *b), void (*free_data)(void *data), void (*print_data)(void *data));

void arvore_insert(Arvore *a, item data);

void arvore_remove(Arvore *a, item key);



item arvore_search(Arvore *a, item key); 

void arvore_print(Arvore *a);

void arvore_destroy(Arvore *a);

bool arvore_isEmpty(Arvore *a);


item arvore_getNodeData(ArvNode *n);

int arvore_getHeight(Arvore *a);

int arvore_getSize(Arvore *a);

ArvNode* arvore_getRootNode(Arvore *a);

ArvNode* arvore_getRightNode(ArvNode *n);

ArvNode* arvore_getLeftNode(ArvNode *n);



#endif