#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "visibilidade.h"
#include "lista.h"
#include "formas.h"
#include "poligono.h"
#include "arvore.h"
#include "sort.h"
#include "pontos.h"
#include "anteparo.h"
#include "geometria.h"


#define PI 3.14159265358
#define EPSILON 1e-10

typedef struct stInfo_segmento 
{
    int id;
    Anteparo seg;
    double angulo_inicial;
    double angulo_final;
    double distancia;
} info_segmento;

typedef struct stSegmento_ativo 
{
    Anteparo seg;
    int id;
    double dist_bomba;
} segmento_ativo;

static void free_segmento_ativo(void *data) 
{
    if (data) {
        free(data);
    }
}

static int comparar_segmentos_ativos(const void *a, const void *b) 
{
    segmento_ativo *sa = (segmento_ativo*)a;
    segmento_ativo *sb = (segmento_ativo*)b;

    if (sa -> dist_bomba < sb -> dist_bomba - EPSILON) return -1;
    if (sa -> dist_bomba > sb -> dist_bomba + EPSILON) return 1;

    if (sa -> id < sb -> id) return -1;
    if (sa -> id > sb -> id) return 1;

    return 0;
}

forma *criar_forma_anteparo_teste(int id, double x0, double y0, double x1, double y1) 
{
    Ponto p0 = ponto_create(x0, y0);
    Ponto p1 = ponto_create(x1, y1);
    forma f = anteparo_create(0 ,ponto_getCoordX(p0), ponto_getCoordY(p0), ponto_getCoordX(p1), ponto_getCoordY(p1), "black");

    ponto_destroy(p0);
    ponto_destroy(p1);

    return f;
}

void free_forma_anteparo_teste(void *data) 
{
    if (!data) return;

    forma_destroy(data);
}


void print_double_array(const double *arr, int num) 
{
    if (!arr) return;
    printf("[");
    for (int i = 0; i < num; i++) {
        printf(" %.2f ", arr[i] * 180.0 / PI);
    }
    printf("]\n");
}

void print_info_segmento(void *data) {
    if (!data) return;
    info_segmento *info = (info_segmento*) data;
    printf("  ID: %d, Ang0: %.2f, Ang1: %.2f, Dist: %.2f\n",
           info -> id,
           info -> angulo_inicial * 180.0 / PI,
           info -> angulo_final * 180.0 / PI,
           info -> distancia);
}

void print_poligono_vertices(Poligono p) {
    if (!p) {
        printf("Poligono nulo.\n");
        return;
    }
    printf("  Vertices do poligono (%d):\n", lista_getSize(poligono_getVerticeList(p)));

    Lista *vertices = poligono_getVerticeList(p);

    for(int i = 0; i < lista_getSize(vertices); i++) {
        Ponto v = lista_getItem(vertices, i);
        printf("    (%.2f, %.2f)\n", ponto_getCoordX(v), ponto_getCoordY(v));
    }
}

void print_segmento_ativo_data(void *data) 
{
    if (!data) return;
    segmento_ativo *sa = (segmento_ativo*)data;
    printf("(ID: %d, Dist: %.2f) ", sa->id, sa->dist_bomba);
}


void teste_geometria_distAnteBomb() 
{
    printf("\n--- Teste: geometria_distAnteBomb ---\n");

    Ponto bomba = ponto_create(0, 0);

    Ponto p0_v = ponto_create(5, -5);
    Ponto p1_v = ponto_create(5, 5);
    Anteparo a_v = anteparo_create(0 ,ponto_getCoordX(p0_v), ponto_getCoordY(p0_v), ponto_getCoordX(p1_v), ponto_getCoordY(p1_v), "black");


    double ang0 = 0.0;
    double dist0 = geometria_distAnteBomb(a_v, bomba, ang0, 10000.0);
    printf("Vertical, Ang 0: %.2f (Esperado: 5.00)\n", dist0);

    double ang90 = PI / 2.0;
    double dist90 = geometria_distAnteBomb(a_v, bomba, ang90, 10000.0);
    printf("Vertical, Ang 90: ");
    if (dist90 == DBL_MAX) printf("inf (Esperado: inf)\n");
    else printf("%.2f (Esperado: inf)\n", dist90);

    double ang45 = PI / 4.0;
    double dist45 = geometria_distAnteBomb(a_v, bomba, ang45, 10000.0);
    printf("Vertical, Ang 45: %.2f (Esperado: 7.07)\n", dist45);

    double ang180 = PI;
    double dist180 = geometria_distAnteBomb(a_v, bomba, ang180, 10000.0);
    printf("Vertical, Ang 180: ");
    if (dist180 == DBL_MAX) printf("inf (Esperado: inf)\n");
    else printf("%.2f (Esperado: inf)\n", dist180);

    Ponto p0_h = ponto_create(-5, 10);
    Ponto p1_h = ponto_create(5, 10);
    Anteparo a_h = anteparo_create(0 ,ponto_getCoordX(p0_h), ponto_getCoordY(p0_h), ponto_getCoordX(p1_h), ponto_getCoordY(p1_h), "black");


    double dist90_h = geometria_distAnteBomb(a_h, bomba, ang90, 10000.0);
    printf("Horizontal, Ang 90: %.2f (Esperado: 10.00)\n", dist90_h);

    double dist0_h = geometria_distAnteBomb(a_h, bomba, ang0, 10000.0);
    printf("Horizontal, Ang 0: ");
    if (dist0_h == DBL_MAX) printf("inf (Esperado: inf)\n");
    else printf("%.2f (Esperado: inf)\n", dist0_h);

    ponto_destroy(bomba);
    ponto_destroy(p0_h);
    ponto_destroy(p1_h);
    ponto_destroy(p0_v);
    ponto_destroy(p1_v);
    free_anteparo(a_v);
    free_anteparo(a_h);
}

void teste_get_angulos_criticos() {
    printf("\n--- Teste: get_angulos_criticos ---\n");

    Ponto bomba = ponto_create(0, 0);
    Lista *anteparos = lista_create();

    lista_insertTail(anteparos, criar_forma_anteparo_teste(1, 5, -5, 5, 5));
    lista_insertTail(anteparos, criar_forma_anteparo_teste(2, -5, 10, 5, 10));

    double *angulos = NULL;
    int num_angulos = 0;

    get_angulos_criticos(bomba, anteparos, &angulos, &num_angulos);

    printf("Caso 1 (2 segmentos): Num angulos: %d (Esperado: 4)\n", num_angulos);
    printf("  Angulos (graus): ");
    print_double_array(angulos, num_angulos);
    free(angulos);
    angulos = NULL;
    num_angulos = 0;

    lista *anteparos_vazia = lista_create();
    get_angulos_criticos(bomba, anteparos_vazia, &angulos, &num_angulos);
    printf("Caso 2 (lista vazia): Num angulos: %d (Esperado: 0)\n", num_angulos);
    if (angulos == NULL) printf("  Array de angulos: NULL (Correto)\n");

    free(angulos);
    free_lista(anteparos, free_forma_anteparo_teste);
    free_lista(anteparos_vazia, free_forma_anteparo_teste);
    ponto_destroy(bomba);
}

void teste_preparar_segmentos() {
    printf("\n--- Teste: preparar_segmentos ---\n");

    Ponto bomba = ponto_create(0, 0);
    lista *anteparos = lista_create();

    lista_insertTail(anteparos, criar_forma_anteparo_teste(1, 5, -5, 5, 5));
    lista_insertTail(anteparos, criar_forma_anteparo_teste(2, -1, 1, -1, -1));


    lista *info = preparar_segmentos(bomba, anteparos);

    printf("Infos dos segmentos:\n");
    print_lista(info, print_info_segmento);

    free_lista(info, free);
    free_lista(anteparos, free_forma_anteparo_teste);
    ponto_destroy(bomba);
}


void teste_update_AVL_angulo() {
    printf("\n--- Teste: update_AVL_angulo ---\n");

    Ponto bomba = ponto_create(0, 0);
    lista *anteparos = lista_create();

    lista_insertTail(anteparos, criar_forma_anteparo_teste(1, 5, -5, 5, 5));
    lista_insertTail(anteparos, criar_forma_anteparo_teste(2, 10, -10, 10, 10));

    lista *info = preparar_segmentos(bomba, anteparos);

    double *angulos = NULL;
    int num_angulos = 0;
    get_angulos_criticos(bomba, anteparos, &angulos, &num_angulos);
    qsort(angulos, num_angulos, sizeof(double), cmp_double);

    double ang_menos_45 = angulos[0];
    double ang_mais_45 = angulos[2];

    arvore *seg_ativos = init_arvore(comparar_segmentos_ativos, free_segmento_ativo, print_segmento_ativo_data);

    printf("AVL antes: Tam %d\n", get_tam_AVL(seg_ativos));

    update_AVL_angulo(seg_ativos, ang_menos_45, info);
    printf("AVL em -45deg: Tam %d (Esperado: 2)\n  Conteudo: ", get_tam_AVL(seg_ativos));
    printf("\n");

    update_AVL_angulo(seg_ativos, ang_mais_45, info);
    printf("AVL em +45deg: Tam %d (Esperado: 0)\n", get_tam_AVL(seg_ativos));

    free(angulos);
    free_arvore(seg_ativos);
    free_lista(info, free);
    free_lista(anteparos, free_forma_anteparo_teste);
    ponto_destroy(bomba);
}

void teste_raio_ate_anteparo_avl() {
    printf("\n--- Teste: raio_ate_anteparo_avl ---\n");

    Ponto bomba = ponto_create(0, 0);
    arvore *seg_ativos = init_arvore(comparar_segmentos_ativos, free_segmento_ativo, print_segmento_ativo_data);
    double raio_max = 100.0;
    double ang0 = 0.0;

    Ponto p1 = raio_ate_anteparo_avl(seg_ativos, bomba, ang0, raio_max);
    printf("Caso 1 (AVL Vazia): Ponto (%.2f, %.2f) (Esperado: 100.00, 0.00)\n",
           ponto_getCoordX(p1), ponto_getCoordY(p1));
    ponto_destroy(p1);

    Ponto p0_s1 = ponto_create(10, -5); Ponto p1_s1 = ponto_create(10, 5);
    Anteparo a1 = anteparo_create(0 ,ponto_getCoordX(p0_s1), ponto_getCoordY(p0_s1), ponto_getCoordX(p1_s1), ponto_getCoordY(p1_s1), "black");

    segmento_ativo *s1 = malloc(sizeof(segmento_ativo));
    s1 -> id = 1; s1->dist_bomba = 10.0; s1->seg = a1;

    Ponto p0_s2 = ponto_create(5, -5); Ponto p1_s2 = ponto_create(5, 5);
    Anteparo a2 = anteparo_create(0 ,ponto_getCoordX(p0_s2), ponto_getCoordY(p0_s2), ponto_getCoordX(p1_s2), ponto_getCoordY(p1_s2), "black");
    segmento_ativo *s2 = malloc(sizeof(segmento_ativo));
    s2->id = 2; s2->dist_bomba = 5.0; s2->seg = a2;

    arvore_insere(seg_ativos, s1);
    arvore_insere(seg_ativos, s2);

    printf("AVL com 2 segs: Tam %d (Esperado: 2)\n", get_tam_AVL(seg_ativos));

    Ponto p2 = raio_ate_anteparo_avl(seg_ativos, bomba, ang0, raio_max);
    printf("Caso 2 (2 Segs): Ponto (%.2f, %.2f) (Esperado: 5.00, 0.00)\n",
           ponto_getCoordX(p2), ponto_getCoordY(p2));
    ponto_destroy(p2);

    Ponto p3 = raio_ate_anteparo_avl(seg_ativos, bomba, ang0, 3.0); // Raio 3
    printf("Caso 3 (Raio Curto): Ponto (%.2f, %.2f) (Esperado: 3.00, 0.00)\n",
           ponto_getCoordX(p3), ponto_getCoordY(p3));
    ponto_destroy(p3);

    free_arvore(seg_ativos);
    ponto_destroy(p0_s1);
    ponto_destroy(p0_s2);
    ponto_destroy(p1_s1);
    ponto_destroy(p1_s2);
    free_anteparo(a1);
    free_anteparo(a2);
    ponto_destroy(bomba);
}

void teste_calc_regiao_visibilidade() {
    printf("\n--- Teste: calc_regiao_visibilidade ---\n");

    Ponto bomba = ponto_create(0, 0);
    double raio_max = 100.0;
    int threshold = 10;

    printf("Caso 1: Uma parede (qsort)\n");
    lista *anteparos1 = lista_create();
    lista_insertTail(anteparos1, criar_forma_anteparo_teste(1, 5, -50, 5, 50));

    Poligono p1 = calc_regiao_visibilidade(bomba, anteparos1, 'q', raio_max, threshold);
    print_poligono_vertices(p1);

    free_poligono(p1);
    free_lista(anteparos1, free_forma_anteparo_teste);

    printf("\nCaso 2: Sem paredes (mergesort)\n");
    lista *anteparos2 = lista_create();

    Poligono p2 = calc_regiao_visibilidade(bomba, anteparos2, 'm', raio_max, threshold);
    print_poligono_vertices(p2);

    free_poligono(p2);
    free_lista(anteparos2, free_forma_anteparo_teste);

    printf("\nCaso 3: Caixa fechada (qsort)\n");
    lista *anteparos3 = lista_create();
    lista_insertTail(anteparos3, criar_forma_anteparo_teste(1, 5, -5, 5, 5));   // Direita
    lista_insertTail(anteparos3, criar_forma_anteparo_teste(2, -5, -5, 5, -5)); // Cima
    lista_insertTail(anteparos3, criar_forma_anteparo_teste(3, -5, -5, -5, 5)); // Esquerda
    lista_insertTail(anteparos3, criar_forma_anteparo_teste(4, -5, 5, 5, 5));   // Baixo

    Poligono p3 = calc_regiao_visibilidade(bomba, anteparos3, 'q', raio_max, threshold);
    print_poligono_vertices(p3);

    free_poligono(p3);
    free_lista(anteparos3, free_forma_anteparo_teste);

    ponto_destroy(bomba);
}


int main() {

    teste_geometria_distAnteBomb();
    teste_get_angulos_criticos();
    teste_preparar_segmentos();
    teste_update_AVL_angulo();
    teste_raio_ate_anteparo_avl();
    teste_calc_regiao_visibilidade();

    printf("\nTestes de varredura concluidos.\n");

    return 0;
}