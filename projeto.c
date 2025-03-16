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

void printarMateria(Materia m, int cod, int nome, int per, int pre, int ch, int enf, int comp, int hora, int dia) {
    printf("------------------------\n");
    if (cod) printf("Código: %s\n", m.codigo);
    if (nome) printf("Nome: %s\n", m.nome);
    if (per){ 
        printf("Período: ");
        if(m.periodo == -999) printf("Optativa\n");
        else printf("%d°\n", m.periodo);
    }
    if (pre){
        printf("Pré-requisitos: ");
        if (m.qtdPreRequisitos == 0) printf("Nenhum\n");
        else{   
            for (int i = 0; i < m.qtdPreRequisitos; i++) printf("%s ", m.prerequisitos[i].codigo);
            printf("\n");
        }
    }
    if (ch) printf("Carga Horária: %d\n", m.cargaHoraria);
    if (enf) printf("Ênfase: %s\n", m.enfase);
    if (comp) printf("Completa: %d\n", m.completa);
    if (hora) printf("Horário de Aula: %s\n", m.horarioDeAula);
    if (dia) printf("Dia de Aula: %s\n", m.diaDeAula);
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

    // preenche a struct com as disciplinas 
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

    // verifica reprovações
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
                    printf("Matéria marcada como reprovada.\n");
                    break;
                   }
                   j++;
                }
            }
 

            printf("Você reprovou em mais alguma? [S/N] ");
            getchar();
            scanf("%c", &reprovado);
        }        
    }

    // verifica se o aluno já cursou alguma matéria optativa
    if (periodoUsuario > 1){
    char optativa;
    printf("Você já cursou alguma matéria optativa? [S/N] ");
    getchar();
    scanf("%c", &optativa);
    while(1){
        if (optativa == 'N') break;
        else {
            char mat[TAM];
            printf("Qual matéria optativa você cursou? (Digite o código da disciplina)\n");
            j = 0;

            while (j < 46) {
                if (oferta.disciplina[j].periodo == VAZIO) printarMateria(oferta.disciplina[j], 1, 1, 0, 0, 0, 0, 0, 0, 0);
                j++;
            }

            scanf("%s", mat);

            j = 0;
            while (j < 46) {
                if(strcmp(mat, oferta.disciplina[j].codigo) == 0) {
                    oferta.disciplina[j].completa = 1;
                    printf("Matéria marcada como cursada.\n");
                    break;
                }
                j++;
            }
        }

        printf("Você já cursou mais alguma? [S/N] ");
        getchar();
        scanf("%c", &optativa);
    }
}

    // pergunta qual enfase o aluno deseja seguir
    char enfase;
    printf("Qual ênfase você deseja seguir? [1 - Computação Visual, 2 - Sistemas Inteligentes, 3 - Sistemas de Computação, 4 - Sistemas de Informação] ");
    scanf(" %c", &enfase);


    // cria um vetor com as matérias que faltam para o aluno concluir o curso
    Ofertas matFaltando;
    int index = 0;
    for (int i = 0; i < 46; i++) {
        if (oferta.disciplina[i].completa == 0 && oferta.disciplina[i].periodo != VAZIO) {
            matFaltando.disciplina[index++] = oferta.disciplina[i];
        } else if (oferta.disciplina[i].completa == 0 && oferta.disciplina[i].periodo == VAZIO && strchr(oferta.disciplina[i].enfase, enfase) != NULL) {
            matFaltando.disciplina[index++] = oferta.disciplina[i];
        }
    }

    

    

    fclose(arquivo);
    return 0;
}
