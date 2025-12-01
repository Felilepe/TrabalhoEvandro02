#ifndef DISPARADOR_H
#define DISPARADOR_H

#include "carregador.h"
#include "formas.h"
#include "arena.h" // Necessário para disparador_rajada



typedef struct disparador Disparador;

#ifndef ITEM_T_DEFINED
typedef void* item;
#define ITEM_T_DEFINED
#endif

/******************************************************************
*@brief Cria um novo Disparador (jogador).
*@param id O ID único do disparador.
*@param x A coordenada X inicial do disparador.
*@param y A coordenada Y inicial do disparador.
*@param cesq O Carregador esquerdo (pode ser NULL).
*@param cdir O Carregador direito (pode ser NULL).
*@return Um ponteiro para o Disparador recém-criado.
******************************************************************/
Disparador *disparador_create(int id, double x, double y, Carregador cesq, Carregador cdir); 


/******************************************************************
*@brief Obtém o ID do Disparador.
*@param d O Disparador.
*@return O ID (int) do Disparador.
******************************************************************/
int disparador_getID(Disparador *d);

/******************************************************************
*@brief Obtém a coordenada X atual do Disparador.
*@param d O Disparador.
*@return A coordenada X (double).
******************************************************************/
double disparador_getCoordX(Disparador *d);

/******************************************************************
*@brief Obtém a coordenada Y atual do Disparador.
*@param d O Disparador.
*@return A coordenada Y (double).
******************************************************************/
double disparador_getCoordY(Disparador *d);

/******************************************************************
*@brief Obtém o carregador direito atual do Disparador.
*@param d O Disparador.
*@return O Carregador Direito. NULL se não houver
******************************************************************/
Carregador disparador_getCDir(Disparador *d);

/******************************************************************
*@brief Obtém o carregador esquerdo atual do Disparador.
*@param d O Disparador.
*@return O Carregador Esquerdo. NULL não houver
******************************************************************/
Carregador disparador_getCEsq(Disparador *d);

/******************************************************************
*@brief Obtém a forma de disparo atual do Disparador.
*@param d O Disparador.
*@return A forma. NULL se estiver vazio
******************************************************************/
forma disparador_getFormaDisparo(Disparador *d);


/******************************************************************
*@brief Dispara a forma na posição de lançamento (pos_lanc).
*@param d O Disparador.
*@param dx O vetor de direção X do disparo.
*@param dy O vetor de direção Y do disparo.
*@return A forma (void*) que foi disparada, ou NULL se não havia forma.
******************************************************************/
forma disparador_disparar(Disparador *d, double dx, double dy);

/******************************************************************
*@brief Realiza uma rajada de disparos (comando 'rjd').
*@param d O Disparador.
*@param botao 'e' (esquerda) ou 'd' (direita) para a fonte da rajada.
*@param dx Vetor de direção X base.
*@param dy Vetor de direção Y base.
*@param ix Incremento X para cada disparo da rajada.
*@param iy Incremento Y para cada disparo da rajada.
*@param a A Arena (para onde as formas são disparadas).
*@return Uma Fila* com as formas que foram disparadas.
******************************************************************/
Fila *disparador_rajada(Disparador *d, char botao, double dx, double dy, double ix, double iy, Arena *a);

/******************************************************************
*@brief Move o disparador para uma nova coordenada absoluta.
*@param d O Disparador.
*@param x A nova coordenada X.
*@param y A nova coordenada Y.
******************************************************************/
void disparador_move(Disparador *d, double x, double y);

/******************************************************************
*@brief Anexa carregadores a um disparador (comando 'atch').
*@param d O Disparador.
*@param cesq O Carregador esquerdo.
*@param cdir O Carregador direito.
******************************************************************/
void disparador_attachCarregador(Disparador *d, Carregador cesq, Carregador cdir);

/******************************************************************
*@brief Movimenta formas entre os carregadores e a posição de disparo (shft).
*@param d O Disparador.
*@param b 'e' (esquerda) ou 'd' (direita).
*@param n O número de movimentos.
*@return O item (forma) que terminou na posição de lançamento (pos_lanc).
******************************************************************/
item disparador_shift(Disparador *d, char b, int n);

/******************************************************************
*@brief Remove forma da posição de disparo, caso seja igual ao index
*@param d O Disparador.
*@param f O Index.
*@return A forma que foi removida.
******************************************************************/
void disparador_removeForma(Disparador *d, forma f);

/******************************************************************
*@brief Libera a memória do Disparador.
*@param d O Disparador a ser destruído.
*@note NÃO libera a memória dos Carregadores anexados.
******************************************************************/
void disparador_destroy(Disparador **d);



#endif