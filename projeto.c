#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define TAM 50
#define VAZIO -999

typedef struct {
    char codigo[TAM];
    char nome[TAM]; 
    int periodo;
    char codigoPreRequisito[TAM];
    int cargaHoraria;
    // 0 - obrigat�ria, 1 - computa��o visual, 2 - sistemas inteligentes
    // 3 - sistemas de computa��o, 4 - sistemas de informa��o
    char enfase[TAM];
    int completa;
    char horarioDeAula[TAM];
    char diaDeAula[TAM];
} Materia;

// Fun��o para testar a leitura dos campos da mat�ria
void testarLeituraMateria(Materia m) {
    printf("-------------------------\n");
    printf("C�digo: %s\n", m.codigo);
    printf("Nome: %s\n", m.nome);
    printf("Per�odo: %d\n", m.periodo);
    printf("Pr�-requisito: %s\n", m.codigoPreRequisito);
    printf("Carga Hor�ria: %d\n", m.cargaHoraria);
    printf("�nfase: %s\n", m.enfase);
    printf("Completa: %d\n", m.completa);
    printf("Hor�rio de Aula: %s\n", m.horarioDeAula);
    printf("Dia de Aula: %s\n", m.diaDeAula);
    printf("-------------------------\n");
}

int main () {
    setlocale(LC_ALL, "Portuguese");
    Materia m[TAM];
    FILE *arquivo = fopen("C:/ProjetoC/projeto-roberta/materias.txt", "r");

    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int i = 0;
    char linha[256];  // Buffer para armazenar uma linha do arquivo

    while (fgets(linha, sizeof(linha), arquivo) != NULL && i < 41) {
        // Usamos sscanf para separar os campos corretamente
        int periodo, cargaHoraria, completa;
        char codigo[TAM], nome[TAM], codigoPreRequisito[TAM], enfase[TAM], horarioDeAula[TAM], diaDeAula[TAM];

        // Tentamos separar a linha em partes usando delimitadores
        sscanf(linha, "%s %s %d %s %d %s %d %s %s", codigo, nome, &periodo, codigoPreRequisito, &cargaHoraria, enfase, &completa, horarioDeAula, diaDeAula);

        strcpy(m[i].codigo, codigo);
        strcpy(m[i].nome, nome);
        m[i].periodo = periodo;
        strcpy(m[i].codigoPreRequisito, codigoPreRequisito);
        m[i].cargaHoraria = cargaHoraria;
        strcpy(m[i].enfase, enfase);
        m[i].completa = completa;
        strcpy(m[i].horarioDeAula, horarioDeAula);
        strcpy(m[i].diaDeAula, diaDeAula);

        // Testar a leitura da mat�ria
        testarLeituraMateria(m[i]);
        i++;
    }

    fclose(arquivo);
    return 0;
}
