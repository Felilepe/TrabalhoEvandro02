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

// FUNÇÕES AUXILIARES GEOMÉTRICAS

static double calcula_angulo(double x_obs, double y_obs, double x_p, double y_p) 
{

    double angulo = atan2(y_p - y_obs, x_p - x_obs);
    if (angulo < 0) {
        angulo += 2.0 * PI;
    }

    return angulo;
}

static double distancia_quadrada(double x1, double y1, double x2, double y2) 
{
    double deltaX = x1 - x2;
    double deltaY = y1 - y2;
    return (deltaX * deltaX) + (deltaY * deltaY);
}

static int produto_vetorial(double px, double py, double qx, double qy, double rx, double ry) 
{
    double val = (qx - px) * (ry - py) - (qy - py) * (rx - px);
    if (fabs(val) < 1e-10) return 0;
    return (val > 0) ? 1 : 2;
}

static bool is_ponto_no_segmento(double px, double py, double qx, double qy, double rx, double ry) {
    return (qx <= fmax(px, rx) && qx >= fmin(px, rx) && qy <= fmax(py, ry) && qy >= fmin(py, ry));
}

double calcular_distancia_ponto_segmento(double px, double py, Anteparo a) 
{
    double x1 = anteparo_getX1(a);
    double y1 = anteparo_getY1(a);
    double x2 = anteparo_getX2(a);
    double y2 = anteparo_getY2(a);

    double dx = x2 - x1;
    double dy = y2 - y1;

    double len_sq = dx * dx + dy * dy;
    if (fabs(len_sq) < EPSILON) {
        return sqrt(distancia_quadrada(px, py, x1, y1));
    }

    double t = ((px - x1) * dx + (py - y1) * dy) / len_sq;

    t = fmax(0.0, fmin(1.0, t));

    double proj_x = x1 + t * dx;
    double proj_y = y1 + t * dy;

    return sqrt(distancia_quadrada(px, py, proj_x, proj_y));
}

double calc_dist_anteparo_bomba(Anteparo a, double x_bomba, double y_bomba, double angulo, double raio_max) 
{
    if (a == NULL) return DBL_MAX;


    double rx = x_bomba + raio_max * cos(angulo);
    double ry = y_bomba + raio_max * sin(angulo);


    double s1x = anteparo_getX1(a);
    double s1y = anteparo_getY1(a);
    double s2x = anteparo_getx2(a);
    double s2y = anteparo_getY2(a);

    double denom = (s2y - s1y) * (rx - x_bomba) - (s2x - s1x) * (ry - y_bomba);

    if (fabs(denom) < EPSILON) return DBL_MAX;

    double ua = ((s2x - s1x) * (y_bomba - s1y) - (s2y - s1y) * (x_bomba - s1x)) / denom;
    double ub = ((rx - x_bomba) * (y_bomba - s1y) - (ry - y_bomba) * (x_bomba - s1x)) / denom;

    if (ua >= -EPSILON && ua <= 1.0 + EPSILON && ub >= -EPSILON && ub <= 1.0 + EPSILON) {
        double dist = ua * raio_max;

        if (dist >= -EPSILON && dist <= raio_max + EPSILON) {
            return (dist < 0) ? 0 : dist;
        }
    }
    return DBL_MAX;
}



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

static void buscar_intersecao_avl_rec(node_AVL *node, double x_bomba, double y_bomba, double angulo, double *dist_min, ponto **ponto_intersecao, double raio_max) 
{
    if (node == NULL) return;

    buscar_intersecao_avl_rec(get_esquerda_node(node), bomba, angulo, dist_min, ponto_intersecao, raio_max);

    segmento_ativo *sa = (segmento_ativo*)get_node_dataAVL(node);
    double dist = calc_dist_anteparo_bomba(sa -> seg, bomba, angulo, raio_max);

    if (dist < *dist_min) {
        *dist_min = dist;

        if (*ponto_intersecao != NULL) free_ponto(*ponto_intersecao);
        double x = get_x_ponto(bomba) + dist * cos(angulo);
        double y = get_y_ponto(bomba) + dist * sin(angulo);

        *ponto_intersecao = init_ponto(x, y);
    }

    buscar_intersecao_avl_rec(get_direita_node(node), bomba, angulo, dist_min, ponto_intersecao, raio_max);
}

static void update_evento_arvore(Arvore *seg_ativo, evento *e) 
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

static void capturar_ponto_visibilidade(Poligono vis, Arvore *seg_ativos, double x_bomba, double y_bomba, double angulo, double raio_max) 
{
    ponto *p = raio_ate_anteparo_avl(seg_ativos, bomba, angulo, raio_max);
    if (p != NULL) {
        if (validar_ponto(p, bomba)) {
            ponto *ultimo = get_ultimo_vertice(vis);
            if (!ultimo || dist_pontos(p, bomba)) {
                insert_ponto_poligono(vis, p);
            }
        }
            free_ponto(p);
    }
}


// FUNÇÕES PRINCIPAIS

Evento visibilidade_prepare(double x_bomba, double y_bomba, Lista *anteparos, int *num_eventos)
{
    if (anteparos == NULL){
        printf("Erro: lista de anteparos nula na preparação de visibilidade.\n");
        exit(1);
    }

    int size_lista = get_tam_lista(anteparos);
    int max_eventos = 4 * size_lista;
    evento *vetor_eventos = malloc(max_eventos * sizeof(evento));

    node *atual = get_head_node(anteparos);
    int k = 0;

    while (atual != NULL) {
        forma *f = get_node_data(atual);
        Anteparo *a = get_dados_forma(f);

        double ang1 = normalizar_angulo(calcula_angulo(x_bomba, y_bomba, anteparo_getX1(a), anteparo_getY1(a)));
        double ang2 = normalizar_angulo(calcula_angulo(x_bomba, y_bomba, anteparo_getX2(a), anteparo_getY2(a)));

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

        double dist = calcular_distancia_ponto_segmento(x_bomba, y_bomba, a);
        int id = anteparo_getId(a);

        if (ang1 > ang2) {
            vetor_eventos[k++] = (evento){id, ang1, dist, INICIO, a};
            vetor_eventos[k++] = (evento){id, PI, dist, FIM, a};

            vetor_eventos[k++] = (evento){id, 0.0, dist, INICIO, a};
            vetor_eventos[k++] = (evento){id, ang2, dist, FIM, a};
        }

        else {
            vetor_eventos[k++] = (evento){id, ang1, dist, INICIO, a};
            vetor_eventos[k++] = (evento){id, ang2, dist, FIM, a};
        }
        atual = go_next_node(atual);
    }

    *num_eventos = k;
    return vetor_eventos;
}



double visibilidade_getXRay(Arvore *segmentos_visiveis, double x_origem, double y_origem, double angulo, double max_dist)
{
    if (arvore_getSize(segmentos_visiveis) == 0) {
        double x = x_origem + max_dist * cos(angulo);
        return x;
    }

    ponto *p_intersecao = NULL;
    double dist_min = max_dist;
    buscar_intersecao_avl_rec(arvore_get_root_data(segmentos_visiveis), x_origem, y_origem, angulo, &dist_min, &p_intersecao, max_dist);
    if (p_intersecao != NULL) return p_intersecao;

    double x =  x_origem + max_dist * cos(angulo);
    return x;
}

double visibilidade_getYRay(Arvore *segmentos_visiveis, double x_origem, double y_origem, double angulo, double max_dist)
{
    if (arvore_getSize(segmentos_visiveis) == 0) {
        double y = y_origem + max_dist * sin(angulo);
        return y;
    }

    ponto *p_intersecao = NULL;
    double dist_min = max_dist;
    buscar_intersecao_avl_rec(arvore_get_root_data(segmentos_visiveis), x_origem, y_origem, angulo, &dist_min, &p_intersecao, max_dist);
    if (p_intersecao != NULL) return p_intersecao;

    double y = y_origem + max_dist * sin(angulo);
    return y;
}

Poligono visibilidade_calc(double x_bomba, double y_bomba, Lista *anteparos, char tipo_ord, double max_dist, int insert_limit)
{
    Poligono *visibilidade = poligono_create();

    Retangulo *box_mundo = criaRetangulo(-1, 5, 5, 1010, 1010, "none", "none");
    forma *f_box = cria_forma(-1, TIPO_R, box_mundo);
    Lista *paredes_mundo = forma_anteparo(f_box, 'h');

    int qtd_paredes = 0;
    node *aux = get_head_node(paredes_mundo);
    while(aux != NULL) {
        forma *f = get_node_data(aux);
        insert_tail(anteparos, f);
        qtd_paredes++;
        aux = go_next_node(aux);
    }

    int num_eventos = 0;
    evento *eventos = preparar_segmentos(bomba, anteparos, &num_eventos);

    // 2. CASO SEM NENHUM ANTEPARO -> SIMPLESMENTE CRIA UM CÍRCULO QUE ABRANGE A ÁREA TODA
    if (num_eventos == 0) {
        for (int i = 0; i < 32; i++) {
            double ang = (2.0 * PI * i) / 32.0;
            double x = get_x_ponto(bomba) + raio_max * cos(ang);
            double y = get_y_ponto(bomba) + raio_max * sin(ang);
            insert_vertice(visibilidade, x, y);
        }
        free(eventos); return visibilidade;
    }

    // 3. ORDENAÇÃO -> ORDENA OS EVENTOS CONFORME O ÂNGULO OBTIDO ENTRE O PONTO DA BOMBA E O PONTO (p0 ou p1) DO ANTEPARO
    if (tipo_ord == 'm') {
        merge_sort_generico(eventos, num_eventos, sizeof(evento), cmp_eventos, threshold_i);
    } else {
        qsort(eventos, num_eventos, sizeof(evento), cmp_eventos);
    }

    Arvore *seg_ativos = init_arvore(comparar_segmentos_ativos, free_segmento_ativo, NULL);

    // 4. PRÉ-CARREGAMENTO (Segmentos que cruzam o zero)
    for (int i = 0; i < num_eventos; i++) {
        if (eventos[i].tipo == INICIO && eventos[i].angulo < EPSILON_ANGULAR) {
            update_evento_arvore(seg_ativos, &eventos[i]);
        }
    }

    // 5. SWEEP LINE
    printf("Iniciando algoritmo de sweep line...\n");
    for (int i = 0; i < num_eventos; i++) {
        evento *atual = &eventos[i];
        bool ja_processado_no_inicio = (atual->tipo == INICIO && atual -> angulo < EPSILON_ANGULAR);

        // Disparo ANTES (Captura estado atual / parede de fundo)
        capturar_ponto_visibilidade(visibilidade, seg_ativos, bomba, atual -> angulo, raio_max);

        // Atualização
        if (!ja_processado_no_inicio) {
            update_evento_arvore(seg_ativos, atual);
        }

        // Disparo DEPOIS (Captura novo estado / quina do obstáculo)
        capturar_ponto_visibilidade(visibilidade, seg_ativos, bomba, atual -> angulo, raio_max);
    }

    // 6. LIMPEZA
    for(int i = 0; i < qtd_paredes; i++) {
        remove_tail(anteparos);
    }

    free_lista(paredes_mundo, (void(*)(void*))destrutor_forma);
    free(f_box);
    destrutorRetangulo(box_mundo);

    free(eventos);
    free_arvore(seg_ativos);

    // 7. CALCULA A BOUNDING BOX DO POLÍGONO PARA OTIMIZAÇÃO DO ALGORITMO DE CÁLCULO DE SOBREPOSIÇÃO
    if (get_num_vertices(visibilidade) > 0) {
        calcular_bounding_box(visibilidade);
    }

    return visibilidade;
}