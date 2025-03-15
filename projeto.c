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
    // 0 - obrigatória, 1 - computação visual, 2 - sistemas inteligentes
    // 3 - sistemas de computação, 4 - sistemas de informação
    char enfase[TAM];
    int completa;
    char horarioDeAula[TAM];
    char diaDeAula[TAM];
} Materia;

typedef struct {
    Materia disciplina[TAM];
} Ofertas;

// Função para testar a leitura dos campos da matéria
void printarMateria(Materia m, int cod, int nome, int per, int pre, int ch, int enf, int comp, int hora, int dia) {
    printf("------------------------\n");
    if (cod) printf("Código: %s\n", m.codigo);
    if (nome) printf("Nome: %s\n", m.nome);
    if (per) printf("Período: %d\n", m.periodo);
    if (pre) printf("Pré-requisito: %s\n", m.codigoPreRequisito);
    if (ch) printf("Carga Horária: %d\n", m.cargaHoraria);
    if (enf) printf("Ênfase: %s\n", m.enfase);
    if (comp) printf("Completa: %d\n", m.completa);
    if (hora) printf("Horário de Aula: %s\n", m.horarioDeAula);
    if (dia) printf("Dia de Aula: %s\n", m.diaDeAula);
    printf("------------------------\n");
}

int main () {
    setlocale(LC_ALL, "Portuguese");
    Ofertas oferta;
    FILE *arquivo = fopen("C:/ProjetoC/projeto-roberta/materias.txt", "r");

    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int i = 0;
    char linha[256];  // Buffer para armazenar uma linha do arquivo

    while (fgets(linha, sizeof(linha), arquivo) != NULL && i < 46) {
        // Usamos sscanf para separar os campos corretamente
        int periodo, cargaHoraria, completa;
        char codigo[TAM], nome[TAM], codigoPreRequisito[TAM], enfase[TAM], horarioDeAula[TAM], diaDeAula[TAM];

        // Tentamos separar a linha em partes usando delimitadores
        sscanf(linha, "%s %s %d %s %d %s %d %s %s", codigo, nome, &periodo, codigoPreRequisito, &cargaHoraria, enfase, &completa, horarioDeAula, diaDeAula);

        strcpy(oferta.disciplina[i].codigo, codigo);
        strcpy(oferta.disciplina[i].nome, nome);
        oferta.disciplina[i].periodo = periodo;
        strcpy(oferta.disciplina[i].codigoPreRequisito, codigoPreRequisito);
        oferta.disciplina[i].cargaHoraria = cargaHoraria;
        strcpy(oferta.disciplina[i].enfase, enfase);
        oferta.disciplina[i].completa = completa;
        strcpy(oferta.disciplina[i].horarioDeAula, horarioDeAula);
        strcpy(oferta.disciplina[i].diaDeAula, diaDeAula);

        i++;
    }

    int periodoUsuario;
    
    printf("Bem vindo ao programa de aconselhamento do curso de Ciência da Computação!\n");
    printf("Em qual período você está? ");
    scanf("%d", &periodoUsuario);

    // considera que as matérias do período passado já foram concluídas
    int j = 0;
    while (oferta.disciplina[j].periodo < periodoUsuario && oferta.disciplina[j].periodo != VAZIO) {
        oferta.disciplina[j].completa = 1;
        j++;
    }

    // verifica se o usuário perdeu alguma matéria
    if (periodoUsuario > 1) {
        char reprovado;
        char mat[TAM];
        printf("Você reprovou em alguma matéria? [S/N] ");
        getchar();
        scanf("%c", &reprovado);
        while (1) {
            if (reprovado == 'N') break;
            else {
                printf("Em qual dessas você reprovou? (Digite o código da disciplina)\n");
                j = 0;
                while (oferta.disciplina[j].periodo < periodoUsuario && oferta.disciplina[j].periodo != VAZIO) {
                   printarMateria(oferta.disciplina[j], 1, 1, 0, 0, 0, 0, 0, 0, 0);
                   j++;
                }
                scanf("%s", mat);
                j = 0;
                while (oferta.disciplina[j].periodo < periodoUsuario && oferta.disciplina[j].periodo != VAZIO) {
                   if(strcmp(mat, oferta.disciplina[j].codigo) == 0) {
                    oferta.disciplina[j].completa = 0;
                   }
                   j++;
                }
            }
            printf("Você reprovou em mais alguma? [S/N] ");
            getchar();
            scanf("%c", &reprovado);
        }        
    }

    
    printf("Você já cursou alguma matéria eletiva? [S/N] ");
    getchar();
    char eletiva;
    scanf("%c", &eletiva);
    while (1) {
        if (eletiva == 'N') break;
        else {
            printf("Qual dessas você já cursou? (Digite o código da disciplina)\n");
            j = 0;
            while (j < 46) {
                if (!strchr(oferta.disciplina[j].enfase, '0')) {
                    printarMateria(oferta.disciplina[j], 1, 1, 0, 0, 0, 0, 0, 0, 0);
                }
                j++;
            }
            char cod[TAM];
            scanf("%s", cod);
            j = 0;
            while (j < 46) {
                if(strcmp(cod, oferta.disciplina[j].codigo) == 0) {
                    oferta.disciplina[j].completa = 1;
                    break;
                }
                j++;
            }
        }
        printf("Você já cursou mais alguma? [S/N] ");
        getchar();
        scanf("%c", &eletiva);
    }
        
    // somente para verificação
    // j = 0;
    // while (j < 46) {
    //     printarMateria(oferta.disciplina[j], 1, 1, 1, 1, 1, 1, 1, 1, 1);
    //     j++;
    // }

    fclose(arquivo);
    return 0;
}
