#include "telac.h"
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

static struct termios oldt;

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

void limpar() { printf("\033[2J\033[H"); }

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

// ---------- TERMINAL SIZE ----------
void tamanho_terminal(int *rows, int *cols) {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  *rows = w.ws_row;
  *cols = w.ws_col;
}

// ---------- INPUT ----------
char ler_tecla() {
  char c;
  if (read(STDIN_FILENO, &c, 1) > 0)
    return c;
  return 0;
}

// ---------- DESENHAR ----------
void desenhar_borda(int rows, int cols) {
  for (int y = 1; y <= rows; y++) {
    printf("\033[%d;1H", y);

    for (int x = 1; x <= cols; x++) {
      if (y == 1 || y == rows || x == 1 || x == cols)
        printf("#");
      else
        printf(" ");
    }
  }
}

void desenhar_borda_xy(int x_inicial, int y_inicial, int x_final, int y_final) {
  for (int y = y_inicial; y <= y_final; y++) {
    printf("\033[%d;%dH", y, x_inicial);

    for (int x = x_inicial; x <= x_final; x++) {
      if ((y == y_inicial || y == y_final) && (x == x_inicial || x == x_final))
        printf("+");
      else if (y == y_inicial || y == y_final)
        printf("-");
      else if (x == x_inicial || x == x_final)
        printf("|");
      else
        printf(" ");
    }
  }
}
