#Makefile para  testes

PROJ_NAME := ted
TEST_NAME := rodar_testes
CC        := gcc
# O flag -I. diz para procurar headers na pasta atual (raiz)
CFLAGS    := -W -Wall -Wextra -ggdb -std=c99 -fstack-protector-all -I.
LIBS      := -lm

# 1. Lista todos os arquivos .c da raiz
SOURCES   := $(wildcard *.c)

# 2. Define qual é o arquivo principal do projeto (que tem o main) para excluí-lo do teste
MAIN_FILE := main.c 

# 3. Define onde está o seu teste
TEST_SRC  := testes/teste_anteparos.c

# 4. Objetos comuns (Tudo na raiz, exceto o main.c)
OBJ_COMMON := $(filter-out $(MAIN_FILE:.c=.o), $(SOURCES:.c=.o))

# --- Regras ---

all: $(PROJ_NAME)

# Compila o executável principal
$(PROJ_NAME): $(SOURCES:.c=.o)
	@echo "Linkando $(PROJ_NAME)..."
	@$(CC) -o $@ $^ $(LIBS)
	@echo "✓ Build do projeto principal completo!"

# Compila o executável de teste
# Ele depende dos objetos comuns + o arquivo .c do teste
teste: $(OBJ_COMMON)
	@echo "Compilando e Linkando o Teste..."
	@# Compila o teste_anteparos.c diretamente junto com os objetos da raiz
	@$(CC) $(CFLAGS) -o $(TEST_NAME) $(TEST_SRC) $(OBJ_COMMON) $(LIBS)
	@echo "✓ Build do teste completo! Rodando..."
	@./$(TEST_NAME)

# Regra genérica para transformar .c em .o (apenas para arquivos da raiz)
%.o: %.c
	@echo "Compilando $<..."
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	@echo "Limpando arquivos..."
	@rm -f *.o $(PROJ_NAME) $(TEST_NAME)
	@echo "✓ Limpeza concluída!"

.PHONY: all clean teste



#Makefile original

#PROJ_NAME := ted
#CC        := gcc
#CFLAGS    := -W -Wall -Wextra -ggdb -std=c99 -fstack-protector-all
#LIBS      := -lm

#SOURCES   := $(wildcard *.c)
#OBJETOS   := $(SOURCES:.c=.o)


# Regras de compilação:

#all: $(PROJ_NAME)

#$(PROJ_NAME): $(OBJETOS)
#	@echo "Linkando $(PROJ_NAME)..."
#	@$(CC) -o $@ $^ $(LIBS)
#	@echo "✓ Build completo!"
#
#.o: %.c
#	@echo "Compilando $<..."
#	@$(CC) -c $(CFLAGS) $< -o $@
#
#clean:
#	@echo "Limpando arquivos..."
#	@rm -f *.o $(PROJ_NAME)
#	@echo "✓ Limpeza concluída!"

#.PHONY: all clean