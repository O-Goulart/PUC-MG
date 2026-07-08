#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

int tamanho() {
    int tam = 0;
    for (Celula* i = primeiro->prox; i != NULL; i = i->prox) {
        tam++;
    }
    return tam;
}

void inserirInicio(Restaurante x) {
    Celula* tmp = novaCelula(x);
    tmp->prox = primeiro->prox;
    primeiro->prox = tmp;
    if (primeiro == ultimo) {                    
        ultimo = tmp;
    }
}

void inserirFim(Restaurante x) {
    ultimo->prox = novaCelula(x);
    ultimo = ultimo->prox;
}

void inserir(Restaurante x, int pos) {
    int tam = tamanho();
    if (pos < 0 || pos > tam) return;

    if (pos == 0) {
        inserirInicio(x);
    } else if (pos == tam) {
        inserirFim(x);
    } else {
        Celula* i = primeiro;
        for (int j = 0; j < pos; j++, i = i->prox);
        
        Celula* tmp = novaCelula(x);
        tmp->prox = i->prox;
        i->prox = tmp;
    }
}

Restaurante removerInicio() {
    if (primeiro == ultimo) exit(1);
    
    Celula* tmp = primeiro->prox;
    Restaurante resp = tmp->elemento;
    
    primeiro->prox = tmp->prox;
    if (ultimo == tmp) {
        ultimo = primeiro;
    }
    free(tmp);
    return resp;
}

Restaurante removerFim() {
    if (primeiro == ultimo) exit(1);
    
    Celula* i;
    for (i = primeiro; i->prox != ultimo; i = i->prox);
    
    Restaurante resp = ultimo->elemento;
    free(ultimo);
    ultimo = i;
    ultimo->prox = NULL;
    
    return resp;
}

Restaurante remover(int pos) {
    int tam = tamanho();
    if (primeiro == ultimo || pos < 0 || pos >= tam) exit(1);
    
    if (pos == 0) {
        return removerInicio();
    } else if (pos == tam - 1) {
        return removerFim();
    } else {
        Celula* i = primeiro;
        for (int j = 0; j < pos; j++, i = i->prox);
        
        Celula* tmp = i->prox;
        Restaurante resp = tmp->elemento;
        
        i->prox = tmp->prox;
        free(tmp);
        
        return resp;
    }
}

void mostrar() {
    int j = 0;
    for (Celula* i = primeiro->prox; i != NULL; i = i->prox, j++) {
        imprimir(i->elemento);
    }
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

    int numOps;
    if (scanf("%d", &numOps) == 1) {
        for (int k = 0; k < numOps; k++) {
            char comando[5];
            scanf("%s", comando);

            if (strcmp(comando, "II") == 0) {
                int id;
                scanf("%d", &id);
                for (int i = 0; i < tamanhoColecao; i++) {
                    if (restaurantes[i].id == id) {
                        inserirInicio(restaurantes[i]);
                        break;
                    }
                }
            } 
            else if (strcmp(comando, "I*") == 0) {
                int pos, id;
                scanf("%d %d", &pos, &id);
                for (int i = 0; i < tamanhoColecao; i++) {
                    if (restaurantes[i].id == id) {
                        inserir(restaurantes[i], pos);
                        break;
                    }
                }
            } 
            else if (strcmp(comando, "IF") == 0) {
                int id;
                scanf("%d", &id);
                for (int i = 0; i < tamanhoColecao; i++) {
                    if (restaurantes[i].id == id) {
                        inserirFim(restaurantes[i]);
                        break;
                    }
                }
            } 
            else if (strcmp(comando, "RI") == 0) {
                Restaurante removido = removerInicio();
                printf("(R)%s\n", removido.nome);
            } 
            else if (strcmp(comando, "R*") == 0) {
                int pos;
                scanf("%d", &pos);
                Restaurante removido = remover(pos);
                printf("(R)%s\n", removido.nome);
            } 
            else if (strcmp(comando, "RF") == 0) {
                Restaurante removido = removerFim();
                printf("(R)%s\n", removido.nome);
            }
        }
    }
    mostrar();

    return 0;
}