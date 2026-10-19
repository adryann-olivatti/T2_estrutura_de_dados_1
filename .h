#ifndef BARALHO_H
#define BARALHO_H

typedef struct no_ NO;
typedef struct baralho_ BARALHO;
BARALHO* baralho_baralho(int n);
void baralho_corta(BARALHO *b, int x);
void baralho_sobe(BARALHO *b, int x);
void baralho_vira(BARALHO *b);
void baralho_viratopo(BARALHO *b, int k);
void baralho_desfaz(BARALHO *b);
int baralho_carta(BARALHO *b, int p);
int baralho_posicao(BARALHO *b, int x);
void baralho_mostra(BARALHO *b);

#endif
