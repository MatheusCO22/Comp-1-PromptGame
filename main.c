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
#include <ctype.h>
#include "cores.h"
#include "funcoesMapa.c"
#include "arquivo.c"

int jogando = 1;
int continuarjogando = 1;

struct Avatar{
    char nome[15];
    int posicao[2];
    int vidas;
    int pontuacao;
    int movimentos;
};
typedef struct Avatar avatar;

void mover (char direcao, int posicao[2]);
void verificarColisao(avatar *jogador, int mapa[10][10], int flag);
void imprimirIntro();
void imprimeHud(avatar *jogador);
void fimDeJogo(int resultadoJogo, avatar *jogador);

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
        fimDeJogo(1, jogador);
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
    printf("\n--- HISTÓRIA ---\n");

    printf("Use as teclas WASD para se mover pelo mapa.\nPressione qualquer tecla para iniciar o jogo\n");
    getch();
}
void imprimeMenu(){
    int i;
    printf("\n\u256D");
    for (i=0; i<24; i++) {
        printf("\u2500"); // quadro
    }
    printf("\u256E\n"); //canto superior direito
    printf("%-26s %s", "\u2502", "\u2502\n");

    printf("\u2502 %-22s %s", "1. Nova Partida", "\u2502\n");
    printf("\u2502 %-22s %s", "2. Ver Ranking", "\u2502\n");
    printf("\u2502 %-22s %s", "3. Sair do Jogo", "\u2502\n");
    printf("%-26s %s", "\u2502", "\u2502\n");
    
    printf("\u2570"); // canto inferior esquerdo
    for (i=0; i<24; i++) {
        printf("\u2500"); // quadro
    }
    printf("\u256F"); // canto inferior direito
    printf("\n");
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
    //printf("\nLINHA: %d, COLUNA: %d\n", (linhajogador + 1), (colunajogador + 1));
    printf("\nPONTOS: %d", 1000);
    printf("\n");
}

void fimDeJogo(int resultadoJogo, avatar *jogador){
    char jogarnovamente;
    // 1 - Vitoria // 0 - Derrota

    jogando = 0;
    pontuacao ranking[3];
    obtemRanking(ranking);
    
    int pontos = jogador->pontuacao;
    char *nome = jogador->nome;

    if(resultadoJogo == 1){
        system(CLEAR);
        printf("Parabéns pela vitória, %s! Você fez %d pontos.", nome, pontos);

        atualizaRanking(ranking, pontos, nome);
        printf("\nNossos melhores 3 jogadores até agora são: \n");
        imprimeRankingArquivo();
    }
    else if(resultadoJogo == 0){
        //dostuff
    }

    printf("\nPressione qualquer tecla para voltar ao menu.\n");
    getch();
}

int main () {
    setlocale(LC_ALL,"Portuguese_Brasil");
    system(CLEAR);
    
    int mapa[10][10], posicao_anterior[2];
    int opcaomenu;
    char direcao;
    char nome[15];
    int tamNome = 0;

    printf("\nOlá, seja bem-vindo! Antes de começar a jogar, digite seu nick: ");

    do{
        scanf("%s", &nome);

        tamNome = strlen(nome);

        if(tamNome > 15 || tamNome < 3){
            printf("\nTamanho inválido, seu nick deve conter entre 3 e 15 caracteres.\n");
        }
    } while (tamNome > 15 || tamNome < 3 );

    avatar jogador;

    do {
        jogando = 1;
        opcaomenu = 0;

        system(CLEAR);

        imprimeMenu();
        do{
            scanf("%d", &opcaomenu);
            if(!(opcaomenu == 1 || opcaomenu == 2 || opcaomenu == 3)){
                printf("\nOpção inválida!\n");
            }
        } while(!(opcaomenu == 1 || opcaomenu == 2 || opcaomenu == 3));
        //opcaomenu = getch();

        if(opcaomenu == 1){
            system(CLEAR);
            
            imprimirIntro();
            printf("Gerando mapa...\n");
            Sleep(500);
            system(CLEAR);
            geraMapa(mapa, 3); //DIFICULDADE 3 - DIFÍCIL

            //avatar *pontJogador = &jogador;
            int *pontPosicaoInical = getPosicaoInicial();
            
            strcpy(jogador.nome, nome);
            jogador.posicao[0] = *(pontPosicaoInical); //linha

            jogador.posicao[1] = *(pontPosicaoInical + 1); //coluna
            jogador.vidas = 3;
            jogador.pontuacao = 20;

            imprimeMapa(mapa);
            imprimeHud(&jogador);

            do {
                direcao = tolower(getch());

                if(direcao == '9'){
                    fimDeJogo(1, &jogador);
                }
                else if(!(direcao == 'a' || direcao == 's' || direcao == 'd' || direcao == 'w')){
                    printf("\n\nMovimento invalido\n");
                    getch();
                }
                else{
                    posicao_anterior[0] = jogador.posicao[0];
                    posicao_anterior[1] = jogador.posicao[1];

                    mover(direcao, jogador.posicao);
                    verificarColisao(&jogador, mapa, 0);

                    if(jogando == 0){
                        break;
                    }

                    mapa[jogador.posicao[0]][jogador.posicao[1]] = 1;
                    mapa[posicao_anterior[0]][posicao_anterior[1]] = 0;

                    system(CLEAR);

                    imprimeMapa(mapa);
                    imprimeHud(&jogador);

                    Sleep(250);

                    system(CLEAR);
                    atualizaMapa(mapa);
                    imprimeMapa(mapa);
                    imprimeHud(&jogador);
                    verificarColisao(&jogador, mapa, 1);
                    //sdasdsada
                }
            } while (jogando == 1);
        }
        else if(opcaomenu == 2){
            printf("\nRANKING TESTE\n");
            printf("\nPressione qulquer tecla para voltar ao menu.\n");
            getch();
        }
        else if(opcaomenu == 3){
            exit(0);
        }
    } while (continuarjogando == 1);
    return 0;
}


/*POSSÍVEIS FUNÇÕES
0 - MENU (JOGAR, MOSTRAR RANKING)
1 - MOVIMENTAR AVATAR ---------------------------------- OK
2 - FAZER PERGUNTA ------------------------------------- 
3 - ONDE ESTÁ O AVATAR? -------------------------------- OK
4 - ATUALIZAR A TELA ----------------------------------- OK
5 - VERIFICAR COLISÃO DO AVATAR ------------------------ OK
6 - VERIFICAR COLISÃO DA ATUALIZAÇÃO ------------------- OK
7 - VERIFICAR SE SAIU DO MAPA -------------------------- OK
8 - CALCULAR A PONTUACAO ------------------------------- 
9 - VERIFICAR VITÓRIA ---------------------------------- 
10 - FINALIZAR JOGO ------------------------------------ 
11 - LER ARQUIVO RANKING ------------------------------- OK
12 - GRAVAR ARQUIVO RANKING ---------------------------- OK
13 - VERIFICA SE PRECISA ATUALIZAR O RANKING ----------- OK
14 - IMPRIMIR PONTUAÇÃO E RANKING AO FINAL DO JOGO ----- OK
*/