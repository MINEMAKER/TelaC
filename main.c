#include "telac.h"
#include <stdio.h>
#include <unistd.h>

int main() {
  int rows = 0, cols = 0;
  int rows_ant = 0, cols_ant = 0;

  init_terminal();

  while (1) {
    char tecla = ler_tecla();

    if (tecla == 'q')
      break;

    tamanho_terminal(&rows, &cols);

    if (rows != rows_ant || cols != cols_ant) {
      clear();
      desenhar_borda(rows, cols);
      rows_ant = rows;
      cols_ant = cols;
    }

    gotoxy(3, 3);
    printf("Linhas: %d, Colunas: %d", rows, cols);
    usleep(100000);
  }

  restore_terminal();
  return 0;
}
