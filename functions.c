#include <stdio.h>
#include "baralho.h"
#include <string.h>
#include <stdlib.h>

struct no_{
    ITEM item;
    NO *ant;
    NO *prox;
};

struct baralho_{
    NO *sentinela;
    int tamanho;
};

/* ---------- historico (vetor estatico circular, ate 100 operacoes) ---------- */   // NOVO
#define CORTA 0                                                                      // NOVO
#define SOBE 1                                                                       // NOVO
#define VIRA 2                                                                       // NOVO
#define VIRATOPO 3                                                                   // NOVO
                                                                                     // NOVO
typedef struct {                                                                     // NOVO
    int tipo;                                                                        // NOVO
    NO *no;   /* CORTA: antigo topo | SOBE: no que estava acima de x */              // NOVO
    int k;    /* VIRATOPO: k */                                                      // NOVO
} OPERACAO;                                                                          // NOVO
                                                                                     // NOVO
static OPERACAO historico[100];                                                      // NOVO
static int h_inicio = 0;    /* posicao da operacao mais antiga */                    // NOVO
static int h_qtd = 0;       /* quantas operacoes estao guardadas */                  // NOVO
static int desfazendo = 0;  /* 1 enquanto o desfaz roda: nao registra */             // NOVO
                                                                                     // NOVO
static void registra(int tipo, NO *no, int k) {                                      // NOVO
    if (desfazendo) {                                                                // NOVO
        return;                                                                      // NOVO
    }                                                                                // NOVO
    if (h_qtd == 100) {                 /* cheio: descarta a mais antiga */          // NOVO
        h_inicio = (h_inicio + 1) % 100;                                             // NOVO
        h_qtd--;                                                                     // NOVO
    }                                                                                // NOVO
    historico[(h_inicio + h_qtd) % 100].tipo = tipo;                                 // NOVO
    historico[(h_inicio + h_qtd) % 100].no = no;                                     // NOVO
    historico[(h_inicio + h_qtd) % 100].k = k;                                       // NOVO
    h_qtd++;                                                                         // NOVO
}                                                                                    // NOVO

BARALHO* criar_baralho(int n) {
    BARALHO *b = (BARALHO *) malloc(sizeof(BARALHO)); 
    if (b == NULL) {
        return NULL;
    }

    b->sentinela = (NO *) malloc(sizeof(NO));
    if (b->sentinela == NULL) {
        free(b);
        return NULL;
    }
    
    b->sentinela->prox = b->sentinela;
    b->sentinela->ant = b->sentinela;
    b->tamanho = 0;
    h_inicio = 0;   // NOVO
    h_qtd = 0;      // NOVO

    for (int i = 1; i <= n; i++) {
        NO *novo = (NO *) malloc(sizeof(NO));
        if (novo == NULL) {
            NO *atual = b->sentinela->prox;
            while (atual != b->sentinela) {
                NO *temp = atual;
                atual = atual->prox;
                free(temp);
            }
            free(b->sentinela);
            free(b);
            return NULL;
        }
        novo->item = i;

        NO *ultimo = b->sentinela->ant;
        novo->prox = b->sentinela;
        novo->ant = ultimo;
        ultimo->prox = novo;
        b->sentinela->ant = novo;
        b->tamanho++;
    }

    return b; 
}

boolean corta_baralho(BARALHO *b, ITEM x) {
   if (b == NULL || b->tamanho == 0) {
       return FALSE;
   }

   NO *atual = b->sentinela->prox;
   while (atual != b->sentinela && atual->item != x) {
       atual = atual->prox;
   }

   if (atual == b->sentinela) {
       return FALSE;
   }

   registra(CORTA, b->sentinela->prox, 0);   // NOVO

   if (atual == b->sentinela->prox) {
       return TRUE;
   }

   NO *primeiro = b->sentinela->prox; 
   NO *ultimo   = b->sentinela->ant;  
   NO *no_x     = atual;              
   NO *ant_x    = no_x->ant;          

   ultimo->prox = primeiro;
   primeiro->ant = ultimo;

   ant_x->prox = b->sentinela;
   b->sentinela->ant = ant_x;

   b->sentinela->prox = no_x;
   no_x->ant = b->sentinela;

   return TRUE; 
}

boolean sobe_baralho(BARALHO *b, int x) {
    if (b == NULL || b->tamanho == 0) {
        return FALSE;
    }

    NO *atual = b->sentinela->prox;

    while (atual != b->sentinela && atual->item != x) {
        atual = atual->prox;
    }

    if (atual == b->sentinela) {
        return FALSE;
    }

    registra(SOBE, atual->ant, 0);   // NOVO

    if (atual == b->sentinela->prox) {
        return TRUE;
    }

    atual->ant->prox = atual->prox;
    atual->prox->ant = atual->ant;

    atual->prox = b->sentinela->prox;
    atual->ant = b->sentinela;

    b->sentinela->prox->ant = atual;
    b->sentinela->prox = atual;

    return TRUE;
}

boolean vira_baralho(BARALHO *b) {
    if (b == NULL) {
        return FALSE;
    }

    registra(VIRA, NULL, 0);   // NOVO

    NO *atual = b->sentinela;

    do {
        NO *temp = atual->prox;
        atual->prox = atual->ant;
        atual->ant = temp;
        atual = temp;
    } while (atual != b->sentinela);

    return TRUE;
}

boolean viratopo_baralho(BARALHO *b, int k) {
    if (b == NULL || k < 1 || k > b->tamanho) {
        return FALSE;
    }

    registra(VIRATOPO, NULL, k);   // NOVO

    if (k == 1) {
        return TRUE;
    }

    NO *primeiro = b->sentinela->prox;
    NO *ultimo_k = primeiro;

    for (int i = 1; i < k; i++) {
        ultimo_k = ultimo_k->prox;
    }

    NO *restante = ultimo_k->prox;
    NO *anterior = restante;
    NO *atual = primeiro;

    for (int i = 0; i < k; i++) {
        NO *proximo = atual->prox;
        atual->prox = anterior;
        atual->ant = proximo;
        anterior = atual;
        atual = proximo;
    }

    b->sentinela->prox = ultimo_k;
    ultimo_k->ant = b->sentinela;

    if (restante != b->sentinela) {
        restante->ant = primeiro;
    } else {
        b->sentinela->ant = primeiro;
    }

    return TRUE;
}
boolean desfaz_baralho(BARALHO *b) {
    if (b == NULL) {
        return FALSE;
    }
    if (h_qtd == 0) {
        return TRUE;
    }

    h_qtd--;
    OPERACAO op = historico[(h_inicio + h_qtd) % 100];

    desfazendo = 1;
    if (op.tipo == CORTA) {
        corta_baralho(b, op.no->item);
    } else if (op.tipo == SOBE) {
        NO *no_x = b->sentinela->prox;

        no_x->ant->prox = no_x->prox;
        no_x->prox->ant = no_x->ant;

        no_x->ant = op.no;
        no_x->prox = op.no->prox;
        op.no->prox->ant = no_x;
        op.no->prox = no_x;
    } else if (op.tipo == VIRA) {
        vira_baralho(b);
    } else {
        viratopo_baralho(b, op.k);
    }
    desfazendo = 0;
    return TRUE;
}
ITEM carta_baralho(BARALHO *b, int p) {
     if (b == NULL || p < 1 || p > b->tamanho) {
        return ERRO;
    }

    NO *atual = b->sentinela->prox;
    for (int i = 1; i < p; i++) {
        atual = atual->prox;
    }
    return atual->item;
}
int posicao_baralho(BARALHO *b, int x) {
    if (b == NULL) {
        return ERRO;
    }

    int pos = 1;
    NO *atual = b->sentinela->prox;
    while (atual != b->sentinela && atual->item != x) {
        atual = atual->prox;
        pos++;
    }
    if (atual == b->sentinela) {
        return ERRO;
    }
    return pos;
}
ITEM mostra_baralho(BARALHO *b) {
    if (b == NULL) {
        return ERRO;
    }
    for (NO *atual = b->sentinela->prox; atual != b->sentinela; atual = atual->prox) {
        printf("%d", atual->item);

        if (atual->prox == b->sentinela) {
            printf("\n");
        } else {
            printf(" ");
        }
    }
    return 0;
}
boolean apagar_baralho(BARALHO **b) {
     if (b == NULL || *b == NULL) {
        return FALSE;
    }

    NO *atual = (*b)->sentinela->prox;
    while (atual != (*b)->sentinela) {
        NO *temp = atual;
        atual = atual->prox;
        free(temp);
    }
    free((*b)->sentinela);
    free(*b);
    *b = NULL;
    return TRUE;
}
