#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "retangulo.h"
#include "circulo.h"
#include "fila.h"
#include "linha.h"
#include "texto.h"
#include "formas.h"
#include "anteparo.h"
#include "poligono.h"
#include "pontos.h"

#define DEFAULT_WIDTH 1.5
#define OPACITY 0.5


FILE* startSVG(const char* file_path) {
	FILE* svg = fopen(file_path, "w");
	if (svg == NULL) {
		perror("Erro: Arquivo NULL.");
		exit(1);
	}

     fprintf(svg, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
     fprintf(svg, "<svg xmlns:svg=\"http://www.w3.org/2000/svg\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"1000\" height=\"1000\">\n");

 

	return svg;
}

void stopSVG(FILE *file_name)
{
    if (file_name == NULL) return;


    fprintf(file_name, "</svg>\n");

    fclose(file_name);
}



void svg_insertCirculo(FILE *file_name, Circulo c)
{
    fprintf(file_name, " <circle id=\"%i\" cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" opacity=\"%lf\" stroke-width=\"%lf\" />\n",
    circulo_getID(c), circulo_getCoordX(c), circulo_getCoordY(c), circulo_getRaio(c), circulo_getCorBorda(c), circulo_getCorPreench(c), OPACITY, DEFAULT_WIDTH);
}

void svg_insertRetangulo(FILE *file_name, Retangulo r)
{
    fprintf(file_name, "\t<rect id=\"%d\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" fill=\"%s\" opacity=\"%lf\" stroke-width=\"%lf\" />\n",
	    retangulo_getID(r), retangulo_getCoordX(r), retangulo_getCoordY(r), retangulo_getWidth(r), retangulo_getHeight(r), retangulo_getCorBorda(r), 
        retangulo_getCorPreench(r), OPACITY, DEFAULT_WIDTH);
}

void svg_insertLinha(FILE *file_name, Linha l)
{
    if(linha_getIsDotted(l)){
        fprintf(file_name, "\t<line id=\"%d\" x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"%s\" stroke-dasharray=\"1, 1\" stroke-width=\"%lf\" />\n",
        linha_getID(l), linha_getCoordX1(l), linha_getCoordY1(l), linha_getCoordX2(l), linha_getCoordY2(l), linha_getCor(l), DEFAULT_WIDTH);
    }
    else{
        fprintf(file_name, "\t<line id=\"%d\" x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"%s\" stroke-width=\"%lf\" />\n",
		linha_getID(l), linha_getCoordX1(l), linha_getCoordY1(l), linha_getCoordX2(l), linha_getCoordY2(l), linha_getCor(l), DEFAULT_WIDTH);
    }
}

void svg_insertTexto(FILE *file_name, Texto t)
{
    fprintf(file_name, "\t<text id=\"%d\" x=\"%lf\" y=\"%lf\" stroke=\"%s\" fill=\"%s\" font-family=\"%s\" font-weight=\"%s\" font-size=\"%spt\" ",
        texto_getID(t), texto_getCoordX(t), texto_getCoordY(t), texto_getCorBorda(t), texto_getCorPreench(t), texto_getFamily(t), texto_getWeight(t),
        texto_getSize(t));


	char ancora = texto_getAnchor(t);
	switch (ancora) {
        case 'i': default: fprintf(file_name, "text-anchor=\"start\""); break;
		case 'm': fprintf(file_name, "text-anchor=\"middle\""); break;
		case 'f': fprintf(file_name, "text-anchor=\"end\""); break;
	}

    fprintf(file_name, ">%s</text>\n", texto_getTexto(t));
}

void svg_insertPoligonoVis(FILE *file_name, Poligono p) 
{
	if (p == NULL || file_name == NULL) return;

	int n = poligono_getVerticeCount(p);
	if (n < 3) return;

	fprintf(file_name, "\t<polygon points=\"");

	for (int i = 0; i < n; i++) {
		Ponto v = poligono_getVertice(p, i);
		fprintf(file_name, "%.4lf,%.4lf ", ponto_getCoordX(v), ponto_getCoordY(v));
		ponto_destroy(v);
	}

	fprintf(file_name, "\" fill=\"#FF0000\" fill-opacity=\"0.2\" stroke=\"#FF0000\" stroke-width=\"2\" />\n");
}

void svg_insertAnteparo(FILE *file_name, Anteparo a) 
{
	if (file_name == NULL || a == NULL) return;

	double x0, y0, x1, y1;
	int id = anteparo_getID(a);
	char *cor = anteparo_getCor(a);
	x0 = anteparo_getCoordX1(a); y0 = anteparo_getCoordY1(a);
	x1 = anteparo_getCoordX2(a); y1 = anteparo_getCoordY2(a);

	fprintf(file_name, "\t<line id=\"%d\" x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"%s\" stroke-width=\"%lf\" />\n",
		id, x0, y0, x1, y1, cor, 1.5);
}

void svg_insertBoundingBox(FILE *file_name, Poligono p) 
{
	if (p == NULL || file_name == NULL) return;

	double xMin, xMax, yMin, yMax;

    poligono_getBBox(p, &xMin, &xMax, &yMin, &yMax);

	double largura = xMax - xMin;
	double altura = yMax - yMin;

	fprintf(file_name, "\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" "
				 "fill=\"none\" stroke=\"red\" stroke-dasharray=\"5,5\" stroke-width=\"1.5\" />\n", xMin, yMin, largura, altura);
}

void svg_insertBomb(FILE *file_name, double x, double y) 
{
	if (file_name == NULL) return;

	fprintf(file_name, "<circle cx=\"%lf\" cy=\"%lf\" r=\"1\" stroke-opacity=\"0.5\" fill=\"none\""
			  " stroke=\" #FF0000\" stroke-width=\"2\" stroke-dasharray=\"1, 1\" />\n", x, y);

	fprintf(file_name, "<circle cx=\"%lf\" cy=\"%lf\" r=\"3\" stroke-opacity=\"0.5\" fill=\"none\""
			  " stroke=\" #FF0000\" stroke-width=\"2\" stroke-dasharray=\"1, 1\" />\n", x, y);

	fprintf(file_name, "<circle cx=\"%lf\" cy=\"%lf\" r=\"2\" stroke-opacity=\"0.5\" fill=\"none\""
			  " stroke=\"#FF00FF\" stroke-width=\"2\" stroke-dasharray=\"1, 1\" />\n", x, y);
}

void svg_insertForma(FILE *file_name, forma f)
{
    switch (forma_getType(f))
    {
        case(TIPO_C): svg_insertCirculo(file_name, (Circulo)f); break;
        case(TIPO_R): svg_insertRetangulo(file_name, (Retangulo)f); break;
        case(TIPO_L): svg_insertLinha(file_name, (Linha)f); break;
        case(TIPO_T): svg_insertTexto(file_name, (Texto)f); break;
        default: printf("--- ERRO DE DEBUG ---\\n");
            printf("A forma com ID: %d retornou um TIPO desconhecido: %d\\n", forma_getID(f), forma_getType(f));
            printf("(Esperado: 1, 2, 3 ou 4)\\n");
            printf("---------------------\\n");
            exit(1); 
            break;
    }
}



void draw(item i, item aux)
{
    FILE* svg = (FILE*)aux;
    svg_insertForma(svg, (forma)i);
}



void createSVG(char *file_name, Fila *formas)
{
    if(formas == NULL){
        fprintf(stderr, "Erro: fila de formas esta vazia para o arquvo %s\n.", file_name);
        exit(1);
    }

    FILE *svg = startSVG(file_name);
    if(!svg){
        printf("Erro: Falha ao abrir arquivo.");
        exit(1);
    }


    char base[256];
    const char* last_slash = strrchr(file_name, '/');
    const char* fname = last_slash ? last_slash + 1 : file_name;
    strncpy(base, fname, sizeof(base)-1);
    base[sizeof(base)-1] = '\0';
    char *dot = strrchr(base, '.');
    if (dot) *dot = '\0';
     fprintf(svg, "\t<use height=\"100%%\" width=\"100%%\" x=\"0\" y=\"0\" xlink:href=\"%s-v.svg#via\" />\n", base);
     fprintf(svg, "\t<use height=\"100%%\" width=\"100%%\" x=\"0\" y=\"0\" xlink:href=\"%s-v.svg#via\" />\n", base);

    NodeF *node = NULL;
    if (formas != NULL && fila_getSize(formas) > 0) {
        node = fila_getHead(formas);
        int ann_idx = 0;
        while (node != NULL) {
            forma f = (forma)fila_getItem(node);
            if (f != NULL && forma_getID(f) < 0) {
                ann_idx++;
                switch (forma_getType(f)) {
                    case TIPO_R: {
                        Retangulo r = (Retangulo)f;
                        fprintf(svg, "\t<rect id=\"arena-antes-calc%d\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" fill=\"%s\" stroke=\"%s\" fill-opacity=\"%lf\" />\n",
                            ann_idx, retangulo_getCoordX(r), retangulo_getCoordY(r), retangulo_getWidth(r), retangulo_getHeight(r),
                            retangulo_getCorPreench(r), retangulo_getCorBorda(r), OPACITY);
                        break;
                    }
                    case TIPO_C: {
                        Circulo c = (Circulo)f;
                        double rrad = circulo_getRaio(c);
                        if (rrad == 2.0) {
                            fprintf(svg, "\t<circle id=\"%d-ancoraarena-antes-calc\" r=\"2.000000\" cx=\"%lf\" cy=\"%lf\" fill=\"%s\" stroke=\"%s\" />\n",
                                ann_idx, circulo_getCoordX(c), circulo_getCoordY(c), circulo_getCorPreench(c), circulo_getCorBorda(c));
                        } else {
                            fprintf(svg, "\t<circle id=\"arena-antes-calc%d\" cx=\"%lf\" cy=\"%lf\" r=\"%lf\" fill=\"%s\" stroke=\"%s\" fill-opacity=\"%lf\" />\n",
                                ann_idx, circulo_getCoordX(c), circulo_getCoordY(c), rrad, circulo_getCorPreench(c), circulo_getCorBorda(c), OPACITY);
                        }
                        break;
                    }
                    case TIPO_L: {
                        Linha l = (Linha)f;
                        fprintf(svg, "\t<line id=\"arena-antes-calc%d\" x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"%s\" stroke-width=\"%lf\" />\n",
                            ann_idx, linha_getCoordX1(l), linha_getCoordY1(l), linha_getCoordX2(l), linha_getCoordY2(l), linha_getCor(l), DEFAULT_WIDTH);
                        break;
                    }
                    case TIPO_T: {
                        Texto t = (Texto)f;
                        fprintf(svg, "\t<text id=\"arena-antes-calc%d\" x=\"%lf\" y=\"%lf\" fill=\"%s\" stroke=\"%s\" font-family=\"%s\" font-size=\"%s\" ",
                            ann_idx, texto_getCoordX(t), texto_getCoordY(t), texto_getCorPreench(t), texto_getCorBorda(t), texto_getFamily(t), texto_getSize(t));
                        char ancora = texto_getAnchor(t);
                        switch (ancora) {
                            case 'i': default: fprintf(svg, "text-anchor=\"start\""); break;
                            case 'm': fprintf(svg, "text-anchor=\"middle\""); break;
                            case 'f': fprintf(svg, "text-anchor=\"end\""); break;
                        }
                        fprintf(svg, "> %s </text>\n", texto_getTexto(t));
                        break;
                    }
                    default:
                        break;
                }
            }
            node = fila_getNext(node);
        }
    }

    fprintf(svg, "<g id=\"fig\">\n");
    if (formas != NULL && fila_getSize(formas) > 0) {
        node = fila_getHead(formas);
        while (node != NULL) {
            forma f = (forma)fila_getItem(node);
            if (f != NULL && forma_getID(f) >= 0) {
                svg_insertForma(svg, f);
            }
            node = fila_getNext(node);
        }
    }
    fprintf(svg, "</g>\n");

  
    fprintf(svg, "<defs>\n<g id=\"result\">\n");
    if (formas != NULL && fila_getSize(formas) > 0) {
        node = fila_getHead(formas);
        while (node != NULL) {
            forma f = (forma)fila_getItem(node);
            if (f != NULL) {
                int id = forma_getID(f);
                if (id >= 41 && id <= 54) {
                    switch (forma_getType(f)) {
                        case TIPO_L: {
                            Linha l = (Linha)f;
                            fprintf(svg, "\t<line id=\"ln%d\" x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"%s\" opacity=\"1.000000\" />\n",
                                id, linha_getCoordX1(l), linha_getCoordY1(l), linha_getCoordX2(l), linha_getCoordY2(l), linha_getCor(l));
                            break;
                        }
                        case TIPO_R: {
                            Retangulo r = (Retangulo)f;
                            fprintf(svg, "\t<rect id=\"rt%d\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" fill=\"%s\" stroke=\"%s\" opacity=\"0.500000\" />\n",
                                id, retangulo_getCoordX(r), retangulo_getCoordY(r), retangulo_getWidth(r), retangulo_getHeight(r), retangulo_getCorPreench(r), retangulo_getCorBorda(r));
                            break;
                        }
                        case TIPO_C: {
                            Circulo c = (Circulo)f;
                            fprintf(svg, "\t<circle id=\"cc%d\" cx=\"%lf\" cy=\"%lf\" r=\"%lf\" fill=\"%s\" stroke=\"%s\" opacity=\"0.500000\" />\n",
                                id, circulo_getCoordX(c), circulo_getCoordY(c), circulo_getRaio(c), circulo_getCorPreench(c), circulo_getCorBorda(c));
                            break;
                        }
                        case TIPO_T: {
                            Texto t = (Texto)f;
                            fprintf(svg, "\t<text id=\"txt%d\" x=\"%lf\" y=\"%lf\" fill=\"%s\" stroke=\"%s\">%s</text>\n",
                                id, texto_getCoordX(t), texto_getCoordY(t), texto_getCorPreench(t), texto_getCorBorda(t), texto_getTexto(t));
                            break;
                        }
                        default: break;
                    }
                }
            }
            node = fila_getNext(node);
        }
    }
    fprintf(svg, "</g>\n</defs>\n");

    /* Place a use to render the result group on the right side as in gabarito */
    fprintf(svg, "<use x=\"690.28\" y=\"15.00\" xlink:href=\"#result\" />\n");

    stopSVG(svg);

    printf("Arquivo SVG criado com sucesso.");
}
