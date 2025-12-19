#include <stdio.h>
#include <string.h>

#include "formas.h"
#include "lista.h"
#include "poligono.h"
#include "colisoes.h"
#include "svg.h"
#include "visibilidade.h"

#define BUFFER_SIZE 512
#define COMANDO_SIZE 16

// --- Struct auxiliares(callback)

// Estrutura para carregar os dados para dentro do callback
typedef struct {
    int id_i;
    int id_j;
    char orientacao;
    Lista *lista_anteparos; // Lista onde vamos guardar os novos anteparos
    Lista *lista_remover;   // Lista temporária para guardar o que deve ser deletado
    FILE *txt;
} ContextoQry;

// Estrutura de Contexto para carregar dados para os callbacks
typedef struct {
    Poligono visi;            // Polígono de visibilidade calculado
    Lista *remover_formas;    // Lista para guardar formas a remover
    Lista *remover_anteparos; // Lista para guardar anteparos a remover
    FILE *txt;                // Arquivo de log
} ContextoD;

typedef struct {
    Poligono visi;   // O polígono de visibilidade calculado
    char *cor;       // A cor a ser aplicada
    FILE *txt;       // O arquivo de log
} ContextoPintura;

typedef struct {
    Poligono visi;       // Polígono de visibilidade para teste
    double dx, dy;       // Valores de translação
    Lista *lista_clones; // Lista temporária para guardar os clones
    FILE *txt;           // Arquivo de log
} ContextoClone;

//--- Funções Auxiliares ---

static void processar_forma_callback(void* item, void* aux)
{
    forma f = (forma)item;
    ContextoQry *ctx = (ContextoQry*)aux;

    int id = forma_getID(f);

    // Verifica se está dentro do intervalo [id_i, id_j]
    if (id >= ctx->id_i && id <= ctx->id_j) {
        
        // Log da forma original
        const char* char_type[] = {"DESCONHECIDO", "CIRCULO", "RETANGULO", "LINHA", "TEXTO", "ANTEPARO"};
        // Nota: Ajuste os índices do array char_type conforme seus #defines (TIPO_C, TIPO_R, etc)
        // Assumindo que TIPO_C=1, etc. O indice 0 fica como placeholder ou ajuste conforme necessário.
        
        fprintf(ctx->txt, "Forma original: ID -> %i | Tipo -> %s\n", 
                id, char_type[forma_getType(f)]);

        // Transforma a forma em anteparos
        Lista *novos_segmentos = forma_toAnteparo(f, ctx->orientacao);
        
        fprintf(ctx->txt, "Segmento(s) produzidos pela forma de ID -> %i:\n", id);

        // Processa os segmentos gerados (Iteração normal pois é uma lista pequena recém criada)
        // Nota: Como 'novos_segmentos' é uma lista temporária, podemos iterar removendo ou usando passthrough
        // Aqui usaremos um loop simples com removeHead para mover os nós de uma lista para outra
        
        while(!lista_isEmpty(novos_segmentos)){
            // Removemos da lista temporária para não precisar clonar
            // Assumindo que forma_toAnteparo retorna uma lista de Formas/Linhas alocadas
            forma seg = lista_getHead(novos_segmentos); 
            lista_removeHead(novos_segmentos);
            
            // Log das coordenadas
            // Nota: Casting para Anteparo ou Linha conforme seu typedef
            // Assumindo que Anteparo tem getters compatíveis
            fprintf(ctx->txt, "Segmento ID %d: (%.2f, %.2f) <-> (%.2f, %.2f)\n",
                    forma_getID(seg), 
                    anteparo_getCoordX1((Anteparo)seg), anteparo_getCoordY1((Anteparo)seg), 
                    anteparo_getCoordX2((Anteparo)seg), anteparo_getCoordY2((Anteparo)seg));

            // Insere na lista final de anteparos
            lista_insertTail(ctx->lista_anteparos, seg);
        }

        fprintf(ctx->txt, "\n");

        // Limpa a lista temporária (a estrutura da lista, os nós já foram movidos)
        lista_destroy(novos_segmentos);

        // Marca a forma original para remoção
        lista_insertTail(ctx->lista_remover, f);
    }
}

static void check_forma_callback(void* item, void* aux)
{
    forma f = (forma)item;
    ContextoD *ctx = (ContextoD*)aux;

    // Checa colisão entre a forma e o polígono de visibilidade
    if(checarColisao(f, ctx->visi)){
        // Log
        const char* tipo_nome[] = {"DESCONHECIDO", "CIRCULO", "RETANGULO", "LINHA", "TEXTO", "ANTEPARO"}; 
        // Ajuste os índices do array acima conforme seus #defines reais (ex: TIPO_C é 1?)
        
        fprintf(ctx->txt, "Forma destruída - ID -> %i | Tipo -> %s\n", 
                forma_getID(f), tipo_nome[forma_getType(f)]);
        
        // Marca para remoção
        lista_insertTail(ctx->remover_formas, f);
    }
}

static void check_anteparo_callback(void* item, void* aux)
{
    forma f = (forma)item; // Assumindo que Anteparo pode ser tratado como Forma ou tem cast compatível
    ContextoD *ctx = (ContextoD*)aux;

    // Filtra IDs inválidos e checa colisão
    if(forma_getID(f) >= 0 && checarColisao(f, ctx->visi)){
        fprintf(ctx->txt, "Anteparo destruído - ID: %d\n", forma_getID(f));
        
        // Marca para remoção
        lista_insertTail(ctx->remover_anteparos, f);
    }
}

static void pintar_forma_callback(void* item, void* aux)
{
    forma f = (forma)item;
    ContextoPintura *ctx = (ContextoPintura*)aux;

    // Verifica Colisão
    if(checarColisao(f, ctx->visi)){
        
        // Log
        // Ajuste o array conforme seus defines (Ex: se TIPO_C=1, usamos o índice 0 como placeholder)
        const char* tipo_nome[] = {"DESCONHECIDO", "CÍRCULO", "RETÂNGULO", "LINHA", "TEXTO", "ANTEPARO"};
        int tipo = forma_getType(f);
        // Proteção simples de índice
        const char* nome = (tipo >= 0 && tipo <= 5) ? tipo_nome[tipo] : "OUTRO";

        fprintf(ctx->txt, "Forma pintada - ID -> %i | Tipo -> %s\n", forma_getID(f), nome);

        // Aplica a cor (Borda e Preenchimento)
        forma_setCorBorda(f, ctx->cor);
        forma_setCorPreench(f, ctx->cor);
    }
}

static void pintar_anteparo_callback(void* item, void* aux)
{
    forma f = (forma)item;
    ContextoPintura *ctx = (ContextoPintura*)aux;

    if(checarColisao(f, ctx->visi)){
        fprintf(ctx->txt, "Forma pintada - ID -> %d | Tipo -> ANTEPARO\n", forma_getID(f));

        // Anteparos no seu código original recebem apenas cor de borda
        forma_setCorBorda(f, ctx->cor);
    }
}

static void clonar_callback(void* item, void* aux)
{
    forma f = (forma)item;
    ContextoClone *ctx = (ContextoClone*)aux;

    // Teste de colisão
    if(checarColisao(f, ctx->visi)){
        
        // Clona a forma
        forma clone = forma_clonar(f);

        // -- Log (Mantendo sua formatação original) --
        // Ajuste os índices do array conforme seus #defines (ex: se TIPO_C = 1, o índice 0 é dummy)
        const char* tipo_nome[] = {"DESCONHECIDO", "CÍRCULO", "RETÂNGULO", "LINHA", "TEXTO", "ANTEPARO"};
        int t_orig = forma_getType(f);
        int t_clone = forma_getType(clone);
        
        // Proteção simples de índice para evitar segfault no print
        const char* nome_orig = (t_orig >= 0 && t_orig <= 5) ? tipo_nome[t_orig] : "OUTRO";
        const char* nome_clone = (t_clone >= 0 && t_clone <= 5) ? tipo_nome[t_clone] : "OUTRO";

        fprintf(ctx->txt, "Forma original - ID -> %i | Tipo -> %s\n ", forma_getID(f), nome_orig);
        fprintf(ctx->txt, "Forma clonada - ID -> %i | Tipo -> %s\n ", forma_getID(clone), nome_clone);

        // -- Translação --
        // Nota: Assumindo que sua implementação de forma_setCoord funciona para alterar a posição base
        forma_setCoordX(clone, forma_getCoordX(f) + ctx->dx);
        forma_setCoordY(clone, forma_getCoordY(f) + ctx->dy);

        // Insere na lista temporária (não podemos inserir na lista 'formas' enquanto percorremos ela)
        lista_insertTail(ctx->lista_clones, clone);
    }
}


// --- Tratadores de Comandos ---

static void qry_command_a(char* buffer, Lista* formas, Lista * anteparos, FILE *txt)
{
    int id_i, id_j;
    char orientacao = 'h';

    sscanf(buffer, "a %i %i %c", &id_i, &id_j, &orientacao);
    fprintf(txt, "[*] a %i %i %c\n", id_i, id_j, orientacao);

    ContextoQry ctx;
    ctx.id_i = id_i;
    ctx.id_j = id_j;
    ctx.orientacao = orientacao;
    ctx.lista_anteparos = anteparos;
    ctx.lista_remover = lista_create();
    ctx.txt = txt;

    lista_passthrough(formas, processar_forma_callback, &ctx);

    
    if (!lista_isEmpty(ctx.lista_remover)) {
        
        while(!lista_isEmpty(ctx.lista_remover)) {
            forma f_to_kill = lista_getHead(ctx.lista_remover);
            lista_removeHead(ctx.lista_remover);

            
            for(int i = 0; i < lista_getSize(formas); i++){
                if(lista_getItem(formas, i) == f_to_kill){
                    lista_removeNode(formas, i);
                    forma_destroy(f_to_kill);
                    break; 
                }
            }
        }
    }

    lista_destroy(ctx.lista_remover);
}

static void qry_command_d(int max_inserts, char sort_type, char *buffer, Lista *formas, Lista *anteparos, FILE *txt,
    const char *path_base_svg, const char *path_saida, FILE *svg_final)
{
    double x_impacto, y_impacto;  
    char sfx[64] = "-";

    sscanf(buffer, "d %lf %lf %s", &x_impacto, &y_impacto, sfx);
    fprintf(txt, "[*] d %.2lf %.2lf %s\n", x_impacto, y_impacto, sfx);

    double max_radius = 10000.0;
    Ponto bomb = ponto_create(x_impacto, y_impacto);
    
    Poligono visi = visibilidade_calc(anteparos, bomb, sort_type, max_radius, max_inserts);

    ContextoD ctx;
    ctx.visi = visi;
    ctx.remover_formas = lista_create();
    ctx.remover_anteparos = lista_create();
    ctx.txt = txt;

    lista_passthrough(formas, check_forma_callback, &ctx);
    lista_passthrough(anteparos, check_anteparo_callback, &ctx);

    if(strcmp(sfx, "-") != 0){
        char path_svg[512];
        sprintf(path_svg, "%s/%s-%s.svg", path_saida, path_base_svg, sfx);

        FILE *svg_vis = startSVG(path_svg);
        if (svg_vis) {
            svg_insertPoligonoVis(svg_vis, visi);
            svg_insertBomb(svg_vis, x_impacto, y_impacto);
            stopSVG(svg_vis);
        }
    }
    else {
        if(svg_final != NULL){
            svg_insertPoligonoVis(svg_final, visi); 
            svg_insertBomb(svg_final, x_impacto, y_impacto);
        }
    }

    while(!lista_isEmpty(ctx.remover_formas)){
        forma f_kill = lista_getHead(ctx.remover_formas);
        lista_removeHead(ctx.remover_formas);

        for(int i = 0; i < lista_getSize(formas); i++){
            if(lista_getItem(formas, i) == f_kill){
                lista_removeNode(formas, i);
                forma_destroy(f_kill);
                break;
            }
        }
    }

    while(!lista_isEmpty(ctx.remover_anteparos)){
        forma a_kill = lista_getHead(ctx.remover_anteparos);
        lista_removeHead(ctx.remover_anteparos);

        for(int i = 0; i < lista_getSize(anteparos); i++){
            if(lista_getItem(anteparos, i) == a_kill){
                lista_removeNode(anteparos, i);
                forma_destroy(a_kill); 
                break;
            }
        }
    }

    lista_destroy(ctx.remover_formas);
    lista_destroy(ctx.remover_anteparos);
    ponto_destroy(bomb);
    poligono_destroy(visi);
}

static void qry_command_p(Lista *formas, Lista *anteparos, FILE *txt, char *buffer,
    int max_inserts, char sort_type, const char* path_out, const char *path_svg_base, FILE *svg_final)
{
    double x_impacto, y_impacto;
    char sfx[64] = "-";
    char cor[16] = "#FF0000"; // Valor default caso sscanf falhe parcialmente, mas sscanf sobrescreve

    sscanf(buffer, "p %lf %lf %s %s", &x_impacto, &y_impacto, cor, sfx);

    // 1. Cálculo Matemático (Visibilidade)
    Ponto bomb = ponto_create(x_impacto, y_impacto);
    double max_radius = 10000.0;
    Poligono visi = visibilidade_calc(anteparos, bomb, sort_type, max_radius, max_inserts);

    fprintf(txt, "[*] p %.2lf %.2lf %s %s\n", x_impacto, y_impacto, cor, sfx);

    // 2. Preparação do Contexto
    ContextoPintura ctx;
    ctx.visi = visi;
    ctx.cor = cor;
    ctx.txt = txt;

    // 3. Iteração Otimizada O(N) - Formas
    lista_passthrough(formas, pintar_forma_callback, &ctx);

    // 4. Iteração Otimizada O(N) - Anteparos
    lista_passthrough(anteparos, pintar_anteparo_callback, &ctx);

    // 5. Geração de SVG
    if(strcmp(sfx, "-") != 0){
        char path_svg[512];
        sprintf(path_svg, "%s/%s-%s.svg", path_out, path_svg_base, sfx);

        FILE *svg_vis = startSVG(path_svg);
        if(svg_vis){
            svg_insertPoligonoVis(svg_vis, visi);
            svg_insertBomb(svg_vis, x_impacto, y_impacto);
            stopSVG(svg_vis);
        }
    }
    else{
        if(svg_final != NULL){
            svg_insertPoligonoVis(svg_final, visi);
            svg_insertBomb(svg_final, x_impacto, y_impacto);
        }
    }

    // 6. Limpeza
    ponto_destroy(bomb);
    poligono_destroy(visi);
}

static void qry_command_cln(Lista *formas, Lista *anteparos, FILE *txt,
    int max_inserts, char sort_type, const char *path_out, const char *path_svg_base, char *buffer, FILE *svg_final)
{
    double x_impacto, y_impacto;
    double dx_translacao, dy_translacao;
    char sfx[64] = "-";

    sscanf(buffer, "cln %lf %lf %lf %lf %s", &x_impacto, &y_impacto, &dx_translacao, &dy_translacao, sfx);

    Ponto bomb = ponto_create(x_impacto, y_impacto);
    double max_radius = 10000.0;
    Poligono visi = visibilidade_calc(anteparos, bomb, sort_type, max_radius, max_inserts);

    fprintf(txt, "[*] cln %.2lf %.2lf %.2lf %.2lf %s\n", x_impacto, y_impacto, dx_translacao, dy_translacao, sfx);

    ContextoClone ctx;
    ctx.visi = visi;
    ctx.dx = dx_translacao;
    ctx.dy = dy_translacao;
    ctx.txt = txt;
    ctx.lista_clones = lista_create(); 

    lista_passthrough(formas, clonar_callback, &ctx);

    while(!lista_isEmpty(ctx.lista_clones)){
        forma clone_para_inserir = lista_getHead(ctx.lista_clones);
        lista_removeHead(ctx.lista_clones); 
        
        lista_insertTail(formas, clone_para_inserir);
    }

    if(strcmp(sfx, "-") != 0) {
        char path_svg[512];
        sprintf(path_svg, "%s/%s-%s.svg", path_out, path_svg_base, sfx);

        FILE *svg_vis = startSVG(path_svg);
        if(svg_vis){
            svg_insertPoligonoVis(svg_vis, visi);
            svg_insertBomb(svg_vis, x_impacto, y_impacto);
            stopSVG(svg_vis);
        }
    }
    else{
        if(svg_final != NULL){
            svg_insertPoligonoVis(svg_final, visi);
            svg_insertBomb(svg_final, x_impacto, y_impacto);
        }
    }

    lista_destroy(ctx.lista_clones); 
    ponto_destroy(bomb);
    poligono_destroy(visi);
}



// --- Função Pública ---

void lerQry(Lista *formas, Lista *anteparos, char *path_qry, char *nome_path_txt,
    int max_insertions, char sort_type, const char *path_out, const char *path_base_svg, FILE *svg_final)
{
    FILE *qry = fopen(path_qry, "r");
    if (qry == NULL) {
        printf("Erro ao abrir o arquivo .qry: %s\n", path_qry);
        return;
    }

    FILE *txt = fopen(nome_path_txt, "w");
    if (txt == NULL) {
        printf("Erro ao abrir o arquivo de saida .txt: %s\n", nome_path_txt);
        fclose(qry); 
        return;
    }


    char command[COMANDO_SIZE];
    char buffer[BUFFER_SIZE];

    while(fgets(buffer, sizeof(buffer), qry)){
        
        command[0] = '\0';

        if(sscanf(buffer, "%s", command) != 1) continue;

        if(strcmp(command, "a") == 0){
            qry_command_a(buffer, formas, anteparos, txt);
        }
        else if(strcmp(command, "d") == 0){
            qry_command_d(max_insertions, sort_type, buffer, formas, anteparos, 
                txt, path_base_svg, path_out, svg_final);
        }
        else if(strcmp(command, "p") == 0){
             qry_command_p(formas, anteparos, txt, buffer, 
                max_insertions, sort_type, path_out, path_base_svg, svg_final);
        }
        else if(strcmp(command, "cln") == 0){
            qry_command_cln(formas, anteparos, txt, 
                max_insertions, sort_type, path_out, path_base_svg, buffer, svg_final);
        }
    }

    fclose(qry);
    fclose(txt);
}