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

typedef struct No {
    char letra;
    struct No* esq;
    struct No* dir;
    Celula* primeiro;
} No;

No* raiz = NULL;

void inserirLista(No* no, Restaurante x) {
    Celula* i = no->primeiro;
    while (i->prox != NULL && strcmp(x.nome, i->prox->elemento.nome) > 0) {
        i = i->prox;
    }
    Celula* tmp = (Celula*)malloc(sizeof(Celula));
    tmp->elemento = x;
    tmp->prox = i->prox;
    i->prox = tmp;
}

No* inserirRec(No* i, Restaurante x) {
    char letra = x.nome[0];
    if (i == NULL) {
        i = (No*)malloc(sizeof(No));
        i->letra = letra;
        i->esq = i->dir = NULL;
        i->primeiro = (Celula*)malloc(sizeof(Celula));
        i->primeiro->prox = NULL;
        inserirLista(i, x);
    } else if (letra < i->letra) {
        i->esq = inserirRec(i->esq, x);
    } else if (letra > i->letra) {
        i->dir = inserirRec(i->dir, x);
    } else {
        inserirLista(i, x);
    }
    return i;
}

void inserir(Restaurante x) {
    raiz = inserirRec(raiz, x);
}

Restaurante* pesquisarRec(No* i, char* nome) {
    if (i == NULL) {
        return NULL;
    }
    char letra = nome[0];
    if (letra == i->letra) {
        Celula* j = i->primeiro->prox;
        while (j != NULL) {
            numComparacoes++;
            int comp = strcmp(nome, j->elemento.nome);
            if (comp == 0) {
                return &(j->elemento);
            } else if (comp < 0) {
                return NULL;
            }
            printf("%s ", j->elemento.nome);
            j = j->prox;
        }
        return NULL;
    } else if (letra < i->letra) {
        printf("ESQ ");
        return pesquisarRec(i->esq, nome);
    } else {
        printf("DIR ");
        return pesquisarRec(i->dir, nome);
    }
}

int main() {
    lerCsv();
    
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
        
        printf("RAIZ ");
        Restaurante* r = pesquisarRec(raiz, nomeBuscado);
        if (r != NULL) {
            printf("SIM ");
            imprimir(*r);
        } else {
            printf("NAO\n");
        }
    }

    clock_t fimTime = clock();
    double tempoTotalms = ((double)(fimTime - inicioTime)) / CLOCKS_PER_SEC * 1000.0;

    FILE* log = fopen("855217_hibrida_arvore_lista.txt", "w");
    if (log != NULL) {
        fprintf(log, "855217\t%d\t%.2lfms\n", numComparacoes, tempoTotalms);
        fclose(log);
    }

    return 0;
}