#include "cores.h"

int pos_inicial_avatar[2];

int * getPosicaoInicial(){
    return pos_inicial_avatar;
}

int randomInteger (int low, int high) {
    int k;
    double d;

    d = (double) rand () / ((double) RAND_MAX + 1);
    k = d * (high - low + 1);

    return low + k;
}

void geraMapa(int mapa[10][10], int nivel) {
    int i, j, qtdO, qtdA, lin, col, infO=5, supO=15;

    switch (nivel) {
        case 2: infO=5; supO=25; break;
        case 3: infO=15; supO=35; break;
    }

    srand((unsigned int)time(NULL));

    for (i=0; i<10; i++)
        for (j=0; j<10; j++)
            mapa[i][j] = 0;

    int l_inicial_avatar = randomInteger(0,9);
    int l_inicial_objetivo = randomInteger(0,9);

    pos_inicial_avatar[0] = l_inicial_avatar;
    pos_inicial_avatar[1] = 0;

    mapa[l_inicial_avatar][0]= 1;
    mapa[l_inicial_objetivo][9]= 2;
    qtdA = randomInteger(3,10);
    qtdO = randomInteger(infO, supO);

    while (qtdO>0) {
        lin = randomInteger(0,9);
        col = randomInteger(1,8);
        if (mapa[lin][col] == 0) {
            mapa[lin][col] = 3;
            qtdO--;
        }
    }

    while (qtdA>0) {
        lin = randomInteger(0,9);
        col = randomInteger(1,8);
        if (mapa[lin][col] == 0) {
            mapa[lin][col] = 4;
            qtdA--;
        }
    }
}

void atualizaMapa(int mapa[10][10]) {
    int i, j, upd, atualizado[10][10];
    for (i=0; i<10; i++) for (j=0; j<10; j++) atualizado[i][j]=0;

    for (i=0; i<10; i++)
        for (j=0; j<10; j++) {
            if ((mapa[i][j] == 2 || mapa[i][j] == 3 || mapa[i][j] == 4) && atualizado[i][j] == 0) {
                upd = randomInteger(1,4);
                switch (upd) {
                    case 1:
                        if ((mapa[i-1][j] == 0 || (mapa[i-1][j] == 1 && mapa[i][j] == 3)) && i>0) {
                            mapa[i-1][j] = mapa[i][j];
                            mapa[i][j] = 0;
                            atualizado[i-1][j]=1;
                        }
                        break;
                    case 2:
                        if ((mapa[i+1][j] == 0 || (mapa[i+1][j] == 1 && mapa[i][j] == 3)) && i<9) {
                            mapa[i+1][j] = mapa[i][j];
                            mapa[i][j] = 0;
                            atualizado[i+1][j]=1;
                        }
                        break;
                    case 3:
                        if ((mapa[i][j+1] == 0 || (mapa[i][j+1] == 1 && mapa[i][j] == 3)) && j<9) {
                            mapa[i][j+1] = mapa[i][j];
                            mapa[i][j] = 0;
                            atualizado[i][j+1]=1;
                        }
                        break;
                    case 4:
                        if ((mapa[i][j-1] == 0 || (mapa[i][j-1] == 1 && mapa[i][j] == 3)) && j>0) {
                            mapa[i][j-1] = mapa[i][j];
                            mapa[i][j] = 0;
                            atualizado[i][j-1]=1;
                        }
                }
            }
       }
}

/*

// DECLARE A MATRIZ mapa, COM ESCOPO GLOBAL, VEJA O EXEMPLO A SEGUIR
int mapa[10][10];

// TODA VEZ QUE VOCE DESEJAR GERAR UM NOVO MAPA, BASTA CHAMAR A FUNCAO geraMapa, conforme o exemplo a seguir
// O primeiro parâmetro é a sua matriz 10x10, o segundo é o nível de dificuldade do jogo: 1-Fácil, 2-Intermediário, 3-Difícil
geraMapa(mapa, 1);

// TODA VEZ QUE VOCE DESEJAR ATUALIZAR UM MAPA, BASTA CHAMAR A FUNCAO atualizaMapa, conforme o exemplo a seguir
atualizaMapa(mapa);
*/

//A PARTIR DAQUI VOCÊS ENCONTRAM UM PROGRAMA-EXEMPLO DE COMO USAR AS FUNÇÕES PARA GERAÇÃO E ATUALIZAÇÃO DOS MAPAS

void imprimeMapa(int mapa[10][10]) {
    int i, j;
    for(i = 0; i<=10; i++){
        if(i == 0){
            printf("    ", i); //indicador colunas
        }else{
            printf(" %d  ", i); //indicador colunas
        }
    }

    //printf("\n   \u250C"); //canto superior esquerdo
    printf("\n   \u256D"); //canto superior esquerdo

    for(int i = 0; i < 40; i++){
        printf("\u2500"); // quadro
    }
    //printf("\u2510\n"); //canto superior direito
    printf("\u256E\n"); //canto superior direito

    for (i=0; i<10; i++) {
        printf("%-3d\u2502", i+1); // quadro
        for (j=0; j<10; j++) {
            if (mapa[i][j]==0)
                printf("    ", mapa[i][j]);
            else if (mapa[i][j]==1)
                //printf(YEL " 1  " RESET);
                printf(YEL " \u263B  " RESET);
            else if (mapa[i][j]==2)
                printf(WHT " \u25C9  " RESET);
            else if (mapa[i][j]==3)
                printf(RED " \u2620  " RESET);
            else if (mapa[i][j]==4)
                printf(GRN " \u2665  " RESET);
            else
                printf(" %d  ", mapa[i][j]);
        }
        printf("\u2502\n"); // quadro
    }
    printf("   \u2570"); // canto inferior esquerdo
    for(int i = 0; i < 40; i++){
        printf("\u2500");
    }
    printf("\u256F"); // canto inferior direito

    printf("\n");
}