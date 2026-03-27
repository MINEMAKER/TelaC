#ifndef TELAC_H
#define TELAC_H
#include <unistd.h>

void ativar_alt();
void desativar_alt();
#define INICIAR_TELA(titulo)                                                   \
  int rows = 0, cols = 0;                                                      \
  int rows_ant = 0, cols_ant = 0;                                              \
  char tecla = 0;                                                              \
  init_terminal();                                                             \
  definir_titulo(titulo);                                                      \
  while (1) {                                                                  \
    tecla = ler_tecla();                                                       \
    if (tecla == 'q')                                                          \
      break;                                                                   \
    tamanho_terminal(&rows, &cols);                                            \
    if (rows != rows_ant || cols != cols_ant) {                                \
      clear();                                                                 \
      desenhar_borda(rows, cols);                                              \
      rows_ant = rows;                                                         \
      cols_ant = cols;                                                         \
    }

#define FINALIZAR_TELA                                                         \
  usleep(100000);                                                              \
  }                                                                            \
  restore_terminal();

typedef enum {
  PRETO = 30,
  VERMELHO = 31,
  VERDE = 32,
  AMARELO = 33,
  AZUL = 34,
  MAGENTA = 35,
  CIANO = 36,
  BRANCO = 37,
  PADRAO = 0
} Cor;

void texto_cor(Cor cor);
void fundo_cor(Cor cor);
void print_cor(Cor cor, const char *formato, ...);

void hide_cursor();
void show_cursor();

void clear();
void gotoxy(int x, int y);
void definir_titulo(const char *titulo);

void tamanho_terminal(int *rows, int *cols);
void desenhar_borda(int rows, int cols);

void init_terminal();
void restore_terminal();

char ler_tecla();

#endif
