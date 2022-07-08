#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

void print_options(char* options[], size_t size, int* pos, const char* code) {
	for(size_t i=0;i<size;i++) {
		if(pos != NULL && i == (*pos)-1) {
			printf("%s", code);
		}
		if(i == size-1) {
			size_t n = strlen(options[i])+3;
			printf("\033[31m%ld.%s\n\033[%ldD\033[A\033[0m", i+1, options[i], n);
			continue;
		}
		printf("%ld.%s\n", i+1, options[i]);
	}
}

int main() {
	srand(time(NULL));
	char buff[20] = {0}, c = 0,
	     *options[] = {"Use simple calculator",
		     "Draw a triangle of random size",
		     "Draw a rectangle of random size"};
	size_t size = sizeof(options)/sizeof(char*);
	printf("Choose an option down below (up and down arrow keys):\n");
	print_options(options, size, NULL, 0);
	struct termios d_tio, new_tio;
	tcgetattr(0, &d_tio);
	new_tio = d_tio;
	new_tio.c_lflag = 0;
	tcsetattr(0, TCSANOW, &new_tio);
	cfmakeraw(&new_tio);
	int option = 0, pos = 2, col = 1;
	while((c = fgetc(stdin))) {
		if(c == '\033') {
			option = 1;
		}
		if(option == 1) {
			option = 2;
		}
		if(option == 2) {
			if(c == 'A' && pos > 0) { // up arrow key
				// pos = 2, curr_opt = options[2], next_opt = options[1], pos = 1;
				// pos = 1, curr_opt = options[1], next_opt = options[0], pos = 0;
				char* curr_opt = options[pos];
				size_t len = strlen(curr_opt) + 3;
				printf("\033[%ldD\033[0m%d.%s\033[%ldD", len, pos+1, curr_opt, len);
				pos--;
				char* next_opt = options[pos];
				len = strlen(next_opt) + 3;
				printf("\033[%ldD\033[A\033[31m%d.%s\033[0m\033[%ldD", len, pos+1, next_opt, len);
				//printf("\033[");
			} else if(c == 'B' && pos < 2) { // down arrow key
				char* curr_opt = options[pos];
				size_t len = strlen(curr_opt) + 3;
				printf("\033[%ldD\033[0m%d.%s\033[%ldD", len, pos+1, curr_opt, len);
				pos++;
				char* next_opt = options[pos];
				len = strlen(next_opt) + 3;
				printf("\033[B\033[31m%d.%s\033[0m\033[%ldD", pos+1, next_opt, len);
			} else if(c == 'C') { // right arrow key
				printf("\033[C");
				col++;
			} else if(c == 'D') { // left arrow key
				printf("\033[D");
				col--;
			}
		}

		if(c == '\n') {
			int opt_num = pos+1;
			if(opt_num == 1) {
				int a, b;
				printf("\033[3B\033[%dD\033[0mOpening simple calculator\n", col+1);
				printf("Enter the two numbers separated by spaces to add:\n");
				tcsetattr(0, TCSANOW, &d_tio);
				scanf("%d%d", &a, &b);
				printf("%d + %d = %d\n", a, b, a + b);
				break;
			} else if(opt_num == 2) {
				int height = rand() % 101;
				height = (height == 0) ? 2 : height;
				printf("\033[2B\033[%dD\033[0mDrawing triangle\n", col+1);
				for(int i=0;i<height;i++) {
					for(int j=0;j<i+1;j++) {
						printf("*");
					}
					printf("\n");
				}
				break;
			} else if(opt_num == 3) {
				int height = rand() % 101, width = rand() % 101;
				height = (height == 0) ? 2 : height;
				width = (width == 0) ? 2 : width;
				printf("\033[1B\033[%dD\033[0mDrawing rectangle\n", col+1);
				for(int i=0;i<height;i++) {
					for(int j=0;j<width;j++) {
						printf("*");
					}
					printf("\n");
				}
				break;
			}
		}

		if(c == 'q') {
			printf("Quiting userinput\n");
			break;
		}
	}
	tcsetattr(0, TCSANOW, &d_tio);
	return 0;
}
