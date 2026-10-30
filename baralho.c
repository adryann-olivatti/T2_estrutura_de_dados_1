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


#define CORTA 0                                                                      
#define SOBE 1                                                                       
#define VIRA 2                                                                       
#define VIRATOPO 3                                                                   
                                                                                     
typedef struct {                                                                    
    int tipo;                                                                        
    NO *no;                 
    int k;                                                          
} OPERACAO;                                                                          
                                                                                     
static OPERACAO historico[100];                                                      
static int h_inicio = 0;    
static int h_qtd = 0;       
static int desfazendo = 0;  
                                                                                     
static void registra(int tipo, NO *no, int k) {                                      
    if (desfazendo) {                                                                
        return;                                                                      
    }                                                                                
    if (h_qtd == 100) {                 
        h_inicio = (h_inicio + 1) % 100;                                             
        h_qtd--;                                                                     
    }                                                                                
    historico[(h_inicio + h_qtd) % 100].tipo = tipo;                                 
    historico[(h_inicio + h_qtd) % 100].no = no;                                     
    historico[(h_inicio + h_qtd) % 100].k = k;                                       
    h_qtd++;                                                                         
}                                                                                    

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
    h_inicio = 0;   
    h_qtd = 0;      

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

   registra(CORTA, b->sentinela->prox, 0);   

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

    registra(SOBE, atual->ant, 0);   

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

    registra(VIRA, NULL, 0);   

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

    registra(VIRATOPO, NULL, k);   

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

    h_inicio = 0;  
    h_qtd = 0;

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
