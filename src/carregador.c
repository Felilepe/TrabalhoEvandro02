#include <stdio.h>
#include <stdlib.h>
#include "fila.h"
#include "formas.h"
#include <stdbool.h>
#include "carregador.h"
#include "pilha.h"

typedef struct carregador
{
    int id;
    Pilha *p;
} carregador;

Carregador carregador_create(int id)
{
    carregador *c = (carregador*)malloc(sizeof(carregador));
    if(c == NULL){
        printf("Erro na alocacao de memoria ao criar carregador.");
        return NULL;
    }
    c -> id = id;
    c -> p = pilha_create();

    return c;
}

int carregador_getID(Carregador c) {return ((carregador*)c) -> id;}

forma carregador_loadForma(Carregador c, forma f)
{
    if(c == NULL || f == NULL){
        printf("Erro: carregador e/ou forma identificado(s) como sendo nulo.\n");
        return NULL;
    }
    
    carregador *mag = (carregador*)c;
    if(mag -> p == NULL){
        printf("Erro: pilha do carregador identificada como sendo nula.\n");
        return NULL;
    }

    pilha_push(mag -> p, f);

    return f;
}

forma carregador_loadFromChao(Carregador c, Chao *h)
{
    if (c == NULL || h == NULL) {
		printf("Erro: chao e/ou carregador identificado(s) como sendo nulo.\n");
	}

    if (fila_isEmpty(h)) {
        return NULL;
    }


    item original = fila_dequeue(h);
    if (original == NULL) return NULL;

    forma f = (forma)original;
    printf("[DIAG] carregador %d: moveu forma id=%d do chao para o carregador\n", carregador_getID(c), forma_getID(f));

    carregador_loadForma(c, f);

    return f;
}

Chao *carregador_loadAmount(Chao *h, Carregador c, int n)
{
    if (c == NULL || h == NULL) {
        if(n < 0) return fila_create();
        printf("Erro: chao e/ou carregador identificado(s) como sendo nulo.\n");
        return NULL;
	}

    Fila *Historico = fila_create();

    for(int i = 0; i < n; i++){
        if(fila_isEmpty(h)){
            printf("Chao foi esvaziado.\n");
            break;
        }

        forma f = carregador_loadFromChao(c, h);
        if (f != NULL) {
            fila_queue(Historico, f);
        }
    }

    return Historico;
}

bool carregador_isEmpty(Carregador c)
{
    carregador *mag = (carregador*)c;
    if(c == NULL) return true;

    return pilha_isEmpty(mag -> p);
}



forma carregador_remove(Carregador c)
{
    if(c == NULL){
        printf("Erro: Carregador Nulo.");
        return NULL;
    }
    
    carregador* mag = (carregador*)c;
    
    forma removed = pilha_pop(mag -> p);
    if (removed != NULL) {
        printf("[DIAG] carregador %d: removeu forma id=%d\n", carregador_getID(c), forma_getID(removed));
    } else {
        printf("[DIAG] carregador %d: removeu forma NULL\n", carregador_getID(c));
    }
    return removed;
}

void carregador_destroy(Carregador c)
{
    if (c == NULL) {
        return;
    }

    carregador* temp = (carregador*)c;

    pilha_destroy(temp -> p);
    free(temp);
}

forma carregador_peek(Carregador c)
{
    if (c == NULL) return NULL;
    carregador* mag = (carregador*)c;
    if (mag->p == NULL) return NULL;
    if (pilha_isEmpty(mag->p)) return NULL;
    return (forma)pilha_peek(mag->p);
}

int carregador_getSize(Carregador c)
{
    if (c == NULL) return 0;
    carregador* mag = (carregador*)c;
    if (mag->p == NULL) return 0;
    return pilha_getSize(mag->p);
}