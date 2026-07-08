#include <stdio.h>

int main(){

    char numero[100];
    while(fgets(numero, sizeof(numero), stdin) != NULL){
        int tamanho = 0;
        int contador = 0;
        while(numero[tamanho] != '\n' && numero[tamanho] != '\0'){tamanho++;}
        for(int i=0; i<tamanho; i++){
            contador += numero[i] - '0';
        }
        printf("%d\n", contador);
    }
    return 0;
}