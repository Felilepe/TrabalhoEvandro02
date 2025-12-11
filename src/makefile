#  Autor: Pietro Fernando Pelizon

PROJ_NAME := ted
CC        := gcc
CFLAGS    := -W -Wall -Wextra -ggdb -std=c99 -fstack-protector-all
LIBS      := -lm

EXCLUDES  := lerQry_backup.c lerQry.c lerGeo.c lerGeo.h sort.c
SOURCES   := $(filter-out $(EXCLUDES),$(wildcard *.c))
OBJETOS   := $(SOURCES:.c=.o)


# Regras de compilação:

all: $(PROJ_NAME)

$(PROJ_NAME): $(OBJETOS)
	@echo "Linkando $(PROJ_NAME)..."
	@$(CC) -o $@ $^ $(LIBS)
	@echo "✓ Build completo!"

%.o: %.c
	@echo "Compilando $<..."
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	@echo "Limpando arquivos..."
	@rm -f *.o $(PROJ_NAME)
	@echo "✓ Limpeza concluída!"

.PHONY: all clean

# Targets to build and run unit tests placed under the `testes/` folder
TEST_SRCS := $(wildcard testes/*.c)
TEST_BINS := $(patsubst testes/%.c,testes/%,$(TEST_SRCS))

# All sources except the program entry `main.c` so tests (which provide their own main)
# can be linked with project code without conflicting with `main.c`.
SRC_NO_MAIN := $(filter-out main.c parser_qry.c lerQry.c lerQry_backup.c,$(SOURCES))

.PHONY: tests
tests: $(TEST_BINS)
	@echo "Executando todos os testes..."
	@for t in $(TEST_BINS); do \
		echo "=== $$t ==="; \
		./$$t || exit 1; \
	done
	@echo "✓ Todos os testes executados."

testes/%: testes/%.c $(SRC_NO_MAIN)
	@echo "Compilando teste $<..."
	@$(CC) $(CFLAGS) -I. -o $@ $< $(SRC_NO_MAIN) $(LIBS)
