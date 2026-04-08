#include "telac.h"
#include <stdio.h>

int main() {
  INICIAR_TELA("Tela com menu");

  // Menu = 0
  criar_menu(3, "Jogar", "Configuracoes", "Sair");

  // Tela 1: Jogo
  if (tela == 1) {
    desenhar_borda(rows, cols);
    gotoxy(5, (rows - 1));
    printf(" Pressione 'v' para voltar ao menu principal ");
    voltar_menu('v');
  }

  // Tela 2: Opções
  if (tela == 2) {
    desenhar_borda_xy(5, 5, 65, 18);
    gotoxy(7, 7);
    print_cor(AZUL, "C O N F I G U R A C O E S");
    criar_menu_xy(2, 7, 10, 4, "Facil", "Medio", "Dificil", "Impossivel");

    gotoxy(7, 15);
    AO_CLICAR_MENU(2, 0) print_cor(VERMELHO, "Voce clicou em Facil     ");
    AO_CLICAR_MENU(2, 1) print_cor(VERMELHO, "Voce clicou em Medio     ");
    AO_CLICAR_MENU(2, 2) print_cor(VERMELHO, "Voce clicou em Dificil   ");
    AO_CLICAR_MENU(2, 3) print_cor(VERMELHO, "Voce clicou em Impossivel");
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
