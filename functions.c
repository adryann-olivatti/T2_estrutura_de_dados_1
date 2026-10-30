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

struct historico_{
    char comandos[100][50]; 
    int indice_inicio;             
    int quantidade_total;         
};

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
       return false;
   }

   NO *atual = b->sentinela->prox;
   while (atual != b->sentinela && atual->item != x) {
       atual = atual->prox;
   }

   if (atual == b->sentinela) {
       return false;
   }

   if (atual == b->sentinela->prox) {
       return true;
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

   return true; 
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
boolean desfaz_baralho(BARALHO *b)
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

boolean inicializar_historico(HISTORICO *h) {
    if (h == NULL) {
        return FALSE;
    }
    
    h->indice_inicio = 0;
    h->quantidade_total = 0;
    
    return TRUE;
}

boolean adicionar_historico(HISTORICO *h, char *comando) {
    if (h == NULL || comando == NULL) {
        return FALSE;
    }

    int pos;

    if (h->quantidade_total < 100) {
        pos = (h->indice_inicio + h->quantidade_total) % 100;
        h->quantidade_total++;
    } 
    else {
        pos = h->indice_inicio;
        h->indice_inicio = (h->indice_inicio + 1) % 100;
    }

    strncpy(h->comandos[pos], comando, 49);
    h->comandos[pos][49] = '\0';

    return TRUE;
}

boolean remover_historico(HISTORICO *h, char *comando) {

    if (h == NULL || h->quantidade_total == 0) {
        return FALSE;
    }

    int pos_topo = (h->indice_inicio + h->quantidade_total - 1) % 100;

    strcpy(comando, h->comandos[pos_topo]);

    h->quantidade_total--;

    return TRUE;
}

