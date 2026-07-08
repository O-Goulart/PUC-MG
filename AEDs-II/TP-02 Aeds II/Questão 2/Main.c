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
    // estou fatiando a linha inteira pelas vírgulas e guardando no vetor(fiz isso especificamente por causa do tipos de restaurantes, que envolvem o ponto e virgula)
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


int main() {
    lerCsv(); 
    
    char entrada[100];

    while (scanf("%s", entrada) == 1) {
       //strcmp é igual a entrada.equals no Java)
        if (strcmp(entrada, "FIM") == 0 || strcmp(entrada, "-1") == 0) {
            break;
        }

        int idBuscado = atoi(entrada); // atoi() transforma String em Inteiro

        for (int i = 0; i < tamanhoColecao; i++) {
            if (restaurantes[i].id == idBuscado) {
                imprimir(restaurantes[i]);
                break;
            }
        }
    }

    return 0;
}