#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main() {
	struct termios new_tio, old_tio;
	tcgetattr(0, &old_tio);
	new_tio = old_tio;
	new_tio.c_lflag = 0;
	tcsetattr(0, TCSANOW, &new_tio);
	cfmakeraw(&new_tio);
	char* table_options[] = {
		"Products", "Fruits",
		"Houses", "Cars"
	}, *color = "\033[48;2;50;10;40m\033[38;2;209;190;56m", c = 0;
	int size = sizeof(table_options) / sizeof(char*), row = size,
	    esc = 0, selected = 0;
	printf("Select a table below:\n");
	for(int i=0;i<size-1;i++) {
		printf("%d.%s\n", i+1, table_options[i]);
	}
	printf("%s%d.%s\033[0m\n\033[A", color, size, table_options[size-1]);
	fflush(stdout);
	while((c = fgetc(stdin))) {
		if(c == '\033') {
			esc = 1;
		} else if(c == '[' && esc == 1) {
			esc = 2;
		} else if(esc == 2) {
			if(c == 'A' && row > 1) {
				row--;
				printf("\033[10D%d.%s", row+1, table_options[row]);
				printf("\033[A\033[10D%s%d.%s\033[0m", color, row, table_options[row-1]);
				fflush(stdout);
			} else if(c == 'B' && row < size) {
				printf("\033[10D%d.%s", row, table_options[row-1]);
				row++;
				printf("\033[B\033[10D%s%d.%s\033[0m", color, row, table_options[row-1]);
			}
			fflush(stdout);
			esc = 0;
		} else if(c == 3 || c == 4 || c == 'q') {
			printf("\033[10D\033[%dB", (size - row) + 1);
			if(selected) {
				printf("\033[B");
			}
			break;
		} else if(c == '\n') {
			printf("\033[10D%d.%s", row, table_options[row-1]);
			printf("\033[%dB\033[10DYou selected option number %d\n\033[%dA", (size - row) + 1, row, (size - row) + 2);
			printf("\033[10D%s%d.%s\033[0m", color, row, table_options[row-1]);
			if(!selected) { selected = 1; }
		}
	}
	tcsetattr(0, TCSANOW, &old_tio);
	return 0;
}
