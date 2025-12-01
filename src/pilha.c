#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pilha.h"

typedef struct node
{
    item forma;
    struct node *next;

} node;

 struct pilha
{
    node *topo;
    int size;
};

Pilha* pilha_create()
{
    Pilha *p = (Pilha*)malloc(sizeof(Pilha));
    if(p == NULL){
        printf("Erro na alocacao de memoria ao criar pilha.");
        exit(1);
    }
    p -> topo = NULL;
    p -> size = 0;
    return p;
}

bool pilha_isEmpty(Pilha *p){return p -> size == 0;}

void pilha_push(Pilha *p, item forma)
{
    node *novaNode = (node*)malloc(sizeof(node));
    novaNode -> forma = forma;
    novaNode -> next = p -> topo;
    p -> topo = novaNode;
    p -> size++;
}

item pilha_pop(Pilha *p)
{
    if(pilha_isEmpty(p)){
        printf("Erro: Pilha esta vazia.");
        exit(1);
    }
    node *temp = p -> topo;
    void *forma = temp -> forma;
    p -> topo = temp -> next;
    free(temp);
    p -> size--;
    return forma;
}

item pilha_peek(Pilha *p)
{
    if(pilha_isEmpty(p)){
        printf("Erro: pilha esta vazia.");
        exit(1);
    }
    return p -> topo -> forma;
}

int pilha_getSize(Pilha *p)
{
    if(p == NULL) return 0;
    return p -> size;
}

void pilha_destroy(Pilha *p)
{
    while(!pilha_isEmpty(p)){
        pilha_pop(p);    
    }
    free(p);
}