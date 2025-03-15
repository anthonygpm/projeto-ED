#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define TAM 50
#define VAZIO -999

typedef struct {
    char codigo[TAM];
} prerequisito;

typedef struct {
    char codigo[TAM];
    char nome[TAM]; 
    int periodo;
    prerequisito prerequisitos[TAM];
    int qtdPreRequisitos;
    int cargaHoraria;
    char enfase[TAM];
    int completa;
    char horarioDeAula[TAM];
    char diaDeAula[TAM];
} Materia;

typedef struct {
    Materia disciplina[TAM];
} Ofertas;

void processarPreRequisitos(char *preRequisitosStr, Materia *materia) {
    materia->qtdPreRequisitos = 0; 
    int len = strlen(preRequisitosStr);
    int i = 0;

    while (i < len) {
        if (strncmp(&preRequisitosStr[i], "COMP", 4) == 0) {
            strncpy(materia->prerequisitos[materia->qtdPreRequisitos].codigo, &preRequisitosStr[i], 7);
            materia->prerequisitos[materia->qtdPreRequisitos].codigo[7] = '\0'; 
            materia->qtdPreRequisitos++;
            i += 7; 
        } else {
            i++;
        }
    }
}

void printarMateria(Materia m) {
    printf("------------------------\n");
    printf("Código: %s\n", m.codigo);
    printf("Nome: %s\n", m.nome);
    printf("Período: %d\n", m.periodo);
    printf("Pré-requisitos: ");
    if(m.qtdPreRequisitos == 0) printf("Nenhum\n");
    else{
        for (int k = 0; k < m.qtdPreRequisitos; k++) {
            printf("%s ", m.prerequisitos[k].codigo);
    }}
    printf("\nCarga Horária: %d\n", m.cargaHoraria);
    printf("Ênfase: %s\n", m.enfase);
    printf("Completa: %d\n", m.completa);
    printf("Horário de Aula: %s\n", m.horarioDeAula);
    printf("Dia de Aula: %s\n", m.diaDeAula);
    printf("------------------------\n");
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    Ofertas oferta;
    FILE *arquivo = fopen("materias.txt", "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int i = 0;
    char linha[256];

    while (fgets(linha, sizeof(linha), arquivo) != NULL && i < 46) {
        int periodo, cargaHoraria, completa;
        char codigo[TAM], nome[TAM], preRequisitosStr[256], enfase[TAM], horarioDeAula[TAM], diaDeAula[TAM];

        sscanf(linha, "%s %s %d %s %d %s %d %s %s", codigo, nome, &periodo, preRequisitosStr, &cargaHoraria, enfase, &completa, horarioDeAula, diaDeAula);

        strcpy(oferta.disciplina[i].codigo, codigo);
        strcpy(oferta.disciplina[i].nome, nome);
        oferta.disciplina[i].periodo = periodo;
        oferta.disciplina[i].cargaHoraria = cargaHoraria;
        strcpy(oferta.disciplina[i].enfase, enfase);
        oferta.disciplina[i].completa = completa;
        strcpy(oferta.disciplina[i].horarioDeAula, horarioDeAula);
        strcpy(oferta.disciplina[i].diaDeAula, diaDeAula);

        processarPreRequisitos(preRequisitosStr, &oferta.disciplina[i]);
        i++;
    }

    fclose(arquivo);

    for (int j = 0; j < i; j++) {
        printarMateria(oferta.disciplina[j]);
    }

    return 0;
}
