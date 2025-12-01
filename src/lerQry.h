#ifndef LERQRY_H
#define LERQRY_H

#include "fila.h"
#include "lista.h"      
#include "formas.h"
#include "lerGeo.h"     // Para o tipo Chao
#include "arena.h"      // Para o tipo Arena


typedef struct repositorio Repositorio; 

// --- Funções de Gestão do Repositório ---

/******************************************************************
*@brief Cria e inicializa um novo Repositório (para Disparadores e Carregadores).
*@return Um ponteiro para o Repositório recém-criado.
******************************************************************/
Repositorio* criarRepositorio();

/******************************************************************
*@brief Libera a memória do Repositório e de todos os itens dentro dele.
*@param repo Ponteiro para o Repositório a ser destruído.
******************************************************************/
void destrutorRepositorio(Repositorio *repo);

// --- Funções de Gestão de Formas ---

/******************************************************************
*@brief Remove uma forma específica de todos os Disparadores (ex: foi esmagada).
*@param repo O Repositório.
*@param f A forma (void*) a ser removida.
******************************************************************/
void limpaFormaDeTodosDisparadores(Repositorio *repo, forma f);

/******************************************************************
*@brief Devolve todas as formas (em pos_lanc e pilhas) dos Disparadores para o Chão.
*@param repo O Repositório.
*@param c O Chão (Fila*).
******************************************************************/
void devolveFormasDisparadoresParaChao(Repositorio *repo, Chao *c);

/******************************************************************
*@brief Devolve todas as formas dos Carregadores (ainda não anexados) para o Chão.
*@param repo O Repositório.
*@param c O Chão (Fila*).
******************************************************************/
void devolveFormasCarregadoresParaChao(Repositorio *repo, Chao *c);

// --- Função Principal do Módulo ---

/******************************************************************
*@brief Processa um arquivo .qry e executa os comandos do jogo.
*@param repo O Repositório.
*@param nome_path_qry O caminho completo para o arquivo .qry.
*@param nome_txt O caminho completo para o arquivo .txt de saída (log).
*@param arena A Arena do jogo.
*@param chao O Chão do jogo.
*@param pontuacao_total Ponteiro para a pontuação global.
*@param filaSVG Fila para anotações de SVG.
*@param formas_clonadas Ponteiro para o contador de clones.
*@param formas_esmagadas Ponteiro para o contador de formas destruídas.
******************************************************************/
void processaQry(Repositorio *repo, char *nome_path_qry, const char *nome_txt, 
                 Arena *arena, Chao *chao, double *pontuacao_total,
                 Fila *filaSVG, int *formas_clonadas, int *formas_esmagadas);
#endif