#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fila.h"

typedef struct node
{
    item forma;
    struct node *next;
}node;

struct fila
{
    node *head;
    node *tail;
    int size;
};

Fila *fila_create()
{
    Fila *f = (Fila*)malloc(sizeof(Fila));
    if(f == NULL){
        printf("Erro na alocacao de memoria ao criar fila");
        exit(1);
    }
    
    f -> head = NULL;
    f -> tail = NULL;
    f -> size = 0;

    return f;
}



bool fila_isEmpty(Fila *f){return f -> size == 0;}



void fila_queue(Fila *f, item i)
{
    node *novoNode = (node*)malloc(sizeof(node));
    if(novoNode == NULL){
        printf("Erro na alocacao de memoria ao criar novo elemento para fila.");
        exit(1);
    }
    
    
    novoNode -> forma = i;
    novoNode -> next = NULL;
    
    
    if(f -> head == NULL){
        f -> head = novoNode;
        f -> tail = novoNode;
    } else{
        f -> tail -> next = novoNode;
        f -> tail = novoNode;
    }

    f ->size++;
}

item fila_dequeue(Fila *f)
{
    if(fila_isEmpty(f)){
        printf("Erro: Fila esta vazia");
        exit(1);
    }

    node *auxNode = f -> head;

    item formaReturn = auxNode -> forma;
    
    f -> head = auxNode -> next;

    if(f -> head == NULL){
        f -> tail = NULL;
    }
    
    free(auxNode);


    f -> size--;

    return formaReturn;
}


NodeF *fila_getHead(Fila *f)
{
    if(fila_isEmpty(f)){
        printf("Erro: Fila esta vazia.");
        exit(1);
    }

    return f -> head;
}

NodeF *fila_getTail(Fila *f)
{
    if(fila_isEmpty(f)){
        printf("Erro: Fila esta vazia.");
        exit(1);
    }

    return f -> tail;
}

item fila_getItem(NodeF *n) {return n -> forma;} 

NodeF *fila_getNext(NodeF *n) {return n -> next;}

int fila_getSize(Fila *f)
{
    if(f == NULL) return 0;
    return f -> size;
}



void fila_destroy(Fila* f)
{
    while(!fila_isEmpty(f)){
        fila_dequeue(f);
    }
    free(f);
}



void fila_passthrough(Fila *f, void (*acao)(item i, item aux_data), item aux_data) 
{
	if (f == NULL || acao == NULL || fila_isEmpty(f)) {
		return;
	}

	NodeF *atual = f -> head;

	while (atual != NULL) {
		acao(atual -> forma, aux_data);

		atual = atual -> next;
	}
}