#ifndef TELAC_H
#define TELAC_H
#include <unistd.h>

extern int delay_ms;
extern int rows;
extern int cols;
extern int tecla;
extern int tela;
extern int hover_menu;

#define TECLA_CIMA 1001
#define TECLA_BAIXO 1002
#define TECLA_DIREITA 1003
#define TECLA_ESQUERDA 1004
#define TECLA_ENTER 1005
#define TECLA_ESC 27

void ativar_alt();
void desativar_alt();
#define INICIAR_TELA(titulo)                                                   \
  int rows_ant = 0, cols_ant = 0;                                              \
  init_terminal();                                                             \
  definir_titulo(titulo);                                                      \
  while (1) {                                                                  \
    int resize_event = 0;                                                      \
    tecla = ler_tecla();                                                       \
    if (tecla == 'q')                                                          \
      break;                                                                   \
    atualizar_tamanho_terminal();                                              \
    if (rows != rows_ant || cols != cols_ant) {                                \
      limpar();                                                                \
      rows_ant = rows;                                                         \
      cols_ant = cols;                                                         \
      resize_event = 1;                                                        \
    }

#define FINALIZAR_TELA                                                         \
  limpar_eventos();                                                            \
  usleep(delay_ms);                                                            \
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
void resetar_cores();
void texto_negrito();
void texto_sublinhado();

void definir_fps(int fps);
int obter_fps();

void esconder_cursor();
void mostrar_cursor();

void limpar();
void gotoxy(int x, int y);
void definir_titulo(const char *titulo);

void atualizar_tamanho_terminal();
void desenhar_borda(int rows, int cols);
void desenhar_borda_xy(int x_inicial, int y_inicial, int x_final, int y_final);

void init_terminal();
void restore_terminal();

int ler_tecla();

void criar_menu(int qtd_opcoes, ...);
void voltar_menu(int tecla_desejada);

void criar_menu_xy(int id_menu, int inicio_x, int inicio_y, int qtd_opcoes, ...);
int clicou_menu(int id_menu, int indice_opcao);
int criar_botao(int x, int y, const char *texto);
void limpar_eventos();

#endif