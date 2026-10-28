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
            // Libera a memória alocada previamente em caso de falha
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

        // Inserção no final
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

