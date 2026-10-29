#include <stdio.h>
#include <string.h>
#include "baralho.h"

int main(void) {
    char linha[100];
    char palavra[50];
    int num;
    BARALHO *b = NULL;

    while (fgets(linha, sizeof(linha), stdin) != NULL) {
        int lidos = sscanf(linha, "%49s %d", palavra, &num);

        if (lidos < 1) {
            continue;
        }

        if (strcmp(palavra, "BARALHO") == 0 && lidos == 2) {
            if (b != NULL) {
                apagar_baralho(&b);
            }
            b = criar_baralho(num);
        }
        else if (b != NULL) {
            if (strcmp(palavra, "CORTA") == 0 && lidos == 2) {
                corta_baralho(b, num);
            }
            else if (strcmp(palavra, "SOBE") == 0 && lidos == 2) {
                sobe_baralho(b, num);
            }
            else if (strcmp(palavra, "VIRA") == 0 && lidos == 1) {
                vira_baralho(b);
            }
            else if (strcmp(palavra, "VIRATOPO") == 0 && lidos == 2) {
                viratopo_baralho(b, num);
            }
            else if (strcmp(palavra, "DESFAZ") == 0 && lidos == 1) {
                desfaz_baralho(b);
            }
            else if (strcmp(palavra, "CARTA") == 0 && lidos == 2) {
                carta_baralho(b, num);
            }
            else if (strcmp(palavra, "POSICAO") == 0 && lidos == 2) {
                posicao_baralho(b, num);
            }
            else if (strcmp(palavra, "MOSTRA") == 0 && lidos == 1) {
                mostra_baralho(b);
            }
            else if (strcmp(palavra, "FIM") == 0 && lidos == 1) {
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