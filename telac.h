#ifndef TELAC_H
#define TELAC_H

void ativar_alt();
void desativar_alt();

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

void tamanho_terminal(int *rows, int *cols);
void desenhar_borda(int rows, int cols);

void init_terminal();
void restore_terminal();

char ler_tecla();

#endif
