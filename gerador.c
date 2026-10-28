/*
 * gerador.c - gera rajadas (bursts) de comandos para medir o programa do baralho.
 * Nao conhece nenhum baralho: apenas imprime comandos na saida padrao.
 *
 * Uso:    ./gerador <RAJADA> <n> [semente] > entrada.txt
 * Rajada: SOBE | CARTA | POSICAO | CORTA | DESFAZ
 *
 * Saida: "BARALHO n", depois m = 2n comandos da rajada e, por fim, "FIM".
 * Os sorteios sao uniformes; cada x ou p sai de 1 a n.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Rajada DESFAZ: cada grupo e' (OPS_POR_GRUPO operacoes sorteadas) + DESFAZ.
 * Com 1, o grupo e' "operacao, DESFAZ", como descrito no enunciado.
 * Se os monitores confirmarem que a "trinca" tem 2 operacoes + DESFAZ, troque por 2. */
#define OPS_POR_GRUPO 1

static uint64_t estado;

/* xorshift64*: gerador pseudoaleatorio simples; independe do RAND_MAX da plataforma */
static uint64_t proximo(void) {
    estado ^= estado >> 12;
    estado ^= estado << 25;
    estado ^= estado >> 27;
    return estado * 2685821657736338717ULL;
}

/* sorteio uniforme em 1..n */
static long sorteia(long n) { return 1 + (long)(proximo() % (uint64_t)n); }

int main(int argc, char **argv) {
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Uso: %s <SOBE|CARTA|POSICAO|CORTA|DESFAZ> <n> [semente]\n", argv[0]);
        return 1;
    }

    const char *rajada = argv[1];
    char *resto;
    long n = strtol(argv[2], &resto, 10);
    if (*resto != '\0' || n < 1) {
        fprintf(stderr, "n invalido: %s\n", argv[2]);
        return 1;
    }
    if (strcmp(rajada, "SOBE") && strcmp(rajada, "CARTA") && strcmp(rajada, "POSICAO") &&
        strcmp(rajada, "CORTA") && strcmp(rajada, "DESFAZ")) {
        fprintf(stderr, "Rajada desconhecida: %s\n", rajada);
        return 1;
    }

    /* semente fixa por padrao: as medicoes ficam reprodutiveis */
    estado = (argc == 4) ? strtoull(argv[3], NULL, 10) : 88172645463325252ULL;
    if (estado == 0) estado = 1; /* xorshift nao aceita estado zero */

    long m = 2 * n;
    printf("BARALHO %ld\n", n);

    for (long i = 0; i < m; i++) {
        if (!strcmp(rajada, "SOBE")) {
            printf("SOBE %ld\n", sorteia(n));
        } else if (!strcmp(rajada, "CARTA")) {
            printf("CARTA %ld\n", sorteia(n));
        } else if (!strcmp(rajada, "POSICAO")) {
            printf("POSICAO %ld\n", sorteia(n));
        } else if (!strcmp(rajada, "CORTA")) {
            /* CORTA x ou VIRA, com a mesma chance */
            if (sorteia(2) == 1) printf("CORTA %ld\n", sorteia(n));
            else printf("VIRA\n");
        } else { /* DESFAZ */
            if (i % (OPS_POR_GRUPO + 1) == OPS_POR_GRUPO) {
                printf("DESFAZ\n");
            } else {
                long op = sorteia(3); /* CORTA x, VIRA ou SOBE x, igualmente provaveis */
                if (op == 1) printf("CORTA %ld\n", sorteia(n));
                else if (op == 2) printf("VIRA\n");
                else printf("SOBE %ld\n", sorteia(n));
            }
        }
    }

    printf("FIM\n");
    return 0;
}
