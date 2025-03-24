#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>

// Definições de tipos e structs
typedef struct {
    char dia[4];
} DiaDeAula;

typedef struct {
    char codigo[50];
} prerequisito;

typedef struct {
    int id;
    char codigo[8];
    char nome[50];
    int periodo;
    prerequisito prerequisitos[100];
    int qtdPreRequisitos;
    int cargaHoraria;
    // 0 - obrigatória, 1 - computação visual, 2 - sistemas inteligentes,
    // 3 - sistemas de computação, 4 - sistemas de informação
    char enfase[5];
    int escolha;
    int completa;
    int alocada;
    char horarioDeAula[5];
    DiaDeAula diasDeAula[2];
    int qtdDiasDeAula;
} Materia;

// Protótipos das funções
int compararMaterias(const void *a, const void *b);
void ordenarMateriasPorRequisitos(Materia *disciplinas, int tamanho);
void processarDiasDeAula(char *diasStr, Materia *disciplina);
void processarPreRequisitos(char *preRequisitosStr, Materia *disciplina);
void preencherMaterias(Materia *disciplina, FILE *arquivo);
void printarMateria(Materia m, int id, int cod, int nome, int per, int pre, int ch, int enf, int esc, int comp, int hora, int dia, int aloc);
int numDiferenca(char *str1, char *str2);
int verificaMateriaColide(int periodo[11][6], char *horarioDeAula, DiaDeAula *dias, char *codigo, int semestre, Materia *disciplinas);
int verificaPreReq(Materia disciplina, Materia *oferta, int *periodo);
int somaCargaHorariaPeriodo(int periodo[11][6], int semestre, Materia *disciplinas);

#endif
