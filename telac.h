#ifndef TELAC_H
#define TELAC_H

void get_resize(int sig);
void tamanho_terminal(int *rows, int *cols);
void ativar_alt(); //ativar tela alternativa
void desativar_alt(); //desativar tela alternativa
void gotoxy(int x, int y); //"cursor para (x, y)"
void clear(); //limpar tela
void hide_cursor(); //esconder cursor
void show_cursor(); //mostrar cursor
void desenhar_manual(int x, int y, int largura, int altura); //desenhar tela
void desenhar_auto();
char getch(); //identificar tecla pressionada


#endif
