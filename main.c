#include <stdio.h>
#include <string.h>
#include "baralho.h"
 
int main(void) {
    char linha[100];
    char comando[50];
    int num;
    BARALHO *b = NULL;
    
 
 
    while (fgets(linha, sizeof(linha), stdin) != NULL) {
        int lidos = sscanf(linha, "%49s %d", comando, &num);
 
        if (lidos < 1) {
            continue;
        }
 
        if (strcmp(comando, "BARALHO") == 0 && lidos == 2) {
            if (b != NULL) {
                apagar_baralho(&b);
            }
            if(1 <= num && num <= 100000){
            b = criar_baralho(num);
            }
        }
        else if (b != NULL) {
            if (strcmp(comando, "CORTA") == 0 && lidos == 2) {
                corta_baralho(b, num);
                
 
            }
            else if (strcmp(comando, "SOBE") == 0 && lidos == 2) {
                sobe_baralho(b, num);
            }
            else if (strcmp(comando, "VIRA") == 0 && lidos == 1) {
                vira_baralho(b);
            }
            else if (strcmp(comando, "VIRATOPO") == 0 && lidos == 2) {
                viratopo_baralho(b, num);
            }
            else if (strcmp(comando, "DESFAZ") == 0 && lidos == 1) {
                desfaz_baralho(b);
            }
            else if (strcmp(comando, "CARTA") == 0 && lidos == 2) {
                printf("%d\n", carta_baralho(b, num));
            }
            else if (strcmp(comando, "POSICAO") == 0 && lidos == 2) {
                printf("%d\n", posicao_baralho(b, num));
            }
            else if (strcmp(comando, "MOSTRA") == 0 && lidos == 1) {
                mostra_baralho(b);
            }
            else if (strcmp(comando, "FIM") == 0 && lidos == 1) {
                apagar_baralho(&b);
                break;
            }
        }
    }
 
    if (b != NULL) {
        apagar_baralho(&b);
    }
 
    return 0;
}
