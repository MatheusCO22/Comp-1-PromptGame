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
#include "perguntas.c"

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
void verificarColisao(avatar *jogador, int mapa[10][10], int flag, struct pergunta perguntas[], int posicao_anterior[]);
void imprimirIntro();
void imprimeHud(avatar *jogador);
void fimDeJogo(int resultadoJogo, avatar *jogador);
void fazerPergunta(avatar *jogador, struct pergunta perguntas[], int mapa[10][10], int posicao_anterior[]);

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
}

void verificarColisao(avatar *jogador, int mapa[10][10], int flag, struct pergunta perguntas[], int posicao_anterior[]){
    int linhajogador = jogador->posicao[0];
    int colunajogador = jogador->posicao[1];

    if((linhajogador < 0 || colunajogador < 0) || (linhajogador > 9 || colunajogador > 9)){
        printf("\nSaiu do mapa\n");
        fimDeJogo(2, jogador);
        //getch();
    }
    else if(mapa[linhajogador][colunajogador] == 0){
        mapa[jogador->posicao[0]][jogador->posicao[1]] = 1;
        mapa[posicao_anterior[0]][posicao_anterior[1]] = 0;
    }
    else if(mapa[linhajogador][colunajogador] == 2){
        mapa[jogador->posicao[0]][jogador->posicao[1]] = 1;
        mapa[posicao_anterior[0]][posicao_anterior[1]] = 0;

        printf("\nChegou no objetivo.\n");
        getch();
        fimDeJogo(1, jogador);
    }
    else if(mapa[linhajogador][colunajogador] == 3){
        if(flag == 0){
            printf("\nVoc?? encontrou com um professor. Pressione qualquer bot??o para responder ?? pergunta. \n");
        } else{
            printf("\nUm professor te encontrou. Pressione qualquer bot??o para responder ?? pergunta.\n");
        }
        getch();
        fazerPergunta(jogador, perguntas, mapa, posicao_anterior);
        //imprimeMapa(mapa);
        //imprimeHud(jogador);
    }
    else if(mapa[linhajogador][colunajogador] == 4){
        mapa[jogador->posicao[0]][jogador->posicao[1]] = 1;
        mapa[posicao_anterior[0]][posicao_anterior[1]] = 0;

        if(jogador->vidas<3){
            jogador->vidas +=1;
            printf("\nVoc?? encontrou um b??nus (+ 1 vida)\n");
        }else{
            printf("\nVoc?? encontrou um b??nus (+ X pontos)\n");

        }
        getch();
    }
}

void imprimirIntro(){
    printf("Voc?? est?? sobrevivendo ??s semanas finais do semestre letivo, e no momento, tem um ??nico desejo: Comprar um delicioso brigadeiro para aliviar o estresse.\n");
    printf("Por??m, a mo??a que vende tais doces est?? se movendo, em busca de novos clientes.\n");
    printf("\nAt?? aqui tudo parece simples, basta ir em dire????o ?? mo??a. Por??m, voc?? est?? praticamente cercado por professores de C??lculo, que podem ou n??o ser fruto da sua imagina????o, j?? um tanto deteriorada.\n");
    printf("Caso voc?? encontre com um professor, ele lhe far?? uma pergunta. A depender da sua resposta, voc?? perder?? uma vida, podendo ou n??o continuar sua jornada em busca do brigadeiro.\n");
    printf("Ao longo do percurso, voc?? poder?? encontrar alguns b??nus, que aumentar??o sua pontua????o final, e lhe dar??o uma vida extra, caso voc?? tenha perdido uma das 3 vidas iniciais.\n");
    printf("Voc?? n??o pode sair do mapa durante o jogo, isso resultar?? em uma derrota instant??nea.");
    printf("\nOs ??cones do mapa s??o os seguintes:\n\n");

    printf(YEL "\u263B" RESET);
    printf(" - Seu avatar \t");
    printf(WHT " \u25C9" RESET);
    printf(" - Seu objetivo (brigadeiro)\n");
    printf(GRN "\u2665" RESET);
    printf(" - Os b??nus \t");
    printf(RED " \u2620" RESET);
    printf(" - Os professores\n");


    printf("\nUse as teclas WASD para se mover pelo mapa.\nPressione qualquer tecla para iniciar o jogo\n");
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
    printf("\nLINHA: %d, COLUNA: %d\n", (linhajogador + 1), (colunajogador + 1));
    printf("\nPONTOS: %d", 1000);
    printf("\n");
}

void fazerPergunta(avatar *jogador, struct pergunta perguntas[], int mapa[10][10], int posicao_anterior[]){
    int vidas = jogador->vidas;
    int codPergunta;
    float respostaJogador;
    int acerto = 0;

    codPergunta = randomInteger(1,numPerguntas);
    struct pergunta perguntaSorteada = perguntas[codPergunta - 1];

    //system(CLEAR);

    imprimirPergunta(perguntaSorteada);
    printf("\n");

    printf("Digite a resposta (valor num??rico apenas): ");
    scanf("%f", &respostaJogador);

    if(perguntaSorteada.tipo == 'N'){
        if(respostaJogador == perguntaSorteada.respN){
            acerto = 1;
        }
    }

    if(perguntaSorteada.tipo == 'O'){
        if(respostaJogador == perguntaSorteada.respO){
            acerto = 1;
        }
    }
    if(acerto == 1){
        mapa[jogador->posicao[0]][jogador->posicao[1]] = 1;
        mapa[posicao_anterior[0]][posicao_anterior[1]] = 0;

        
        printf("\nVoc?? acertou! Parab??ns - Pressione qualquer tecla para continuar o jogo\n");
        getch();
        //Sleep(500);
    }
    else{
        jogador->vidas--;
        if(jogador->vidas == 0){
            fimDeJogo(0,jogador);
        }
        else{
            mapa[jogador->posicao[0]][jogador->posicao[1]] = 0;
            mapa[posicao_anterior[0]][posicao_anterior[1]] = 1;

            jogador->posicao[0] = posicao_anterior[0];
            jogador->posicao[1] = posicao_anterior[1];

            printf("\nResposta incorreta. Seu movimento n??o ser?? executado, e voc?? perder?? uma vida :( - Pressione qualquer tecla para continuar o jogo\n");
            getch();
            //Sleep(500);
        }
    }
}

void fimDeJogo(int resultadoJogo, avatar *jogador){
    char jogarnovamente;
    // 1 - Vitoria // 0 - Derrota (sem vidas) // 2 - Derrota (saiu do mapa)

    jogando = 0;
    pontuacao ranking[3];
    obtemRanking(ranking);
    
    int pontos = jogador->pontuacao;
    char *nome = jogador->nome;

    if(resultadoJogo == 1){
        system(CLEAR);
        printf("Parab??ns pela vit??ria, %s! Voc?? fez %d pontos.", nome, pontos);

        atualizaRanking(ranking, pontos, nome);
    }
    else if(resultadoJogo == 0){
        system(CLEAR);
        printf("\nQue pena, %s! Voc?? ficou sem vidas, mais sorte na pr??xima vez...", nome);
    }
    else if(resultadoJogo == 2){
        printf("\nOps, %s... Voc?? saiu da universidade, sua busca pelo brigadeiro se encerra sem sucesso :(", nome);
    }
    
    printf("\nNossos melhores 3 jogadores at?? agora s??o: \n");
    imprimeRankingArquivo();

    printf("\nPressione qualquer tecla para voltar ao menu.\n");
    getch();
}

void inicializarVetorPerguntas(struct pergunta perguntas[], int tam){
    inicializarPerguntas(perguntas, tam);
    carregarPerguntasArquivo(perguntas);

    /* imprimirTodasPerguntas(perguntas, tam);
    getch();
    system(CLEAR);
    printf("\n\nNUM PERGUNTAS: %d\n", contarPerguntas(perguntas, tam));
    getch(); */
    
    system(CLEAR);
}

int main () {
    setlocale(LC_ALL,"Portuguese_Brasil");
    system(CLEAR);
    
    int mapa[10][10], posicao_anterior[2];
    int opcaomenu;
    char direcao;
    char nome[15];
    int tamNome = 0;
    int tam = 50;
    struct pergunta perguntas[tam];

    inicializarVetorPerguntas(perguntas, tam);

    printf("\nOl??, seja bem-vindo! Antes de come??ar a jogar, digite seu nick: ");

    do{
        scanf("%s", &nome);

        tamNome = strlen(nome);

        if(tamNome > 15 || tamNome < 3){
            printf("\nTamanho inv??lido, seu nick deve conter entre 3 e 15 caracteres.\n");
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
                printf("\nOp????o inv??lida!\n");
            }
        } while(!(opcaomenu == 1 || opcaomenu == 2 || opcaomenu == 3));
        //opcaomenu = getch();

        if(opcaomenu == 1){
            system(CLEAR);
            
            imprimirIntro();
            printf("Gerando mapa...\n");
            Sleep(500);
            system(CLEAR);
            geraMapa(mapa, 3); //DIFICULDADE 3 - DIF??CIL

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

                //Tecla para encerramento instantaneo para fins de teste
                /* if(direcao == '9'){
                    fimDeJogo(1, &jogador);
                } */
                if(!(direcao == 'a' || direcao == 's' || direcao == 'd' || direcao == 'w')){
                    printf("\n\nMovimento invalido\n");
                    getch();
                }
                else{
                    posicao_anterior[0] = jogador.posicao[0];
                    posicao_anterior[1] = jogador.posicao[1];

                    mover(direcao, jogador.posicao);
                    verificarColisao(&jogador, mapa, 0, perguntas, posicao_anterior);

                    if(jogando == 0){
                        break;
                    }

                    system(CLEAR);

                    imprimeMapa(mapa);
                    imprimeHud(&jogador);

                    Sleep(250);

                    atualizaMapa(mapa);
                    system(CLEAR);

                    imprimeMapa(mapa);
                    imprimeHud(&jogador);
                    verificarColisao(&jogador, mapa, 1, perguntas, posicao_anterior);
                    
                    system(CLEAR);

                    imprimeMapa(mapa);
                    imprimeHud(&jogador);
                }
            } while (jogando == 1);
        }
        else if(opcaomenu == 2){
            system(CLEAR);
            printf("\nTOP 3 Lendas do Brigadeiro\n");
            imprimeRankingArquivo();
            printf("\nPressione qualquer tecla para voltar ao menu.\n");
            getch();
        }
        else if(opcaomenu == 3){
            exit(0);
        }
    } while (continuarjogando == 1);
    return 0;
}

/*POSS??VEIS FUN????ES
0 - MENU (JOGAR, MOSTRAR RANKING)
0 - HIST??RIA -------- ---------------------------------- OK
1 - MOVIMENTAR AVATAR ---------------------------------- OK
2 - FAZER PERGUNTA ------------------------------------- OK
3 - ONDE EST?? O AVATAR? -------------------------------- OK
4 - ATUALIZAR A TELA ----------------------------------- OK
5 - VERIFICAR COLIS??O DO AVATAR ------------------------ OK
6 - VERIFICAR COLIS??O DA ATUALIZA????O ------------------- OK
7 - VERIFICAR SE SAIU DO MAPA -------------------------- OK
8 - CALCULAR A PONTUACAO ------------------------------- 
9 - VERIFICAR VIT??RIA ---------------------------------- OK
10 - FINALIZAR JOGO ------------------------------------ OK
11 - LER ARQUIVO RANKING ------------------------------- OK
12 - GRAVAR ARQUIVO RANKING ---------------------------- OK
13 - VERIFICA SE PRECISA ATUALIZAR O RANKING ----------- OK
14 - IMPRIMIR PONTUA????O E RANKING AO FINAL DO JOGO ----- OK

CADASTRAR MAIS PERGUNTAS
ELIMINAR ETAPAS INTERMEDIARIAS (PAUSAS NO TERMINAL)
*/
