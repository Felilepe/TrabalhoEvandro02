#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "../poligono.h"
#include "../pontos.h"
#include "../lista.h"
#include "../linha.h" // CORREÇÃO: Alterado para minúsculo para garantir compatibilidade

int g_falhas = 0;

void print_teste_status(bool sucesso, const char* nome_teste) {
    if (sucesso) {
        printf("[  OK  ] %s\n", nome_teste);
    } else {
        printf("[FALHA] %s\n", nome_teste);
        g_falhas++;
    }
}

bool double_equals(double a, double b) {
    return fabs(a - b) < 0.0001;
}

// --- Teste do TAD Ponto ---
void testar_ponto() {
    printf("\n--- Testando TAD Ponto ---\n");

    Ponto p1 = ponto_create(10.5, 20.0);
    print_teste_status(p1 != NULL, "ponto_create (alocacao)");
    
    if (p1) {
        print_teste_status(double_equals(ponto_getCoordX(p1), 10.5) && 
                           double_equals(ponto_getCoordY(p1), 20.0), 
                           "ponto_getCoordX / ponto_getCoordY");

        Ponto p2 = ponto_clone(p1);
        print_teste_status(p2 != NULL && p1 != p2, "ponto_clone (alocacao distinta)");
        
        if (p2) {
            print_teste_status(double_equals(ponto_getCoordX(p2), 10.5) && 
                               double_equals(ponto_getCoordY(p2), 20.0), 
                               "ponto_clone (valores)");
            ponto_destroy(p2);
        }
        ponto_destroy(p1);
    }
    print_teste_status(true, "ponto_destroy (executado)");
}

// --- Teste Básico do Polígono ---
void testar_poligono_basico() {
    printf("\n--- Testando TAD Poligono (Basico) ---\n");
    Poligono p = poligono_create();

    print_teste_status(p != NULL, "poligono_create");
    print_teste_status(poligono_isEmpty(p) == true, "poligono_isEmpty (novo)");
    print_teste_status(poligono_getVerticeCount(p) == 0, "poligono_getVerticeCount (novo)");

    Ponto v1 = ponto_create(0.0, 0.0);
    Ponto v2 = ponto_create(10.0, 0.0);

    poligono_insertVertice(p, v1);
    poligono_insertVertice(p, v2);

    Ponto v3 = ponto_create(10.0, 10.0);
    poligono_copyPasteVertice(p, v3);

    print_teste_status(poligono_isEmpty(p) == false, "poligono_isEmpty (com dados)");
    print_teste_status(poligono_getVerticeCount(p) == 3, "poligono_getVerticeCount (3 vertices)");

    Ponto v_teste = poligono_getVertice(p, 1);
    print_teste_status(v_teste != NULL && double_equals(ponto_getCoordX(v_teste), 10.0), "poligono_getVertice (indice 1)");

    Ponto v3_interno = lista_getItem(poligono_getVerticeList(p), 2);
    print_teste_status(v3 != v3_interno, "poligono_copyPasteVertice (insere copia, nao original)");

    // CORREÇÃO: Destruímos apenas o v3 original (usado no copyPaste).
    // NÃO destruímos v_teste, pois ele aponta para dentro do polígono (evita Double Free).
    ponto_destroy(v3);
    
    poligono_destroy(p);
}

// --- Teste de Bounding Box ---
void testar_poligono_bbox() {
    printf("\n--- Testando Poligono (Bounding Box) ---\n");
    Poligono p = poligono_create();

    poligono_insertVertice(p, ponto_create(10.0, 20.0));
    poligono_insertVertice(p, ponto_create(100.0, 5.0));
    poligono_insertVertice(p, ponto_create(50.0, 80.0));
    poligono_insertVertice(p, ponto_create(0.0, 40.0));

    double xMin, xMax, yMin, yMax;
    poligono_getBBox(p, &xMin, &xMax, &yMin, &yMax);

    bool calc1_ok = double_equals(xMin, 0.0) && double_equals(xMax, 100.0) &&
                    double_equals(yMin, 5.0) && double_equals(yMax, 80.0);
    print_teste_status(calc1_ok, "Bounding Box (calculo inicial)");

    poligono_insertVertice(p, ponto_create(200.0, -10.0));
    poligono_getBBox(p, &xMin, &xMax, &yMin, &yMax);

    bool calc2_ok = double_equals(xMin, 0.0) && double_equals(xMax, 200.0) &&
                    double_equals(yMin, -10.0) && double_equals(yMax, 80.0);
    print_teste_status(calc2_ok, "Bounding Box (re-calculo apos insercao)");

    poligono_destroy(p);
}

// --- Wrapper para evitar Memory Leak no isInside ---
// Cria o ponto, testa e destrói imediatamente
bool check_inside_wrapper(Poligono p, double x, double y) {
    Ponto pt = ponto_create(x, y);
    bool result = poligono_isInside(p, pt);
    ponto_destroy(pt); // CORREÇÃO: Memória limpa aqui!
    return result;
}

void testar_poligono_isInside() {
    printf("\n--- Testando Poligono (poligono_isInside) ---\n");

    Poligono p = poligono_create();

    poligono_insertVertice(p, ponto_create(0.0, 0.0));
    poligono_insertVertice(p, ponto_create(10.0, 0.0));
    poligono_insertVertice(p, ponto_create(10.0, 10.0));
    poligono_insertVertice(p, ponto_create(0.0, 10.0));

    print_teste_status(check_inside_wrapper(p, 5.0, 5.0) == true, "poligono_isInside (ponto central)");
    print_teste_status(check_inside_wrapper(p, 100.0, 100.0) == false, "poligono_isInside (ponto fora - BBox)");
    print_teste_status(check_inside_wrapper(p, -5.0, 5.0) == false, "poligono_isInside (ponto fora - adjacente)");
    print_teste_status(check_inside_wrapper(p, 5.0, 10.0) == false, "poligono_isInside (ponto na borda horizontal)");
    print_teste_status(check_inside_wrapper(p, 10.0, 5.0) == false, "poligono_isInside (ponto na borda vertical)");
    print_teste_status(check_inside_wrapper(p, 0.0, 0.0) == true, "poligono_isInside (ponto no vertice)");

    poligono_destroy(p);
}

// --- Teste de Segmentos ---
static void helper_linha_destroy(void *dado, void *ignore) {                                                       
    linha_destroy(dado); 
}

void testar_poligono_segmentos() 
{
    printf("\n--- Testando Poligono (poligono_getSegments) ---\n");

    Poligono p = poligono_create();
    poligono_insertVertice(p, ponto_create(0.0, 0.0));
    poligono_insertVertice(p, ponto_create(10.0, 0.0));
    poligono_insertVertice(p, ponto_create(5.0, 10.0));

    Lista* segmentos = poligono_getSegments(p);
    print_teste_status(segmentos != NULL, "poligono_getSegments (alocacao)");
    print_teste_status(lista_getSize(segmentos) == 3, "poligono_getSegments (contagem = 3)");

    Linha s1 = lista_getItem(segmentos, 0);
    Linha s2 = lista_getItem(segmentos, 1);
    Linha s3 = lista_getItem(segmentos, 2);

    bool seg1_ok = double_equals(linha_getCoordX1(s1), 0.0) && double_equals(linha_getCoordY1(s1), 0.0) &&
                   double_equals(linha_getCoordX2(s1), 10.0) && double_equals(linha_getCoordY2(s1), 0.0);
    
    bool seg2_ok = double_equals(linha_getCoordX1(s2), 10.0) && double_equals(linha_getCoordY1(s2), 0.0) &&
                   double_equals(linha_getCoordX2(s2), 5.0) && double_equals(linha_getCoordY2(s2), 10.0);
    
    bool seg3_ok = double_equals(linha_getCoordX1(s3), 5.0) && double_equals(linha_getCoordY1(s3), 10.0) &&
                   double_equals(linha_getCoordX2(s3), 0.0) && double_equals(linha_getCoordY2(s3), 0.0);

    print_teste_status(seg1_ok, "getSegmentos (Segmento 1: (0,0)->(10,0))");
    print_teste_status(seg2_ok, "getSegmentos (Segmento 2: (10,0)->(5,10))");
    print_teste_status(seg3_ok, "getSegmentos (Segmento 3: (5,10)->(0,0))");

    // CORREÇÃO: Limpeza completa da lista de segmentos
    lista_passthrough(segmentos, helper_linha_destroy, NULL); // 1. Destrói o conteúdo
    lista_destroy(segmentos); // 2. Destrói a estrutura da lista

    poligono_destroy(p);
}

int main() {
    testar_ponto();
    testar_poligono_basico();
    testar_poligono_bbox();
    testar_poligono_isInside();
    testar_poligono_segmentos();

    printf("\n--- Testes Concluidos ---\n");
    if (g_falhas == 0) {
        printf(">>> SUCESSO: Todos os testes passaram!\n");
    } else {
        printf(">>> FALHA: %d teste(s) falharam.\n", g_falhas);
    }

    return g_falhas;
}