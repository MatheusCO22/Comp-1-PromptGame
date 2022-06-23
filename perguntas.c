#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

int numPerguntas;
//estrutura para armazenar as perguntas que serão lidas do arquivo
struct pergunta {
    char tipo; //tipo da pergunta: N-Numérica, O-Objetiva
    char enunciado[150]; //enunciado da pergunta
    char opcoes[5][45]; //armazena as 5 opções das perguntas objetivas. o Campo respO diz qual delas é a correta.
    float respN; //resposta para perguntas do tipo numérico
    int codigo; //cédigo da pergunta
    int respO; //resposta correta entre as opções para perguntas do tipo objetiva. Valor varia de 1 a 5.
};

void inicializarPerguntas(struct pergunta perguntas[], int tam) {
    int i;
    for(i=0; i<tam; i++) {
        perguntas[i].codigo=-1;
    }
}

void carregarPerguntasArquivo(struct pergunta perguntas[]) {
    FILE *arq = fopen("pergs.txt", "r");
    int linha=0, parte;
    char frase[1000];

    while (fscanf(arq, "%[^\n] ", frase) != EOF){
        char *token = strtok(frase, ";");
        parte=0;
        while(token != NULL) {
            if (parte == 0)
                perguntas[linha].codigo = atoi(token);
            else if (parte == 1)
                perguntas[linha].tipo = token[0];
            else if (parte == 2)
                strcpy(perguntas[linha].enunciado, token);
            else if (parte == 3 && perguntas[linha].tipo == 'N') {
                perguntas[linha].respN = atof(token);
            } else if (parte == 3 && perguntas[linha].tipo == 'O') {
                perguntas[linha].respO = atoi(token);
            } else if (parte ==4)
                strcpy(perguntas[linha].opcoes[0], token);
            else if (parte ==5)
                strcpy(perguntas[linha].opcoes[1], token);
            else if (parte ==6)
                strcpy(perguntas[linha].opcoes[2], token);
            else if (parte ==7)
                strcpy(perguntas[linha].opcoes[3], token);
            else if (parte ==8)
                strcpy(perguntas[linha].opcoes[4], token);

            token = strtok(NULL, ";");
            parte++;
        }
        linha++;
    };
}

void imprimirTodasPerguntas(struct pergunta perguntas[], int tam) {
    int i,j;
    for(i=0; i<tam; i++) {
        if (perguntas[i].codigo < 0){
            break;
        }
        printf("\n\nPERGUNTA: %d -- %s", perguntas[i].codigo, perguntas[i].enunciado);
        if (perguntas[i].tipo == 'N') {
            printf("\n\tTipo: Numérica");
            printf("\n\tResposta: %f", perguntas[i].respN);
        } else {
            printf("\n\tTipo: Objetiva");
            for(j=0; j<5; j++) {
                printf("\n\tOpção %d: %s", j+1, perguntas[i].opcoes[j]);
            }
            printf("\n\tResposta (Opção): %d", perguntas[i].respO);
        }
    }
}

void imprimirPergunta(struct pergunta perguntaSorteada) {
    printf("\n\nPERGUNTA -- %s", perguntaSorteada.enunciado);
    if (perguntaSorteada.tipo == 'N') {
        printf("\n\tTipo: Numérica");
        printf("\n\tResposta: %f", perguntaSorteada.respN);
    } else {
        printf("\n\tTipo: Objetiva");
        for(int j=0; j<5; j++) {
            printf("\n\tOpção %d: %s", j+1, perguntaSorteada.opcoes[j]);
        }
        printf("\n\tResposta (Opção): %d", perguntaSorteada.respO);
    }
}

int contarPerguntas(struct pergunta perguntas[], int tam){
    int i;
    numPerguntas = 0;

    for(i=0; i<tam; i++) {
        if (perguntas[i].codigo < 0){
            break;
        }
        else{
            numPerguntas++;
        }
    }
    return numPerguntas;
}
