#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <termios.h>

int stop = 0;
typedef struct ball_info {
	int* length, *width, *x, *y;
} ball_t;

void* quit(void* arg) {
	ball_t* ball = (void*)arg;
	char c = 0;
	while((c = fgetc(stdin))) {
		if(c == 'q') {
			stop = 1; break;
		}
	}
}

void set_raw(struct termios* old_tio) {
	struct termios new_tio;
	tcgetattr(0, old_tio);
	new_tio = *old_tio;
	new_tio.c_lflag = 0;
	tcsetattr(0, TCSANOW, &new_tio);
}

void set_tio(struct termios* tio) {
	tcsetattr(0, TCSANOW, tio);
}

int main() {
	char* ball = "\033[32mo\033[0m";
	int length = 10, width = 40, ball_x = 0, ball_y = 0;
	ball_t ball_info;
	ball_info.length = &length, ball_info.width = &width;
	ball_info.x = &ball_x, ball_info.y = &ball_y;
	struct termios old_tio;
	set_raw(&old_tio);
	pthread_t thread;
	pthread_create(&thread, NULL, quit, &ball_info);

	for(int i=0;i<width;i++) { printf("-"); }
	printf("\n");
	for(int i=0;i<length;i++) {
		printf("|");
		for(int j=0;j<width-2;j++) {
			printf(" ");
		}
		printf("|\n");
	}
	for(int i=0;i<width;i++) { printf("-"); }
	printf("\n\033[2A\033[C%s", ball);
	fflush(stdout);
	while(1) {
		if(ball_x < width-3 && ball_y < length-1) {
			printf("\033[D ");
			printf("\033[A"); // move upwards the ball upwards on the y axis
			printf("%s", ball);
			usleep(2 * 100000); // sleep for 0.2 seconds
			fflush(stdout);
			ball_x++; ball_y++;
		} else if(ball_x < width-3 && ball_y == length-1) {
			while(ball_x < width-3 && ball_y > 0) {
				printf("\033[D \033[B");
				printf("%s", ball);
				usleep(2 * 100000); // sleep 0.2 seconds
				fflush(stdout);
				ball_y--; ball_x++;
			}
		} else if(ball_x == width-3 && ball_y < length-1) {
			while(ball_x > 0 && ball_y < length-1) {
				printf("\033[D \033[A\033[2D");
				printf("%s", ball);
				usleep(2 * 100000);
				fflush(stdout);
				ball_y++; ball_x--;
			}
			while(ball_x > 0 && ball_y > 0) {
				printf("\033[D \033[2D\033[B");
				printf("%s", ball);
				usleep(2 * 100000);
				fflush(stdout);
				ball_y--; ball_x--;
			}
			printf("\033[D \033[%dD", ball_x+1);
			printf("%s", ball);
			usleep(2 * 100000);
			fflush(stdout);
			ball_x = 0;
		}
		if(stop) {
			set_tio(&old_tio);
			printf("\033[%dB\n", (length - (length - ball_y)) + 1);
			printf("Quiting\n");
			break;
		}
	}
	pthread_join(thread, NULL);
	return 0;
}
