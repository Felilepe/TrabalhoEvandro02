    #include "lerGeo.h"
    #include "circulo.h"
    #include "linha.h"
    #include "retangulo.h"
    #include "texto.h"
    #include "formas.h"
    #include "lista.h"

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #define BUFFER_SIZE 512
    #define COMMAND_SIZE 16
    #define COR_SIZE 64
    #define TEXT_SIZE 256
    #define STYLE_FAMILY_SIZE 64
    #define STYLE_WEIGHT_SIZE 16
    #define STYLE_SIZE_SIZE 16

    static void geo_extrairTexto(const char* Linha_buffer, int offset, char* conteudo_Texto, size_t tamanho_max) 
    {
        if (offset <= 0) {
            conteudo_Texto[0] = '\0';
            return;
        }

        const char *inicio_Texto = Linha_buffer + offset;
        while (*inicio_Texto && (*inicio_Texto == ' ' || *inicio_Texto == '\t')) {
            inicio_Texto++;
        }

        strncpy(conteudo_Texto, inicio_Texto, tamanho_max - 1);
        conteudo_Texto[tamanho_max - 1] = '\0';
        conteudo_Texto[strcspn(conteudo_Texto, "\r\n")] = '\0';
    }


    static void geo_processarCirc(const char* Linha, Lista* formas) 
    {
        int id;
        double x, y, r;
        char corb[COR_SIZE] = "", corp[COR_SIZE] = "";

        int num_lidos = sscanf(Linha, "c %i %lf %lf %lf %s %s", &id, &x, &y, &r, corb, corp);
        if (num_lidos < 4) return;

        Circulo c = circulo_create(id, x, y, r, corb, corp);
        lista_insertTail(formas, c);
    }

    static void geo_processarRet(const char* Linha, Lista* formas) 
    {
        int id;
        double x, y, w, h;
        char corb[COR_SIZE] = "", corp[COR_SIZE] = "";

        int num_lidos = sscanf(Linha, "r %i %lf %lf %lf %lf %s %s", &id, &x, &y, &w, &h, corb, corp);
        if (num_lidos < 5) return;

        Retangulo r = retangulo_create(id, x, y, w, h, corb, corp);
        lista_insertTail(formas, r);
    }

    static void geo_processarLinha(const char* Linha_buffer, Lista *formas) 
    {
        int id;
        double x1, y1, x2, y2;
        char cor[COR_SIZE] = "";

        int num_lidos = sscanf(Linha_buffer, "l %i %lf %lf %lf %lf %s", &id, &x1, &y1, &x2, &y2, cor);
        if (num_lidos < 5) return;

        Linha l = linha_create(id, x1, y1, x2, y2, cor, false);
        lista_insertTail(formas, l);
    }

    static void geo_processarTxto(const char* Linha_buffer, Lista *formas) 
    {
        int id;
        double x, y;
        char corp[COR_SIZE] = "", corb[COR_SIZE] = "";
        char ancora = 'i';
        char conteudo_Texto[TEXT_SIZE] = "";
        int offset = 0;

        int num_lidos = sscanf(Linha_buffer, "t %i %lf %lf %s %s %c %n", &id, &x, &y, corb, corp, &ancora, &offset);


        if (num_lidos < 5) return; 

        if (!(ancora == 'i' || ancora == 'm' || ancora == 'f')) {
            ancora = 'i';
        }

        if (offset == 0) {

            char *pos = strstr(Linha_buffer, corp);
            if (pos != NULL) {
                pos += strlen(corp);
                while (*pos == ' ' || *pos == '\t') pos++;
                offset = (int)(pos - Linha_buffer);
            }
        }

        geo_extrairTexto(Linha_buffer, offset, conteudo_Texto, sizeof(conteudo_Texto));

        Texto t = texto_create(id, x, y, corb, corp, ancora, conteudo_Texto);

        if (t) {
            lista_insertTail(formas, t);
        }
    }

    static void processar_estilo_Texto(const char* Linha) 
    {
        char familia[STYLE_FAMILY_SIZE];
        char peso[STYLE_WEIGHT_SIZE];
        char tamanho[STYLE_SIZE_SIZE];

        
        int num_lidos = sscanf(Linha, "ts %s %s %s", familia, peso, tamanho);
        switch(peso[0]) {
            case 'n': strcpy(peso, "normal"); break;
            case 'b': switch(peso[1]){
                case '+': strcpy(peso, "bolder"); break;
                default: strcpy(peso, "bold"); break;
            } break;
            case 'l': strcpy(peso, "lighter"); break;
            default: strcpy(peso, "normal"); break;
        }
        if (num_lidos == 3) {
            
            texto_setNewStyle(familia, peso, tamanho);
        }
    }


    static void processar_comando(const char* Linha_buffer, const char* comando, Lista* formas) 
    {    
        if (strcmp(comando, "c") == 0) {
            geo_processarCirc(Linha_buffer, formas);
        }
        else if (strcmp(comando, "r") == 0) {
            geo_processarRet(Linha_buffer, formas);
        }
        else if (strcmp(comando, "l") == 0) {
            geo_processarLinha(Linha_buffer, formas);
        }
        else if (strcmp(comando, "t") == 0) {
            geo_processarTxto(Linha_buffer, formas);
        }
        else if (strcmp(comando, "ts") == 0) {
            processar_estilo_Texto(Linha_buffer);
        }
    }


    Lista* processaGeo(const char *nome_path_geo) 
    {
        FILE *arquivo_geo = fopen(nome_path_geo, "r");
        if (arquivo_geo == NULL) {
            perror("Erro ao abrir o arquivo .geo");
            return NULL;
        }

        Lista *formas = lista_create();
        if (formas == NULL) {
            fclose(arquivo_geo);
            return NULL;
        }

    char Linha_buffer[BUFFER_SIZE];
        char comando[COMMAND_SIZE];

        while (fgets(Linha_buffer, sizeof(Linha_buffer), arquivo_geo) != NULL) {
            comando[0] = '\0';
            sscanf(Linha_buffer, "%s", comando);


            processar_comando(Linha_buffer, comando, formas);
        }

        fclose(arquivo_geo);
        return formas;
    }