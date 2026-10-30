#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPS_POR_GRUPO 2

static uint64_t estado;

static uint64_t proximo(void) {
    estado ^= estado >> 12;
    estado ^= estado << 25;
    estado ^= estado >> 27;
    return estado * 2685821657736338717ULL;
}

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

    estado = (argc == 4) ? strtoull(argv[3], NULL, 10) : 88172645463325252ULL;
    if (estado == 0) estado = 1; 

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
            if (sorteia(2) == 1) printf("CORTA %ld\n", sorteia(n));
            else printf("VIRA\n");
        } else { 
            if (i % (OPS_POR_GRUPO + 1) == OPS_POR_GRUPO) {
                printf("DESFAZ\n");
            } else {
                long op = sorteia(3);
                if (op == 1) printf("CORTA %ld\n", sorteia(n));
                else if (op == 2) printf("VIRA\n");
                else printf("SOBE %ld\n", sorteia(n));
            }
        }
    }

    printf("FIM\n");
    return 0;
}
