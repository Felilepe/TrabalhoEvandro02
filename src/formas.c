#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "circulo.h"
#include "linha.h"
#include "retangulo.h"
#include "texto.h"
#include "formas.h"
#include "anteparo.h"


typedef struct forma_g
{
    int id;   
    int type; 
} FormaG;




void forma_destroy(forma f)
{
    FormaG* forma_generica = (FormaG*)f;
    if(forma_generica == NULL) return;

    switch(forma_generica -> type){
        case(TIPO_C): circulo_destroy((Circulo)f); break;
        case(TIPO_R): retangulo_destroy((Retangulo)f); break;
        case(TIPO_L): linha_destroy((Linha)f); break;
        case(TIPO_T): texto_destroy((Texto)f); break;
        case(TIPO_A): anteparo_destroy((Anteparo)f); break;
        default: printf("Erro: tipo de forma invalido em forma_destroy."); exit(1); break;
    }
}



int forma_getType(forma f)
{
    FormaG* forma_generica = (FormaG*)f;
    if(forma_generica == NULL){
        printf("Erro: forma no forma_getType é nula.");
        exit(1);
    }
    int type;


    switch(forma_generica -> type){
        case(TIPO_C):type = circulo_getType((Circulo)f); break;
        case(TIPO_R):type = retangulo_getType((Retangulo)f); break;
        case(TIPO_L):type = linha_getType((Linha)f); break;
        case(TIPO_T):type = texto_getType((Texto)f); break;
        case(TIPO_A):type = anteparo_getType((Anteparo)f); break; 
        default: printf("Erro: tipo de forma invalido no forma_getType. Tipo %d", forma_generica -> type); exit(1); break;
    }
    return type;
}


double forma_calcArea(forma f)
{
    FormaG* forma_generica = (FormaG*)f;
    double area;
    
    switch(forma_generica -> type){
        case(TIPO_C): area = circulo_calcArea((Circulo)f); break;
        case(TIPO_R): area = retangulo_calcArea((Retangulo)f); break;
        case(TIPO_L): area = linha_calcArea((Linha)f); break; 
        case(TIPO_T): area = texto_calcArea((Texto)f); break;
        case(TIPO_A):
        default: printf("Erro: tipo de forma invalido no forma_calcArea."); exit(1); break;
    }
    return area;
}

void forma_trocarCores(forma f)
{
    if (f == NULL) return;
    char* corB = forma_getCorBorda(f);
    char* corP = forma_getCorPreench(f);
    if (corB == NULL || corP == NULL) return;

    size_t len = strlen(corB) + 1;
    char* buffer = (char*)malloc(len);
    if (buffer == NULL) {
        printf("Falha na alocacao de memoria em forma_trocarCores.\n");
        return;
    }

    memcpy(buffer, corB, len);

    forma_setCorBorda(f, corP);
    forma_setCorPreench(f, buffer);
    free(buffer);
}

void forma_trocarCoresEntreFormas(forma f1, forma f2)
{
    if (f1 == NULL || f2 == NULL) return;
    char* corP1 = forma_getCorPreench(f1);
    char* corB2 = forma_getCorBorda(f2);
    if (corP1 == NULL || corB2 == NULL) return;

    size_t len2 = strlen(corB2) + 1;
    char* buffer = (char*)malloc(len2);
    if (buffer == NULL) {
        printf("Falha na alocacao de memoria em forma_trocaCoresEntreFormas.\n");
        return;
    }

    memcpy(buffer, corB2, len2);

    forma_setCorBorda(f2, corP1);
    forma_setCorPreench(f1, buffer);
    free(buffer);
}

void forma_exportarDados(forma f, FILE *file_name, char* report_QRY)
{
    int tipo = forma_getType(f);
    Circulo c = (Circulo)f;
    Retangulo r = (Retangulo)f;
    Linha l = (Linha)f;
    Texto t = (Texto)f;

    const char* report_safe = (report_QRY != NULL) ? report_QRY : "";

    switch(tipo){
        case(TIPO_C): fprintf(file_name, "%s\n Círculo\n ID: %i\n Âncora em: (%.2lf, %.2lf)\n Raio: %lf\n Cor de borda: %s\n Cor de preenchimento: %s\n",
            report_safe, circulo_getID(c), circulo_getCoordX(c), circulo_getCoordY(c), circulo_getRaio(c), circulo_getCorBorda(c),
            circulo_getCorPreench(c)); break;

        case(TIPO_R): fprintf(file_name, "%s\n Retângulo\n ID: %i\n Âncora em: (%.2lf, %.2lf)\n Altura: %lf\n Largura: %lf\n Cor de borda: %s\n Cor de preenchimento: %s\n",
            report_safe, retangulo_getID(r), retangulo_getCoordX(r), retangulo_getCoordY(r), retangulo_getHeight(r), retangulo_getWidth(r), retangulo_getCorBorda(r),
            retangulo_getCorPreench(r)); break; 

        case(TIPO_L): fprintf(file_name, "%s\n Linha\n ID: %i\n Âncora de início em: (%.2lf, %.2lf)\n Âncora de fim em: (%.2lf, %.2lf)\n Cor: %s\n",
            report_safe, linha_getID(l), linha_getCoordX1(l), linha_getCoordY1(l), linha_getCoordX2(l), linha_getCoordY2(l), linha_getCor(l)); break; 

        case(TIPO_T): fprintf(file_name, "%s Texto\n ID: %d\n Âncora em: (%.2f, %.2f)\n Posição da Âncora: %c\n Conteúdo: \"%s\"\n Cor de borda: %s\n Cor de preenchimento: %s\n",
            report_safe, texto_getID(t), texto_getCoordX(t), texto_getCoordY(t), texto_getAnchor(t), texto_getTexto(t), texto_getCorBorda(t), 
            texto_getCorPreench(t)); 
            fprintf(file_name, " Família da fonte: %s\n Peso da fonte: %s\n Tamanho da fonte: %s\n\n",
            texto_getFamily(t), texto_getWeight(t), texto_getSize(t)); break;

        case(TIPO_A): fprintf(file_name, "%s\n Anteparo\n ID: %i\n Ponto 1 em: (%.2lf, %.2lf)\n Ponto 2 em: (%.2lf, %.2lf)\n Cor: %s\n",
            report_safe, anteparo_getID((Anteparo)f), anteparo_getCoordX1((Anteparo)f), anteparo_getCoordY1((Anteparo)f),
            anteparo_getCoordX2((Anteparo)f), anteparo_getCoordY2((Anteparo)f), anteparo_getCor((Anteparo)f)); break;

        default: fprintf(file_name, "Tipo de forma desconhecido no forma_exportarDados."); break;
    }
}

forma forma_clonar(forma f) {
    if (f == NULL) return NULL;

    static int max_id = 10000;
    int novo_id = ++max_id;
    int tipo = forma_getType(f); 
    
    forma novo_clone = NULL;

    switch (tipo) {
        case TIPO_C: {
            novo_clone = (forma)circulo_create(novo_id,
                circulo_getCoordX((Circulo)f),
                circulo_getCoordY((Circulo)f),
                circulo_getRaio((Circulo)f),      // Precisa de circulo.h
                circulo_getCorBorda((Circulo)f),
                circulo_getCorPreench((Circulo)f)
            );
            break;
        }
        case TIPO_R: {
            novo_clone = (forma)retangulo_create(novo_id,
                retangulo_getCoordX((Retangulo)f),
                retangulo_getCoordY((Retangulo)f),
                retangulo_getWidth((Retangulo)f),   // Precisa de retangulo.h
                retangulo_getHeight((Retangulo)f),  // Precisa de retangulo.h
                retangulo_getCorBorda((Retangulo)f),
                retangulo_getCorPreench((Retangulo)f)
            );
            break;
        }
        case TIPO_L: {
            novo_clone = (forma)linha_create(novo_id,
                linha_getCoordX1((Linha)f), 
                linha_getCoordY1((Linha)f), 
                linha_getCoordX2((Linha)f),
                linha_getCoordY2((Linha)f), 
                linha_getCor((Linha)f),
                linha_getIsDotted((Linha)f) 
            );
            break;
        }
        case TIPO_T: {
           
            novo_clone = (forma)texto_create(novo_id,
                texto_getCoordX((Texto)f),
                texto_getCoordY((Texto)f),
                texto_getCorBorda((Texto)f),
                texto_getCorPreench((Texto)f),
                texto_getAnchor((Texto)f), 
                texto_getTexto((Texto)f)   
            );

       
            if (novo_clone != NULL) {
                texto_setFamily(novo_clone, texto_getFamily((Texto)f));
                texto_setWeight(novo_clone, texto_getWeight((Texto)f));
                texto_setSize(novo_clone, texto_getSize((Texto)f));
            }
            break;
        }
        case TIPO_A: {
            novo_clone = (forma)anteparo_create(novo_id,
                anteparo_getCoordX1((Anteparo)f),
                anteparo_getCoordY1((Anteparo)f),
                anteparo_getCoordX2((Anteparo)f),
                anteparo_getCoordY2((Anteparo)f),
                anteparo_getCor((Anteparo)f)
            );
            break;
        }
        default:
            printf("Erro: tipo de forma invalido no forma_clonar.\n");
            exit(1);
    }

    
    return novo_clone;
}



int forma_getID(forma f)
{
    FormaG* forma_generica = (FormaG*)f;
    int ID;
    
    switch(forma_generica -> type){
        case(TIPO_C): ID = circulo_getID((Circulo)f); break;
        case(TIPO_R): ID = retangulo_getID((Retangulo)f); break;
        case(TIPO_L): ID = linha_getID((Linha)f); break;
        case(TIPO_T): ID = texto_getID((Texto)f); break;
        case(TIPO_A): ID = anteparo_getID((Anteparo)f); break;
        default: printf("Erro: tipo de forma invalido no forma_getID."); exit(1); break;
    }
    return  ID;
}

double forma_getCoordX(forma f) 
{
    FormaG* forma_generica = (FormaG*)f;
    double coordx;
    
    switch(forma_generica -> type){
        case(TIPO_C): coordx = circulo_getCoordX((Circulo)f); break;
        case(TIPO_R): coordx = retangulo_getCoordX((Retangulo)f); break;
        case(TIPO_L): {
            Linha l = (Linha)f;
            double x1 = linha_getCoordX1(l);
            double x2 = linha_getCoordX2(l);

            if (x1 < x2) {
                return x1; 
            } else if (x1 > x2) {
                return x2; 
            } else {

                return x1;
            }
            break;
        }
        case(TIPO_T): coordx = texto_getCoordX((Texto)f); break;
        case(TIPO_A): coordx = (anteparo_getCoordX1((Anteparo)f) < anteparo_getCoordX2((Anteparo)f)) ? anteparo_getCoordX1((Anteparo)f) : anteparo_getCoordX2((Anteparo)f); break;
        default: printf("Erro: tipo de forma invalido no forma_getCoordX."); exit(1); break;
    }
    return coordx;
}

double forma_getCoordY(forma f)
{
    FormaG* forma_generica = (FormaG*)f;
    double coordy;

    switch(forma_generica -> type){
        case(TIPO_C): coordy = circulo_getCoordY((Circulo)f); break;
        case(TIPO_R): coordy = retangulo_getCoordY((Retangulo)f); break;
        case(TIPO_L): {
            Linha l = (Linha)f;
            double x1 = linha_getCoordX1(l);
            double y1 = linha_getCoordY1(l);
            double x2 = linha_getCoordX2(l);
            double y2 = linha_getCoordY2(l);

            if (x1 < x2) {
                return y1; 
            } else if (x1 > x2) {
                return y2; 
            } else {
                if (y1 <= y2) {
                    return y1; 
                } else {
                    return y2; 
                }
            }
            break;
        }
        case(TIPO_T): coordy = texto_getCoordY((Texto)f); break;
        case(TIPO_A): coordy = (anteparo_getCoordY1((Anteparo)f) < anteparo_getCoordY2((Anteparo)f)) ? anteparo_getCoordY1((Anteparo)f) : anteparo_getCoordY2((Anteparo)f); break;  
        default: printf("Erro: tipo de forma invalido no forma_getCoordY."); exit(1); break;
    }
    return coordy;
}

char* forma_getCorBorda(forma f)
{
    FormaG* forma_generica = (FormaG*)f;
    char* corb;

    switch(forma_generica -> type){
        case(TIPO_C): corb =  circulo_getCorBorda((Circulo)f); break;
        case(TIPO_R): corb =  retangulo_getCorBorda((Retangulo)f); break;
        case(TIPO_L): corb =  linha_getCor((Linha)f); break; 
        case(TIPO_T): corb = texto_getCorBorda((Texto)f); break;
        case(TIPO_A): corb = anteparo_getCor((Anteparo)f); break;
        default: printf("Erro: tipo de forma invalido no forma_getCorBorda."); exit(1); break;
    }
    return corb;
}

char* forma_getCorPreench(forma f)
{
    FormaG* forma_generica = (FormaG*)f;
    char* corp;

    switch(forma_generica -> type){
        case(TIPO_C): corp = circulo_getCorPreench((Circulo)f); break;
        case(TIPO_R): corp = retangulo_getCorPreench((Retangulo)f); break;
        case(TIPO_L): printf("Erro: tipo de forma invalido. Para linhas, utilize 'forma_getCorBorda'"); break; 
        case(TIPO_A): printf("Erro: tipo de forma invalido. Para anteparos, utilize 'forma_getCorBorda'"); break;
        case(TIPO_T): corp = texto_getCorPreench((Texto)f); break;
        default: printf("Erro: tipo de forma invalido no forma_getCorPreench."); exit(1); break;
    }
    return corp;
}

char* forma_getCorComp(char* cor_original)
{
    int r = 0, g = 0, b = 0; // Padrão é preto

    if (cor_original != NULL && strcmp(cor_original, "none") != 0) 
    {
        const char *cor_str = (cor_original[0] == '#') ? cor_original + 1 : cor_original;
        int len = strlen(cor_str);

        if (len == 6) {
            sscanf(cor_str, "%02x%02x%02x", &r, &g, &b);
        }
        else if (len == 3) {
            int r_short, g_short, b_short;
            sscanf(cor_str, "%1x%1x%1x", &r_short, &g_short, &b_short);
            
            r = r_short * 17; 
            g = g_short * 17;
            b = b_short * 17;
        }
    }

    int comp_r = 255 - r;
    int comp_g = 255 - g;
    int comp_b = 255 - b;

    char* cor_complementar = (char*) malloc(8 * sizeof(char));
    if (cor_complementar == NULL) {
        printf("Falha na alocacao de memoria em forma_getCorComp.");
        return NULL;
    }

    sprintf(cor_complementar, "#%02x%02x%02x", comp_r, comp_g, comp_b);

    return cor_complementar;
}

void forma_setCoordX(forma f, double x)
{
    FormaG* forma_generica = (FormaG*)f;

    switch(forma_generica -> type){
        case(TIPO_C): circulo_setCoordX((Circulo)f, x); break;
        case(TIPO_R): retangulo_setCoordX((Retangulo)f, x); break;
        case(TIPO_T): texto_setCoordX((Texto)f, x); break;
        case(TIPO_L): {
            Linha l = (Linha)f;
            double x1_atual = linha_getCoordX1(l);
            double y1_atual = linha_getCoordY1(l); 
            double x2_atual = linha_getCoordX2(l);
            double y2_atual = linha_getCoordY2(l); 

            double delta_x;
            bool p1_era_ancora;

            if (x1_atual < x2_atual) {
                p1_era_ancora = true;
            } else if (x1_atual > x2_atual) {
                p1_era_ancora = false;
            } else {
                p1_era_ancora = (y1_atual <= y2_atual);
            }

            if (p1_era_ancora) {
                delta_x = x2_atual - x1_atual;
                
                linha_setCoordX1(l, x);           
                linha_setCoordX2(l, x + delta_x);
            } else {
                delta_x = x1_atual - x2_atual;

                linha_setCoordX2(l, x);           
                linha_setCoordX1(l, x + delta_x); 
            }
            break;
        }
        case TIPO_A:
            {
                double x1_atual = anteparo_getCoordX1((Anteparo)f);
                double y1_atual = anteparo_getCoordY1((Anteparo)f);
                double x2_atual = anteparo_getCoordX2((Anteparo)f);
                double y2_atual = anteparo_getCoordY2((Anteparo)f);

                double delta_x;
                bool p1_era_ancora;

                if (x1_atual < x2_atual) {
                    p1_era_ancora = true;
                } else if (x1_atual > x2_atual) {
                    p1_era_ancora = false;
                } else {
                    p1_era_ancora = (y1_atual <= y2_atual);
                }

                if (p1_era_ancora) {
                    delta_x = x2_atual - x1_atual;

                    anteparo_setCoordX1((Anteparo)f, x);           
                    anteparo_setCoordX2((Anteparo)f, x + delta_x);
                } else {
                    delta_x = x1_atual - x2_atual;

                    anteparo_setCoordX2((Anteparo)f, x);           
                    anteparo_setCoordX1((Anteparo)f, x + delta_x); 
                }
                break;
            }
        default: printf("Erro: tipo de forma invalido em forma_setCoordX."); exit(1); break;
    }
}

void forma_setCoordY(forma f, double y)
{
    FormaG* forma_generica = (FormaG*)f;

    switch(forma_generica -> type){
        case(TIPO_C): circulo_setCoordY((Circulo)f, y); break;
        case(TIPO_R): retangulo_setCoordY((Retangulo)f, y); break;
        case(TIPO_T): texto_setCoordY((Texto)f, y); break;
        case(TIPO_L): {
            Linha l = (Linha)f;
            double x1_atual = linha_getCoordX1(l);
            double y1_atual = linha_getCoordY1(l);
            double x2_atual = linha_getCoordX2(l);
            double y2_atual = linha_getCoordY2(l);

            double delta_y;
            bool p1_era_ancora;

            if (x1_atual < x2_atual) {
                p1_era_ancora = true;
            } else if (x1_atual > x2_atual) {
                p1_era_ancora = false;
            } else {
                p1_era_ancora = (y1_atual <= y2_atual);
            }

            if (p1_era_ancora) {
                delta_y = y2_atual - y1_atual;

                linha_setCoordY1(l, y);          
                linha_setCoordY2(l, y + delta_y); 
            } else {
                delta_y = y1_atual - y2_atual;

                linha_setCoordY2(l, y);        
                linha_setCoordY1(l, y + delta_y); 
            }
            break;
        }
        case TIPO_A:
            {
                double x1_atual = anteparo_getCoordX1((Anteparo)f);
                double y1_atual = anteparo_getCoordY1((Anteparo)f);
                double x2_atual = anteparo_getCoordX2((Anteparo)f);
                double y2_atual = anteparo_getCoordY2((Anteparo)f);

                double delta_y;
                bool p1_era_ancora;

                if (x1_atual < x2_atual) {
                    p1_era_ancora = true;
                } else if (x1_atual > x2_atual) {
                    p1_era_ancora = false;
                } else {
                    p1_era_ancora = (y1_atual <= y2_atual);
                }

                if (p1_era_ancora) {
                    delta_y = y2_atual - y1_atual;

                    anteparo_setCoordY1((Anteparo)f, y);          
                    anteparo_setCoordY2((Anteparo)f, y + delta_y); 
                } else {
                    delta_y = y1_atual - y2_atual;

                    anteparo_setCoordY2((Anteparo)f, y);        
                    anteparo_setCoordY1((Anteparo)f, y + delta_y); 
                }
                break;
            }
        default: printf("Erro: tipo de forma invalido em forma_setCoordY."); exit(1); break;
    }
}

void forma_setCorBorda(forma f, char* corb)
{
    FormaG* forma_generica = (FormaG*)f;

    switch(forma_generica -> type){
        case(TIPO_C): circulo_setCorBorda((Circulo)f, corb); break;
        case(TIPO_R): retangulo_setCorBorda((Retangulo)f, corb); break;
        case(TIPO_L): linha_setCor((Linha)f, corb); break; 
        case(TIPO_T): texto_setCorBorda((Texto)f, corb); break;
        case(TIPO_A): anteparo_setCor((Anteparo)f, corb); break;
        default: printf("Erro: tipo de forma invalido em forma_setCorBorda."); exit(1); break;
    }
}

void forma_setCorPreench(forma f, char* corp)
{
    FormaG* forma_generica = (FormaG*)f;

    switch(forma_generica -> type){
        case(TIPO_C): circulo_setCorPreench((Circulo)f, corp); break;
        case(TIPO_R): retangulo_setCorPreench((Retangulo)f, corp); break;
        case(TIPO_L): printf("Erro: tipo de forma invalido. Para linhas, utilize 'forma_setCorBorda'"); break;         
        case(TIPO_T): texto_setCorPreench((Texto)f, corp); break;
        case(TIPO_A): printf("Erro: tipo de forma invalido. Para anteparos, utilize 'forma_setCorBorda'"); break;
        default: printf("Erro: tipo de forma invalido em forma_setCorPreench."); exit(1); break;
    }
}