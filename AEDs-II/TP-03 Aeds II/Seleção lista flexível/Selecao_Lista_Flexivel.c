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
int numComparacoes = 0;
int numMovimentacoes = 0;

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
        dinner.aberto = (strcmp(campos[9], "true") == 0);

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
    for (int i = 0; i < dinner.faixa_preco; i++) printf("$");
    
    printf(" ## %02d:%02d-%02d:%02d ## %02d/%02d/%04d ## %s]\n", 
            dinner.horario_abertura.hora, dinner.horario_abertura.minuto, 
            dinner.horario_fechamento.hora, dinner.horario_fechamento.minuto,
            dinner.data_abertura.dia, dinner.data_abertura.mes, dinner.data_abertura.ano,
            dinner.aberto ? "true" : "false");
}

typedef struct Celula {
    Restaurante elemento;
    struct Celula* prox;
} Celula;

Celula* novaCelula(Restaurante elemento) {
    Celula* nova = (Celula*) malloc(sizeof(Celula));
    nova->elemento = elemento;
    nova->prox = NULL;
    return nova;
}

Celula* primeiro;
Celula* ultimo;

void start() {
    Restaurante cabeca;
    cabeca.id = -1; // Célula cabeça
    primeiro = novaCelula(cabeca);
    ultimo = primeiro;
}

void inserirFim(Restaurante x) {
    ultimo->prox = novaCelula(x);
    ultimo = ultimo->prox;
}

void mostrar() {
    for (Celula* i = primeiro->prox; i != NULL; i = i->prox) {
        imprimir(i->elemento);
    }
}

void selecao() {
    for (Celula* i = primeiro->prox; i != NULL; i = i->prox) {
        Celula* menor = i;
        
        for (Celula* j = i->prox; j != NULL; j = j->prox) {
            numComparacoes++;
            if (strcmp(j->elemento.nome, menor->elemento.nome) < 0) {
                menor = j;
            }
        }
        
        if (menor != i) {
            Restaurante tmp = i->elemento;
            i->elemento = menor->elemento;
            menor->elemento = tmp;
            numMovimentacoes += 3; // Troca de 3 elementos
        }
    }
}

void criarLog(clock_t tempoInicial, clock_t tempoFinal) {
    FILE *log = fopen("855217_selecao_flexivel.txt", "w");
    double tempoExecucao = ((double)(tempoFinal - tempoInicial)) / CLOCKS_PER_SEC * 1000.0;
    fprintf(log, "855217\t%d\t%d\t%.2lfms", numComparacoes, numMovimentacoes, tempoExecucao);
    fclose(log);
}

int main() {
    lerCsv();
    start();
    
    char entrada[100];

    while (scanf("%s", entrada) == 1) {
        if (strcmp(entrada, "FIM") == 0 || strcmp(entrada, "-1") == 0) {
            break;
        }

        int idBuscado = atoi(entrada);
        for (int i = 0; i < tamanhoColecao; i++) {
            if (restaurantes[i].id == idBuscado) {
                inserirFim(restaurantes[i]);
                break; 
            }
        }
    }


    clock_t tempoInicial = clock();
    
    selecao();
    
    clock_t tempoFinal = clock();
    
    mostrar();
    criarLog(tempoInicial, tempoFinal);

    return 0;
}