#include "telac.h"
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

static struct termios oldt;

int delay_ms = 100000;
int rows = 0;
int cols = 0;
int tecla = 0;
int tela = 0;
int hover_menu = 0;

// ---------- TERMINAL ----------
void init_terminal() {
  struct termios newt;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);

  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
  setbuf(stdout, NULL);

  ativar_alt();
  esconder_cursor();
}

void restore_terminal() {
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  mostrar_cursor();
  desativar_alt();
}

// ---------- ANSI ----------
void ativar_alt() { printf("\033[?1049h"); }

void desativar_alt() { printf("\033[?1049l"); }

void esconder_cursor() { printf("\033[?25l"); }

void mostrar_cursor() { printf("\033[?25h"); }

void limpar() {
  printf("\033[2J\033[H");
}

void gotoxy(int x, int y) { printf("\033[%d;%dH", y, x); }

void definir_titulo(const char *titulo) { printf("\033]0;%s\007", titulo); }

// ---------- CORES ----------
void texto_cor(Cor cor) { printf("\033[%dm", cor); }

void fundo_cor(Cor cor) {
  if (cor == PADRAO) {
    printf("\033[0m");
  } else {
    printf("\033[%dm", cor + 10);
  }
}

void print_cor(Cor cor, const char *formato, ...) {
  texto_cor(cor);

  va_list args;
  va_start(args, formato);
  vprintf(formato, args);
  va_end(args);

  texto_cor(PADRAO);
}

void resetar_cores() { printf("\033[0m"); }
void texto_negrito() { printf("\033[1m"); }
void texto_sublinhado() { printf("\033[4m"); }

void definir_fps(int fps) {
  if (fps > 0)
    delay_ms = 1000000 / fps;
}

int obter_fps() {
  if (delay_ms > 0) return 1000000 / delay_ms;
  return 0;
}

// ---------- TERMINAL SIZE ----------
void atualizar_tamanho_terminal() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  rows = w.ws_row;
  cols = w.ws_col;
}

// ---------- INPUT ----------
int ler_tecla() {
  char c;
  if (read(STDIN_FILENO, &c, 1) > 0) {
    if (c == '\033') {
      char seq[2];
      if (read(STDIN_FILENO, &seq[0], 1) == 0) return TECLA_ESC;
      if (read(STDIN_FILENO, &seq[1], 1) == 0) return TECLA_ESC;

      if (seq[0] == '[') {
        if (seq[1] == 'A') return TECLA_CIMA;
        if (seq[1] == 'B') return TECLA_BAIXO;
        if (seq[1] == 'C') return TECLA_DIREITA;
        if (seq[1] == 'D') return TECLA_ESQUERDA;
      }
      return TECLA_ESC;
    }
    if (c == '\r' || c == '\n') return TECLA_ENTER;
    return c;
  }
  return 0;
}

// ---------- DESENHAR ----------
void desenhar_borda(int rows, int cols) {
  gotoxy(1, 1);
  for (int x = 1; x <= cols; x++) printf("#");
  
  gotoxy(1, rows);
  for (int x = 1; x <= cols; x++) printf("#");

  for (int y = 2; y < rows; y++) {
    gotoxy(1, y);
    printf("#");
    gotoxy(cols, y);
    printf("#");
  }
}

void desenhar_borda_xy(int x_inicial, int y_inicial, int x_final, int y_final) {
  gotoxy(x_inicial, y_inicial);
  printf("+");
  for (int x = x_inicial + 1; x < x_final; x++) printf("-");
  printf("+");

  gotoxy(x_inicial, y_final);
  printf("+");
  for (int x = x_inicial + 1; x < x_final; x++) printf("-");
  printf("+");

  for (int y = y_inicial + 1; y < y_final; y++) {
    gotoxy(x_inicial, y);
    printf("|");
    gotoxy(x_final, y);
    printf("|");
  }
}

void criar_menu(int qtd_opcoes, ...) {
  if (tela != 0)
    return;

  if (tecla == 'w' || tecla == 'W' || tecla == TECLA_CIMA) {
    if (hover_menu > 0)
      hover_menu--;
  }
  if (tecla == 's' || tecla == 'S' || tecla == TECLA_BAIXO) {
    if (hover_menu < qtd_opcoes - 1)
      hover_menu++;
  }

  int inicio_y = (rows / 2) - (qtd_opcoes / 2);
  int inicio_x = (cols / 2) - 10;

  va_list args;
  va_start(args, qtd_opcoes);

  for (int i = 0; i < qtd_opcoes; i++) {
    const char *opcao = va_arg(args, const char *);
    gotoxy(inicio_x, inicio_y + i);
    if (i == hover_menu)
      print_cor(VERDE, "-> %s", opcao);
    else
      printf("   %s", opcao);
  }
  va_end(args);

  if (tecla == TECLA_ENTER || tecla == ' ') {
    tela = hover_menu + 1;
    limpar();
  }
}

void voltar_menu(int tecla_desejada) {
  if (tecla == tecla_desejada) {
    tela = 0;
    limpar();
  }
}
