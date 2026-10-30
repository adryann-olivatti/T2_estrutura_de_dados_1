#ifndef BARALHO_H
#define BARALHO_H

#define FALSE 0
#define TRUE 1
typedef int ITEM;
#define ERRO -32000
#define boolean int

typedef struct no_ NO;
typedef struct baralho_ BARALHO;
BARALHO* criar_baralho(int n);
boolean corta_baralho(BARALHO *b, int x);
boolean sobe_baralho(BARALHO *b, int x);
boolean vira_baralho(BARALHO *b);
boolean viratopo_baralho(BARALHO *b, int k);
boolean desfaz_baralho(BARALHO *b);
ITEM carta_baralho(BARALHO *b, int p);
int posicao_baralho(BARALHO *b, int x);
ITEM mostra_baralho(BARALHO *b);
boolean apagar_baralho(BARALHO **b);
#endif
