#include <stdio.h>

int main(){

    char palavra[100];
    while(fgets(palavra, sizeof(palavra), stdin) != NULL){
        int tamanho = 0;
        while(palavra[tamanho] != '\0' && palavra[tamanho] != '\n'){tamanho++;}
        char invertida[100];
        for(int i = 0; i < tamanho; i++){
            invertida[i] = palavra[tamanho - 1 - i];
        }
        for(int i = 0; i < tamanho; i++){
            printf("%c", invertida[i]);
        }
        printf("\n");
    }
    return 0;
}