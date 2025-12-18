#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../lista.h"
#include "../formas.h"
#include "../circulo.h"
#include "../retangulo.h"
#include "../linha.h"
#include "../texto.h"

static void helper_destruir_forma(void *dado, void *ignore) //"Unused parameter" é um parâmetro de                                                       
{                                                       //compatibilidade com lista_passthrough
    forma_destroy(dado); 
}


void test_circulo_anteparo() {
    printf("=== Teste Círculo -> Anteparo ===\n");
    forma circulo = circulo_create(1, 100.0, 100.0, 50.0, "red", "blue");
    
    // --- Teste Horizontal ---
    Lista *anteparos_h = forma_toAnteparo(circulo, 'h');
    printf("---LINHA DE DEBUG---\n");
    assert(anteparos_h != NULL);
    assert(lista_getSize(anteparos_h) == 1);
    printf("Círculo -> 1 anteparo horizontal OK\n");

    // Limpeza Horizontal
    lista_passthrough(anteparos_h, helper_destruir_forma, NULL);
    lista_destroy(anteparos_h);

    // --- Teste Vertical ---
    Lista *anteparos_v = forma_toAnteparo(circulo, 'v');
    assert(anteparos_v != NULL);
    assert(lista_getSize(anteparos_v) == 1);
    printf("Círculo -> 1 anteparo vertical OK\n");

    lista_passthrough(anteparos_v, helper_destruir_forma, NULL);
    lista_destroy(anteparos_v);    
    
    circulo_destroy(circulo);
}

void test_retangulo_anteparo() {
    printf("=== Teste Retângulo -> Anteparos ===\n");

    forma retangulo = retangulo_create(2, 50.0, 50.0, 100.0, 80.0, "green", "yellow");

    Lista *anteparos = forma_toAnteparo(retangulo, 'h');
    assert(anteparos != NULL);
    assert(lista_getSize(anteparos) == 4);
    printf("Retângulo -> 4 anteparos OK\n");

    int ids[4] = {0};
    printf("IDs dos anteparos do retângulo:\n");
    for (int i = 0; i < 4; i++) {
        forma f = lista_getItem(anteparos, i);
        assert(f != NULL);
        ids[i] = forma_getID(f);
        printf("  Anteparo %d: ID = %d\n", i, ids[i]);
        assert(forma_getType(f) == TIPO_A);
    }

    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (ids[i] == ids[j]) {
                printf("ERRO: IDs duplicados encontrados: %d (índices %d e %d)\n", ids[i], i, j);
                assert(ids[i] != ids[j]);
            }
        }
    }
    printf("IDs únicos OK\n");

    lista_passthrough(anteparos, helper_destruir_forma, NULL);
    lista_destroy(anteparos);    
    retangulo_destroy(retangulo);
}

void test_linha_anteparo() {
    printf("=== Teste Linha -> Anteparo ===\n");

    forma linha_forma = linha_create(3, 10.0, 10.0, 200.0, 150.0, "black", false);

    Lista *anteparos = forma_toAnteparo(linha_forma, 'h');
    assert(anteparos != NULL);
    assert(lista_getSize(anteparos) == 1);
    printf(" Linha -> 1 anteparo OK\n");

    forma anteparo = lista_getHead(anteparos);
    assert(anteparo != NULL);
    assert(forma_getType(anteparo) == TIPO_A);

    assert(forma_getID(anteparo) != forma_getID(linha_forma));
    printf("Cópia com ID diferente OK\n");

    lista_passthrough(anteparos, helper_destruir_forma, NULL);
    lista_destroy(anteparos);   
    forma_destroy(linha_forma);
}

void test_texto_anteparo() {
    printf("=== Teste Texto -> Anteparo ===\n");

    forma texto_forma = texto_create(4, 100.0, 100.0, "white", "black", 'i', "Hello");

    Lista *anteparos = forma_toAnteparo(texto_forma, 'h');
    assert(anteparos != NULL);
    assert(lista_getSize(anteparos) == 1);
    printf("Texto -> 1 anteparo OK\n");

    forma anteparo = lista_getHead(anteparos);
    assert(anteparo != NULL);
    assert(forma_getType(anteparo) == TIPO_A);

    lista_passthrough(anteparos, helper_destruir_forma, NULL);
    lista_destroy(anteparos);      
    forma_destroy(texto_forma);
}

void test_multiplas_formas_isoladas() {
    printf("=== Teste Múltiplas Formas (Isoladas) ===\n");


    forma formas[4];
    int total_anteparos_esperado = 0;

    formas[0] = circulo_create(10, 50.0, 50.0, 20.0, "red", "blue");

    total_anteparos_esperado += 1;

    formas[1] = retangulo_create(20, 0.0, 0.0, 100.0, 50.0, "green", "yellow");
    
    total_anteparos_esperado += 4;

     formas[2] = linha_create(30, 0.0, 0.0, 100.0, 100.0, "black", false);
     
    
    total_anteparos_esperado += 1;

    formas[3] = texto_create(40, 50.0, 50.0, "white", "black", 'i', "Test");
    
    total_anteparos_esperado += 1;

    int total_anteparos_gerados = 0;

    for (int i = 0; i < 4; i++) {
        Lista *anteparos = forma_toAnteparo(formas[i], 'h');
        assert(anteparos != NULL);

        int qtd_anteparos = lista_getSize(anteparos);
        total_anteparos_gerados += qtd_anteparos;

        for (int j = 0; j < qtd_anteparos; j++) {
            forma f = lista_getItem(anteparos, j);
            assert(forma_getType(f) == TIPO_A);
        }

    lista_passthrough(anteparos, helper_destruir_forma, NULL);
    lista_destroy(anteparos);          
    forma_destroy(formas[i]);
    }

    assert(total_anteparos_gerados == total_anteparos_esperado);
    printf("Total de anteparos gerados: %d (esperado: %d)\n",
           total_anteparos_gerados, total_anteparos_esperado);
}

int main() 
{
    printf("INICIANDO TESTES DA FUNÇÃO FORMA_toTIPO_A\n\n");

    test_circulo_anteparo();
    test_retangulo_anteparo();
    test_linha_anteparo();
    test_texto_anteparo();
    test_multiplas_formas_isoladas();

    printf("Testes concluídos! Todos os testes passaram!\n");

    return 0;
}