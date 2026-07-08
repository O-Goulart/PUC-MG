#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    int hora;
    int minuto;
} Hora;

typedef struct {
    int ano;
    int mes;
    int dia;
} Data;

typedef struct {
    int id;
    char nome[100];
    char cidade[100];
    int capacidade;
    double avaliacao;
    char tipos_cozinha[20][50];
    int num_tipos;              
    int faixa_preco;
    Hora horario_abertura;
    Hora horario_fechamento;
    Data data_abertura;
    bool aberto;
} Restaurante;

Restaurante restaurantes[1000];
int tamanhoColecao = 0;

int comparacoesPesquisa = 0;

void lerCsv() {
    FILE *arquivo = fopen("/tmp/restaurantes.csv", "r");
    if (arquivo == NULL) {
        arquivo = fopen("restaurantes.csv", "r");
        if (arquivo == NULL) return;
    }

    char linha[1024];
    
    if (fgets(linha, sizeof(linha), arquivo) == NULL) return;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';
        Restaurante dinner; 
        char *campos[10];
        int c = 0;
        char *token = strtok(linha, ",");
        while (token != NULL && c < 10) {
            campos[c] = token;
            c++;
            token = strtok(NULL, ",");
        }

        dinner.id = atoi(campos[0]);
        strcpy(dinner.nome, campos[1]);
        strcpy(dinner.cidade, campos[2]);
        dinner.capacidade = atoi(campos[3]);
        dinner.avaliacao = atof(campos[4]);
        
        dinner.num_tipos = 0;
        char *tipo = strtok(campos[5], ";");
        while (tipo != NULL) {
            strcpy(dinner.tipos_cozinha[dinner.num_tipos], tipo);
            dinner.num_tipos++;
            tipo = strtok(NULL, ";");
        }

        dinner.faixa_preco = strlen(campos[6]);
        sscanf(campos[7], "%d:%d-%d:%d", &dinner.horario_abertura.hora, &dinner.horario_abertura.minuto, &dinner.horario_fechamento.hora, &dinner.horario_fechamento.minuto);
        sscanf(campos[8], "%d-%d-%d", &dinner.data_abertura.ano, &dinner.data_abertura.mes, &dinner.data_abertura.dia);

        if (strcmp(campos[9], "true") == 0) {
            dinner.aberto = true;
        } else {
            dinner.aberto = false;
        }

        restaurantes[tamanhoColecao] = dinner;
        tamanhoColecao++;
    }
    fclose(arquivo);
}

void sort(Restaurante *array, int n) {
    for (int i = 0; i < (n - 1); i++) {
        int menor = i;
        for (int j = (i + 1); j < n; j++) {
            if (strcmp(array[menor].nome, array[j].nome) > 0) {
                menor = j;
            }
        }
        if (menor != i) {
            Restaurante aux = array[i];
            array[i] = array[menor];
            array[menor] = aux;
        }
    }
}

bool pesqBin(Restaurante *vet, int n, char *x) {
    bool resp = false;
    int dir = n - 1, esq = 0, meio;

    while (esq <= dir) {
        meio = (esq + dir) / 2;
        int cmp = strcmp(x, vet[meio].nome);
        
        comparacoesPesquisa++;
        if (cmp == 0) {
            resp = true;
            esq = dir + 1;
        } else {
            comparacoesPesquisa++;
            if (cmp > 0) {
                esq = meio + 1;
            } else {
                dir = meio - 1;
            }
        }
    }
    return resp;
}

int main() {
    lerCsv();
    Restaurante selecionados[1000];
    int num_selecionados = 0;
    char entrada[100];

    while (scanf(" %[^\r\n]", entrada) == 1) {
        if (strcmp(entrada, "FIM") == 0 || strcmp(entrada, "-1") == 0) {
            break;
        }

        int idBuscado = atoi(entrada);
        
        for (int i = 0; i < tamanhoColecao; i++) {
            if (restaurantes[i].id == idBuscado) {
                selecionados[num_selecionados] = restaurantes[i];
                num_selecionados++;
                break; 
            }
        }
    }

    sort(selecionados, num_selecionados);

    clock_t inicio_tempo = clock();

    while (scanf(" %[^\r\n]", entrada) == 1) {
        if (strcmp(entrada, "FIM") == 0) {
            break;
        }

        bool achou = pesqBin(selecionados, num_selecionados, entrada);
        if (achou) {
            printf("SIM\n");
        } else {
            printf("NAO\n");
        }
    }

    clock_t fim_tempo = clock();
    double tempo_execucao = ((double)(fim_tempo - inicio_tempo)) / CLOCKS_PER_SEC;

    FILE *log = fopen("855217_binaria.txt", "w");
    if (log != NULL) {
        fprintf(log, "855217\t%d\t%f\n", comparacoesPesquisa, tempo_execucao);
        fclose(log);
    }

    return 0;
}