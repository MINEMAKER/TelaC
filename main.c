#include "telac.h"
#include <stdio.h>

int main() {
  INICIAR_TELA("Tela com menu");

  // Menu = 0
  criar_menu(3, "Jogar", "Configuracoes", "Sair");

  // Tela 1: Jogo
  if (tela == 1) {
    desenhar_borda(rows, cols);
    gotoxy(5, rows);
    printf(" Pressione 'v' para voltar ao menu principal ");
    voltar_menu('v');
  }

  // Tela 2: Opções
  if (tela == 2) {
    desenhar_borda_xy(5, 5, 40, 15);
    gotoxy(7, 7);
    print_cor(AZUL, "C O N F I G U R A C O E S");
    gotoxy(7, 10);
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
