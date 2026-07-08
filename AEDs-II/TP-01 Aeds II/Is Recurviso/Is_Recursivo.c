#include <stdio.h>

int vogal(char palavra[], int i) {
    if (palavra[i] == '\0') {
        return 1;
    }
    char c = palavra[i];
    if (c != 'A' && c != 'a' && c != 'E' && c != 'e' && c != 'I' && c != 'i' && c != 'O' && c != 'o' && c != 'U' && c != 'u') {
        return 0;
    }
    return vogal(palavra, i + 1);
}

int consoante(char palavra[], int i) {
    if (palavra[i] == '\0') {
        return 1;
    }
    char c = palavra[i];
    int letra = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    int is_vogal = (c == 'A' || c == 'a' || c == 'E' || c == 'e' || c == 'I' || c == 'i' || c == 'O' || c == 'o' || c == 'U' || c == 'u');
    if (!letra || is_vogal) {
        return 0;
    }
    return consoante(palavra, i + 1);
}

int inteiro(char palavra[], int i) {
    if (palavra[i] == '\0') {
        return 1;
    }
    char c = palavra[i];
    if (c < '0' || c > '9') {
        return 0;
    }
    return inteiro(palavra, i + 1);
}

int real(char palavra[], int i, int acc) {
    if (palavra[i] == '\0') {
        return 1;
    }
    char c = palavra[i];
    if (c == '.' || c == ',') {
        acc++;
        if (acc > 1) {
            return 0;
        }
    } else if (c < '0' || c > '9') {
        return 0;
    }
    return real(palavra, i + 1, acc);
}

int main() {
    char palavra[500];

    while (fgets(palavra, sizeof(palavra), stdin) != NULL) {
        int tamanho = 0;
        while (palavra[tamanho] != '\0' && palavra[tamanho] != '\n') {
            tamanho++;
        }
        palavra[tamanho] = '\0';

        int resp1 = vogal(palavra, 0);
        int resp2 = consoante(palavra, 0);
        int resp3 = inteiro(palavra, 0);
        int resp4 = real(palavra, 0, 0);

        printf("%s %s %s %s\n", 
               resp1 ? "SIM" : "NAO", 
               resp2 ? "SIM" : "NAO", 
               resp3 ? "SIM" : "NAO", 
               resp4 ? "SIM" : "NAO");
    }

    return 0;
}