#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"



int main()
{
    Materia disciplinas[46];
    FILE *arquivo = fopen("materias.txt", "r");
    int periodo[11][6];
    for (int i = 0; i < 11; i++)
        for (int j = 0; j < 6; j++)
            periodo[i][j] = -1;

    char calouro;
    int periodoUsuario = 0;

    preencherMaterias(disciplinas, arquivo);

    ordenarMateriasPorRequisitos(disciplinas, 46);

    printf("Olá! Bem-vindo ao programa de aconselhamento do curso de Ciência da Computação!\n");
    printf("Você é calouro? [s/n] ");
    scanf("%c", &calouro);
    if (calouro == 'n')
    {
        printf("Em qual período você está? ");
        scanf("%d", &periodoUsuario);

        // cadastro das matérias que já foram concluídas
        char continuar;
        int encontrada = 0;
        do
        {
            printf("Digite o código da matéria que você deseja cadastrar: ");
            char codigo[8];
            scanf("%s", codigo);

            for (int i = 0; i < 46; i++)
            {
                if (strcmp(codigo, disciplinas[i].codigo) == 0)
                {
                    double media;
                    printf("Qual foi sua média nessa matéria? ");
                    scanf("%lf", &media);
                    if (media >= 7)
                    {
                        disciplinas[i].completa = 1;
                        if (strcmp(codigo, "COMP382") == 0)
                            disciplinas[i + 1].completa = 1;
                    }
                    encontrada = 1;
                    break;
                }
            }
            if (encontrada)
                printf("Matéria cadastrada com sucesso.\n");
            else
                printf("Matéria não encontrada.\n");

            printf("Deseja cadastrar mais alguma? [s/n] ");
            scanf(" %c", &continuar);

        } while (continuar == 's');
    }

    // pergunta qual enfase o aluno deseja seguir
    char enfase;
    printf("Qual ênfase você deseja seguir? [1 - Computação Visual, 2 - Sistemas Inteligentes, 3 - Sistemas de Computação, 4 - Sistemas de Informação] ");
    scanf(" %c", &enfase);

    // as matérias que serão cursadas, terão o atributo escolha == 1
    int qtdMatFaltando = 0;
    for (int i = 0; i < 46; i++)
    {
        if (disciplinas[i].periodo != -999 && disciplinas[i].completa == 0)
        {
            disciplinas[i].escolha = 1;
            qtdMatFaltando++;
        }
        else if (strchr(disciplinas[i].enfase, enfase) != NULL && disciplinas[i].completa == 0)
        {
            disciplinas[i].escolha = 1;
            qtdMatFaltando++;
        }
    }

    // começa a lógica de aconselhamento
    int semestresFaltando = 10 - periodoUsuario;
    int mediaMateriasPorSemestre = qtdMatFaltando / semestresFaltando;
    int restoMediaMateriasPorSemestre = qtdMatFaltando % semestresFaltando;

    int materiasAlocadas = 0;
    for (int semestre = periodoUsuario; semestre < 10; semestre++)
    {
        if (materiasAlocadas > 0 && semestre > periodoUsuario)
        {
            for (int k = 0; k < 6; k++)
            {
            if (periodo[semestre - 1][k] != -1)
            {
                for (int l = 0; l < 46; l++)
                {
                if (disciplinas[l].id == periodo[semestre - 1][k])
                {
                    disciplinas[l].completa = 1;
                    break;
                }
                }
            }
            }
        }

        materiasAlocadas = 0;
        char turnoAtual;
        int j = 0;
        int aumentou = 0;
        

        for (int i = 0; materiasAlocadas <= mediaMateriasPorSemestre && i < 46; i++)
        {
            if (disciplinas[i].escolha == 0 || disciplinas[i].completa == 1 || disciplinas[i].alocada == 1)
                continue;

            if (materiasAlocadas == 0)
            {
                periodo[semestre][j++] = disciplinas[i].id;
                disciplinas[i].alocada = 1;
                turnoAtual = disciplinas[i].horarioDeAula[0];
                materiasAlocadas++;
            }
            else if (disciplinas[i].horarioDeAula[0] == turnoAtual &&
                     !verificaMateriaColide(periodo, disciplinas[i].horarioDeAula, disciplinas[i].diasDeAula, disciplinas[i].codigo, semestre, disciplinas) &&
                     verificaPreReq(disciplinas[i], disciplinas, periodo[i]) &&
                     (somaCargaHorariaPeriodo(periodo, semestre, disciplinas) + disciplinas[i].cargaHoraria <= 360))
            {
                periodo[semestre][j++] = disciplinas[i].id;
                disciplinas[i].alocada = 1;
                materiasAlocadas++;
            }

            if (materiasAlocadas >= 6)
                break;
        }
    }

    for (int i = periodoUsuario; i < 10; i++)
    {
        printf("Período %d:\n", i + 1);
        for (int j = 0; j < 6; j++)
        {
            if (periodo[i][j] != -1)
            {
                for (int k = 0; k < 46; k++)
                {
                    if (disciplinas[k].id == periodo[i][j])
                    {
                        printarMateria(disciplinas[k], 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
                        break;
                    }
                }
            }
        }
        printf("\n----------------------------------------------------\n");
    }

    printf("Matérias não alocadas:\n");
    int count = 0;
    for (int i = 0; i < 46; i++)
    {
        if (disciplinas[i].escolha == 1 && disciplinas[i].alocada == 0)
        {
            printarMateria(disciplinas[i], 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            count++;
        }
    }
    if(!count){
        printf("Nenhuma\n");
    }


    return 0;
}
