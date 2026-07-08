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

    for (int i = 0; i < dinner.faixa_preco; i++) {
        printf("$");
    }
    printf(" ## ");

    printf("%02d:%02d-%02d:%02d ## ", dinner.horario_abertura.hora, dinner.horario_abertura.minuto, dinner.horario_fechamento.hora, dinner.horario_fechamento.minuto);
    printf("%02d/%02d/%04d ## ", dinner.data_abertura.dia, dinner.data_abertura.mes, dinner.data_abertura.ano);

    if (dinner.aberto) printf("true]\n");
    else printf("false]\n");
}

typedef struct Celula {
    Restaurante elemento;
    struct Celula* prox;
} Celula;

Celula* tabela[31];

void start() {
    for (int i = 0; i < 31; i++) {
        tabela[i] = NULL;
    }
}

int hash(char* nome) {
    int soma = 0;
    int len = strlen(nome);
    for (int i = 0; i < len; i++) {
        soma += nome[i];
    }
    return soma % 31;
}

void inserir(Restaurante x) {
    int pos = hash(x.nome);
    Celula* nova = (Celula*)malloc(sizeof(Celula));
    nova->elemento = x;
    nova->prox = NULL;
    
    if (tabela[pos] == NULL) {
        tabela[pos] = nova;
    } else {
        Celula* i = tabela[pos];
        while (i->prox != NULL) {
            i = i->prox;
        }
        i->prox = nova;
    }
}

void pesquisar(char* nome) {
    int pos = hash(nome);
    Celula* i = tabela[pos];
    
    while (i != NULL) {
        numComparacoes++;
        if (strcmp(i->elemento.nome, nome) == 0) {
            printf("%d ", pos);
            imprimir(i->elemento);
            return;
        }
        i = i->prox;
    }
    
    printf("-1\n");
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
                inserir(restaurantes[i]);
                break; 
            }
        }
    }

    clock_t inicioTime = clock();

    char nomeBuscado[100];
    while (scanf(" %[^\r\n]", nomeBuscado) == 1) {
        nomeBuscado[strcspn(nomeBuscado, "\r\n")] = '\0';
        if (strcmp(nomeBuscado, "FIM") == 0 || strcmp(nomeBuscado, "-1") == 0) {
            break;
        }
        
        pesquisar(nomeBuscado);
    }

    clock_t fimTime = clock();
    double tempoTotalms = ((double)(fimTime - inicioTime)) / CLOCKS_PER_SEC * 1000.0;

    FILE* log = fopen("855217_hash_indireta.txt", "w");
    if (log != NULL) {
        fprintf(log, "855217\t%d\t%.2lfms\n", numComparacoes, tempoTotalms);
        fclose(log);
    }

    return 0;
}