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

typedef struct No {
    Restaurante elemento;
    struct No* esq;
    struct No* dir;
    bool cor; 
} No;

No* raiz = NULL;

No* rotacionarEsq(No* no) {
    No* noDir = no->dir;
    No* noDirEsq = noDir->esq;
    noDir->esq = no;
    no->dir = noDirEsq;
    return noDir;
}

No* rotacionarDir(No* no) {
    No* noEsq = no->esq;
    No* noEsqDir = noEsq->dir;
    noEsq->dir = no;
    no->esq = noEsqDir;
    return noEsq;
}

void balancear(No* bisavo, No* avo, No* pai, No* i) {
    if (pai->cor == true) {
        if (pai == avo->dir) {
            if (i == pai->dir) {
                avo->cor = true;
                pai->cor = false;
                No* tmp = rotacionarEsq(avo);
                if (bisavo == NULL) raiz = tmp;
                else if (bisavo->esq == avo) bisavo->esq = tmp;
                else bisavo->dir = tmp;
            } else {
                avo->cor = true;
                i->cor = false;
                avo->dir = rotacionarDir(pai);
                No* tmp = rotacionarEsq(avo);
                if (bisavo == NULL) raiz = tmp;
                else if (bisavo->esq == avo) bisavo->esq = tmp;
                else bisavo->dir = tmp;
            }
        } else {
            if (i == pai->esq) {
                avo->cor = true;
                pai->cor = false;
                No* tmp = rotacionarDir(avo);
                if (bisavo == NULL) raiz = tmp;
                else if (bisavo->esq == avo) bisavo->esq = tmp;
                else bisavo->dir = tmp;
            } else {
                avo->cor = true;
                i->cor = false;
                avo->esq = rotacionarEsq(pai);
                No* tmp = rotacionarDir(avo);
                if (bisavo == NULL) raiz = tmp;
                else if (bisavo->esq == avo) bisavo->esq = tmp;
                else bisavo->dir = tmp;
            }
        }
    }
}

void inserirRec(Restaurante x, No* bisavo, No* avo, No* pai, No* i) {
    if (i == NULL) {
        i = (No*)malloc(sizeof(No));
        i->elemento = x;
        i->esq = i->dir = NULL;
        i->cor = true;
        if (pai == NULL) {
            raiz = i;
        } else {
            if (strcmp(x.nome, pai->elemento.nome) < 0) pai->esq = i;
            else pai->dir = i;
        }
        balancear(bisavo, avo, pai, i);
    } else {
        if (i->esq != NULL && i->esq->cor == true && i->dir != NULL && i->dir->cor == true) {
            i->cor = true;
            i->esq->cor = false;
            i->dir->cor = false;
            if (i == raiz) i->cor = false;
            else balancear(bisavo, avo, pai, i);
        }
        int comp = strcmp(x.nome, i->elemento.nome);
        if (comp < 0) {
            inserirRec(x, avo, pai, i, i->esq);
        } else if (comp > 0) {
            inserirRec(x, avo, pai, i, i->dir);
        }
    }
}

void inserir(Restaurante x) {
    if (raiz == NULL) {
        raiz = (No*)malloc(sizeof(No));
        raiz->elemento = x;
        raiz->esq = raiz->dir = NULL;
        raiz->cor = false;
    } else if (raiz->esq != NULL && raiz->esq->cor == true && raiz->dir != NULL && raiz->dir->cor == true) {
        raiz->esq->cor = false;
        raiz->dir->cor = false;
        inserirRec(x, NULL, NULL, NULL, raiz);
    } else {
        inserirRec(x, NULL, NULL, NULL, raiz);
    }
    raiz->cor = false;
}

void pesquisar(No* i, char* nome) {
    if (i == NULL) {
        printf("NAO\n");
        return;
    }
    numComparacoes++;
    int comp = strcmp(nome, i->elemento.nome);
    if (comp == 0) {
        printf("SIM\n");
    } else if (comp < 0) {
        printf("esq ");
        pesquisar(i->esq, nome);
    } else {
        printf("dir ");
        pesquisar(i->dir, nome);
    }
}

void caminharEmOrdem(No* i) {
    if (i != NULL) {
        caminharEmOrdem(i->esq);
        imprimir(i->elemento);
        caminharEmOrdem(i->dir);
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
        printf("raiz ");
        pesquisar(raiz, nomeBuscado);
    }

    clock_t fimTime = clock();
    double tempoTotalms = ((double)(fimTime - inicioTime)) / CLOCKS_PER_SEC * 1000.0;

    caminharEmOrdem(raiz);

    FILE* log = fopen("855217_arvore_bicolor.txt", "w");
    if (log != NULL) {
        fprintf(log, "855217\t%d\t%.2lfms\n", numComparacoes, tempoTotalms);
        fclose(log);
    }

    return 0;
}