#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "geometria.h"
#include "sort.h"
#include "formas.h"
#include "poligono.h"
#include "retangulo.h"
#include "visibilidade.h"

#define PI 3.14159265358
#define EPSILON 1e-10
#define EPSILON_ANGULAR 1e-4


typedef enum EnEventType 
{
    INICIO,
    FIM
}event_type;

struct stEvent 
{
    int id;
    double angle;
    double static_dist;
    event_type tipo;
    Anteparo segment;
};

typedef struct stSegmento_ativo 
{
    Anteparo seg;
    int id;
    double dist_bomb;
}active_seg;



// --- Funções Auxiliares ---

static int event_compare(const void *a, const void *b)
{
    const Evento *e1 = (const Evento*)a;
    const Evento *e2 = (const Evento*)b;
    
    if (fabs(e1->angle - e2->angle) < EPSILON_ANGULAR) {
        
        if (e1->tipo != e2->tipo) {
            return (e1->tipo == INICIO) ? -1 : 1;
        }

        if (e1->static_dist < e2->static_dist - EPSILON) return -1;
        if (e1->static_dist > e2->static_dist + EPSILON) return 1;
        return 0;
    }

    if (e1->angle < e2->angle) return -1;
    return 1;
}
static double normalize(double angle)
{
    while (angle < 0) angle += 2.0 * PI;

    while (angle >= 2.0 * PI) angle -= 2.0 * PI;

    return angle;
}

static void free_active_segment(void* data) 
{
    if (data) free(data);
}

static bool validar_ponto(Ponto p, Ponto bomb) 
{
    if (p == NULL || bomb == NULL) return false;

    double x = ponto_getCoordX(p);
    double y = ponto_getCoordY(p);

    if (!isfinite(x) || !isfinite(y)) return false;

    return true;
}

static int compare_active_seg(const void* a, const void* b) 
{
    active_seg *seg_a = (active_seg*)a;
    active_seg *seg_b = (active_seg*)b;

    if (seg_a -> dist_bomb < seg_b -> dist_bomb - EPSILON) return -1;
    if (seg_a -> dist_bomb > seg_b -> dist_bomb + EPSILON) return 1;

    if (seg_a -> id < seg_b -> id) return -1;
    if (seg_a -> id > seg_b -> id) return 1;
    return 0;
}

static void buscar_intersecao_avl_rec(ArvNode *node, Ponto bomb, double angle, double *min_dist, Ponto *intersec_point, double max_radius) 
{
    if (node == NULL) return;

    buscar_intersecao_avl_rec(arvore_getLeftNode(node), bomb, angle, min_dist, intersec_point, max_radius);

    active_seg *sa = (active_seg*)arvore_getNodeData(node);
    double dist = geometria_distAnteBomb(sa -> seg, bomb, angle, max_radius);

    if (dist < *min_dist) {
        *min_dist = dist;

        if (*intersec_point != NULL) ponto_destroy(*intersec_point);
        double x = ponto_getCoordX(bomb) + dist * cos(angle);
        double y = ponto_getCoordY(bomb) + dist * sin(angle);

        *intersec_point = ponto_create(x, y);
    }

    buscar_intersecao_avl_rec(arvore_getRightNode(node), bomb, angle, min_dist, intersec_point, max_radius);
}

static void update_evento(Arvore *seg_ativo, Evento *e) 
{
    if (e -> tipo == INICIO) {
        active_seg *novo_ativo = malloc(sizeof(active_seg));
        if (novo_ativo != NULL) {
            novo_ativo -> id = e -> id;
            novo_ativo -> seg = e -> segment;
            novo_ativo -> dist_bomb = e -> static_dist;
            arvore_insert(seg_ativo, novo_ativo);
        }
    }

    else {
        active_seg chave_busca;
        chave_busca.id = e->id;
        chave_busca.dist_bomb = e->static_dist;
        

        arvore_remove(seg_ativo, &chave_busca);
        
    }
}

static void insert_visibility_point(Poligono vis, Arvore *seg_ativos, Ponto bomb, double angle, double max_radius) 
{
    Ponto p = visibilidade_rayCollision(seg_ativos, bomb, angle, max_radius);
    if (p != NULL) {
        if (validar_ponto(p, bomb)) {
            Ponto ultimo = poligono_getLastVertice(vis);
            if (!ultimo || geometria_calcDistPoints(p, bomb)) {
                poligono_copyPasteVertice(vis, p);
            }
        }
        
        ponto_destroy(p);
    }
}


// Função auxiliar para inserir itens de uma lista em outra
void helper_inserir_na_lista(void *dado, void *lista_destino) 
{
    Lista *dest = (Lista *)lista_destino;
    lista_insertTail(dest, dado);
}

// Função auxiliar para destruir anteparos (adaptador para assinatura do passthrough)
void helper_destruir_anteparo(void *dado, void *ignore) //"Unused parameter" é um parâmetro de                                                       
{                                                       //compatibilidade com lista_passthrough
    anteparo_destroy(dado); 
}


// --- Funções Pública ---

Ponto visibilidade_rayCollision(Arvore *active_seg, Ponto bomb, double angle, double max_radius)
{
    if (arvore_getSize(active_seg) == 0) {
        double x = ponto_getCoordX(bomb) + max_radius * cos(angle);
        double y = ponto_getCoordY(bomb) + max_radius * sin(angle);
        return ponto_create(x, y);
    }

    Ponto p_intersecao = NULL;
    double dist_min = max_radius;
    buscar_intersecao_avl_rec(arvore_getRootNode(active_seg), bomb, angle, &dist_min, &p_intersecao, max_radius);
    if (p_intersecao != NULL) return p_intersecao;

    double x = ponto_getCoordX(bomb) + max_radius * cos(angle);
    double y = ponto_getCoordY(bomb) + max_radius * sin(angle);
    return ponto_create(x, y);
}

Evento *visibilidade_prepSegments(Lista *anteparos, Ponto bomb, int *event_count)
{
    if (bomb == NULL || anteparos == NULL) return NULL;

    int size_lista = lista_getSize(anteparos); 
    
    int max_eventos = 4 * size_lista;
    Evento *vetor_eventos = malloc(max_eventos * sizeof(Evento));
    if (vetor_eventos == NULL) return NULL; 

    int k = 0;
    
    for(int i = 0; i < size_lista; i++) {
        
        forma f = lista_getItem(anteparos, i); 

        Anteparo a = (Anteparo)f; 

        Ponto p0 = anteparo_getP1(a);
        Ponto p1 = anteparo_getP2(a);

        double ang1 = normalize(geometria_calcAngle(bomb, p0));
        double ang2 = normalize(geometria_calcAngle(bomb, p1));

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


        double dist = geometria_distPointSeg(bomb, a);
        int id = anteparo_getID(a);

        if (ang1 > ang2) {
            vetor_eventos[k++] = (Evento){id, ang1, dist, INICIO, a};
            vetor_eventos[k++] = (Evento){id, 2.0 * PI, dist, FIM, a}; 

            vetor_eventos[k++] = (Evento){id, 0.0, dist, INICIO, a};
            vetor_eventos[k++] = (Evento){id, ang2, dist, FIM, a};
        }
        else {
            vetor_eventos[k++] = (Evento){id, ang1, dist, INICIO, a};
            vetor_eventos[k++] = (Evento){id, ang2, dist, FIM, a};
        }
    }

    *event_count = k;
    return vetor_eventos;
}

Poligono visibilidade_calc(Lista *anteparos, Ponto bomb, char sort_type, double max_radius, int max_inserts) 
{
    Poligono visibilidade = poligono_create();


    Retangulo box_mundo = retangulo_create(-1, 5, 5, 1010, 1010, "none", "none");
    forma f_box = forma_clonar(box_mundo); 
    Lista *paredes_mundo = forma_toAnteparo(f_box, 'h');

    int qtd_paredes_mundo = lista_getSize(paredes_mundo);

    lista_passthrough(paredes_mundo, helper_inserir_na_lista, anteparos);

    int num_eventos = 0;
    Evento *eventos = visibilidade_prepSegments(anteparos, bomb, &num_eventos);

    Arvore *seg_ativos = NULL;

    if (num_eventos == 0) {
        goto limpar_memoria; 
    }

    if (sort_type == 'm') {
        mergeSort(eventos, num_eventos, sizeof(Evento), event_compare, max_inserts);
    } else {
        qsort(eventos, num_eventos, sizeof(Evento), event_compare);
    }

    seg_ativos = arvore_create(compare_active_seg, free_active_segment, NULL);

    for (int i = 0; i < num_eventos; i++) {
        if (eventos[i].angle > EPSILON_ANGULAR) break; 
        if (eventos[i].tipo == INICIO) {
            update_evento(seg_ativos, &eventos[i]);
        }
    }

    for (int i = 0; i < num_eventos; i++) {
        Evento *atual = &eventos[i];
        bool ja_processado = (atual->tipo == INICIO && atual->angle < EPSILON_ANGULAR);

        insert_visibility_point(visibilidade, seg_ativos, bomb, atual->angle, max_radius);

        if (!ja_processado) {
            update_evento(seg_ativos, atual);
        }

        insert_visibility_point(visibilidade, seg_ativos, bomb, atual->angle, max_radius);
    }

    limpar_memoria: // Label para o goto caso precise sair cedo

    for(int i = 0; i < qtd_paredes_mundo; i++) {
        lista_removeTail(anteparos); 
    }

    lista_passthrough(paredes_mundo, helper_destruir_anteparo, NULL);
    lista_destroy(paredes_mundo);
    

    forma_destroy(f_box);
    

    retangulo_destroy(box_mundo);

    if(eventos) free(eventos);
    if(seg_ativos) arvore_destroy(seg_ativos);

    if (poligono_getVerticeCount(visibilidade) > 0) {
        poligono_calcBBox(visibilidade);
    }

    return visibilidade;
}