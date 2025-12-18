#include <stdio.h>
#include <stdlib.h>
#include "poligono.h"
#include "linha.h"
#include <float.h>

#define EPSILON 1e-10
#define SEGMENT_ID_START 9000

typedef struct stPoligono
{
    Lista *vertices;
    double xMin, xMax, yMin, yMax;
    bool valid_bbox;

}poligono;



Poligono poligono_create()
{
    poligono *p = malloc(sizeof(poligono));
    if(p == NULL){
        printf("Erro: Poligono nulo em poligono_create.\n");
        exit(1);
    }

    p -> vertices = lista_create();
    if(p -> vertices == NULL){
        printf("Erro: Lista nula em poligono_create\n");
        exit(1);
    }
    
    p -> valid_bbox = false;

    return p;
}

static void atualizar_limites_callback(void* item, void* aux) //Função auxiliar para poligono_calcBBox
{ 
    Ponto pt = (Ponto)item;
    poligono* p = (poligono*)aux; // Recuperamos o polígono passado como contexto

    double x = ponto_getCoordX(pt);
    double y = ponto_getCoordY(pt);

    // Lógica padrão de Min/Max
    if (x < p->xMin) p->xMin = x;
    if (x > p->xMax) p->xMax = x;
    if (y < p->yMin) p->yMin = y;
    if (y > p->yMax) p->yMax = y;
}

void poligono_calcBBox(Poligono p) 
{
    if (p == NULL || poligono_getVerticeCount(p) == 0) return;

    poligono *poli = (poligono*)p;

    poli->xMin = DBL_MAX;
    poli->yMin = DBL_MAX;
    poli->xMax = -DBL_MAX;
    poli->yMax = -DBL_MAX;


    lista_passthrough(poli->vertices, atualizar_limites_callback, poli);

    poli -> valid_bbox = true;
}

void poligono_insertVertice(Poligono p, Ponto v)
{
    if(p == NULL || v == NULL){
        printf("Erro: Poligono ou ponto nulo(s) em poligono_insertVertice.\n");
        exit(1);
    }

    poligono *poli = (poligono*)p;

    lista_insertTail(poli -> vertices, v);

    poli -> valid_bbox = false;     
}

void poligono_copyPasteVertice(Poligono p, Ponto v)
{
    if(p == NULL || v == NULL){
        printf("Erro: poligono e/ou ponto nulo(s) em poligono_copyPasteVertice.\n");
        exit(1);
    }

    poligono *poli = (poligono*)p;

    double x = ponto_getCoordX(v);
    double y = ponto_getCoordY(v);

        if (lista_getSize(poli -> vertices) > 0) {
        double dx = x - ponto_getCoordX(lista_getTail(poli -> vertices));
        double dy = y - ponto_getCoordY(lista_getTail(poli -> vertices));
        double dist_sq = dx * dx + dy * dy;

        if (dist_sq < EPSILON * EPSILON) {
            return;
        }
    }

    Ponto copy = ponto_clone(v);
    poligono_insertVertice(p, copy);
}

bool poligono_isEmpty(Poligono p)
{
    if(p == NULL){
        printf("Erro: Poligono nulo em poligono_isEmpty.\n");
        exit(1);
    }

    return (!poligono_getVerticeCount(p));
}

typedef struct  //1. Struct auxiliar para poligono_isInside
{
    double px, py;        // O ponto que estamos testando
    Ponto anterior;       // O vértice da iteração passada
    Ponto primeiro;       // O primeiro vértice (para fechar o loop no final)
    int intersecoes;      // Contador
} RayCastContext;

static void testar_aresta(Ponto p1, Ponto p2, RayCastContext* ctx) // 2. Função Auxiliar que calcula a interseção de UMA aresta
{
    double x1 = ponto_getCoordX(p1);
    double y1 = ponto_getCoordY(p1);
    double x2 = ponto_getCoordX(p2);
    double y2 = ponto_getCoordY(p2);

    if ((y1 > ctx->py) != (y2 > ctx->py)) {
        double xIntersecao = (x2 - x1) * (ctx->py - y1) / (y2 - y1) + x1;
        
        if (ctx->px < xIntersecao) {
            ctx->intersecoes++;
        }
    }
}

static void ray_cast_callback(void* item, void* aux) //3. Função auxiliar de callback
{
    Ponto atual = (Ponto)item;
    RayCastContext* ctx = (RayCastContext*)aux;

    if (ctx->anterior == NULL) {
        ctx->primeiro = atual; 
        ctx->anterior = atual; 
        return;
    }

    testar_aresta(ctx->anterior, atual, ctx);

    ctx->anterior = atual;
}

bool poligono_isInside(Poligono p, Ponto pp) //4. Função principal

{
    if (p == NULL || poligono_getVerticeCount(p) < 3) return false;

    poligono* poli = (poligono*)p;
    double px = ponto_getCoordX(pp);
    double py = ponto_getCoordY(pp);

    if (!poli->valid_bbox) {
        poligono_calcBBox(p);
    }
    if (px < poli->xMin || px > poli->xMax || py < poli->yMin || py > poli->yMax) {
        return false;
    }

    RayCastContext ctx;
    ctx.px = px;
    ctx.py = py;
    ctx.intersecoes = 0;
    ctx.anterior = NULL;
    ctx.primeiro = NULL;

    lista_passthrough(poli->vertices, ray_cast_callback, &ctx);

    if (ctx.anterior != NULL && ctx.primeiro != NULL) {
        testar_aresta(ctx.anterior, ctx.primeiro, &ctx);
    }

    return (ctx.intersecoes % 2 != 0);
}

static void wrapper_destroy_ponto(void* item, void* aux) // Essa função serve apenas para casar a assinatura que o lista_passthrough pede em poligono_destroy
{
    ponto_destroy((Ponto)item); 
}

void poligono_destroy(Poligono p) 
{
    if (p == NULL) return;

    poligono *poli = (poligono*)p;


    lista_passthrough(poli->vertices, wrapper_destroy_ponto, NULL);


    lista_destroy(poli->vertices);

    free(poli);
}


int poligono_getVerticeCount(Poligono p) 
{
    if(p == NULL){
        printf("Erro: Poligono nulo em poligono_getVerticeCount.\n");
        return -1;
    }
    return lista_getSize(((poligono*)p) -> vertices);
}

Ponto poligono_getVertice(Poligono p, int index)
{
if(p == NULL){
    printf("Erro: Poligono nulo em poligono_getVertice.\n");
    exit(1);
}

poligono *poli = (poligono*)p;

int size = poligono_getVerticeCount(p);

if(index < 0 || index >= size){
        printf("Erro: Indice %d invalido. A lista possui indices de 0 a %d.\n", index, size - 1);
        exit(1);
    }

return lista_getItem(poli -> vertices, index);
}

Ponto poligono_getLastVertice(Poligono p)
{
    if(p == NULL){
        printf("Erro: Poligono nulo em poligono_getLastVertice.\n");
        exit(1);
    }

    poligono *poli = (poligono*)p;

    return lista_getTail(poli -> vertices);
}

Lista *poligono_getVerticeList(Poligono p)
{
    if(p == NULL){
        printf("Erro: Poligono nulo em poligono_getVerticeList.\n");
        exit(1);
    }

    return ((poligono*)p) -> vertices;
}

typedef struct //Struct auxiliar para a função poligono_getSegments
{ 
    Lista *listaSegmentos; // Onde vamos guardar os resultados
    Ponto pontoAnterior;   // O ponto da iteração passada
    Ponto pontoPrimeiro;   // O primeiro ponto (para fechar o polígono no final)
    int *id_counter;       // Ponteiro para o contador de IDs
} ContextoPoligono;

static void gerar_segmento_callback(void *item_atual, void *aux) //Função auxiliar para a função poligono_getSegments

{
    Ponto pAtual = (Ponto)item_atual;
    ContextoPoligono *ctx = (ContextoPoligono*)aux;

    // Se é a primeira iteração (pontoAnterior ainda é NULL)
    if (ctx->pontoAnterior == NULL) {
        ctx->pontoPrimeiro = pAtual; // Guarda o primeiro para fechar depois
        ctx->pontoAnterior = pAtual; // Define o atual como anterior para a próxima volta
        return; // Não cria segmento ainda, só temos um ponto
    }

    // Cria o segmento entre Anterior -> Atual
    double x1 = ponto_getCoordX(ctx->pontoAnterior);
    double y1 = ponto_getCoordY(ctx->pontoAnterior);
    double x2 = ponto_getCoordX(pAtual);
    double y2 = ponto_getCoordY(pAtual);

    int id = ++(*ctx->id_counter); // Incrementa o ID
    
    // Cria e insere
    Linha segmento = linha_create(id, x1, y1, x2, y2, "#000000", false);
    lista_insertTail(ctx->listaSegmentos, segmento);

    // Atualiza o anterior para ser o atual (para a próxima iteração)
    ctx->pontoAnterior = pAtual;
}

Lista *poligono_getSegments(Poligono p)
{
    if(p == NULL){
        printf("Erro: Poligono nulo em poligono_getSegments;\n");
        exit(1);
    } 

    poligono *poli = (poligono*)p;
    
    Lista *segment_list = lista_create();
    static int segment_id = SEGMENT_ID_START;

    ContextoPoligono ctx;
    ctx.listaSegmentos = segment_list;
    ctx.pontoAnterior = NULL;     
    ctx.pontoPrimeiro = NULL;
    ctx.id_counter = &segment_id; 

    lista_passthrough(poli->vertices, gerar_segmento_callback, &ctx);

    if (ctx.pontoAnterior != NULL && ctx.pontoPrimeiro != NULL) {
        double x1 = ponto_getCoordX(ctx.pontoAnterior); 
        double y1 = ponto_getCoordY(ctx.pontoAnterior);
        double x2 = ponto_getCoordX(ctx.pontoPrimeiro);
        double y2 = ponto_getCoordY(ctx.pontoPrimeiro);

        int id = ++segment_id;
        Linha segmento = linha_create(id, x1, y1, x2, y2, "#000000", false);
        lista_insertTail(segment_list, segmento);
    }

    return segment_list;
}

void poligono_getBBox(Poligono p, double *xMin, double *xMax, double *yMin, double *yMax)
{
    if (p == NULL || xMin == NULL || xMax == NULL || yMin == NULL || yMax == NULL) {
        printf("Erro: Poligono e/ou coordenada(s) nulo(s) em poligono_getBBox.\n");
        exit(1);
    }

    poligono* poli = (poligono*)p;

    if(!poligono_getVerticeCount(p)){
        *yMax = *yMin = *xMax = *xMin = 0.0;
        return;
    }

    if(!poli -> valid_bbox){
        poligono_calcBBox(p);
    }

    *xMax = poli -> xMax;
    *xMin = poli -> xMin;
    *yMax = poli -> yMax;
    *yMin = poli -> yMin;
}
