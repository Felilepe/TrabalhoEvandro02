#ifndef SOBREPOSICAO_H
#define SOBREPOSICAO_H
#include <stdbool.h>

#include "formas.h"
#include "poligono.h"

/******************************************************************
*@brief TAD SOBREPOSICAO - Detecção de interseções entre formas.
*
*Fornece funcionalidades para verificar se uma forma geométrica
*(círculo, retângulo, texto ou linha) sobrepõe um polígono,
*sendo útil para determinar colisões e áreas de visibilidade.
******************************************************************/

/******************************************************************
*@brief Verifica se há sobreposição entre uma forma e um polígono.
*@param f Ponteiro para a forma a ser testada.
*@param p Ponteiro para o polígono de comparação.
*@return true se há sobreposição, false caso contrário.
*@details Testa formas do tipo círculo, retângulo, texto ou linha
*contra os segmentos e interior do polígono.
******************************************************************/
bool colisao_formaPoli(forma f, Poligono p);

#endif //SOBREPOSICAO_H
