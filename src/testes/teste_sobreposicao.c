#include <stdio.h>
#include <stdbool.h>

#include "../colisoes.h"
#include "../formas.h"
#include "../poligono.h"
#include "../circulo.h"
#include "../retangulo.h"
#include "../linha.h"
#include "../texto.h"

int g_falhas_sobreposicao = 0;

void print_teste_status(bool sucesso, const char* nome_teste) 
{
    if (sucesso) {
        printf("[  OK  ] %s\n", nome_teste);
    } else {
        printf("[FALHA] %s\n", nome_teste);
        g_falhas_sobreposicao++;
    }
}

Poligono criar_poligono_alvo() 
{
    Poligono p = poligono_create();
    poligono_insertVertice(p, ponto_create(0.0, 10.0));
    poligono_insertVertice(p, ponto_create(20.0, 10.0));
    poligono_insertVertice(p, ponto_create(20.0, 20.0));
    poligono_insertVertice(p, ponto_create(10.0, 20.0));
    return p;
}

void testar_circulo_vs_poligono(Poligono p_alvo) 
{
    printf("--- Testando Círculo vs. Polígono ---\n");

    forma c_dentro = circulo_create(1, 15, 15, 2, "blue", "blue"); 
    print_teste_status(checarColisao(c_dentro, p_alvo) == true, "Círculo DENTRO");

    forma c_fora = circulo_create(2, 100, 100, 5, "red", "red");
    print_teste_status(checarColisao(c_fora, p_alvo) == false, "Círculo FORA");
    forma c_intersecta = circulo_create(3, 21, 15, 2, "green", "green");
    print_teste_status(checarColisao(c_intersecta, p_alvo) == true, "Círculo INTERSECTA");
    printf("-- DEBUG LINE ---\n");
    
    forma c_envolve = circulo_create(4, 15, 15, 100, "yellow", "yellow");
    print_teste_status(checarColisao(c_envolve, p_alvo) == true, "Círculo ENVOLVE");

    forma_destroy(c_dentro);
    forma_destroy(c_fora);
    forma_destroy(c_intersecta);
    forma_destroy(c_envolve);
}

void testar_retangulo_vs_poligono(Poligono p_alvo) 
{
    printf("--- Testando Retângulo vs. Polígono ---\n");

    forma r_dentro = retangulo_create(5, 12, 12, 2, 2, "blue", "blue");
    print_teste_status(checarColisao(r_dentro, p_alvo) == true, "Retângulo DENTRO");

    forma r_fora = retangulo_create(6, 50, 50, 5, 5, "red", "red");
    print_teste_status(checarColisao(r_fora, p_alvo) == false, "Retângulo FORA");

    forma r_intersecta = retangulo_create(7, 18, 15, 4, 2, "green", "green");
    print_teste_status(checarColisao(r_intersecta, p_alvo) == true, "Retângulo INTERSECTA");

    forma_destroy(r_dentro);
    forma_destroy(r_fora);
    forma_destroy(r_intersecta);
}

void testar_linha_vs_poligono(Poligono p_alvo) 
{
    printf("--- Testando Linha vs. Polígono ---\n");

    forma l_dentro = linha_create(8, 12, 12, 18, 18, "blue", false);
    print_teste_status(checarColisao(l_dentro, p_alvo) == true, "Linha DENTRO");

    forma l_fora = linha_create(9, 100, 100, 120, 120, "red", false);
    print_teste_status(checarColisao(l_fora, p_alvo) == false, "Linha FORA");

    forma l_intersecta = linha_create(10, 5, 15, 25, 15, "green", false);
    print_teste_status(checarColisao(l_intersecta, p_alvo) == true, "Linha INTERSECTA");

    forma_destroy(l_dentro);
    forma_destroy(l_fora);
    forma_destroy(l_intersecta);
}

void testar_texto_vs_poligono(Poligono p_alvo) 
{
    printf("--- Testando Texto vs. Polígono ---\n");
    texto_setNewStyle("sans", "b", "10");

    forma t_fora = texto_create(11, 100, 100, "red", "red", 'i', "fora");
    print_teste_status(checarColisao(t_fora, p_alvo) == false, "Texto FORA");

    forma t_intersecta = texto_create(12, 5, 15, "green", "green", 'i', "abcde");
    print_teste_status(checarColisao(t_intersecta, p_alvo) == true, "Texto INTERSECTA");

    forma_destroy(t_fora);
    forma_destroy(t_intersecta);
}


int main() {
    printf("--- Iniciando Testes do TAD Sobreposição ---\n");

    Poligono p_alvo = criar_poligono_alvo();



    testar_circulo_vs_poligono(p_alvo);
    testar_retangulo_vs_poligono(p_alvo);
    testar_linha_vs_poligono(p_alvo);
    testar_texto_vs_poligono(p_alvo);

    poligono_destroy(p_alvo);

    printf("\n--- Testes de Sobreposição Concluídos ---\n");
    if (g_falhas_sobreposicao == 0) {
        printf(">>> SUCESSO: Todos os testes de sobreposição passaram!\n");
    } else {
        printf(">>> FALHA: %d teste(s) de sobreposição falharam.\n", g_falhas_sobreposicao);
    }

    return g_falhas_sobreposicao;
}