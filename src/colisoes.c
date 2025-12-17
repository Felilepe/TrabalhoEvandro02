#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "colisoes.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "formas.h"
#include "poligono.h"
#include "geometria.h"
#include "anteparo.h" // Incluído caso não estivesse implícito em formas.h

#define EPSILON 1e-10

// --- Funções Auxiliares de Destruição ---

// Função auxiliar para destruir linhas (adaptador para assinatura do passthrough)
static void helper_destruir_linha(void *dado, void *ignore) 
{
    linha_destroy(dado);
}

// --- Funções Auxiliares de Lógica ---

static bool ponto_dentro_retangulo(Retangulo r, Ponto p) 
{
    double rx = retangulo_getCoordX(r);
    double ry = retangulo_getCoordY(r);
    double rw = retangulo_getWidth(r);
    double rh = retangulo_getHeight(r);

    double px = ponto_getCoordX(p);
    double py = ponto_getCoordY(p);

    return (px >= rx) && (px <= rx + rw) &&
           (py >= ry) && (py <= ry + rh);
}

static bool ponto_dentro_circulo(Circulo c, Ponto p) 
{
    double r = circulo_getRaio(c);
    
    // Cria ponto temporário para o centro
    Ponto centro = ponto_create(circulo_getCoordX(c), circulo_getCoordY(c));
    
    bool pdc = geometria_distSqrd(centro, p) <= (r * r);

    ponto_destroy(centro);
    return pdc;
}

static bool sobrepoe_circulo_linha_texto(Circulo c, Linha l) 
{
    double cx = circulo_getCoordX(c);
    double cy = circulo_getCoordY(c);
    double cr = circulo_getRaio(c);

    double lx1 = linha_getCoordX1(l);
    double ly1 = linha_getCoordY1(l);
    double lx2 = linha_getCoordX2(l);
    double ly2 = linha_getCoordY2(l);

    Ponto temp_l1 = ponto_create(lx1, ly1);
    Ponto temp_l2 = ponto_create(lx2, ly2);
    Ponto temp_c  = ponto_create(cx, cy);

    double distP1Linha = geometria_distSqrd(temp_c, temp_l1);
    double distP2Linha = geometria_distSqrd(temp_c, temp_l2);
    double raioAoQuadrado = cr * cr;

    // Verifica extremidades
    if (distP1Linha <= raioAoQuadrado || distP2Linha <= raioAoQuadrado) {
        ponto_destroy(temp_c);
        ponto_destroy(temp_l1);
        ponto_destroy(temp_l2);
        return true;
    }

    double comprimentoAoQuadrado = geometria_distSqrd(temp_l1, temp_l2);

    // Caso de linha de comprimento zero (ponto)
    if (comprimentoAoQuadrado < EPSILON) {
        ponto_destroy(temp_c);
        ponto_destroy(temp_l1);
        ponto_destroy(temp_l2);
        return distP1Linha <= raioAoQuadrado;
    }

    // Projeção do centro na linha
    double t = ((cx - lx1) * (lx2 - lx1) + (cy - ly1) * (ly2 - ly1)) / comprimentoAoQuadrado;
    t = fmax(0.0, fmin(1.0, t));

    double px = lx1 + t * (lx2 - lx1);
    double py = ly1 + t * (ly2 - ly1);

    Ponto temp_p = ponto_create(px, py);
    double distanciaAoQuadradoDoCirculo = geometria_distSqrd(temp_c, temp_p);

    // [CORREÇÃO CRÍTICA]: Usar ponto_destroy, não linha_destroy
    ponto_destroy(temp_c);
    ponto_destroy(temp_l1);
    ponto_destroy(temp_l2);
    ponto_destroy(temp_p);

    return distanciaAoQuadradoDoCirculo <= raioAoQuadrado;
}

static bool colisaoLinhaLinha(Linha l1, Linha l2) 
{
    double x1l1 = linha_getCoordX1(l1);
    double x2l1 = linha_getCoordX2(l1);
    double y1l1 = linha_getCoordY1(l1);
    double y2l1 = linha_getCoordY2(l1);
    
    double x1l2 = linha_getCoordX1(l2);
    double x2l2 = linha_getCoordX2(l2);
    double y1l2 = linha_getCoordY1(l2);
    double y2l2 = linha_getCoordY2(l2);

    double D = (x1l1 - x2l1) * (y1l2 - y2l2) - (y1l1 - y2l1) * (x1l2 - x2l2);

    // [MELHORIA]: Usar EPSILON em vez de !D
    if (fabs(D) < EPSILON) return false;

    double t = ((x1l1 - x1l2) * (y1l2 - y2l2) - (y1l1 - y1l2) * (x1l2 - x2l2)) / D;
    double u = -(((x1l1 - x2l1) * (y1l1 - y1l2) - (y1l1 - y2l1) * (x1l1 - x1l2)) / D);

    return ((t >= 0 && t <= 1) && (u >= 0 && u <= 1));
}

bool colisaoLinhaRet(Linha l, Retangulo r) 
{
    double xr = retangulo_getCoordX(r);
    double yr = retangulo_getCoordY(r);
    double wr = retangulo_getWidth(r);
    double hr = retangulo_getHeight(r);

    // Cria as 4 arestas do retângulo temporariamente
    Linha topo_temp = linha_create(-1, xr, yr, (xr + wr), yr, "", false);
    Linha esq_temp  = linha_create(-1, xr, yr, xr, (yr + hr), "", false);
    Linha dir_temp  = linha_create(-1, (xr + wr), yr, (xr + wr), (yr + hr), "", false);
    Linha chao_temp = linha_create(-1, xr, (yr + hr), (xr + wr), (yr + hr), "", false);

    bool colidiu = colisaoLinhaLinha(l, topo_temp) ||
                   colisaoLinhaLinha(l, esq_temp) ||
                   colisaoLinhaLinha(l, dir_temp) ||
                   colisaoLinhaLinha(l, chao_temp);

    linha_destroy(topo_temp);
    linha_destroy(esq_temp);
    linha_destroy(dir_temp);
    linha_destroy(chao_temp);

    if (colidiu) return true;

    // Verifica se a linha está inteiramente dentro
    double x1l = linha_getCoordX1(l);
    double y1l = linha_getCoordY1(l);
    if (x1l >= xr && x1l <= (xr + wr) && y1l >= yr && y1l <= (yr + hr)) return true;

    return false;
}

static bool colisao_retPoly(Retangulo r, Poligono p) 
{
    double x = retangulo_getCoordX(r);
    double y = retangulo_getCoordY(r);
    double w = retangulo_getWidth(r);
    double h = retangulo_getHeight(r);

    // Verifica se os vértices do retângulo estão dentro do polígono
    Ponto cantos[4];
    cantos[0] = ponto_create(x, y);         // infEsq
    cantos[1] = ponto_create(x + w, y);     // supEsq
    cantos[2] = ponto_create(x + w, y + h); // supDir
    cantos[3] = ponto_create(x, y + h);     // infDir

    bool algumCantoDentro = false;
    for (int i = 0; i < 4; i++) {
        if (poligono_isInside(p, cantos[i])) {
            algumCantoDentro = true;
            break;
        }
    }

    // Libera memória dos cantos
    for (int i = 0; i < 4; i++) ponto_destroy(cantos[i]);

    if (algumCantoDentro) return true;

    // Verifica se algum vértice do polígono está dentro do retângulo
    int n_vertices = poligono_getVerticeCount(p);
    for (int i = 0; i < n_vertices; i++) {
        Ponto v = poligono_getVertice(p, i);
        bool inside = ponto_dentro_retangulo(r, v);
        ponto_destroy(v);
        if (inside) return true;
    }

    // Verifica intersecção de arestas
    Lista *bordas_poligonos = poligono_getSegments(p);
    bool interseccao = false;

    for (int i = 0; i < lista_getSize(bordas_poligonos); i++) {
        Linha borda = lista_getItem(bordas_poligonos, i);
        if (colisaoLinhaRet(borda, r)) {
            interseccao = true;
            break;
        }
    }

    lista_passthrough(bordas_poligonos, helper_destruir_linha, NULL);
    lista_destroy(bordas_poligonos);

    return interseccao;
}

static bool colisao_circPoly(Circulo c, Poligono p) 
{
    double cx = circulo_getCoordX(c);
    double cy = circulo_getCoordY(c);

    Ponto temp_c = ponto_create(cx, cy);

    // Centro do círculo dentro do polígono
    if (poligono_isInside(p, temp_c)) {
        ponto_destroy(temp_c);
        return true;
    }

    // Algum vértice do polígono dentro do círculo
    int n_vertices = poligono_getVerticeCount(p);
    for (int i = 0; i < n_vertices; i++) {
        Ponto v = poligono_getVertice(p, i);
        bool inside = ponto_dentro_circulo(c, v);
        ponto_destroy(v);
        if (inside) {
            ponto_destroy(temp_c);
            return true;
        }
    }

    // Intersecção de arestas
    Lista *bordas_poligono = poligono_getSegments(p);
    bool intersecao = false;

    for (int i = 0; i < lista_getSize(bordas_poligono); i++) {
        Linha borda = lista_getItem(bordas_poligono, i);
        if (sobrepoe_circulo_linha_texto(c, borda)) {
            intersecao = true;
            break;
        }
    }

    lista_passthrough(bordas_poligono, helper_destruir_linha, NULL);
    lista_destroy(bordas_poligono);
    ponto_destroy(temp_c);

    return intersecao;
}

static bool sobrepoe_anteparo_linha(Anteparo l1, Linha l2) {
    // [CORREÇÃO]: Usar variáveis locais para evitar chamadas repetidas que alocam memória
    Ponto p0 = anteparo_getP1(l1);
    Ponto p1 = anteparo_getP2(l1);

    double p2_x = linha_getCoordX1(l2);
    double p2_y = linha_getCoordY1(l2);
    double p3_x = linha_getCoordX2(l2);
    double p3_y = linha_getCoordY2(l2);

    Ponto p2 = ponto_create(p2_x, p2_y);
    Ponto p3 = ponto_create(p3_x, p3_y);

    // [CORREÇÃO]: Usar double em vez de int para não perder precisão
    double o1 = geometria_prodVet(p0, p1, p2);
    double o2 = geometria_prodVet(p0, p1, p3);
    double o3 = geometria_prodVet(p2, p3, p0);
    double o4 = geometria_prodVet(p2, p3, p1);

    bool colidiu = false;

    // Caso Geral: Sinais opostos (Cruzamento estrito)
    if (((o1 > EPSILON && o2 < -EPSILON) || (o1 < -EPSILON && o2 > EPSILON)) &&
        ((o3 > EPSILON && o4 < -EPSILON) || (o3 < -EPSILON && o4 > EPSILON))) {
        colidiu = true;
    }
    // Casos Especiais (Toque ou Colinearidade)
    else if (fabs(o1) < EPSILON && geometria_isPointOnSeg(p0, p2, p1)) colidiu = true;
    else if (fabs(o2) < EPSILON && geometria_isPointOnSeg(p0, p3, p1)) colidiu = true;
    else if (fabs(o3) < EPSILON && geometria_isPointOnSeg(p2, p0, p3)) colidiu = true;
    else if (fabs(o4) < EPSILON && geometria_isPointOnSeg(p2, p1, p3)) colidiu = true;

    ponto_destroy(p0);
    ponto_destroy(p1);
    ponto_destroy(p2);
    ponto_destroy(p3);
    
    return colidiu;
}

static bool colisao_antePoly(Anteparo pp, Poligono p) 
{
    // [CORREÇÃO CRÍTICA]: Evitar Memory Leak (recuperar, testar, destruir)
    Ponto p1 = anteparo_getP1(pp);
    if (poligono_isInside(p, p1)) {
        ponto_destroy(p1);
        return true;
    }
    ponto_destroy(p1);

    Ponto p2 = anteparo_getP2(pp);
    if (poligono_isInside(p, p2)) {
        ponto_destroy(p2);
        return true;
    }
    ponto_destroy(p2);

    Lista *bordas_poligono = poligono_getSegments(p);
    bool intersecao = false;

    for (int i = 0; i < lista_getSize(bordas_poligono); i++) {
        Linha borda = lista_getItem(bordas_poligono, i);
        if (sobrepoe_anteparo_linha(pp, borda)) {
            intersecao = true;
            break;
        }
    }

    lista_passthrough(bordas_poligono, helper_destruir_linha, NULL);
    lista_destroy(bordas_poligono);

    return intersecao;
}

static bool colisao_linhaPoly(Linha l, Poligono p) 
{
    Ponto temp_l1 = ponto_create(linha_getCoordX1(l), linha_getCoordY1(l));
    Ponto temp_l2 = ponto_create(linha_getCoordX2(l), linha_getCoordY2(l));

    if (poligono_isInside(p, temp_l1)) {
        ponto_destroy(temp_l1);
        ponto_destroy(temp_l2);
        return true;
    }
    if (poligono_isInside(p, temp_l2)) {
        ponto_destroy(temp_l1);
        ponto_destroy(temp_l2);
        return true;
    }

    Lista *bordas_poligono = poligono_getSegments(p);
    bool intersecao = false;

    for (int i = 0; i < lista_getSize(bordas_poligono); i++) {
        Linha borda = lista_getItem(bordas_poligono, i);
        if (colisaoLinhaLinha(l, borda)) {
            intersecao = true;
            break;
        }
    }

    lista_passthrough(bordas_poligono, helper_destruir_linha, NULL);
    lista_destroy(bordas_poligono);

    ponto_destroy(temp_l1);
    ponto_destroy(temp_l2);
    return intersecao;
}

static bool colisao_txtoPoly(Texto t, Poligono p) {
    Linha temp_linha = conversaoTxtoLinha(t);
    bool resultado = colisao_linhaPoly(temp_linha, p);
    linha_destroy(temp_linha);
    return resultado;
}

// --- Função Pública ---

bool checarColisao(forma f, Poligono p) {
    if (f == NULL || p == NULL) return false;

    int type = forma_getType(f);

    switch (type) {
        case TIPO_C: return colisao_circPoly((Circulo)f, p);
        case TIPO_R: return colisao_retPoly((Retangulo)f, p);
        case TIPO_L: return colisao_linhaPoly((Linha)f, p);
        case TIPO_T: return colisao_txtoPoly((Texto)f, p);
        case TIPO_A: return colisao_antePoly((Anteparo)f, p);
        default:
            printf("Erro: tipo %d desconhecido em checarColisao.\n", type);
            // Opcional: retornar false em vez de exit, dependendo da robustez desejada
            return false;
    }
}