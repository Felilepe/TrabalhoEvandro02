#ifndef PROCESSAGEO_H
#define PROCESSAGEO_H

#include "fila.h"

typedef Fila Chao;

/******************************************************************
*@brief Processa um arquivo .geo e carrega as formas no Chão.
*@param nome_path_geo O caminho completo para o arquivo .geo.
*@return Um ponteiro para o Chão (Fila*) contendo todas as formas lidas.
******************************************************************/
Chao *processaGeo(const char *nome_path_geo);

#endif 