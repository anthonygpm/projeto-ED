#include <stdio.h>
#include <string.h>

typedef struct {
    char codigo[8];
} Prerequisito;

typedef struct {
    char codigo[8];
    char nome[50];
    int periodo;
    int qtdPreRequisitos;
    int cargaHoraria;
    char enfase[5];
    int completa;
    char horarioDeAula[5];
    char diaDeAula[7];
    Prerequisito prerequisitos[10]; // Defina um tamanho máximo adequado
} Materia;

void processarPreRequisitos(char *preRequisitosStr, Materia *materia) {
    materia->qtdPreRequisitos = 0;
    int len = strlen(preRequisitosStr);
    int i = 0;

    while (i < len) {
        if (strncmp(&preRequisitosStr[i], "COMP", 4) == 0) {
            strncpy(materia->prerequisitos[materia->qtdPreRequisitos].codigo, &preRequisitosStr[i], 7);
            materia->prerequisitos[materia->qtdPreRequisitos].codigo[7] = '\0'; // Garante terminação da string
            materia->qtdPreRequisitos++;
            i += 7;
        } else {
            i++;
        }
    }
}

void preencherMaterias(Materia *lista, FILE *arquivo) {
    int i = 0;
    char linha[256];

    // Preenche a struct com as disciplinas
    while (fgets(linha, sizeof(linha), arquivo) != NULL && i < 46) {
        int periodo, cargaHoraria, completa;
        char codigo[8], nome[50], preRequisitosStr[30], enfase[5], horarioDeAula[5], diaDeAula[7];

        sscanf(linha, "%s %s %d %s %d %s %d %s %s", 
               codigo, nome, &periodo, preRequisitosStr, 
               &cargaHoraria, enfase, &completa, horarioDeAula, diaDeAula);

        strcpy(lista[i].codigo, codigo);
        strcpy(lista[i].nome, nome);
        lista[i].periodo = periodo;
        lista[i].cargaHoraria = cargaHoraria;
        strcpy(lista[i].enfase, enfase);
        lista[i].completa = completa;
        strcpy(lista[i].horarioDeAula, horarioDeAula);
        strcpy(lista[i].diaDeAula, diaDeAula);

        processarPreRequisitos(preRequisitosStr, &lista[i]);
        i++;
    }
}

void printarMateria(Materia m, int cod, int nome, int per, int pre, int ch, int enf, int comp, int hora, int dia) {
    printf("------------------------\n");
    if (cod)
        printf("Código: %s\n", m.codigo);
    if (nome)
        printf("Nome: %s\n", m.nome);
    if (per) {
        printf("Período: ");
        printf("%d°\n", m.periodo);
    }
    if (pre) {
        printf("Pré-requisitos: ");
        if (m.qtdPreRequisitos == 0)
            printf("Nenhum\n");
        else {
            for (int i = 0; i < m.qtdPreRequisitos; i++)
                printf("%s ", m.prerequisitos[i].codigo);
            printf("\n");
        }
    }
    if (ch)
        printf("Carga Horária: %d\n", m.cargaHoraria);
    if (enf)
        printf("Ênfase: %s\n", m.enfase);
    if (comp)
        printf("Completa: %d\n", m.completa);
    if (hora)
        printf("Horário de Aula: %s\n", m.horarioDeAula);
    if (dia)
        printf("Dia de Aula: %s\n", m.diaDeAula);
}

int verificaPreReq(Materia disciplina, Materia *ofertaGeral) {
    if (disciplina.qtdPreRequisitos == 0)
        return 1;
    
    for (int i = 0; i < disciplina.qtdPreRequisitos; i++) {
        int requisitoCumprido = 0;
        
        for (int j = 0; j < 46; j++) {
            if (strcmp(disciplina.prerequisitos[i].codigo, ofertaGeral[j].codigo) == 0 &&
                ofertaGeral[j].completa == 1) {
                requisitoCumprido = 1;
                break;
            }
        }
        
        if (!requisitoCumprido)
            return 0;
    }
    
    return 1;
}

int verificaTurnoIgual(Materia matFaltando, Materia add, int qtd) {
    if (qtd == 0) return 1;
    
    if(matFaltando.horarioDeAula[0] == add.horarioDeAula[0]) return 1;

    return 0;
}




int main() {
    FILE *arquivo = fopen("materias.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    Materia ofertaGeral[46];
    preencherMaterias(ofertaGeral, arquivo);
    fclose(arquivo);

    int periodoUsuario;
    printf("Bem vindo ao programa de aconselhamento do curso de Ciência da Computação!\n");
    printf("Em qual período você está? ");
    scanf("%d", &periodoUsuario);

    // Considera que as matérias do período passado já foram concluídas
    for (int i = 0; i < 46; i++) {
        if (ofertaGeral[i].periodo == periodoUsuario) {
            ofertaGeral[i].completa = 1;
        }
    }

    // Pergunta se o aluno já reprovou alguma matéria cursada
    if (periodoUsuario > 1) {
        char reprovado;
        printf("Você reprovou em alguma matéria? [S/N] ");
        scanf(" %c", &reprovado);

        while (reprovado == 'S') {
            char mat[8];
            printf("Qual matéria você reprovou? (Digite o código da disciplina)\n");

            for (int i = 0; i < 46; i++) {
                if (ofertaGeral[i].completa == 1) {
                    printarMateria(ofertaGeral[i], 1, 1, 0, 0, 0, 0, 0, 0, 0);
                }
            }

            scanf("%s", mat);

            int i;
            for (i = 0; i < 46; i++) {
                if (strcmp(mat, ofertaGeral[i].codigo) == 0) {
                    ofertaGeral[i].completa = 0;
                    printf("Matéria marcada como reprovada.\n");
                    break;
                }
            }

            if(i == 46) {
                printf("Matéria não encontrada.\n");
                printf("Deseja tentar novamente? [S/N] ");
                scanf(" %c", &reprovado);
                continue;
            }

            printf("Você reprovou em mais alguma? [S/N] ");
            scanf(" %c", &reprovado);
        }
    }


    // Pergunta se o aluno já cursou alguma matéria optativa
    if (periodoUsuario > 1) {
        char optativa;
        printf("Você já cursou alguma matéria optativa? [S/N] ");
        scanf(" %c", &optativa);

        while (optativa == 'S') {
            char mat[8];
            printf("Qual matéria optativa você cursou? (Digite o código da disciplina)\n");

            for (int i = 0; i < 46; i++) {
                if (ofertaGeral[i].periodo == 0) {
                    printarMateria(ofertaGeral[i], 1, 1, 0, 0, 0, 0, 0, 0, 0);
                }
            }

            scanf("%s", mat);

            int i;
            for (i = 0; i < 46; i++) {
                if (strcmp(mat, ofertaGeral[i].codigo) == 0) {
                    ofertaGeral[i].completa = 1;
                    printf("Matéria marcada como cursada.\n");
                    break;
                }
            }

            if(i == 46) {
                printf("Matéria não encontrada.\n");
                printf("Deseja tentar novamente? [S/N] ");
                scanf(" %c", &optativa);
                continue;
            }

            printf("Você já cursou mais alguma? [S/N] ");
            scanf(" %c", &optativa);
        }
    }

    // Pergunta qual ênfase o aluno deseja seguir
    char enfase;
    printf("Qual ênfase você deseja seguir? [1 - Computação Visual, 2 - Sistemas Inteligentes, 3 - Sistemas de Computação, 4 - Sistemas de Informação] ");
    scanf(" %c", enfase);

    // Cria um vetor de matérias faltando
    Materia matFaltando[46];
    int index = 0;
    for (int i = 0; i < 46; i++) {
        if (ofertaGeral[i].completa == 0) {
            matFaltando[index] = ofertaGeral[i];
            index++;
        }
    }

    // Aloca as matérias no plano de estudos
    int mediaMatSemestre = index / (10 - periodoUsuario);
    int resto = index % (10 - periodoUsuario);
    Materia planoEstudos[10 - periodoUsuario][mediaMatSemestre];
    for(int i = 0; i < 10 - periodoUsuario; i++) {
        index = 0;
        for(int j = 0; j < mediaMatSemestre; j++) {
            if(verificaPreReq(matFaltando[j], ofertaGeral) && 
               verificaTurnoIgual(matFaltando[j], planoEstudos[i][index], index)) {
                planoEstudos[i][index] = matFaltando[j];
                index++;
            }
        }
    }
    





    return 0;
}
