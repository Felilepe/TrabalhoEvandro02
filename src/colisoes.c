#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "colisoes.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "formas.h"
#include "poligono.h"
#include "geometria.h"
#include "lista.h"

#include <math.h>
#define EPSILON 1e-10

static bool colisao_circPoli(Circulo c, Poligono p);
static bool colisao_retPoli(Retangulo r, Poligono p);
static bool colisao_linhaPoli(Linha l, Poligono p);
static bool colisao_txtoPoli(Texto t, Poligono p);
static bool colisao_antePoli(Anteparo pp, Poligono p);

bool colisao_formaPoli(forma f, Poligono p) 
{
    if (f == NULL || p == NULL) return false;

    int tipo = forma_getType(f);

    switch (tipo) {
        case TIPO_C: return colisao_circPoli((Circulo)f, p);
        case TIPO_R: return colisao_retPoli((Retangulo)f, p);
        case TIPO_L: return colisao_linhaPoli((Linha)f, p);
        case TIPO_T: return colisao_txtoPoli((Texto)f, p);
        case TIPO_A: return colisao_antePoli((Anteparo)f, p);
        default: return false;
    }
}

static bool ponto_dentro_retangulo(Retangulo r, double px, double py) 
{
    double rx = retangulo_getCoordX(r);
    double ry = retangulo_getCoordY(r);
    double rw = retangulo_getWidth(r);
    double rh = retangulo_getHeight(r);

    return (px >= rx) && (px <= rx + rw) &&
           (py >= ry) && (py <= ry + rh);
}

static bool ponto_dentro_circulo(Circulo c, double px, double py) 
{
    double cx = circulo_getCoordX(c);
    double cy = circulo_getCoordY(c);
    double r = circulo_getRaio(c);

    return geometria_distSqrd(cx, cy, px, py) <= (r * r);
}

static bool sobrepoe_circulo_linha_texto(Circulo c, Linha l) 
{
    double cx = circulo_getCoordX(c);
    double cy = circulo_getCoordY(c);
    double cr = circulo_getRaio(c);

    double lx1 = linha_getCoordX1(l);
    double lx2 = linha_getCoordX2(l);
    double ly1 = linha_getCoordY1(l);
    double ly2 = linha_getCoordY2(l);

    double distP1Linha = geometria_distSqrd(cx, cy, lx1, ly1);
    double distP2Linha = geometria_distSqrd(cx, cy, lx2, ly2);
    double raioAoQuadrado = cr * cr;

    if (distP1Linha <= raioAoQuadrado || distP2Linha <= raioAoQuadrado) {
        return true;
    }

    double comprimentoAoQuadrado = geometria_distSqrd(lx1, ly1, lx2, ly2);

    if (comprimentoAoQuadrado == 0.0) {
        return distP1Linha <= raioAoQuadrado;
    }

    double t = ((cx - lx1) * (lx2 - lx1) + (cy - ly1) * (ly2 - ly1)) / comprimentoAoQuadrado;

    t = fmax(0.0, fmin(1.0, t));

    double px = lx1 + t * (lx2 - lx1);
    double py = ly1 + t * (ly2 - ly1);
    double distanciaAoQuadradoDoCirculo = geometria_distSqrd(cx, cy, px, py);

    return distanciaAoQuadradoDoCirculo <= raioAoQuadrado;
}

static bool sobrepoe_linha_linha(Linha l1, Linha l2) 
{
    double l1x1 = linha_getCoordX1(l1);
    double l1y1 = linha_getCoordY1(l1);
    double l1x2 = linha_getCoordX2(l1);
    double l1y2 = linha_getCoordY2(l1);

    double l2x1 = linha_getCoordX1(l2);
    double l2y1 = linha_getCoordY1(l2);
    double l2x2 = linha_getCoordX2(l2);
    double l2y2 = linha_getCoordY2(l2);

    int o1 = geometria_prodVet(l1x1, l1y1, l1x2, l1y2, l2x1, l2y1);
    int o2 = geometria_prodVet(l1x1, l1y1, l1x2, l1y2, l2x2, l2y2);
    int o3 = geometria_prodVet(l2x1, l2y1, l2x2, l2y2, l1x1, l1y1);
    int o4 = geometria_prodVet(l2x1, l2y1, l2x2, l2y2, l1x2, l1y2);

    if (o1 != o2 && o3 != o4) {
        return true;
    }

    if (o1 == 0 && geometria_isPointInSeg(l1x1, l1y1, l2x1, l2y1, l1x2, l1y2)) return true;
    if (o2 == 0 && geometria_isPointInSeg(l1x1, l1y1, l2x2, l2y2, l1x2, l1y2)) return true;
    if (o3 == 0 && geometria_isPointInSeg(l2x1, l2y1, l1x1, l1y1, l2x2, l2y2)) return true;
    if (o4 == 0 && geometria_isPointInSeg(l2x1, l2y1, l1x2, l1y2, l2x2, l2y2)) return true;

    return false;
}

static bool colisao_retLinha(Retangulo r, Linha l) 
{
    double lx1 = linha_getCoordX1(l);
    double ly1 = linha_getCoordY1(l);
    double lx2 = linha_getCoordX2(l);
    double ly2 = linha_getCoordY2(l);

    double rx = retangulo_getCoordX(r);
    double ry = retangulo_getCoordY(r);
    double rw = retangulo_getWidth(r);
    double rh = retangulo_getHeight(r);

    double x_min = rx;
    double x_max = rx + rw;
    double y_min = ry;
    double y_max = ry + rh;

    bool p1_dentro = (lx1 >= x_min - EPSILON && lx1 <= x_max + EPSILON &&
                      ly1 >= y_min - EPSILON && ly1 <= y_max + EPSILON);
    bool p2_dentro = (lx2 >= x_min - EPSILON && lx2 <= x_max + EPSILON &&
                      ly2 >= y_min - EPSILON && ly2 <= y_max + EPSILON);

    if (p1_dentro || p2_dentro) {
        return true;
    }

    Linha borda_cima = linha_create(-1, x_min, y_min, x_max, y_min, "temp", false);
    Linha borda_direita = linha_create(-1, x_max, y_min, x_max, y_max, "temp", false);
    Linha borda_baixo = linha_create(-1, x_max, y_max, x_min, y_max, "temp", false);
    Linha borda_esquerda = linha_create(-1, x_min, y_max, x_min, y_min, "temp", false);

    bool resultado = (sobrepoe_linha_linha(l, borda_cima) ||
                      sobrepoe_linha_linha(l, borda_direita) ||
                      sobrepoe_linha_linha(l, borda_baixo) ||
                      sobrepoe_linha_linha(l, borda_esquerda));

    linha_destroy(borda_cima);
    linha_destroy(borda_direita);
    linha_destroy(borda_baixo);
    linha_destroy(borda_esquerda);

    return resultado;
}

static bool colisao_retPoli(Retangulo r, Poligono p) 
{
    double x = retangulo_getCoordX(r);
    double y = retangulo_getCoordY(r);
    double w = retangulo_getWidth(r);
    double h = retangulo_getHeight(r);

    if (poligono_isInside(p, x, y) || 
        poligono_isInside(p, x + w, y) || 
        poligono_isInside(p, x + w, y +h) || 
        poligono_isInside(p, x, y +h)) {
        return true;
    }

    int n_vertices = poligono_getNumVertices(p);
    for (int i = 0; i < n_vertices; i++) {
        double xp = poligono_getXVertice(p, i);
        double yp = poligono_getYVertice(p, i);
        
        if (ponto_dentro_retangulo(r, xp, yp)) {
            return true;
        }
    }

    Lista *bordas_poligonos = poligono_getSegmentos(p);
    int n_bordas = lista_getSize(bordas_poligonos);
    bool interseccao = false;
    
    for (int j = 0; j < n_bordas; j++) {
        Linha borda = (Linha)lista_remove_inicio(bordas_poligonos);
        
        if (!interseccao && colisao_retLinha(r, borda)) {
            interseccao = true;
        }
        
        linha_destroy(borda);
    }
    
    lista_destroy(bordas_poligonos);
    return interseccao;
}

static bool colisao_circPoli(Circulo c, Poligono p) 
{
    double cx = circulo_getCoordX(c);
    double cy = circulo_getCoordY(c);

    if (poligono_isInside(p, cx, cy)) {
        return true;
    }

    int n_vertices = poligono_getNumVertices(p);
    for (int i = 0; i < n_vertices; i++) {
        double xp = poligono_getXVertice(p, i);
        double yp = poligono_getYVertice(p, i);

        if (ponto_dentro_circulo(c, xp, yp)) {
            return true;
        }
    }

    Lista *bordas_poligono = poligono_getSegmentos(p);
    int n_bordas = lista_getSize(bordas_poligono);
    bool intersecao = false;

    for (int j = 0; j < n_bordas; j++) {
        Linha borda = (Linha)lista_remove_inicio(bordas_poligono);

        if (!intersecao && sobrepoe_circulo_linha_texto(c, borda)) {
            intersecao = true;
        }
        
        /* CORREÇÃO CRÍTICA: Destruir a linha */
        linha_destroy(borda);
    }

    lista_destroy(bordas_poligono);
    return intersecao;
}

static bool colisao_anteLinha(Anteparo l1, Linha l2) 
{
    double p0_x = anteparo_getX1(l1);
    double p0_y = anteparo_getY1(l1);
    double p1_x = anteparo_getX2(l1);
    double p1_y = anteparo_getY2(l1);

    double p2_x = linha_getCoordX1(l2);
    double p2_y = linha_getCoordY1(l2);
    double p3_x = linha_getCoordX2(l2);
    double p3_y = linha_getCoordY2(l2);

    int o1 = geometria_prodVet(p0_x, p0_y, p1_x, p1_y, p2_x, p2_y);
    int o2 = geometria_prodVet(p0_x, p0_y, p1_x, p1_y, p3_x, p3_y);
    int o3 = geometria_prodVet(p2_x, p2_y, p3_x, p3_y, p0_x, p0_y);
    int o4 = geometria_prodVet(p2_x, p2_y, p3_x, p3_y, p1_x, p1_y);

    if (o1 != o2 && o3 != o4) {
        return true;
    }

    if (o1 == 0 && geometria_isPointInSeg(p0_x, p0_y, p2_x, p2_y, p1_x, p1_y)) return true;
    if (o2 == 0 && geometria_isPointInSeg(p0_x, p0_y, p3_x, p3_y, p1_x, p1_y)) return true;
    if (o3 == 0 && geometria_isPointInSeg(p2_x, p2_y, p0_x, p0_y, p3_x, p3_y)) return true;
    if (o4 == 0 && geometria_isPointInSeg(p2_x, p2_y, p1_x, p1_y, p3_x, p3_y)) return true;

    return false;
}

static bool colisao_antePoli(Anteparo pp, Poligono p) 
{
    if (poligono_isInside(p, anteparo_getX1(pp), anteparo_getY1(pp))) {
        return true;
    }
    if (poligono_isInside(p, anteparo_getX2(pp), anteparo_getY2(pp))) {
        return true;
    }

    Lista *bordas_poligono = poligono_getSegmentos(p);
    int n_bordas = lista_getSize(bordas_poligono);
    bool intersecao = false;

    for (int j = 0; j < n_bordas; j++) {
        Linha borda = (Linha)lista_remove_inicio(bordas_poligono);

        if (!intersecao && colisao_anteLinha(pp, borda)) {
            intersecao = true;
        }
        
        linha_destroy(borda);
    }

    lista_destroy(bordas_poligono);
    return intersecao;
}

static bool colisao_linhaPoli(Linha l, Poligono p) 
{
    if (poligono_isInside(p, linha_getCoordX1(l), linha_getCoordY1(l))) {
        return true;
    }
    if (poligono_isInside(p, linha_getCoordX2(l), linha_getCoordY2(l))) {
        return true;
    }

    Lista *bordas_poligono = poligono_getSegmentos(p);
    int n_bordas = lista_getSize(bordas_poligono);
    bool intersecao = false;

    for (int j = 0; j < n_bordas; j++) {
        Linha borda = (Linha)lista_remove_inicio(bordas_poligono);

        if (!intersecao && sobrepoe_linha_linha(l, borda)) {
            intersecao = true;
        }
        
        linha_destroy(borda);
    }

    lista_destroy(bordas_poligono);
    return intersecao;
}

static bool colisao_txtoPoli(Texto t, Poligono p) {
    Linha temp_linha = conversaoTxtoLinha(t);

    bool resultado = colisao_linhaPoli(temp_linha, p);

    linha_destroy(temp_linha);
    return resultado;
}