#include "arena.h"
#include "fila.h"
#include "formas.h"
#include "colisoes.h"

#include <stdio.h>
#include <stdlib.h>



struct arena 
{
    Fila *filaArena;
};

Arena *arena_create() 
{
    Arena *a = (Arena*) malloc (sizeof(Arena));
    if (a == NULL) {
        printf("Erro ao criar a arena.\n");
        return NULL;
    }

    a -> filaArena = fila_create();



    return a;
}

forma arena_add(Arena *a, forma f) 
{
    if (f == NULL) {
        return NULL;
    }

    if (a == NULL) {
        printf("Arena inexistente passada para a função 'adicionaFormaArena'\n");
        return NULL;
    }

    fila_queue(a->filaArena, f);

    return f;
}

forma arena_remove(Arena *a) 
{
    if (a == NULL ) return NULL;

    forma removido = fila_dequeue(a -> filaArena);

    return removido;
}

bool arena_isEmpty(Arena *a) 
{
    if (a == NULL) return true;

    return fila_isEmpty(a -> filaArena);
}

void arena_destroy(Arena **a_ptr) 
{
    if (a_ptr == NULL || *a_ptr == NULL) {
        return;
    }
    Arena* a = *a_ptr;

    fila_destroy(a -> filaArena);

    free(a);
    *a_ptr = NULL;
}

int arena_getSize(Arena *a) 
{
    if (a == NULL) {
        return -1;
    }

    return fila_getSize(a -> filaArena);
}


void processaArena(Arena *a, Chao *c, double *pontuacao_total, Fila *anotacoes_svg,
                   FILE *arquivo_txt, int *formas_clonadas, int *formas_esmagadas) 
{
    if (c == NULL || a == NULL || arquivo_txt == NULL) {
        printf("Erro: Parâmetros nulos passados para processaArena!\n");
        return;
    }

    double area_esmagada_round = 0.0;

    
    while (arena_getSize(a) >= 2) {
        
        
        forma forma_I = arena_remove(a);
        forma forma_J = arena_remove(a);

        
    /* Diagnostic: print ids, coords and areas before checking collision */
    double area_I_diag = forma_calcArea(forma_I);
    double area_J_diag = forma_calcArea(forma_J);
    double xi = forma_getCoordX(forma_I);
    double yi = forma_getCoordY(forma_I);
    double xj = forma_getCoordX(forma_J);
    double yj = forma_getCoordY(forma_J);
    bool col = checarColisao(forma_I, forma_J);
    fprintf(arquivo_txt, "[DIAG] Comparando I=%d (x=%.2f,y=%.2f,area=%.2f) vs J=%d (x=%.2f,y=%.2f,area=%.2f) -> colisao=%s\n",
        forma_getID(forma_I), xi, yi, area_I_diag, forma_getID(forma_J), xj, yj, area_J_diag, col ? "TRUE" : "FALSE");

    if (col) {
            

            double area_I = forma_calcArea(forma_I);
            double area_J = forma_calcArea(forma_J);

            fprintf(arquivo_txt, "Forma %d (I) vs Forma %d (J). HOUVE SOBREPOSIÇÃO.\n",
                    forma_getID(forma_I), forma_getID(forma_J));

            if (area_I < area_J) {
                
                fprintf(arquivo_txt,
                        "<<<-- I < J -->>> forma %d (I) (área %.2f) foi esmagada por forma %d (J) (área %.2f).\n",
                         forma_getID(forma_I), area_I, forma_getID(forma_J), area_J);


                double x_esmagada = forma_getCoordX(forma_I);
                double y_esmagada = forma_getCoordY(forma_I);


                Texto asterisco = texto_create(-1, x_esmagada, y_esmagada, "red", "black", 'm', "*");

             
                texto_setFamily(asterisco, "sans-serif");
                texto_setWeight(asterisco, "bold");
                texto_setSize(asterisco, "30px");
                
         
                fila_queue(anotacoes_svg, (forma)asterisco);

                *pontuacao_total += area_I;
                area_esmagada_round += area_I;
                if (formas_esmagadas != NULL) (*formas_esmagadas)++;

                forma_destroy(forma_I);
                fila_queue(c, forma_J); 

            } else if (area_I >= area_J) {
          
                fprintf(arquivo_txt,
                        "<<<-- I >= J -->>>\n forma %d (I) (área %.2f) modificou a forma %d (J) (área %.2f).\n\n",
                         forma_getID(forma_I), area_I, forma_getID(forma_J), area_J);

                forma clone_I = NULL;

                if (forma_getType(forma_I) == TIPO_T) {
                    
                    char* cor_original_I = forma_getCorBorda(forma_I); 
                    char* cor_complementar = forma_getCorComp(cor_original_I); 

                    forma_setCorBorda(forma_J, cor_complementar); 
                    
                    clone_I = forma_clonar(forma_I);

                   
                    forma_setCorBorda(clone_I, cor_complementar);

                    free(cor_complementar);

                } else {
                    forma_setCorBorda(forma_J, forma_getCorPreench(forma_I));
                    clone_I = forma_clonar(forma_I);
                    forma_trocarCores(clone_I);
                }

                if (formas_clonadas != NULL) (*formas_clonadas)++;

                fila_queue(c, forma_I);
                fila_queue(c, forma_J);
                if (clone_I != NULL) {
                    fila_queue(c, clone_I);
                }
            }
        } else {
            fprintf(arquivo_txt, "Forma %d (I) vs Forma %d (J). NÃO HOUVE SOBREPOSIÇÃO.\n",
                    forma_getID(forma_I), forma_getID(forma_J));
            fila_queue(c, forma_I);
            fila_queue(c, forma_J);
        }
    }

    if (!arena_isEmpty(a)) {
        fila_queue(c, arena_remove(a));
    }

    fprintf(arquivo_txt, "Área total esmagada no round: %.2f\n", area_esmagada_round);
}


void arena_passthrough(Arena *a, void (*acao)(item i, item aux_data), item aux_data) {
    if (a == NULL) return;
    if (acao == NULL) return;
    fila_passthrough(a->filaArena, acao, aux_data);
}