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

int _menu_clicado_id = -1;
int _menu_clicado_opcao = -1;
int _menu_processado_frame = 0;

int _botao_hover = 1;
int _botao_count = 0;
int _botao_count_max = 0;
int _botao_clicado = 0;
int _botao_processado_frame = 0;

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

void resetar_cores() { printf("\033[0m"); }
void texto_negrito() { printf("\033[1m"); }
void texto_sublinhado() { printf("\033[4m"); }

void definir_fps(int fps) {
  if (fps > 0)
    delay_ms = 1000000 / fps;
}

int obter_fps() {
  if (delay_ms > 0)
    return 1000000 / delay_ms;
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
      if (read(STDIN_FILENO, &seq[0], 1) == 0)
        goto retorna_esc;
      if (read(STDIN_FILENO, &seq[1], 1) == 0)
        goto retorna_esc;

      if (seq[0] == '[') {
        switch (seq[1]) {
        case 'A':
          return TECLA_CIMA;
        case 'B':
          return TECLA_BAIXO;
        case 'C':
          return TECLA_DIREITA;
        case 'D':
          return TECLA_ESQUERDA;
        }
      }
    retorna_esc:
      return TECLA_ESC;
    }
    if (c == '\r' || c == '\n')
      return TECLA_ENTER;
    return c;
  }
  return 0;
}

// ---------- DESENHAR ----------
void desenhar_borda(int rows, int cols) {
  static char linha[512] = {0};
  if (linha[0] == 0) {
    for (int i = 0; i < 511; i++)
      linha[i] = '#';
  }

  if (cols > 512)
    cols = 512;

  gotoxy(1, 1);
  printf("%.*s", cols, linha);

  gotoxy(1, rows);
  printf("%.*s", cols, linha);

  for (int y = 2; y < rows; y++) {
    gotoxy(1, y);
    printf("#");
    gotoxy(cols, y);
    printf("#");
  }
}

void desenhar_borda_xy(int x_inicial, int y_inicial, int x_final, int y_final) {
  int largura = x_final - x_inicial - 1;
  if (largura < 0)
    largura = 0;
  if (largura > 512)
    largura = 512;

  static char traco[512] = {0};
  if (traco[0] == 0) {
    for (int i = 0; i < 511; i++)
      traco[i] = '-';
  }

  gotoxy(x_inicial, y_inicial);
  printf("+%.*s+", largura, traco);

  gotoxy(x_inicial, y_final);
  printf("+%.*s+", largura, traco);

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

  // Trava de seguranca para menus de tamanhos diferentes
  if (hover_menu >= qtd_opcoes)
    hover_menu = qtd_opcoes - 1;

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
    tecla = 0;      // Consome a tecla!
    hover_menu = 0; // Volta a setinha pro topo da proxima tela
    limpar();
  }
}

void voltar_menu(int tecla_desejada) {
  if (tecla == tecla_desejada) {
    tela = 0;
    tecla = 0;      // Consome a tecla!
    hover_menu = 0; // Zera a selecao do menu principal
    limpar();
  }
}

// ---------- OPCAO B: MENU GENERICO ----------
void criar_menu_xy(int id_menu, int inicio_x, int inicio_y, int qtd_opcoes,
                   ...) {
  // Trava de seguranca
  if (hover_menu >= qtd_opcoes)
    hover_menu = qtd_opcoes - 1;

  if (!_menu_processado_frame) {
    if (tecla == 'w' || tecla == 'W' || tecla == TECLA_CIMA) {
      if (hover_menu > 0)
        hover_menu--;
    }
    if (tecla == 's' || tecla == 'S' || tecla == TECLA_BAIXO) {
      if (hover_menu < qtd_opcoes - 1)
        hover_menu++;
    }
    _menu_processado_frame = id_menu; // Marca qual menu leu o teclado
  }

  va_list args;
  va_start(args, qtd_opcoes);

  for (int i = 0; i < qtd_opcoes; i++) {
    const char *opcao = va_arg(args, const char *);
    gotoxy(inicio_x, inicio_y + i);

    if (i == hover_menu && _menu_processado_frame == id_menu)
      print_cor(VERDE, "-> %s", opcao);
    else
      printf("   %s", opcao);
  }
  va_end(args);

  if ((tecla == TECLA_ENTER || tecla == ' ') &&
      _menu_processado_frame == id_menu) {
    _menu_clicado_id = id_menu;
    _menu_clicado_opcao = hover_menu;
    tecla = 0; // Consome a tecla!
  }
}

int clicou_menu(int id_menu, int indice_opcao) {
  if (_menu_clicado_id == id_menu && _menu_clicado_opcao == indice_opcao) {
    return 1;
  }
  return 0;
}

// ---------- OPCAO C: BOTOES INDEPENDENTES ----------
int criar_botao(int x, int y, const char *texto) {
  _botao_count++;
  int meu_id = _botao_count;

  if (!_botao_processado_frame) {
    if (tecla == 'a' || tecla == 'A' || tecla == TECLA_ESQUERDA) {
      if (_botao_hover > 1)
        _botao_hover--;
    }
    if (tecla == 'd' || tecla == 'D' || tecla == TECLA_DIREITA) {
      if (_botao_hover < _botao_count_max)
        _botao_hover++;
    }
    _botao_processado_frame = 1;
  }

  gotoxy(x, y);
  if (_botao_hover == meu_id) {
    print_cor(VERDE, "[ %s ]", texto);
  } else {
    printf("  %s  ", texto);
  }

  if ((tecla == TECLA_ENTER || tecla == ' ') && _botao_hover == meu_id) {
    _botao_clicado = meu_id;
    tecla = 0; // Consome a tecla!
  }

  if (_botao_clicado == meu_id)
    return 1;
  return 0;
}

// ---------- CLEAR ----------
void limpar_eventos() {
  _menu_clicado_id = -1;
  _menu_clicado_opcao = -1;
  _menu_processado_frame = 0;

  _botao_count_max = _botao_count;
  _botao_count = 0;
  _botao_clicado = 0;
  _botao_processado_frame = 0;

  if (_botao_hover > _botao_count_max && _botao_count_max > 0) {
    _botao_hover = _botao_count_max;
  }
}
