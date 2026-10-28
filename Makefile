# Makefile - Trabalho Pratico 2 (baralho)
# Gera dois executaveis separados: cartaviva (programa principal) e gerador.

CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2 -g -MMD -MP

# Todos os .c, exceto o gerador, formam o programa principal.
# Assim, novos arquivos (.c/.h) do TAD entram na compilacao sem mexer aqui.
SRC_MAIN = $(filter-out gerador.c,$(wildcard *.c))
OBJ_MAIN = $(SRC_MAIN:.c=.o)

# entrada usada por "make valgrind" (gere com ./gerador SOBE 50000 > entrada.txt)
ENTRADA ?= entrada.txt

.PHONY: all clean valgrind zip

all: cartaviva gerador

cartaviva: $(OBJ_MAIN)
	$(CC) $(CFLAGS) -o $@ $^

gerador: gerador.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# verifica vazamentos e acessos invalidos
valgrind: cartaviva
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./cartaviva < $(ENTRADA) > /dev/null

# empacota a entrega: make zip NUSP=<numero USP de quem submete>
# os arquivos ficam na raiz do .zip, sem pasta em volta
zip:
	@test -n "$(NUSP)" || { echo "Uso: make zip NUSP=<numeroUSP>"; exit 1; }
	zip T2_$(NUSP).zip *.c *.h Makefile relatorio.pdf

clean:
	rm -f cartaviva gerador *.o *.d entrada.txt

# dependencias de cabecalho geradas pelo compilador (-MMD)
-include $(wildcard *.d)
