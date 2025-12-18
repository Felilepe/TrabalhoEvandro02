#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../lista.h"

typedef struct 
{
    int id;
    char nome[20];
} TesteData;

void print_teste_data(void *data) 
{
    TesteData *td = (TesteData*)data;
    printf("TesteData{id=%d, nome=%s}", td->id, td->nome);
}

int compare_teste_data(void *a, void *b) 
{
    TesteData *td1 = (TesteData*)a;
    TesteData *td2 = (TesteData*)b;
    return td1->id - td2->id;
}

void free_teste_data(void *data) 
{
    free(data);
}

static void helper_free_teste_data(void *dado, void *ignore) //"Unused parameter" é um parâmetro de                                                       
{                                                       //compatibilidade com lista_passthrough
    free_teste_data(dado); 
}

int predicado_id_par(void *data) 
{
    TesteData *td = (TesteData*)data;
    return td->id % 2 == 0;
}

int predicado_id_maior_que_2(void *data) 
{
    TesteData *td = (TesteData*)data;
    return td->id > 2;
}

void transforma_incrementa_id(void *data) 
{
    TesteData *td = (TesteData*)data;
    td->id += 100;
}

void test_lista_vazia() 
{
    printf("=== Teste Lista Vazia ===\n");

    Lista *l = lista_create();
    assert(l != NULL);
    assert(lista_getSize(l) == 0);
    assert(lista_isEmpty(l) == 1);

    lista_destroy(l);
    printf("Lista vazia OK\n");
}

void test_insercao_remocao_basica() 
{
    printf("=== Teste Inserção e Remoção Básica ===\n");

    Lista *l = lista_create();

    TesteData *td1 = malloc(sizeof(TesteData));
    td1->id = 1;
    snprintf(td1->nome, 20, "Primeiro");
    lista_insertHead(l, td1);

    assert(lista_getSize(l) == 1);
    assert(lista_getHead(l) == td1);
    assert(lista_getTail(l) == td1);

    TesteData *td2 = malloc(sizeof(TesteData));
    td2->id = 2;
    snprintf(td2->nome, 20, "Segundo");
    lista_insertTail(l, td2);

    assert(lista_getSize(l) == 2);
    assert(lista_getTail(l) == td2);

    TesteData *removido_head = lista_getHead(l);
    lista_removeHead(l);
    assert(removido_head == td1);
    assert(lista_getSize(l) == 1);
    free(removido_head);
    TesteData *removido_tail = lista_getHead(l);
    lista_removeHead(l);
    assert(removido_tail == td2);
    assert(lista_getSize(l) == 0);
    free(removido_tail);

    lista_destroy(l);
    printf("Inserção e remoção básica OK\n");
}

// --- Estrutura de Contexto para o Teste ---
typedef struct {
    int contador;       // Conta quantos nós visitamos
    int soma_ids;       // Soma os IDs para provar que leu o dado certo
    int ultimo_id;      // Guarda o ID anterior para verificar a ordem crescente
    int ordem_correta;  // Flag (1 = OK, 0 = Erro)
} ContextoVerificacao;

// --- Helper Específico para o Teste de Passthrough ---
// Esta função será chamada para CADA item da lista
void helper_verificar_integridade(void *dado, void *ctx) 
{
    TesteData *td = (TesteData*)dado;
    ContextoVerificacao *contexto = (ContextoVerificacao*)ctx;

    // 1. Verificação de Ordem: O ID atual deve ser maior que o anterior
    if (td->id <= contexto->ultimo_id) {
        contexto->ordem_correta = 0; // Falha: a lista não está sendo percorrida na ordem
    }
    contexto->ultimo_id = td->id;

    // 2. Acumulação de Dados
    contexto->soma_ids += td->id;
    contexto->contador++;
}

// --- A Função de Teste Principal ---
void test_passthrough_funcionalidade() 
{
    printf("=== Teste Unitario: Validação de lista_passthrough ===\n");

    Lista *l = lista_create();
    int qtd_itens = 5;

    for (int i = 0; i < qtd_itens; i++) {
        TesteData *td = malloc(sizeof(TesteData));
        td->id = i;
        snprintf(td->nome, 20, "Teste%d", i);
        lista_insertTail(l, td);
    }

    ContextoVerificacao ctx;
    ctx.contador = 0;
    ctx.soma_ids = 0;
    ctx.ultimo_id = -1; 
    ctx.ordem_correta = 1;

    lista_passthrough(l, helper_verificar_integridade, &ctx);


    if (ctx.contador != qtd_itens) {
        printf("ERRO: Visitou %d itens, esperava %d.\n", ctx.contador, qtd_itens);
    }
    assert(ctx.contador == qtd_itens);

    if (ctx.soma_ids != 10) {
        printf("ERRO: Soma dos IDs deu %d, esperava 10.\n", ctx.soma_ids);
    }
    assert(ctx.soma_ids == 10);

    if (ctx.ordem_correta == 0) {
        printf("ERRO: A ordem de iteração da lista está incorreta.\n");
    }
    assert(ctx.ordem_correta == 1);

    lista_passthrough(l, helper_free_teste_data, NULL);
    lista_destroy(l);
    
    printf("lista_passthrough validada com sucesso!\n\n");
}

int main() {
    printf("INICIANDO TESTES DA LISTA\n\n");

    test_lista_vazia();
    test_insercao_remocao_basica();
    test_passthrough_funcionalidade();

    printf("TODOS OS TESTES PASSARAM!\n");

    return 0;
}