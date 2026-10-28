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
    BARALHO *p = (BARALHO *p)malloc(sizeof(BARALHO));
    if(p == NULL){
        return NULL;
    }
    p->sentinela = (NO *) malloc(sizeof(NO));
    if (p->sentinela == NULL) {
        free(p);
        return NULL;
    }
    
    p->sentinela->prox = p->sentinela;
    p->sentinela->ant = p->sentinela;
    p->tamanho = 0;

    for(int i = 1; i <= n; i++){
        NO *novo = (NO *)malloc(sizeof(NO));
        if(novo == NULL){
            return NULL;
                }
        novo->item = i;

    // Inserção no final: entre o antigo último nó e o sentinela
        NO *ultimo = p->sentinela->ant;
        novo->prox = p->sentinela;
        novo->ant = ultimo;
        ultimo->prox = novo;
        p->sentinela->ant = novo;
        p->tamanho++;
        }
    }


boolean corta_baralho(BARALHO *p, ITEM x) {

   if (p == NULL || p->tamanho == 0) {
       return false;
   }

   NO *atual = p->sentinela->prox;
   while (atual != p->sentinela && atual->item != x) {
       atual = atual->prox;
   }

   if (atual == p->sentinela) {
       return false;
   }

  
   if (atual == p->sentinela->prox) {
       return true;
   }

    
   NO *primeiro = p->sentinela->prox; 
   NO *ultimo   = p->sentinela->ant;  
   NO *no_x     = atual;              
   NO *ant_x    = no_x->ant;          

   ultimo->prox = primeiro;
   primeiro->ant = ultimo;

   ant_x->prox = p->sentinela;
   p->sentinela->ant = ant_x;

   p->sentinela->prox = no_x;
   no_x->ant = p->sentinela;

   return true; 
}

