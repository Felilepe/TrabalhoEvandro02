#ifndef LERGEO_H
#define LERGEO_H

#include "lista.h"

/******************************************************************
*@brief Processa um arquivo .geo e carrega as formas em uma lista.
*@param nome_path_geo O caminho completo para o arquivo .geo.
*@return Um ponteiro para a Lista contendo todas as formas lidas.
******************************************************************/
Lista *processaGeo(const char *nome_path_geo);

#endif 