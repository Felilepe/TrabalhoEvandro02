#ifndef LERQRY_H
#define LERQRY_H
#include "lista.h"
#include <stdio.h>

/****************************************************************************************
* @brief Lê e processa os comandos de um arquivo .qry.
* @param formas Ponteiro para a lista de formas geométricas (pode ser modificada).
* @param anteparos Ponteiro para a lista de anteparos (pode ser modificada).
* @param path_qry Caminho completo para o arquivo de entrada .qry.
* @param nome_path_txt Caminho completo para o arquivo de saída de relatório .txt.
* @param max_insertions Limite ou parâmetro de controle para inserções/operações.
* @param sort_type Caractere que define o critério de ordenação (se aplicável).
* @param path_out Diretório ou caminho base para a saída dos arquivos gerados.
* @param path_base_svg Caminho ou nome base para a geração de arquivos SVG parciais.
* @param svg_final Ponteiro para o arquivo SVG final (já aberto) onde o resultado será gravado.
****************************************************************************************/
void lerQry(Lista *formas, Lista *anteparos, char *path_qry, char *nome_path_txt,
    int max_insertions, char sort_type, const char *path_out, const char *path_base_svg, FILE *svg_final);



#endif