#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../visibilidade.h"
#include "../lista.h"
#include "../formas.h"
#include "../poligono.h"
#include "../arvore.h"
#include "../pontos.h"
#include "../anteparo.h"
#include "../geometria.h"

#define PI 3.14159265358
#define EPSILON 1e-10

// Helper para destruir formas
static void helper_destruir_forma(void *dado, void *ignore) 
{
    forma_destroy(dado);
}

// Cria um anteparo para teste
forma criar_anteparo_teste(int id, double x0, double y0, double x1, double y1) {
    return anteparo_create(id, x0, y0, x1, y1, "black");
}

void teste_visibilidade_basico() 
{
    printf("\n=== Teste: Visibilidade Básica ===\n");
    
    // Cenário: Uma parede vertical à direita da bomba
    Ponto bomba = ponto_create(0, 0);
    Lista *anteparos = lista_create();
    
    // Parede vertical em x=10, de y=-5 até y=5
    forma parede = criar_anteparo_teste(1, 10, -5, 10, 5);
    lista_insertTail(anteparos, parede);
    
    double max_radius = 100.0;
    char sort_type = 'q';  // Usar qsort
    int max_inserts = 10;
    
    // Calcular polígono de visibilidade
    Poligono vis = visibilidade_calc(anteparos, bomba, sort_type, max_radius, max_inserts);
    
    printf("Polígono calculado: %d vértices\n", poligono_getVerticeCount(vis));
    
    // Verificações básicas
    if (poligono_getVerticeCount(vis) < 3) {
        printf("ERRO: Polígono tem menos de 3 vértices!\n");
    } else {
        printf("OK: Polígono válido\n");
    }
    
    // Cleanup
    poligono_destroy(vis);
    lista_passthrough(anteparos, helper_destruir_forma, NULL);
    lista_destroy(anteparos);
    ponto_destroy(bomba);
}

void teste_visibilidade_sem_obstaculos() 
{
    printf("\n=== Teste: Visibilidade Sem Obstáculos ===\n");
    
    Ponto bomba = ponto_create(50, 50);
    Lista *anteparos = lista_create(); // Lista vazia
    
    double max_radius = 100.0;
    char sort_type = 'm';  // Usar mergesort
    int max_inserts = 10;
    
    Poligono vis = visibilidade_calc(anteparos, bomba, sort_type, max_radius, max_inserts);
    
    int num_vertices = poligono_getVerticeCount(vis);
    printf("Polígono sem obstáculos: %d vértices\n", num_vertices);
    
    // Deve ter 4 vértices (as paredes do mundo: caixa 1010x1010)
    if (num_vertices == 4) {
        printf("OK: Polígono é um quadrado (paredes do mundo)\n");
    } else {
        printf("AVISO: Esperava 4 vértices, obteve %d\n", num_vertices);
    }
    
    // Cleanup
    poligono_destroy(vis);
    lista_destroy(anteparos);
    ponto_destroy(bomba);
}

void teste_visibilidade_caixa_fechada() 
{
    printf("\n=== Teste: Visibilidade em Caixa Fechada ===\n");
    
    Ponto bomba = ponto_create(0, 0);
    Lista *anteparos = lista_create();
    
    // Caixa 10x10 ao redor da bomba
    lista_insertTail(anteparos, criar_anteparo_teste(1, 10, -5, 10, 5));   // Direita
    lista_insertTail(anteparos, criar_anteparo_teste(2, -10, -5, -10, 5)); // Esquerda
    lista_insertTail(anteparos, criar_anteparo_teste(3, -10, -5, 10, -5)); // Cima
    lista_insertTail(anteparos, criar_anteparo_teste(4, -10, 5, 10, 5));   // Baixo
    
    double max_radius = 100.0;
    Poligono vis = visibilidade_calc(anteparos, bomba, 'q', max_radius, 10);
    
    printf("Polígono em caixa: %d vértices\n", poligono_getVerticeCount(vis));
    
    // Verificar que o polígono está dentro da caixa
    double xMin, xMax, yMin, yMax;
    poligono_getBBox(vis, &xMin, &xMax, &yMin, &yMax);
    
    printf("Bounding Box: (%.2f, %.2f) até (%.2f, %.2f)\n", xMin, yMin, xMax, yMax);
    
    if (xMin >= -10.1 && xMax <= 10.1 && yMin >= -5.1 && yMax <= 5.1) {
        printf("OK: Polígono está dentro da caixa\n");
    } else {
        printf("ERRO: Polígono ultrapassou os limites\n");
    }
    
    // Cleanup
    poligono_destroy(vis);
    lista_passthrough(anteparos, helper_destruir_forma, NULL);
    lista_destroy(anteparos);
    ponto_destroy(bomba);
}

void teste_ray_collision() 
{
    printf("\n=== Teste: Ray Collision Direto ===\n");
    
    Ponto bomba = ponto_create(0, 0);
    Arvore *seg_ativos = arvore_create(NULL, NULL, NULL); // Árvore vazia para teste
    
    double angle = 0.0; // Raio na direção +X
    double max_radius = 100.0;
    
    // Com árvore vazia, deve retornar ponto no limite do raio
    Ponto p = visibilidade_rayCollision(seg_ativos, bomba, angle, max_radius);
    
    printf("Ray collision (árvore vazia): (%.2f, %.2f)\n", 
           ponto_getCoordX(p), ponto_getCoordY(p));
    
    if (fabs(ponto_getCoordX(p) - 100.0) < EPSILON && 
        fabs(ponto_getCoordY(p) - 0.0) < EPSILON) {
        printf("OK: Raio atingiu o limite máximo\n");
    } else {
        printf("ERRO: Coordenadas incorretas\n");
    }
    
    // Cleanup
    ponto_destroy(p);
    ponto_destroy(bomba);
    arvore_destroy(seg_ativos);
}

void teste_prep_segments() 
{
    printf("\n=== Teste: Preparação de Segmentos ===\n");
    
    Ponto bomba = ponto_create(0, 0);
    Lista *anteparos = lista_create();
    
    // Adicionar alguns anteparos
    lista_insertTail(anteparos, criar_anteparo_teste(1, 5, -5, 5, 5));
    lista_insertTail(anteparos, criar_anteparo_teste(2, -5, 10, 5, 10));
    
    int event_count = 0;
    Evento *eventos = visibilidade_prepSegments(anteparos, bomba, &event_count);
    
    printf("Eventos criados: %d\n", event_count);
    
    // Cada segmento gera 2 eventos (início e fim), então esperamos 4 eventos
    if (event_count == 4) {
        printf("OK: Número correto de eventos\n");
    } else {
        printf("AVISO: Esperava 4 eventos, obteve %d\n", event_count);
    }
    
    // Cleanup
    if (eventos) free(eventos);
    lista_passthrough(anteparos, helper_destruir_forma, NULL);
    lista_destroy(anteparos);
    ponto_destroy(bomba);
}

int main() 
{
    printf("╔════════════════════════════════════════╗\n");
    printf("║  TESTES DO MÓDULO VISIBILIDADE        ║\n");
    printf("╚════════════════════════════════════════╝\n");
    
    teste_visibilidade_basico();
    teste_visibilidade_sem_obstaculos();
    teste_visibilidade_caixa_fechada();
    teste_ray_collision();
    teste_prep_segments();
    
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║  TESTES CONCLUÍDOS                     ║\n");
    printf("╚════════════════════════════════════════╝\n");
    
    return 0;
}