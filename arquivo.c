#include <stdio.h>

struct pontuacao {
    char nome[15];
    int pontos;
};
typedef struct pontuacao pontuacao;

void obtemRanking(struct pontuacao ranking[3]) {
    FILE *arq = fopen("rank.dat", "rb");
    int total_lido;
    if (arq == NULL) {
        ranking[0].pontos=0;
        ranking[1].pontos=0;
        ranking[2].pontos=0;
        strcpy(ranking[0].nome, "");
        strcpy(ranking[1].nome, "");
        strcpy(ranking[2].nome, "");
    } else {
        total_lido = fread(ranking, sizeof(struct pontuacao), 3, arq);
        if (total_lido == 3) {
            printf("\nSucesso!\n");
        } else
            printf("Erro na gravação dos dados");
    }
    fclose(arq);
}

void gravaRanking(struct pontuacao ranking[3]) {
    FILE *arq = fopen("rank.dat", "wb");
    int total_gravado;
    if (arq == NULL)
        printf("Erro na abertura do arquivo.");
    else {
        total_gravado = fwrite(ranking, sizeof(struct pontuacao), 3, arq);
        if (total_gravado == 3) {
            printf("\nSucesso na gravação!\n");
        } else
            printf("\nErro na gravação dos dados");
    }
    fclose(arq);
}

void atualizaRanking(struct pontuacao ranking[3], int pontuacaoAtual, char nomeAtual[15]) {
    struct pontuacao aux;

    if (pontuacaoAtual > ranking[2].pontos) {
        ranking[2].pontos = pontuacaoAtual;
        strcpy(ranking[2].nome, nomeAtual);
        if (pontuacaoAtual > ranking[1].pontos) {
            aux.pontos = ranking[1].pontos;
            strcpy(aux.nome, ranking[1].nome);

            ranking[1].pontos = ranking[2].pontos;
            strcpy(ranking[1].nome, ranking[2].nome);

            ranking[2].pontos = aux.pontos;
            strcpy(ranking[2].nome, aux.nome);

            if (pontuacaoAtual > ranking[0].pontos) {
                aux.pontos = ranking[0].pontos;
                strcpy(aux.nome, ranking[0].nome);

                ranking[0].pontos = ranking[1].pontos;
                strcpy(ranking[0].nome, ranking[1].nome);

                ranking[1].pontos = aux.pontos;
                strcpy(ranking[1].nome, aux.nome);
            }
        }
        gravaRanking(ranking);
    }
}

void imprimeRankingArquivo() {
    struct pontuacao ranking[3];
    int total_lido;
    FILE *arq = fopen("rank.dat", "rb");
    if (arq == NULL) {
        printf("Erro");
    } else {
        total_lido = fread(ranking, sizeof(struct pontuacao), 3, arq);
        if (total_lido == 3) {
            int i,j;
            printf("\n\u256D");
            for (i=0; i<35; i++) {
                printf("\u2500"); // quadro
            }
            printf("\u256E\n"); //canto superior direito
            printf("%-37s %s", "\u2502", "\u2502\n");
            for (j=0; j<3; j++) {
                printf("\u2502 %d - %-22s %6d %s", (j+1), ranking[j].nome, ranking[j].pontos, "\u2502\n");
                printf("%-37s %s", "\u2502", "\u2502");
                printf("\n");
                //printf("      \u2502\n"); // quadro
            }
            printf("\u2570"); // canto inferior esquerdo
            for (i=0; i<35; i++) {
                printf("\u2500"); // quadro
            }
            printf("\u256F"); // canto inferior direito

        } else
            printf("Erro na gravação dos dados");
    }
    fclose(arq);
}

/*int main() {

    obtemRanking(ranking);
    //atualizaRanking(ranking, 1, "MINITESTER");
    //atualizaRanking(ranking, 2, "MIDTESTER");
    //atualizaRanking(ranking, 3, "BIGTESTER");
    //atualizaRanking(ranking, 10, "MEGATESTER");
    //atualizaRanking(ranking, 100, "ULTRATESTER");
    //atualizaRanking(ranking, 1000, "GIGATESTER");
    imprimeRankingArquivo();

    return 0;
}*/