#if defined(_WIN32) || defined(_WIN64)
    #define CLEAR "cls"
    #define WINDOWS 1
#else
    #define CLEAR "clear"
    #define WINDOWS 0
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <conio.h>
#include <Windows.h>
#include <string.h>
#include "cores.h"
#include "funcoesMapa.c"
#include "arquivo.c"

int jogando = 1;

struct Avatar{
    char nome[15];
    int posicao[2];
    int vidas;
    int pontuacao;
};
typedef struct Avatar avatar;

void mover (char direcao, int posicao[2]){
    if(direcao == 'a'){
        posicao[1] -= 1;
    }
    else if(direcao == 's'){
        posicao[0] += 1;
    }
    else if(direcao == 'd'){
        posicao[1] += 1;
    }
    else if(direcao == 'w'){
        posicao[0] -= 1;
    }
    else if(direcao == '0'){
        printf("Saindo");
        exit(0);
    }
}

void verificarColisao(avatar *jogador, int mapa[10][10], int flag){
    int linhajogador = jogador->posicao[0];
    int colunajogador = jogador->posicao[1];

    if((linhajogador < 0 || colunajogador < 0) || (linhajogador > 9 || colunajogador > 9)){
        printf("\nSaiu do mapa\n");
        getch();
    }
    else if(mapa[linhajogador][colunajogador] == 2){
        printf("\nChegou no objetivo \n");
        getch();
    }
    else if(mapa[linhajogador][colunajogador] == 3){
        if(flag == 0){
            printf("\nbateu no obstaculo \n");
        } else{
            printf("\nobstaculo bateu no jogador \n");
        }
        getch();
    }
    else if(mapa[linhajogador][colunajogador] == 4){
        printf("\nbateu no bonus \n");
        jogador->vidas +=1;
        getch();
    }
}

void imprimirIntro(){
    printf("Use as teclas WASD para se mover pelo mapa.\nPressione qualquer tecla para iniciar o jogo\n");
    getch(); //pausar terminal
    printf("Gerando mapa...\n");
    Sleep(500);
    system(CLEAR);
}

void imprimeHud(avatar *jogador){
    int linhajogador = jogador->posicao[0];
    int colunajogador = jogador->posicao[1];
    int vidas = jogador->vidas;
    char *nome = jogador->nome;

    printf("\nVIDAS: ");
    for(int  i = 0; i<vidas; i++){
        printf(RED "\u2764  " RESET);
    }
    printf("\nLINHA: %d, COLUNA: %d\n", (linhajogador + 1), (colunajogador + 1));
    printf("PONTOS: %d", 1000);
    printf("\n");
}

void fimDeJogo(pontuacao ranking[3], int resultadoJogo, avatar *jogador){
    // 1 - Vitoria // 0 - Derrota
    jogando = 0;
    
    int pontos = jogador->pontuacao;
    char *nome = jogador->nome;

    if(resultadoJogo == 1){
        printf("\n\nFIM DE JOGO \n");
        printf("Parabéns %s, você fez %d pontos.", nome, pontos);

        printf("\nGravando no ranking...\n");
        atualizaRanking(ranking, pontos, nome);
        
        imprimeRankingArquivo();
    }
}

int main () {
    setlocale(LC_ALL,"Portuguese_Brasil");
    system(CLEAR);
    
    int mapa[10][10], posicao_anterior[2];
    char direcao;
    char nome[15];

    printf("\nOlá, seja bem-vindo! Antes de começar a jogar, digite seu nick: ");
    scanf("%s", &nome);
    printf("\nMuito bem, %s, o modo de jogar é muito simples, confira:\n", nome);

    imprimirIntro();

    geraMapa(mapa, 3); //AQUI ESTOU USANDO O NÍVEL DE DIFICULDADE 3 - DIFÍCIL

    avatar jogador;
    //avatar *pontJogador = &jogador;
    int *pontPosicaoInical = getPosicaoInicial();
    
    strcpy(jogador.nome, nome);
    jogador.posicao[0] = *(pontPosicaoInical); //linha
    
    pontuacao ranking[3];
    obtemRanking(ranking);

    jogador.posicao[1] = *(pontPosicaoInical + 1); //coluna
    jogador.vidas = 3;
    jogador.pontuacao = 20;
    

    imprimeMapa(mapa);
    imprimeHud(&jogador);

    do {
        direcao = getch();
        if(direcao == '9'){
            fimDeJogo(ranking, 1, &jogador);
            exit(0);
        }

        posicao_anterior[0] = jogador.posicao[0];
        posicao_anterior[1] = jogador.posicao[1];

        mover(direcao, jogador.posicao);
        //verificarColisao(jogador.posicao, jogador.vidas, mapa);
        verificarColisao(&jogador, mapa, 0);

        mapa[jogador.posicao[0]][jogador.posicao[1]] = 1;
        mapa[posicao_anterior[0]][posicao_anterior[1]] = 0;

        system(CLEAR);

        imprimeMapa(mapa);
        imprimeHud(&jogador);

        Sleep(250);

        //if (at){
            system(CLEAR);
            atualizaMapa(mapa);
            imprimeMapa(mapa);
            imprimeHud(&jogador);
            verificarColisao(&jogador, mapa, 1);
       // }

    } while (jogando == 1);
    return 0;
}


/*POSSÍVEIS FUNÇÕES
0 - MENU (JOGAR, MOSTRAR RANKING)
1 - MOVIMENTAR AVATAR ---------------------------------- OK
2 - FAZER PERGUNTA
3 - ONDE ESTÁ O AVATAR? -------------------------------- OK
4 - VERIFICAR COLISÃO DO AVATAR ------------------------ OK
5 - VERIFICAR COLISÃO DA ATUALIZAÇÃO ------------------- OK
6 - ATUALIZAR A TELA ----------------------------------- OK
7 - CALCULAR A PONTUACAO
8 - VERIFICAR VITÓRIA
9 - VERIFICAR SE SAIU DO MAPA -------------------------- OK
10 - FINALIZAR JOGO
11 - LER ARQUIVO RANKING
12 - GRAVAR ARQUIVO RANKING ---------------------------- OK
13 - VERIFICA SE PRECISA ATUALIZAR O RANKING ----------- OK
14 - IMPRIMIR PONTUAÇÃO E RANKING AO FINAL DO JOGO
*/