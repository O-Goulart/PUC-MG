#include <stdio.h>

int main(){

    char palavra[100];

    while(fgets(palavra, sizeof(palavra), stdin) != NULL){
        int tamanho = 0;
        int recorde = 0;

        while(palavra[tamanho] != '\0' && palavra[tamanho] != '\n'){tamanho++;}
        
        for(int i=0; i<tamanho; i++){
            int tabela[256] = {0}; 
            int contador = 0;
            for(int j=i; j<tamanho; j++){
                char letra = palavra[j];
                if(tabela[(int)letra] == 1){ 
                    break;
                }
                tabela[(int)letra] = 1;
                contador++;
            }
            if(contador > recorde){
                recorde = contador;
            }
        }
        printf("%d\n", recorde);
    }

    return 0;
}