#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "formas.h"
#include "lista.h"
#include "lerGEO.h"
#include "lerQRY.h"
#include "svg.h"

#define PATH_SIZE 520

static void monta_caminho(char* path_completo, const char* base_dir, const char* nome_arquivo) {
    if (base_dir != NULL && strlen(base_dir) > 0) {
        sprintf(path_completo, "%s/%s", base_dir, nome_arquivo);
    } else {
        strcpy(path_completo, nome_arquivo);
    }
}

static void extrair_nome_base(const char *caminho, char *nome_base) {
    const char *ultimo_slash = strrchr(caminho, '/');

    strcpy(nome_base, ultimo_slash ? ultimo_slash + 1 : caminho);

    char* ponto_ext = strrchr(nome_base, '.');
    if (ponto_ext) {
        *ponto_ext = '\0';
    }
}


int main(int argc, char *argv[]) {

    char *path_entrada = NULL;
    char *path_saida = NULL;
    char *path_qry = NULL;
    char *path_geo = NULL;
    char tipo_ord = 'q'; // default é usar quick sort caso nenhum parâmetro seja passado
    int threshold_i = 10; // default é 10 caso nada seja lido na linha de comando

    // --- 1. Tratamento dos parâmetros da linha de comando ---
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            threshold_i = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            path_entrada = argv[++i];
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            path_saida = argv[++i];
        } else if (strcmp(argv[i], "-q") == 0 && i + 1 < argc) {
            path_qry = argv[++i];
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            path_geo = argv[++i];
        } else if (strcmp(argv[i], "-to") == 0 && i + 1 < argc) {
            char *tipo = argv[++i];
            tipo_ord = tipo[0];
        }
    }

    // --- 2. Checagem de parâmetros obrigatórios ---
    if (path_geo == NULL) {
        fprintf(stderr, "ERRO: Parâmetro obrigatório -f <arquivo.geo> não fornecido!\n");
        fprintf(stderr, "Uso: %s -f <arquivo.geo> -o <dir_saida> [-e <dir_entrada>] [-q <arquivo.qry>] [-to q|m] [-i <threshold>]\n", argv[0]);
        return 1;
    }

    if (path_saida == NULL) {
        fprintf(stderr, "ERRO: Argumento obrigatório -o <diretorio_saida> não fornecido!\n");
        return 1;
    }

    // --- 3. Processamento do arquivo .geo ---
    char path_geo_completo[PATH_SIZE];
    monta_caminho(path_geo_completo, path_entrada, path_geo);

    char nome_base_geo[256];
    extrair_nome_base(path_geo, nome_base_geo);

    char path_svg_inicial[PATH_SIZE];
    sprintf(path_svg_inicial, "%s/%s.svg", path_saida, nome_base_geo);

    printf("Lendo arquivo (.geo): %s\n", path_geo_completo);

    lista *formas = parser_geo(path_geo_completo);

    if (formas == NULL) {
        fprintf(stderr, "ERRO: Falha crítica ao processar o arquivo .geo (%s)\n", path_geo_completo);
        return 1;
    }

    // --- 4. Criando o arquivo .svg com todas as formas iniciais ---
    printf("Gerando arquivo (.svg) inicial: %s\n", path_svg_inicial);
    FILE *arq_svg_inicial = gerar_arquivo_svg(path_svg_inicial, formas);
    fecha_svg(arq_svg_inicial);

    // --- 5. Processamento do arquivo .qry ---
    if (path_qry != NULL) {
        char path_qry_completo[PATH_SIZE];
        monta_caminho(path_qry_completo, path_entrada, path_qry);

        char nome_base_qry[256];
        extrair_nome_base(path_qry, nome_base_qry);

        char nome_base_final[512];
        sprintf(nome_base_final, "%s-%s", nome_base_geo, nome_base_qry);

        char path_svg_final[PATH_SIZE];
        char path_txt_final[PATH_SIZE];
        sprintf(path_svg_final, "%s/%s.svg", path_saida, nome_base_final);
        sprintf(path_txt_final, "%s/%s.txt", path_saida, nome_base_final);

        printf("\nProcessando arquivo .qry: %s\n", path_qry_completo);
        if (tipo_ord == 'q') {
            printf("Tipo de ordenação: Quick Sort\n");
        } else {
            printf("Tipo de ordenação: Merge Sort\n");
        }

        printf("Threshold insertion sort: %d\n\n", threshold_i);

        FILE *svg_final = inicializa_svg(path_svg_final);
        lista *anteparos = init_lista();
        parser_qry(formas, anteparos, path_qry_completo, path_txt_final, threshold_i, tipo_ord, path_saida, nome_base_final, svg_final);

        if (svg_final) {
            desenhar_formas_no_svg(svg_final, formas);
        }

        if (anteparos) {
            desenhar_formas_no_svg(svg_final, anteparos);
        }

        fecha_svg(svg_final);

        printf("Gerando arquivo (.svg) final: %s\n", path_svg_final);
        printf("Arquivo (.txt) de log gerado: %s\n", path_txt_final);

        free_lista(anteparos, (void(*)(void*))destrutor_forma);
    }

    // --- 6. Liberando a memória que faltava do programa ---
    printf("\nLimpando memória...\n");
    free_lista(formas, (void(*)(void*))destrutor_forma);

    printf("Execução concluída com sucesso!\n");
    return 0;
}