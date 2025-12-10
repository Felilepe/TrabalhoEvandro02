#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "colisoes.h"
#include "formas.h"
#include "circulo.h"
#include "retangulo.h"
#include "texto.h"
#include "linha.h"

#define TIPO_C 1
#define TIPO_R 2
#define TIPO_L 3
#define TIPO_T 4


bool checarColisao(forma f1, forma f2)
{
    int f1_type = forma_getType(f1);
    int f2_type = forma_getType(f2);

    
    switch(f1_type){
        case(TIPO_C):{
            switch(f2_type){
                case(TIPO_C): return colisaoCircCirc((Circulo)f1, (Circulo)f2); break;
                case(TIPO_R): return colisaoCircRet((Circulo)f1, (Retangulo)f2); break;
                case(TIPO_L): return colisaoCircLinha((Circulo)f1, (Linha)f2); break;
                case(TIPO_T): return colisaoCircTxto((Circulo)f1, (Texto)f2); break;
                default: return false; break;
            }
        }; break;
        case(TIPO_R):{
            switch(f2_type){
                case(TIPO_C): return colisaoCircRet((Circulo)f1, (Retangulo)f2); break;
                case(TIPO_R): return colisaoRetRet((Retangulo)f1, (Retangulo)f2); break;
                case(TIPO_L): return colisaoLinhaRet((Linha)f1, (Retangulo)f2); break;
                case(TIPO_T): return colisaoRetTxto((Retangulo)f1, (Texto)f2); break;
                default: return false;   
            }
        }; break;
        case(TIPO_L):{
            switch(f2_type){
                case(TIPO_C): return colisaoCircLinha((Circulo)f1, (Linha)f2); break;
                case(TIPO_R): return colisaoLinhaRet((Linha)f1, (Retangulo)f2); break;
                case(TIPO_L): return colisaoLinhaLinha((Linha)f1, (Linha)f2); break;
                case(TIPO_T): return colisaoLinhaTxto((Linha)f1, (Texto)f2); break;
                default: return false; break;
            }
        }; break;
        case(TIPO_T):{
            switch(f2_type){
                case(TIPO_C): return colisaoCircTxto((Circulo)f1, (Texto)f2); break;
                case(TIPO_R): return colisaoRetTxto((Retangulo)f1, (Texto)f2); break;
                case(TIPO_L): return colisaoLinhaTxto((Linha)f1, (Texto)f2); break;
                case(TIPO_T): return colisaoTxtoTxto((Texto)f1, (Texto)f2); break;
                default: return false; break;
            }
        }; break;
        default: return false; break;
    }
}



bool colisaoCircCirc(Circulo c1, Circulo c2)
{
    double xc1 = circulo_getCoordX(c1);
    double yc1 = circulo_getCoordY(c1);
    double rc1 = circulo_getRaio(c1);
    double xc2 = circulo_getCoordX(c2);
    double yc2 = circulo_getCoordY(c2);
    double rc2 = circulo_getRaio(c2);
    
    double dist = sqrt(pow((xc1-xc2),2) + pow((yc1-yc2),2));

    return dist <= (rc1 + rc2);
}

bool colisaoRetRet(Retangulo r1, Retangulo r2)
{
    double xr1 = retangulo_getCoordX(r1);
    double yr1 = retangulo_getCoordY(r1); 
    double wr1 = retangulo_getWidth(r1); 
    double hr1 = retangulo_getHeight(r1); 
    double xr2 = retangulo_getCoordX(r2); 
    double yr2 = retangulo_getCoordY(r2); 
    double wr2 = retangulo_getWidth(r2); 
    double hr2 = retangulo_getHeight(r2);
    
    return !(xr1 > xr2 + wr2 || xr1 + wr1 < xr2 || yr1 > yr2 + hr2 || yr1 + hr1 < yr2); 
}

bool colisaoCircRet(Circulo c, Retangulo r)
{
    double xc = circulo_getCoordX(c);
    double yc = circulo_getCoordY(c);
    double rc = circulo_getRaio(c);
    double xr = retangulo_getCoordX(r);
    double yr = retangulo_getCoordY(r); 
    double wr = retangulo_getWidth(r);
    double hr = retangulo_getHeight(r);  

    double ponto_proxX = fmax(xr, xc);

    ponto_proxX = fmin(ponto_proxX, xr + wr);


    double ponto_proxY = fmax(yr, yc);

    ponto_proxY = fmin(ponto_proxY, yr + hr);

    double dist = sqrt(pow((xc-ponto_proxX),2) + pow((yc-ponto_proxY),2));

    return dist <= rc;
}

bool colisaoLinhaLinha(Linha l1, Linha l2)
{
    double x1l1 = linha_getCoordX1(l1);
    double x2l1 = linha_getCoordX2(l1);
    double y1l1 = linha_getCoordY1(l1);
    double y2l1 = linha_getCoordY2(l1);
    double x1l2 = linha_getCoordX1(l2);
    double x2l2 = linha_getCoordX2(l2);
    double y1l2 = linha_getCoordY1(l2);
    double y2l2 = linha_getCoordY2(l2);

    
    double D = (x1l1 - x2l1) * ( y1l2 - y2l2) - (y1l1 - y2l1) * (x1l2 - x2l2);

    if(!D) return false;

    double t = ((x1l1 - x1l2) * ( y1l2 - y2l2) - (y1l1 - y1l2) * (x1l2 - x2l2)) / D;

    double u = -(((x1l1 - x2l1) * (y1l1 - y1l2) - (y1l1 - y2l1) * (x1l1 - x1l2)) / D);

    return((t >= 0 && t <= 1) && (u >= 0 && u <= 1));
}

bool colisaoCircLinha(Circulo c, Linha l)
{
    double xc = circulo_getCoordX(c);
    double yc = circulo_getCoordY(c);
    double rc = circulo_getRaio(c);
    double x1l = linha_getCoordX1(l);
    double x2l = linha_getCoordX2(l);
    double y1l = linha_getCoordY1(l);
    double y2l = linha_getCoordY2(l);
    
    double deltaX = x2l - x1l;
    double deltaY = y2l - y1l;
    
    double linha_lenght = (deltaX * deltaX) + (deltaY * deltaY);
    
    if(!linha_lenght){
        double deltaX_ponto = x1l - xc;
        double deltaY_ponto = y1l - yc;
        double dist_sq_ponto = (deltaX_ponto * deltaX_ponto) + (deltaY_ponto * deltaY_ponto);
        return dist_sq_ponto <= (rc * rc);
    }


    double t = ((xc - x1l) * deltaX + (yc - y1l) * deltaY) / linha_lenght;


    t = fmax(0, t);
    t = fmin(1, t);


    double ponto_proxX = x1l + t * deltaX;
    double ponto_proxY = y1l + t * deltaY;


    return (pow((xc -ponto_proxX), 2) + pow((yc - ponto_proxY), 2)) <= (rc * rc);
}

bool colisaoCircTxto(Circulo c, Texto t)
{
    Linha temp = conversaoTxtoLinha(t);

    bool colisao  = colisaoCircLinha(c, temp);

    linha_destroy(temp);

    return colisao;
}


bool colisaoLinhaRet(Linha l, Retangulo r)
{
    double xr = retangulo_getCoordX(r);
    double yr = retangulo_getCoordY(r); 
    double wr = retangulo_getWidth(r);
    double hr = retangulo_getHeight(r); 
    
    Linha topo_temp = linha_create(-1, xr, yr, (xr + wr), yr, "", false);
    bool colisao_topo = colisaoLinhaLinha(l, topo_temp);
    
    Linha esq_temp = linha_create(-1, xr, yr, xr, (yr + hr), "", false);
    bool colisao_esq = colisaoLinhaLinha(l, esq_temp);
    
    Linha dir_temp = linha_create(-1, (xr + wr), yr, (xr + wr), (yr + hr), "", false);
    bool colisao_dir = colisaoLinhaLinha(l, dir_temp);
    
    Linha chao_temp = linha_create(-1, xr, (yr + hr), (xr + wr), (yr + hr), "", false);
    bool colisao_chao = colisaoLinhaLinha(l, chao_temp);
    
    linha_destroy(topo_temp);
    linha_destroy(esq_temp);
    linha_destroy(dir_temp);
    linha_destroy(chao_temp);
    
    double x1l = linha_getCoordX1(l), y1l = linha_getCoordY1(l);
    if(x1l >= xr && x1l <= (xr + wr) && y1l >= yr && y1l <= (yr + hr)) return true;
    
    
    return (colisao_chao || colisao_dir || colisao_esq || colisao_topo);
}

bool colisaoLinhaTxto(Linha l, Texto t)
{
    Linha temp = conversaoTxtoLinha(t);
    
    bool colisao = colisaoLinhaLinha(l, temp);

    linha_destroy(temp);

    return colisao;
}

bool colisaoRetTxto(Retangulo r, Texto t)
{
    Linha temp = conversaoTxtoLinha(t);
    bool colisao = colisaoLinhaRet(temp, r);

    linha_destroy(temp);

    return colisao;
}

bool colisaoTxtoTxto(Texto t1, Texto t2)
{
    Linha temp1 = conversaoTxtoLinha(t1), temp2 = conversaoTxtoLinha(t2);
    bool colisao = colisaoLinhaLinha(temp1, temp2);

    linha_destroy(temp1);
    linha_destroy(temp2);

    return colisao;
}