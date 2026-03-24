#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include "telac.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>

volatile int resized = 1;

int kbhit() {
    struct timeval tv = {0L, 0L};
    fd_set fds;

    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
}

void get_resize(int sig) {
	resized = 1;
}

void tamanho_terminal(int *rows, int *cols) {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	*rows = w.ws_row;
	*cols = w.ws_col;
}

void ativar_alt() {
	printf("\033[?1049h");
}

void desativar_alt() {
	printf("\033[?1049l");
}

void gotoxy(int x, int y) {
	printf("\033[%d;%dH",y,x);
	fflush(stdout);
}

void clear() {
	printf("\033[2J");
	printf("\033[H");
}

void hide_cursor() {
	printf("\033[?25l");
}

void show_cursor() {
	printf("\033[?25h");
}

void desenhar_manual(int x, int y, int largura, int altura) {
	int i;
	char tecla;

	ativar_alt();
	hide_cursor();
	clear();
	printf("Pressione 'q' para sair\n");
	while (1){

		//desenhar borda superior
		gotoxy(x, y);
		printf("#");
		for (i=0;i<largura;i++)printf("#");
		printf("#");

		//desenhar laterais
		for (i=1; i <= altura; i++) {
			gotoxy(x, y +i);
			printf("#");
			gotoxy(x + largura + 1, y + i);
			printf("#");
		}

		//desenhar borda inferior
		gotoxy(x, y + altura + 1);
		printf("#");
		for (i=0; i < largura; i++)printf("#");
		printf("#");

		tecla = getch();
		if(tecla == 'q'){
			desativar_alt();
			show_cursor();
			break;
		}
	}
}

void desenhar_auto() {
	struct termios oldt, newt;
	int rows, cols;
	char tecla;

	signal(SIGWINCH, get_resize);
	ativar_alt();
	hide_cursor();

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	while(1){
		if(resized){
			tamanho_terminal(&rows, &cols);
			clear();

			for(int y = 1; y <= rows; y++){
				printf("\033[%d;1H",y);
                		for(int x = 1; x <= cols; x++){
                        		if(y==1 || y==rows || x==1 || x==cols){
                                		printf("#");
					}
                        		else
                                		printf(" ");
                		}
        		}
			fflush(stdout);
			resized = 0;
		}
		if (kbhit()) {
                	tecla = getchar();
                	if(tecla == 'q') break;
                }
		usleep(50000);
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	show_cursor();
	desativar_alt();
}


char getch() {
    return getchar();
}
