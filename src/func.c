#include "header.h"
#include <stdlib.h>
#include <string.h>

int compararMaterias(const void *a, const void *b)
{
    Materia *materiaA = (Materia *)a;
    Materia *materiaB = (Materia *)b;
    return materiaA->qtdPreRequisitos - materiaB->qtdPreRequisitos;
}

void ordenarMateriasPorRequisitos(Materia *disciplinas, int tamanho)
{
    qsort(disciplinas, tamanho, sizeof(Materia), compararMaterias);
}

void processarDiasDeAula(char *diasStr, Materia *disciplina)
{
    disciplina->qtdDiasDeAula = 0;
    int len = strlen(diasStr);
    int i = 0;

    while (i < len && disciplina->qtdDiasDeAula < 2)
    {
        strncpy(disciplina->diasDeAula[disciplina->qtdDiasDeAula].dia, &diasStr[i], 3);
        disciplina->diasDeAula[disciplina->qtdDiasDeAula].dia[3] = '\0'; // Garante terminação da string
        disciplina->qtdDiasDeAula++;
        i += 3; // Avança para o próximo grupo de três caracteres
    }
}

void processarPreRequisitos(char *preRequisitosStr, Materia *disciplina)
{
    disciplina->qtdPreRequisitos = 0;
    int len = strlen(preRequisitosStr);
    int i = 0;

    while (i < len)
    {
        if (strncmp(&preRequisitosStr[i], "COMP", 4) == 0)
        {
            strncpy(disciplina->prerequisitos[disciplina->qtdPreRequisitos].codigo, &preRequisitosStr[i], 7);
            disciplina->prerequisitos[disciplina->qtdPreRequisitos].codigo[7] = '\0';
            disciplina->qtdPreRequisitos++;
            i += 7;
        }
        else
            i++;
    }
}

void preencherMaterias(Materia *disciplina, FILE *arquivo)
{
    int i = 0;
    char linha[256];

    while (fgets(linha, sizeof(linha), arquivo) != NULL && i < 46)
    {
        int periodo, cargaHoraria, completa, id;
        char codigo[8], nome[50], preRequisitosStr[100], enfase[5], horarioDeAula[5], diasDeAulaStr[50];

        sscanf(linha, "%d %s %s %d %s %d %s %d %s %s", &id, codigo, nome, &periodo, preRequisitosStr, &cargaHoraria, enfase, &completa, horarioDeAula, diasDeAulaStr);

        disciplina[i].id = id;
        strcpy(disciplina[i].codigo, codigo);
        strcpy(disciplina[i].nome, nome);
        disciplina[i].periodo = periodo;
        disciplina[i].cargaHoraria = cargaHoraria;
        strcpy(disciplina[i].enfase, enfase);
        disciplina[i].escolha = 0;
        disciplina[i].completa = completa;
        disciplina[i].alocada = 0;
        strcpy(disciplina[i].horarioDeAula, horarioDeAula);

        processarPreRequisitos(preRequisitosStr, &disciplina[i]);
        processarDiasDeAula(diasDeAulaStr, &disciplina[i]);

        i++;
    }
}

void printarMateria(Materia m, int id, int cod, int nome, int per, int pre, int ch, int enf, int esc, int comp, int hora, int dia, int aloc)
{
    printf("------------------------\n");
    if (id)
        printf("ID: %d\n", m.id);
    if (cod)
        printf("Código: %s\n", m.codigo);
    if (nome)
        printf("Nome: %s\n", m.nome);
    if (per)
    {
        printf("Período: ");
        if (m.periodo == -999)
            printf("Optativa\n");
        else
            printf("%d°\n", m.periodo);
    }
    if (pre)
    {
        printf("Pré-requisitos: ");
        if (m.qtdPreRequisitos == 0)
            printf("Nenhum\n");
        else
        {
            for (int i = 0; i < m.qtdPreRequisitos; i++)
                printf("%s ", m.prerequisitos[i].codigo);
            printf("\n");
        }
    }
    if (ch)
        printf("Carga Horária: %d\n", m.cargaHoraria);
    if (enf)
        printf("Ênfase: %s\n", m.enfase);
    if (esc)
        printf("Escolha: %d\n", m.escolha);
    if (comp)
        printf("Completa: %d\n", m.completa);
    if (hora)
        printf("Horário de Aula: %s\n", m.horarioDeAula);
    if (dia)
    {
        printf("Dias de Aula: ");
        for (int i = 0; i < m.qtdDiasDeAula; i++)
            printf("%s ", m.diasDeAula[i].dia);
        printf("\n");
    }
    if (aloc)
        printf("Alocada: %d\n", m.alocada);
}

int numDiferenca(char *str1, char *str2)
{
    int diferencas = 0;
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int maxLen = (len1 > len2) ? len1 : len2;

    for (int i = 0; i < maxLen; i++)
    {
        if (str1[i] != str2[i])
        {
            diferencas++;
        }
    }

    return diferencas;
}

int verificaMateriaColide(int periodo[11][6], char *horarioDeAula, DiaDeAula *dias, char *codigo, int semestre, Materia *disciplinas)
{
    for (int j = 0; j < 6; j++)
    {
        if (periodo[semestre][j] == -1)
            break;

        int i;
        for (i = 0; i < 46; i++)
        {
            if (disciplinas[i].id == periodo[semestre][j])
                break;
        }

        if (strcmp(disciplinas[i].codigo, codigo) == 0)
            continue;

        for (int z = 0; z < disciplinas[i].qtdDiasDeAula; z++)
        {
            char dia[4];
            strncpy(dia, dias[z].dia, 3);
            if (strcmp(dia, disciplinas[i].diasDeAula[z].dia) == 0)
            {
                if (numDiferenca(horarioDeAula, disciplinas[i].horarioDeAula) < 2)
                    return 1;
            }
        }
    }

    return 0;
}

int verificaPreReq(Materia disciplina, Materia *oferta, int *periodo)
{
    if (disciplina.qtdPreRequisitos == 0)
        return 1;

    for (int i = 0; i < disciplina.qtdPreRequisitos; i++)
    {
        int requisitoCumprido = 0;
        for (int j = 0; j < 46; j++)
        {
            if (strcmp(disciplina.prerequisitos[i].codigo, oferta[j].codigo) == 0 &&
                (oferta[j].completa == 1))
            {
                requisitoCumprido = 1;
                break;
            }
        }
        if (!requisitoCumprido)
            return 0;
    }

    return 1;
}

int somaCargaHorariaPeriodo(int periodo[11][6], int semestre, Materia *disciplinas)
{
    int soma = 0;
    for (int j = 0; j < 6; j++)
    {
        if (periodo[semestre][j] == -1)
            break;

        for (int i = 0; i < 46; i++)
        {
            if (disciplinas[i].id == periodo[semestre][j])
            {
                soma += disciplinas[i].cargaHoraria;
                break;
            }
        }
    }
    return soma;
}
