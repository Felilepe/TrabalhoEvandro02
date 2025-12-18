#ifndef LERQRY_H
#define LERQRY_H
#include "lista.h"
#include <stdio.h>

void lerQry(Lista *formas, Lista *anteparos, char *path_qry, char *nome_path_txt,
    int max_insertions, char sort_type, const char *path_out, const char *path_base_svg, FILE *svg_final);



#endif