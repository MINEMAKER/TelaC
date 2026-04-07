#include "telac.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_SNAKE 1000

int main() {
  srand(time(NULL));

  // Variáveis do Estado do Jogo (criadas antes da macro pra não resetarem)
  int cobrinha_x[MAX_SNAKE] = {0};
  int cobrinha_y[MAX_SNAKE] = {0};
  int tamanho = 3;
  int dir_x = 1, dir_y = 0;

  int maca_x = 0, maca_y = 0;
  int maca_ativa = 0;
  int pontuacao = 0;
  int game_over = 0;
  int tela_anterior = -1;
  INICIAR_TELA("Jogo da cobrinha");

  if (tela != tela_anterior || resize_event) {
    desenhar_borda(rows, cols);
    tela_anterior = tela;
  }

  // Menu = 0
  criar_menu(3, "Jogar", "Configuracoes", "Sair");

  // Tela 1: Jogo
  if (tela == 1) {
    if (cobrinha_x[0] == 0 && cols > 4) {
      for (int i = 0; i < tamanho; i++) {
        cobrinha_x[i] = (cols / 2) - i;
        cobrinha_y[i] = rows / 2;
      }
    }

    // CONTROLE: Lê as teclas do usuário (usando a var 'tecla' criada na sua
    // macro)
    if (!game_over) {
      if ((tecla == 'w' || tecla == 'W' || tecla == TECLA_CIMA) && dir_y == 0) {
        dir_x = 0;
        dir_y = -1;
      }
      if ((tecla == 's' || tecla == 'S' || tecla == TECLA_BAIXO) &&
          dir_y == 0) {
        dir_x = 0;
        dir_y = 1;
      }
      if ((tecla == 'a' || tecla == 'A' || tecla == TECLA_ESQUERDA) &&
          dir_x == 0) {
        dir_x = -1;
        dir_y = 0;
      }
      if ((tecla == 'd' || tecla == 'D' || tecla == TECLA_DIREITA) &&
          dir_x == 0) {
        dir_x = 1;
        dir_y = 0;
      }
    }

    // UPDATE: Lógica principal
    if (!game_over && cobrinha_x[0] != 0) {
      // Apaga a pontinha do rabo da tela (para não rastro)
      gotoxy(cobrinha_x[tamanho - 1], cobrinha_y[tamanho - 1]);
      printf(" ");

      // Move todos os pedaços do corpo acompanhando a cabeça
      for (int i = tamanho - 1; i > 0; i--) {
        cobrinha_x[i] = cobrinha_x[i - 1];
        cobrinha_y[i] = cobrinha_y[i - 1];
      }

      // Move a cabeça de acordo com a direção
      cobrinha_x[0] += dir_x;
      cobrinha_y[0] += dir_y;

      // COLISÕES
      // Bateu na parede?
      if (cobrinha_x[0] <= 1 || cobrinha_x[0] >= cols || cobrinha_y[0] <= 1 ||
          cobrinha_y[0] >= rows) {
        game_over = 1;
      }
      // Bateu no próprio corpo?
      for (int i = 1; i < tamanho; i++) {
        if (cobrinha_x[0] == cobrinha_x[i] && cobrinha_y[0] == cobrinha_y[i]) {
          game_over = 1;
        }
      }

      // MAÇÃ: Comer maçã e nascer nova
      if (cobrinha_x[0] == maca_x && cobrinha_y[0] == maca_y) {
        if (tamanho < MAX_SNAKE)
          tamanho++;
        maca_ativa = 0;
        pontuacao += 10;
      }

      if (!maca_ativa && cols > 4) {
        maca_x = (rand() % (cols - 2)) + 2;
        maca_y = (rand() % (rows - 2)) + 2;
        maca_ativa = 1;
      }
    }

    // DESENHO EM TELA (RENDER)
    if (!game_over && cobrinha_x[0] != 0) {
      // Desenha a maçã
      if (maca_ativa) {
        gotoxy(maca_x, maca_y);
        print_cor(VERMELHO, "@");
      }

      // Desenha a cabeça
      gotoxy(cobrinha_x[0], cobrinha_y[0]);
      print_cor(VERDE, "O");
      // Opcional: redilha o pescoço usando minúscula (já que só o rabo é
      // apagado)
      if (tamanho > 1) {
        gotoxy(cobrinha_x[1], cobrinha_y[1]);
        print_cor(VERDE, "o");
      }

      // Placar
      gotoxy(4, 1);
      printf(" Placar: %d ", pontuacao);
    }

    if (game_over) {
      // Tela de Fim de Jogo
      gotoxy((cols / 2) - 5, rows / 2);
      print_cor(VERMELHO, "FIM DE JOGO!");
      gotoxy((cols / 2) - 13, (rows / 2) + 1);
      printf("Aperte 'q' para abandonar.");
    }
    gotoxy(5, rows);
    printf(" Pressione 'v' para voltar ao menu principal ");

    voltar_menu('v');
  }

  // Tela 2: Opções
  if (tela == 2) {
    desenhar_borda_xy(5, 5, 65, 18);
    gotoxy(7, 7);
    print_cor(AZUL, "C O N F I G U R A C O E S");
    criar_menu_xy(2, 7, 10, 4, "Facil", "Medio", "Dificil", "Impossivel");

    if (clicou_menu(2, 0)) {
      gotoxy(7, 15);
      print_cor(VERMELHO, "Voce clicou em Facil");
      limpar();
      tela = 0;
    }
    if (clicou_menu(2, 1)) {
      gotoxy(7, 15);
      print_cor(VERMELHO, "Voce clicou em Medio");
      limpar();
      tela = 0;
    }
    if (clicou_menu(2, 2)) {
      gotoxy(7, 15);
      print_cor(VERMELHO, "Voce clicou em Dificil");
      limpar();
      tela = 0;
    }
    if (clicou_menu(2, 3)) {
      gotoxy(7, 15);
      print_cor(VERMELHO, "Voce clicou em Impossivel");
      limpar();
      tela = 0;
    }

    gotoxy(6, 20);
    printf("Aperte 'v' para voltar...");

    voltar_menu('v');
  }

  // Tela 3: Sair
  if (tela == 3) {
    break;
  }

  FINALIZAR_TELA
  return 0;
}