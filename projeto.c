#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char codigo[50];
} prerequisito;

typedef struct
{
    int id;
    char codigo[8];
    char nome[50];
    int periodo;
    prerequisito prerequisitos[100];
    int qtdPreRequisitos;
    int cargaHoraria;
    // 0 - obrigatória, 1 - computação visual, 2 - sistemas inteligentes
    // 3 - sistemas de computação, 4 - sistemas de informação
    char enfase[5];
    int escolha;
    int completa;
    int alocada;
    char horarioDeAula[5];
    char diaDeAula[7];
} Materia;

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
    
    // preenche a struct com as disciplinas
    while (fgets(linha, sizeof(linha), arquivo) != NULL && i < 46)
    {
        int periodo, cargaHoraria, completa, id;
        char codigo[8], nome[50], preRequisitosStr[100], enfase[5], horarioDeAula[5], diaDeAula[7];
        
        sscanf(linha, "%d %s %s %d %s %d %s %d %s %s", &id, codigo, nome, &periodo, preRequisitosStr, &cargaHoraria, enfase, &completa, horarioDeAula, diaDeAula);
        
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
        strcpy(disciplina[i].diaDeAula, diaDeAula);
        
        processarPreRequisitos(preRequisitosStr, &disciplina[i]);
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
        printf("Dia de Aula: %s\n", m.diaDeAula);
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

int verificaMateriaColide(int periodo[11][6], char *horarioDeAula, char *diaDeAula, char *codigo, int semestre, Materia *disciplinas)
{ 
    for(int j = 0; j < 6; j++)
    {
        if (periodo[semestre][j] == -1) 
            break;
        
        int i;
        for (i = 0; i < 46; i++)
        {
            if (disciplinas[i].id = periodo[semestre][j])
                break;
        }
        
        if (strcmp(disciplinas[i].codigo, codigo) == 0)
            continue;

        if (numDiferenca(diaDeAula, disciplinas[i].diaDeAula) == 3 ||
        strcmp(diaDeAula, disciplinas[i].diaDeAula) == 0)
        {
            if (numDiferenca(horarioDeAula, disciplinas[i].horarioDeAula) < 2)
            return 1;
        }
    }

    return 0;
}


int verificaPreReq(Materia disciplina, Materia *oferta)
{
    if (disciplina.qtdPreRequisitos == 0)
        return 1;

    for (int i = 0; i < disciplina.qtdPreRequisitos; i++)
    {
        int requisitoCumprido = 0;
        for (int j = 0; j < 46; j++)
        {
            if (strcmp(disciplina.prerequisitos[i].codigo, oferta[j].codigo) == 0 &&
                (oferta[j].completa == 1 || oferta[j].alocada == 1))
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

int main ()
{
    Materia disciplinas[46];
    FILE *arquivo = fopen("materias.txt", "r");
    int periodo[11][6];
    memset(periodo, -1, sizeof(periodo));
    char calouro;
    int periodoUsuario = 0;

    preencherMaterias(disciplinas, arquivo);

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
                            disciplinas[i+1].completa = 1;
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
    for(int i = 0; i < 46; i++)
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

    for (int semestre = periodoUsuario; semestre < 10; semestre++)
    {        
        int materiasAlocadas = 0;
        char turnoAtual;
        int j = 0;
        
        if (semestresFaltando - semestre == restoMediaMateriasPorSemestre + 1)
            mediaMateriasPorSemestre++;

        for (int i = 0; materiasAlocadas < mediaMateriasPorSemestre && i < 46; i++)
        {
            if (disciplinas[i].escolha == 0 || disciplinas[i].completa == 1 || disciplinas[i].alocada == 1)
                continue;

                if (materiasAlocadas == 0)
            {
                periodo[semestre][j++] = disciplinas[i].id;
                disciplinas[i].alocada = 1;
                disciplinas[i].completa = 1;
                turnoAtual = disciplinas[i].horarioDeAula[0];
                materiasAlocadas++;
            }
            else if (disciplinas[i].horarioDeAula[0] == turnoAtual &&
                !verificaMateriaColide(periodo, disciplinas[i].horarioDeAula, disciplinas[i].diaDeAula, disciplinas[i].codigo, semestre, disciplinas)
                && verificaPreReq(disciplinas[i], disciplinas))
            {
                periodo[semestre][j++] = disciplinas[i].id;
                disciplinas[i].alocada = 1;
                disciplinas[i].completa = 1;
                materiasAlocadas++;   
            }
            
            if (materiasAlocadas >= 6) 
                break;
        }
    }
    
    for(int i = periodoUsuario; i < 10; i++)
    {
        printf("Período %d:\n", i+1);
        for(int j = 0; j < 6; j++)
        {
            if(periodo[i][j] != -1){
                for(int k = 0; k < 46; k++)
                {
                    if(disciplinas[k].id == periodo[i][j])
                    {
                        printarMateria(disciplinas[k], 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
                        break;
                    }
                }
            }
        }
        printf("\n----------------------------------------------------\n");
    }

    return 0;
}
