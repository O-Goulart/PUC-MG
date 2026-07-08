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

// CONTADORES PARA O ARQUIVO DE LOG
int comparacoes = 0;
int movimentacoes = 0;

void lerCsv() {
    FILE *arquivo = fopen("/tmp/restaurantes.csv", "r");
    if (arquivo == NULL) {
        arquivo = fopen("restaurantes.csv", "r");
        if (arquivo == NULL) {
            printf("Arquivo não encontrado!\n");
            return;
        }
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

void imprimir(Restaurante dinner) {
    printf("[%d ## %s ## %s ## %d ## %.1f ## [", dinner.id, dinner.nome, dinner.cidade, dinner.capacidade, dinner.avaliacao);
    
    for (int i = 0; i < dinner.num_tipos; i++) {
        printf("%s", dinner.tipos_cozinha[i]);
        if (i < dinner.num_tipos - 1) printf(",");
    }
    printf("] ## ");

    for (int i = 0; i < dinner.faixa_preco; i++) {
        printf("$");
    }
    printf(" ## ");

    printf("%02d:%02d-%02d:%02d ## ", dinner.horario_abertura.hora, dinner.horario_abertura.minuto, dinner.horario_fechamento.hora, dinner.horario_fechamento.minuto);
    printf("%02d/%02d/%04d ## ", dinner.data_abertura.dia, dinner.data_abertura.mes, dinner.data_abertura.ano);

    if (dinner.aberto) printf("true]\n");
    else printf("false]\n");
}

// Função para comparar a Data de Abertura com desempate por Nome
int comparar(Restaurante a, Restaurante b) {
    comparacoes++;
    if (a.data_abertura.ano != b.data_abertura.ano) {
        return a.data_abertura.ano - b.data_abertura.ano;
    }
    comparacoes++;
    if (a.data_abertura.mes != b.data_abertura.mes) {
        return a.data_abertura.mes - b.data_abertura.mes;
    }
    comparacoes++;
    if (a.data_abertura.dia != b.data_abertura.dia) {
        return a.data_abertura.dia - b.data_abertura.dia;
    }
    
    comparacoes++;
    return strcmp(a.nome, b.nome);
}

void swap(Restaurante *i, Restaurante *j) {
    Restaurante temp = *i;
    *i = *j;
    *j = temp;
    movimentacoes += 3;
}

// Reconstrução do Max-Heap
void reconstruir(Restaurante *array, int tam, int i) {
    int maior = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < tam && comparar(array[esq], array[maior]) > 0) {
        maior = esq;
    }
    if (dir < tam && comparar(array[dir], array[maior]) > 0) {
        maior = dir;
    }

    if (maior != i) {
        swap(&array[i], &array[maior]);
        reconstruir(array, tam, maior);
    }
}

// Constrói o Max-Heap inicial
void construir(Restaurante *array, int tam) {
    for (int i = tam / 2 - 1; i >= 0; i--) {
        reconstruir(array, tam, i);
    }
}

void heapsortParcial(Restaurante *array, int n, int k) {
    int limite = (k < n) ? k : n;

    // Constrói o Max-Heap com os 'k' primeiros elementos
    construir(array, limite);

    //Se o meu elemento for menor que a raiz do Max-Heap, eu troco a raiz
    for (int i = limite; i < n; i++) {
        if (comparar(array[i], array[0]) < 0) {
            swap(&array[i], &array[0]);
            reconstruir(array, limite, 0);
        }
    }

    //Ordena os 'k' elementos do Heap
    for (int i = limite - 1; i > 0; i--) {
        swap(&array[0], &array[i]);
        reconstruir(array, i, 0);
    }
}

int main() {
    lerCsv();
    Restaurante selecionados[1000];
    int num_selecionados = 0;
    char entrada[100];

    while (scanf("%s", entrada) == 1) {
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

    clock_t inicio_tempo = clock();
    
    // Executa a Ordenação Parcial por Heapsort com k = 10
    heapsortParcial(selecionados, num_selecionados, 10);

    clock_t fim_tempo = clock();
    double tempo_execucao = ((double)(fim_tempo - inicio_tempo)) / (CLOCKS_PER_SEC / 1000.0);

    for (int i = 0; i < num_selecionados; i++) {
        imprimir(selecionados[i]);
    }

    FILE *log = fopen("855217_heapsort_parcial.txt", "w");
    if (log != NULL) {
        fprintf(log, "855217\t%d\t%d\t%.0fms\n", comparacoes, movimentacoes, tempo_execucao);
        fclose(log);
    }

    return 0;
}