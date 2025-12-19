#ifndef VISIBILIDADE_H
#define VISIBILIDADE_H
#include "pontos.h"
#include "arvore.h"
#include "lista.h"
#include "poligono.h"

typedef struct stEvent Evento;

/****************************************************************************************
* @brief Prepara e gera o vetor de eventos (pontos iniciais e finais dos segmentos) para o algoritmo de varredura.
* @param anteparos Lista contendo os anteparos (obstáculos) do cenário.
* @param bomb O ponto de origem (observador/bomba) a partir do qual os ângulos são calculados.
* @param event_count Ponteiro onde será armazenado o número total de eventos gerados.
* @return Retorna um vetor alocado dinamicamente contendo as estruturas do tipo Evento.
****************************************************************************************/
Evento *visibilidade_prepSegments(Lista *anteparos, Ponto bomb, int *event_count);

/****************************************************************************************
* @brief Calcula o ponto de colisão mais próximo de um raio (lançado pela bomba) contra os segmentos ativos.
* @param active_seg A árvore AVL contendo os segmentos atualmente interceptados pela linha de varredura.
* @param bomb O ponto de origem do raio.
* @param angle O ângulo do raio em radianos.
* @param max_radius O raio máximo de alcance da visibilidade.
* @return Retorna o objeto Ponto onde ocorreu a colisão (ou o limite do alcance).
****************************************************************************************/
Ponto visibilidade_rayCollision(Arvore *active_seg, Ponto bomb, double angle, double max_radius);

/****************************************************************************************
* @brief Executa o algoritmo de visibilidade (Sweep Line) para gerar o polígono de visão.
* @param anteparos Lista de anteparos (obstáculos).
* @param bomb O ponto do observador.
* @param sort_type Caractere que define o método de ordenação a ser utilizado nos eventos.
* @param max_radius O raio máximo de visão.
* @param max_inserts Limite (threshold) utilizado para otimização da ordenação (Insertion vs Merge).
* @return Retorna o Polígono de visibilidade resultante.
****************************************************************************************/
Poligono visibilidade_calc(Lista *anteparos, Ponto bomb, char sort_type, double max_radius, int max_inserts);

#endif