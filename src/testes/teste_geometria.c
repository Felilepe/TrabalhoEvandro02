#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "../geometria.h"
#include "../pontos.h"
#include "../anteparo.h"

#define EPSILON 1e-10
#define PI 3.14159265358

int main() {
    printf("Iniciando testes do módulo geometria...\n");


    // Teste 1: geometria_distSqrd
    Ponto temp1 = ponto_create(0, 0);
    Ponto temp2 = ponto_create(3, 4);
    Ponto temp3 = ponto_create(-1, -1);
    Ponto temp4 = ponto_create(2, 3);
    Ponto temp5 = ponto_create(5, 5);
    assert(fabs(geometria_distSqrd(temp1, temp2) - 25.0) < EPSILON);
    assert(fabs(geometria_distSqrd(temp3, temp4) - 25.0) < EPSILON);
    assert(fabs(geometria_distSqrd(temp5, temp5) - 0.0) < EPSILON);
    
    ponto_destroy(temp1);
    ponto_destroy(temp2);
    ponto_destroy(temp3);
    ponto_destroy(temp4);
    ponto_destroy(temp5);


    // Teste 2: geometria_calcAngle
    Ponto obs = ponto_create(0, 0);
    Ponto p_dir = ponto_create(1, 0);
    Ponto p_cima = ponto_create(0, 1);
    Ponto p_esq = ponto_create(-1, 0);
    Ponto p_diag = ponto_create(1, 1);

    assert(fabs(geometria_calcAngle(obs, p_dir) - 0.0) < EPSILON);
    assert(fabs(geometria_calcAngle(obs, p_cima) - PI / 2.0) < EPSILON);
    assert(fabs(geometria_calcAngle(obs, p_esq) - PI) < EPSILON);
    assert(fabs(geometria_calcAngle(obs, p_diag) - PI / 4.0) < EPSILON);

    ponto_destroy(obs);
    ponto_destroy(p_dir);
    ponto_destroy(p_cima);
    ponto_destroy(p_esq);
    ponto_destroy(p_diag);


    // Teste 3: geometria_prodVet
    Ponto p = ponto_create(0, 0);
    Ponto q = ponto_create(5, 0);
    Ponto r1 = ponto_create(5, 5);
    Ponto r2 = ponto_create(5, -5);
    Ponto r3 = ponto_create(10, 0);

    assert(geometria_prodVet(p, q, r1) == 1);
    assert(geometria_prodVet(p, q, r2) == 2);
    assert(geometria_prodVet(p, q, r3) == 0);

    ponto_destroy(q);
    ponto_destroy(r1);
    ponto_destroy(r2);
    ponto_destroy(r3);


    // Teste 4: geometria_isPointOnSeg
    
    Ponto r = ponto_create(10, 10);
    Ponto q1 = ponto_create(5, 5);
    Ponto q2 = ponto_create(11, 11);
    Ponto q3 = ponto_create(-1, -1);
    Ponto q4 = ponto_create(5, 5);

    assert(geometria_isPointOnSeg(q1, p, r) == true);
    assert(geometria_isPointOnSeg(q2, p, r) == false);
    assert(geometria_isPointOnSeg(q3, p, r) == false);
    assert(geometria_isPointOnSeg(q4, p, r) == true);

    ponto_destroy(p);
    ponto_destroy(r);
    ponto_destroy(q1);
    ponto_destroy(q2);
    ponto_destroy(q3);
    ponto_destroy(q4);

    // Teste 5: geometria_distPointSeg
    Ponto p_meio = ponto_create(5, 3);
    Ponto p_antes = ponto_create(-2, 3);
    Ponto p_depois = ponto_create(12, 4);
    Anteparo a_horiz = anteparo_create(0, 0, 0, 10, 0, "red");

    assert(fabs(geometria_distPointSeg(p_meio, a_horiz) - 3.0) < EPSILON);
    assert(fabs(geometria_distPointSeg(p_antes, a_horiz) - sqrt(13.0)) < EPSILON);
    assert(fabs(geometria_distPointSeg(p_depois, a_horiz) - sqrt(20.0)) < EPSILON);

    ponto_destroy(p_meio);
    ponto_destroy(p_antes);
    ponto_destroy(p_depois);
    anteparo_destroy(a_horiz);

    printf("... testes do módulo geometria concluídos (sem erros).\n");
    return 0;
}