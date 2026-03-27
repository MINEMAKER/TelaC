#include "telac.h"
#include <stdio.h>

int main() {

  INICIAR_TELA("Teste");

  gotoxy(5, 5);
  printf("Teste");

  FINALIZAR_TELA
  return 0;
}
