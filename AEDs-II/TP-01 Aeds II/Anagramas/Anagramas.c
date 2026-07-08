#include <stdio.h>
#include <stdbool.h>

int verificarTermino(char linha[]);

int main() {
    while (1) {
        char entrada[256];

        fgets(entrada, 256, stdin);

        if (verificarTermino(entrada)) {
            break;
        }
        
        for (int idx = 0; entrada[idx] != '\0'; idx++) {
            if (entrada[idx] == '\n') {
                entrada[idx] = '\0';
            }
        }

        char palavra1[265];
        char palavra2[265];

        int pos = 0; 
        int tam1 = 0;

        while (entrada[pos] != ' ' && entrada[pos] != '\0') {
            palavra1[tam1] = entrada[pos];
            tam1++;
            pos++;
        }

        while (tam1 > 0 && palavra1[tam1-1] == ' ') {
            tam1--;
        }
        palavra1[tam1] = '\0';

        pos++;
        
        while (entrada[pos] == ' ') {
            pos++;
        }

        int tam2 = 0;
        
        while (entrada[pos] != '\0') {
            palavra2[tam2] = entrada[pos];
            tam2++;
            pos++;
        }
        palavra2[tam2] = '\0';

        bool saoIguais = true;
        int ocorrencias[256] = {0};
        
        if (tam1 != tam2) { 
            saoIguais = false;
        }

        for (int x = 0; x < tam1; x++) {
            if (palavra1[x] >= 'A' && palavra1[x] <= 'Z') {
                palavra1[x] += 32;
            }
            ocorrencias[palavra1[x]]++;
        }

        for (int y = 0; y < tam2; y++) {
            if (palavra2[y] >= 'A' && palavra2[y] <= 'Z') {
                palavra2[y] += 32;
            }
            ocorrencias[palavra2[y]]--;
        }

        for (int z = 0; z < 256; z++) {
            if (ocorrencias[z] != 0) {
                saoIguais = false;
                break;
            }
        }

        if (saoIguais) {
            printf("SIM\n");
        } else {
            printf("NAO\n");
        }
    }
    
    return 0;
}

int verificarTermino(char linha[]) {
    return linha[0] == 'F' &&
           linha[1] == 'I' &&
           linha[2] == 'M' &&
           linha[3] == '\0';
}