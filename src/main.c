#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fila.h"
#include "lerGeo.h"      
#include "arena.h"        
#include "lerQry.h"      
#include "svg.h"
#include "formas.h"       


static void montaCaminho(char* path_completo, const char* base_dir, const char* nome_arquivo) {
    if (base_dir != NULL && strlen(base_dir) > 0) {
        sprintf(path_completo, "%s/%s", base_dir, nome_arquivo);
    } else {
        strcpy(path_completo, nome_arquivo);
    }
}

int main(int argc, char *argv[]){

    char* dir_entrada = NULL;
    char* arq_geo_nome = NULL;
    char* dir_saida = NULL;
    char* arq_qry_nome = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            dir_entrada = argv[++i];
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            arq_geo_nome = argv[++i];
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            dir_saida = argv[++i];
        } else if (strcmp(argv[i], "-q") == 0 && i + 1 < argc) {
            arq_qry_nome = argv[++i];
        }
    }

    if (arq_geo_nome == NULL || dir_saida == NULL) {
        printf("ERRO: Parâmetros obrigatórios -f e -o não foram fornecidos.\n");
        printf("Uso: ./ted -f arq.geo -o dir_saida [-e dir_entrada] [-q arq.qry]\n");
        return 1;
    }

    char path_geo_completo[512];
    montaCaminho(path_geo_completo, dir_entrada, arq_geo_nome);
    {
        FILE *fgeo = fopen(path_geo_completo, "r");
        if (fgeo == NULL) {
            if (strstr(arq_geo_nome, ".geo") == NULL) {
                char alt[512];
                snprintf(alt, sizeof(alt), "%s.geo", arq_geo_nome);
                montaCaminho(path_geo_completo, dir_entrada, alt);
                fgeo = fopen(path_geo_completo, "r");
                if (fgeo != NULL) {
                    fclose(fgeo);
                    arq_geo_nome = (char*)malloc(strlen(alt)+1); if(arq_geo_nome){ strcpy(arq_geo_nome, alt); }
                }
            }
        } else {
            fclose(fgeo);
        }
    }


    char nome_base_geo[256];
    char* ultimo_slash = strrchr(arq_geo_nome, '/');
    strcpy(nome_base_geo, ultimo_slash ? ultimo_slash + 1 : arq_geo_nome);
    char* ponto_ext = strrchr(nome_base_geo, '.');
    if (ponto_ext) *ponto_ext = '\0';

    char path_svg_inicial[512];
    sprintf(path_svg_inicial, "%s/%s.svg", dir_saida, nome_base_geo);

    Arena* minha_arena = arena_create();       
    Repositorio* repo = criarRepositorio();  
    Fila* anotacoes_svg = fila_create();       
    double pontuacao = 0.0;
    int formas_clonadas = 0, formas_esmagadas = 0;

    Chao *meu_chao = processaGeo(path_geo_completo);
    printf("Lendo o arquivo .geo e adicionando as formas ao chão...\n");

    if (meu_chao == NULL) {
        fprintf(stderr,"Falha critica ao processar o .geo (%s)\n", path_geo_completo);
        arena_destroy(&minha_arena);    
        destrutorRepositorio(repo);
        fila_destroy(anotacoes_svg);    
        return 1;
    }

    printf("Gerando SVG inicial: %s\n", path_svg_inicial);
    createSVG(path_svg_inicial, meu_chao);

    if (arq_qry_nome != NULL) {
        char path_qry_completo[512];
        montaCaminho(path_qry_completo, dir_entrada, arq_qry_nome);
        {
            FILE *fq = fopen(path_qry_completo, "r");
            if (fq == NULL) {
                char altqry[512];
                snprintf(altqry, sizeof(altqry), "%s/%s", nome_base_geo, arq_qry_nome);
                montaCaminho(path_qry_completo, dir_entrada, altqry);
                fq = fopen(path_qry_completo, "r");
                if (fq != NULL) {
                    fclose(fq);
                }
            } else {
                fclose(fq);
            }
        }


        char nome_base_qry[256];
        char* ultimo_slash_qry = strrchr(arq_qry_nome, '/');
        strcpy(nome_base_qry, ultimo_slash_qry ? ultimo_slash_qry + 1 : arq_qry_nome);
        ponto_ext = strrchr(nome_base_qry, '.');
        if (ponto_ext) *ponto_ext = '\0';

        char path_svg_final[520];
        char path_txt_final[520];
        sprintf(path_svg_final, "%s/%s-%s.svg", dir_saida, nome_base_geo, nome_base_qry);
        sprintf(path_txt_final, "%s/%s-%s.txt", dir_saida, nome_base_geo, nome_base_qry);

        printf("Processando arquivo .qry: %s\n", path_qry_completo);
        processaQry(repo, path_qry_completo, path_txt_final,
                      minha_arena, meu_chao, &pontuacao, anotacoes_svg,
                      &formas_clonadas, &formas_esmagadas);


        while (!fila_isEmpty(anotacoes_svg)) {
            forma anotacao = fila_dequeue(anotacoes_svg);
            
            fila_queue(meu_chao, anotacao);
        }

        printf("Gerando SVG final: %s\n", path_svg_final);
        createSVG(path_svg_final, meu_chao);
    }


    printf("Finalizando e liberando memória...\n");
    devolveFormasCarregadoresParaChao(repo, meu_chao);
    devolveFormasDisparadoresParaChao(repo, meu_chao);
    destrutorRepositorio(repo);
    
    fila_destroy(meu_chao);
    arena_destroy(&minha_arena); 
    fila_destroy(anotacoes_svg);
    printf("Programa finalizado com sucesso.\n");

    return 0;
}