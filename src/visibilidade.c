#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stddef.h>
#include <float.h>


#include "visibilidade.h"
#include "arvore.h"
#include "formas.h"
#include "poligono.h"
#include "retangulo.h"
#include "anteparo.h"
#include "geometria.h"
#include "lista.h"

#define EPSILON 1e-10
#define PI 3.14159265358
#define EPSILON_ANGULAR 1e-4
#define INICIO 0
#define FIM 1

typedef struct stEvento
{
    int id;
    double angulo;
    double distancia_estatica;
    int tipo_evento; 
    Anteparo *segmento;

} evento;

typedef struct stSegmento_ativo 
{
    Anteparo *seg;
    int id;
    double dist_bomba;
} segmento_ativo;

//FUNÇÕES AUXILIARES DE EVENTOS DE VISIBILIDADE

static int cmp_eventos(const Evento a, const Evento b) 
{
    evento *e1 = (evento*)a;
    evento *e2 = (evento*)b;

    if (fabs(e1 -> angulo - e2 -> angulo) < EPSILON_ANGULAR) {
        if (e1 -> tipo_evento != e2 -> tipo_evento) {
            return (e1 -> tipo_evento == INICIO) ? -1 : 1;
        }

        if (e1 -> distancia_estatica < e2 -> distancia_estatica - EPSILON) return -1;
        if (e1 -> distancia_estatica > e2 -> distancia_estatica + EPSILON) return 1;
        return 0;
    }

    if (e1 -> angulo < e2 -> angulo) return -1;
    return 1;
}

static double normalizar_angulo(double ang) 
{
    while (ang < 0) ang += 2.0 * PI;

    while (ang >= 2.0 * PI) ang -= 2.0 * PI;

    return ang;
}

static void free_segmento_ativo(void *data) {if (data) free(data);}

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

static void buscar_intersecao_avl_rec(node_AVL *node, double x_bomba, double y_bomba, double angulo, double *dist_min, double *x_intersecao, double *y_intersecao, double raio_max) 
{
    if (node == NULL) return;

    buscar_intersecao_avl_rec(get_esquerda_node(node), x_bomba, y_bomba, angulo, dist_min, x_intersecao, y_intersecao, raio_max);

    segmento_ativo *sa = (segmento_ativo*)get_node_dataAVL(node);
    double dist = geometria_calcDistAnteBmb(sa -> seg, x_bomba, y_bomba, angulo, raio_max);

    if (dist < *dist_min) {
        *dist_min = dist;
        *x_intersecao = x_bomba + dist * cos(angulo);
        *y_intersecao = y_bomba + dist * sin(angulo);
    }

    buscar_intersecao_avl_rec(get_direita_node(node), x_bomba, y_bomba, angulo, dist_min, x_intersecao, y_intersecao, raio_max);
}

static void update_evento_arvore(arvore *seg_ativo, evento *e) 
{
    if (e -> tipo_evento == INICIO) {
        segmento_ativo *novo_ativo = malloc(sizeof(segmento_ativo));
        if (novo_ativo != NULL) {
            novo_ativo -> id = e -> id;
            novo_ativo -> seg = e -> segmento;
            novo_ativo -> dist_bomba = e -> distancia_estatica;
            arvore_insere(seg_ativo, novo_ativo);
        }
    }

    else {
        segmento_ativo chave_busca;
        chave_busca.id = e -> id;
        chave_busca.dist_bomba = e -> distancia_estatica;
        void *seg_removido = remove_node(seg_ativo, &chave_busca);
        if (seg_removido) free(seg_removido);
    }
}

static void capturar_ponto_visibilidade(Poligono vis, arvore *seg_ativos, double x_bomba, double y_bomba, double angulo, double raio_max) 
{
    double x_intersecao = x_bomba + raio_max * cos(angulo);
    double y_intersecao = y_bomba + raio_max * sin(angulo);
    double dist_min = raio_max;
    
    buscar_intersecao_avl_rec(arvore_getRoot(seg_ativos), x_bomba, y_bomba, angulo, &dist_min, &x_intersecao, &y_intersecao, raio_max);
    
    Lista *vertices = poligono_getVertices(vis);
    
    if (vertices == NULL || lista_isEmpty(vertices)) {
        poligono_insertPoint(vis, x_intersecao, y_intersecao);
    } else {
        item primeiro = lista_getHead(vertices);
        if (primeiro != NULL) {
            typedef struct { double x, y; } VerticeGenerico;
            VerticeGenerico *v = (VerticeGenerico*)primeiro;
            double dist_sq = (x_intersecao - v->x) * (x_intersecao - v->x) + 
                            (y_intersecao - v->y) * (y_intersecao - v->y);
            
            if (dist_sq > EPSILON) {
                poligono_insertPoint(vis, x_intersecao, y_intersecao);
            }
        }
    }
}



typedef struct 
{
    evento *vetor_eventos;
    int *k;
    double x_bomba, y_bomba;
} dados_prepare;

static void processar_anteparo_callback(forma f, item aux_data)
{
    dados_prepare *dp = (dados_prepare*)aux_data;
    if (f == NULL) return;
    
    Anteparo a = lista_getHead(forma_anteparo(f, forma_getID(f), 'H'));
    double x_bomba = dp -> x_bomba;
    double y_bomba = dp -> y_bomba;
    
    double ang1 = normalizar_angulo(geometria_calcAngulo(x_bomba, y_bomba, anteparo_getX1(a), anteparo_getY1(a)));
    double ang2 = normalizar_angulo(geometria_calcAngulo(x_bomba, y_bomba, anteparo_getX2(a), anteparo_getY2(a)));
    
    if (fabs(ang1 - ang2) < 1e-9) {
        ang2 += 1e-8;
    }
    
    double diff = ang2 - ang1;
    if (diff < 0) diff += 2.0 * PI;
    
    if (diff > PI) {
        double temp = ang1;
        ang1 = ang2;
        ang2 = temp;
    }
    
    double dist = geometria_calcDistPointSeg(x_bomba, y_bomba, a);
    int id = anteparo_getId(a);
    
    if (ang1 > ang2) {
        dp->vetor_eventos[(*dp->k)++] = (evento){id, ang1, dist, INICIO, a};
        dp->vetor_eventos[(*dp->k)++] = (evento){id, PI, dist, FIM, a};
        dp->vetor_eventos[(*dp->k)++] = (evento){id, 0.0, dist, INICIO, a};
        dp->vetor_eventos[(*dp->k)++] = (evento){id, ang2, dist, FIM, a};
    } else {
        dp->vetor_eventos[(*dp->k)++] = (evento){id, ang1, dist, INICIO, a};
        dp->vetor_eventos[(*dp->k)++] = (evento){id, ang2, dist, FIM, a};
    }
}



// FUNÇÕES PRINCIPAIS

Evento visibilidade_prepare(double x_bomba, double y_bomba, Lista *anteparos, int *num_eventos)
{
    if (anteparos == NULL){
        printf("Erro: lista de anteparos nula na preparação de visibilidade.\n");
        exit(1);
    }

    int size_lista = lista_getSize(anteparos);
    int max_eventos = 4 * size_lista;
    evento *vetor_eventos = malloc(max_eventos * sizeof(evento));

    int k = 0;
    dados_prepare dp = {vetor_eventos, &k, x_bomba, y_bomba};
    lista_passthrough(anteparos, processar_anteparo_callback, (item)&dp);

    *num_eventos = k;
    return vetor_eventos;
}



double visibilidade_getXRay(arvore *segmentos_visiveis, double x_origem, double y_origem, double angulo, double max_dist)
{
    if (arvore_getSize(segmentos_visiveis) == 0) {
        double x = x_origem + max_dist * cos(angulo);
        return x;
    }

    double x_intersecao = x_origem + max_dist * cos(angulo);
    double y_intersecao = y_origem + max_dist * sin(angulo);
    double dist_min = max_dist;
    buscar_intersecao_avl_rec(arvore_getRoot(segmentos_visiveis), x_origem, y_origem, angulo, &dist_min, &x_intersecao, &y_intersecao, max_dist);
    
    return x_intersecao;
}

double visibilidade_getYRay(arvore *segmentos_visiveis, double x_origem, double y_origem, double angulo, double max_dist)
{
    if (arvore_getSize(segmentos_visiveis) == 0) {
        double y = y_origem + max_dist * sin(angulo);
        return y;
    }

    double x_intersecao = x_origem + max_dist * cos(angulo);
    double y_intersecao = y_origem + max_dist * sin(angulo);
    double dist_min = max_dist;
    buscar_intersecao_avl_rec(arvore_getRoot(segmentos_visiveis), x_origem, y_origem, angulo, &dist_min, &x_intersecao, &y_intersecao, max_dist);
    
    return y_intersecao;
}

static void adicionar_parede_callback(item i, item aux_data)
{
    forma *f = (forma*)i;
    struct { Lista *anteparos; int *qtd; } *dados = (struct { Lista *anteparos; int *qtd; }*)aux_data;
    
    if (f != NULL) {
        lista_insere_fim(dados->anteparos, f);
        (*dados->qtd)++;
    }
}

Poligono visibilidade_calc(double x_bomba, double y_bomba, Lista *anteparos, char tipo_ord, double max_dist, int insert_limit)
{
    Poligono visibilidade = poligono_create();

    Retangulo box_mundo = retangulo_create(-1, 5, 5, 1010, 1010, "none", "none");
    forma f_box = forma_clonar(box_mundo);
    Lista *paredes_mundo = forma_anteparo(f_box, retangulo_getID(box_mundo), 'h');

    int qtd_paredes = 0;
    struct { Lista *anteparos; int *qtd; } dados_paredes = {anteparos, &qtd_paredes};
    lista_passthrough(paredes_mundo, adicionar_parede_callback, (item)&dados_paredes);

    int num_eventos = 0;
    evento *eventos = visibilidade_prepare(x_bomba, y_bomba, anteparos, &num_eventos);

    if (num_eventos == 0) {
        for (int i = 0; i < 32; i++) {
            double ang = (2.0 * PI * i) / 32.0;
            double x = x_bomba + max_dist * cos(ang);
            double y = y_bomba + max_dist * sin(ang);
            poligono_insertPoint(visibilidade, x, y);
        }
        free(eventos); return visibilidade;
    }

    if (tipo_ord == 'm') {
        qsort(eventos, num_eventos, sizeof(evento), cmp_eventos);
    } else {
        qsort(eventos, num_eventos, sizeof(evento), cmp_eventos);
    }

    arvore *seg_ativos = arvore_create(comparar_segmentos_ativos, free_segmento_ativo, NULL);

    for (int i = 0; i < num_eventos; i++) {
        if (eventos[i].tipo_evento == INICIO && eventos[i].angulo < EPSILON_ANGULAR) {
            update_evento_arvore(seg_ativos, &eventos[i]);
        }
    }

    printf("Iniciando algoritmo de sweep line...\n");
    for (int i = 0; i < num_eventos; i++) {
        evento *atual = &eventos[i];
        bool ja_processado_no_inicio = (atual->tipo_evento == INICIO && atual -> angulo < EPSILON_ANGULAR);

        capturar_ponto_visibilidade(visibilidade, seg_ativos, x_bomba, y_bomba, atual -> angulo, max_dist);

        if (!ja_processado_no_inicio) {
            update_evento_arvore(seg_ativos, atual);
        }

        capturar_ponto_visibilidade(visibilidade, seg_ativos, x_bomba, y_bomba, atual -> angulo, max_dist);
    }

    for(int i = 0; i < qtd_paredes; i++) {
        item parede = lista_remove_fim(anteparos);
        if (parede != NULL) forma_destroy((forma)parede);
    }

    lista_destroy(paredes_mundo);
    forma_destroy(f_box);
    retangulo_destroy(box_mundo);

    free(eventos);
    free_arvore(seg_ativos);

    if (poligono_getNumVertices(visibilidade) > 0) {
        poligono_calcBoundingBox(visibilidade);
    }

    return visibilidade;
}