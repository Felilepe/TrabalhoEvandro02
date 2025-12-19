#ifndef COLISOES_H
#define COLISOES_H

#include <stdbool.h>
#include "formas.h"
#include "poligono.h"

/*******************************************************************
 * @brief Verifica se uma forma genérica colide com um polígono.
 * @param f A forma (Círculo, Retângulo, Linha ou Texto).
 * @param p O polígono.
 * @return true Se houver colisão (interseção ou inclusão).
 * @return false Caso contrário.
 *******************************************************************/
bool checarColisao(forma f, Poligono p);



#endif