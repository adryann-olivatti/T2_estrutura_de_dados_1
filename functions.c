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
}

BARALHO* criar_baralho(int n){
    BARALHO *b = (BARALHO *b)malloc(sizeof(BARALHO));
    if(p == NULL){
        return NULL;
    }
    p->sentinela = (NO *) malloc(sizeof(NO));
    if (b->sentinela == NULL) {
        free(b);
        return NULL;
    }
    
    b->sentinela->prox = b->sentinela;
    b->sentinela->ant = b->sentinela;
    b->tamanho = 0;

    for(int i = 1; i <= n; i++){
        NO *novo = (NO *)malloc(sizeof(NO));
        if(novo == NULL){
            return NULL;
                }
        novo->item = i;

    // Inserção no final: entre o antigo último nó e o sentinela
        NO *ultimo = b->sentinela->ant;
        novo->prox = b->sentinela;
        novo->ant = ultimo;
        ultimo->prox = novo;
        b->sentinela->ant = novo;
        b->tamanho++;
        }
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

