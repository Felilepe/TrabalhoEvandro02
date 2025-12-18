#include <stdio.h>
#include <string.h>

#include "formas.h"
#include "lista.h"
#include "poligono.h"
#include "colisoes.h"
#include "svg.h"
#include "visibilidade.h"

#define BUFFER_SIZE 512
#define COMANDO_SIZE 16

// Estrutura para carregar os dados para dentro do callback
typedef struct {
    int id_i;
    int id_j;
    char orientacao;
    Lista *lista_anteparos; // Lista onde vamos guardar os novos anteparos
    Lista *lista_remover;   // Lista temporária para guardar o que deve ser deletado
    FILE *txt;
} ContextoQry;

//--- Funções Auxiliares ---

void processar_forma_callback(void* item, void* aux)
{
    forma f = (forma)item;
    ContextoQry *ctx = (ContextoQry*)aux;

    int id = forma_getID(f);

    // Verifica se está dentro do intervalo [id_i, id_j]
    if (id >= ctx->id_i && id <= ctx->id_j) {
        
        // Log da forma original
        const char* char_type[] = {"DESCONHECIDO", "CIRCULO", "RETANGULO", "LINHA", "TEXTO", "ANTEPARO"};
        // Nota: Ajuste os índices do array char_type conforme seus #defines (TIPO_C, TIPO_R, etc)
        // Assumindo que TIPO_C=1, etc. O indice 0 fica como placeholder ou ajuste conforme necessário.
        
        fprintf(ctx->txt, "Forma original: ID -> %i | Tipo -> %s\n", 
                id, char_type[forma_getType(f)]);

        // Transforma a forma em anteparos
        Lista *novos_segmentos = forma_toAnteparo(f, ctx->orientacao);
        
        fprintf(ctx->txt, "Segmento(s) produzidos pela forma de ID -> %i:\n", id);

        // Processa os segmentos gerados (Iteração normal pois é uma lista pequena recém criada)
        // Nota: Como 'novos_segmentos' é uma lista temporária, podemos iterar removendo ou usando passthrough
        // Aqui usaremos um loop simples com removeHead para mover os nós de uma lista para outra
        
        while(!lista_isEmpty(novos_segmentos)){
            // Removemos da lista temporária para não precisar clonar
            // Assumindo que forma_toAnteparo retorna uma lista de Formas/Linhas alocadas
            forma seg = lista_getHead(novos_segmentos); 
            lista_removeHead(novos_segmentos);
            
            // Log das coordenadas
            // Nota: Casting para Anteparo ou Linha conforme seu typedef
            // Assumindo que Anteparo tem getters compatíveis
            fprintf(ctx->txt, "Segmento ID %d: (%.2f, %.2f) <-> (%.2f, %.2f)\n",
                    forma_getID(seg), 
                    anteparo_getCoordX1((Anteparo)seg), anteparo_getCoordY1((Anteparo)seg), 
                    anteparo_getCoordX2((Anteparo)seg), anteparo_getCoordY2((Anteparo)seg));

            // Insere na lista final de anteparos
            lista_insertTail(ctx->lista_anteparos, seg);
        }

        fprintf(ctx->txt, "\n");

        // Limpa a lista temporária (a estrutura da lista, os nós já foram movidos)
        lista_destroy(novos_segmentos);

        // Marca a forma original para remoção
        lista_insertTail(ctx->lista_remover, f);
    }
}

static bool is_anteparo(void *dados) 
{
    if (dados == NULL) return false;

    return forma_getType(dados) == TIPO_A;
}


// --- Tratadores de Comandos ---

static void qry_command_a(char* buffer, Lista* formas, Lista * anteparos, FILE *txt)
{
    int id_i, id_j;
    char orientacao = 'h';

    sscanf(buffer, "a %i %i %c", &id_i, &id_j, &orientacao);
    fprintf(txt, "[*] a %i %i %c\n", id_i, id_j, orientacao);

    ContextoQry ctx;
    ctx.id_i = id_i;
    ctx.id_j = id_j;
    ctx.orientacao = orientacao;
    ctx.lista_anteparos = anteparos;
    ctx.lista_remover = lista_create();
    ctx.txt = txt;

    lista_passthrough(formas, processar_forma_callback, &ctx);

    
    if (!lista_isEmpty(ctx.lista_remover)) {
        
        while(!lista_isEmpty(ctx.lista_remover)) {
            forma f_to_kill = lista_getHead(ctx.lista_remover);
            lista_removeHead(ctx.lista_remover);

            
            for(int i = 0; i < lista_getSize(formas); i++){
                if(lista_getItem(formas, i) == f_to_kill){
                    lista_removeNode(formas, i);
                    forma_destroy(f_to_kill);
                    break; 
                }
            }
        }
    }

    lista_destroy(ctx.lista_remover);
}