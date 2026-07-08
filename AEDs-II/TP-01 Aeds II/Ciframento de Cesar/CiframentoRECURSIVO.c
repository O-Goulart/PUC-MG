#include <stdio.h>

void criptografar(char str[], char res[], int chave, int i) {
    if (str[i] == '\0') {
        res[i] = '\0';
        return;
    }

    int codigo = (int) str[i];

    if (codigo >= 32 && codigo <= 126) {
        codigo = (codigo - 32 + chave) % 95 + 32;
        res[i] = (char) codigo;
    } else {
        res[i] = str[i];
    }

    criptografar(str, res, chave, i + 1);
}

int main() {
    char string[500];
    int chave = 3;

    while (fgets(string, sizeof(string), stdin) != NULL) {
        int tamanho = 0;
        while (string[tamanho] != '\0' && string[tamanho] != '\n') {
            tamanho++;
        }
        string[tamanho] = '\0';

        if (tamanho == 3 && string[0] == 'F' && string[1] == 'I' && string[2] == 'M') {
            break;
        }

        char palavraCriptografada[500];
        criptografar(string, palavraCriptografada, chave, 0);
        printf("%s\n", palavraCriptografada);
    }

    return 0;
}