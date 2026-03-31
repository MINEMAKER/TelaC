#ifndef TELAC_H
#define TELAC_H
#include <unistd.h>

extern int rows;
extern int cols;
extern char tecla;
extern int tela;
extern int hover_menu;

void ativar_alt();
void desativar_alt();
#define INICIAR_TELA(titulo)                                                   \
  int rows_ant = 0, cols_ant = 0;                                              \
  init_terminal();                                                             \
  definir_titulo(titulo);                                                      \
  while (1) {                                                                  \
    tecla = ler_tecla();                                                       \
    if (tecla == 'q')                                                          \
      break;                                                                   \
    tamanho_terminal(&rows, &cols);                                            \
    if (rows != rows_ant || cols != cols_ant) {                                \
      limpar();                                                                \
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

void esconder_cursor();
void mostrar_cursor();

void limpar();
void gotoxy(int x, int y);
void definir_titulo(const char *titulo);

void tamanho_terminal(int *rows, int *cols);
void desenhar_borda(int rows, int cols);
void desenhar_borda_xy(int x_inicial, int y_inicial, int x_final, int y_final);

void init_terminal();
void restore_terminal();

char ler_tecla();

void criar_menu(int qtd_opcoes, ...);
void voltar_menu(char tecla_desejada);

#endif
