#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

void set_raw(struct termios* old_tio) {
	struct termios new_tio;
	tcgetattr(0, &new_tio);
	*old_tio = new_tio;
	new_tio.c_lflag = 0;
	tcsetattr(0, TCSANOW, &new_tio);
}

void set_tio(struct termios* tio) {
	tcsetattr(0, TCSANOW, tio);
}

void draw_rectangle(int height, int width) {
	for(int i=0;i<height;i++) {
		for(int j=0;j<width;j++) {
			printf("*");
		}
		printf("\n");
	}
	printf("\033[A\033[%dC", width);
}

int main() {
	struct termios old_tio;
	struct winsize win;
	ioctl(1, TIOCGWINSZ, &win);
	printf("\033[H\033[J");
	set_raw(&old_tio);
	char c = 0;
	int width = 6, height = 3,
	    esc = 0, row = 0, column = 1;
	int drawing_row = win.ws_row - height - 1;
	printf("\033[%dB", drawing_row);
	draw_rectangle(height, width);
	while((c = fgetc(stdin))) {
		if(c == '\033') {
			esc = 1;
		} else if(esc == 1 && c == '[') {
			esc = 2;
		} else if(esc == 2 && c == 'D' && column > 1) { // go to the left
			int prev_column = width + 1;
			printf("\033[%dA", height);
			fflush(stdout);
			for(int i=0;i<height;i++) {
				printf("\033[B\033[D ");
				printf("\033[%dD", prev_column);
				for(int j=0;j<width;j++) {
					printf("*");
					fflush(stdout);
				}
				printf("\033[C");
			}	
			printf("\033[D");
			fflush(stdout);
			column--;
			//printf("\nMoving to the left \n\033[2A\033[%dC", (column + width)-1);
			esc = 0;
		} else if(esc == 2 && c == 'C' && column < win.ws_col-width) { // go to the right
			printf("\033[%dA", height);
			for(int i=0;i<height;i++) {
				printf("\033[B\033[%dD", column+width);
				for(int j=0;j<column;j++) { printf(" "); }
				for(int j=0;j<width;j++) { printf("*"); }
				fflush(stdout);
			}
			//printf("\nMoving to the right\n\033[2A\033[%dC", column+width);
			column++;
			fflush(stdout);
			esc = 0;
		} else if(c == ' ') {
			// delete square / rectangle
			for(int i=0;i<height;i++) {
				printf("\033[%dD", width);
				for(int j=0;j<width;j++) {
					printf(" ");
				}
				printf("\033[A");
			}
			// start drawing the jumping position of the square / rectangle
			printf("\033[H");
			if(column > 1) {
				printf("\033[%dC", column-1);
			}
			for(int i=0;i<height;i++) {
				for(int j=0;j<width;j++) { printf("*"); }
				printf("\033[%dD\033[B", width);
				fflush(stdout);
			}
			usleep(100000);
			// delete the jumped position of the square / rectangle
			for(int i=height;i>=0;i--) {
				printf("\033[A");
				for(int j=0;j<width;j++) {
					printf(" ");
				}
				printf("\033[%dD", width);
			}
			fflush(stdout);
			usleep(100000);
			// move back to the drawing position of the rectangle and draw it again
			printf("\033[%dB", drawing_row);
			for(int i=0;i<height;i++) {
				for(int j=0;j<width;j++) {
					printf("*");
				}
				printf("\033[%dD\033[B", width);
			}
			printf("\033[A\033[%dC", width);
			//printf("\nJumping            \n\033[2A\033[%dC", (column + width)-1);
			fflush(stdout);
		} else if(c == 'q' || c == 3 || c == 4) {
			printf("\n");
			break;
		}
	}
	set_tio(&old_tio);
	return 0;
}
