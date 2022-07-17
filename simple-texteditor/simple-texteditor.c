#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include "char.h"
#include "string.h"

string_t file_readlines(const char* filename, size_t* file_size) {
	string_t lines;
	char_t line;
	string_init(&lines);
	char_init(&line);

	FILE* f = fopen(filename, "r");
	fseek(f, 0, SEEK_END);
	size_t size = ftell(f), lines_count = 0;
	rewind(f);
	char* buff = calloc(size, sizeof(char));
	fread(buff, size, 1, f);
	fclose(f); // read the contents of the file
	size_t i=0;
	// split the contents into lines
	while(i < size) {
		char_push(&line, buff[i]);
		if(buff[i] == '\n') {
			string_append(&lines, line.buff);
			char_reinit(&line);
		}
		i++;
	}
	if(line.size != 0) {
		string_append(&lines, line.buff);
	}
	for(i=0;i<lines.size;i++) {
		printf("%s", lines.list[i]);
	}
	free(buff); buff = NULL;
	*file_size = size;
	char_free(&line);
	return lines;
}


void edit_file(const char* filename) {
	size_t file_size = 0, lines_count = 0, line = 1, column = 0;
	string_t lines = file_readlines(filename, &file_size);
	lines_count = lines.size;
	if(lines_count == 0) { lines_count = 1; }
	if(lines_count > 1) { printf("\033[%ldA", lines_count-1); }
	char c = '\0';
	int esc = 0;
	size_t row = 0;
	while((c = fgetc(stdin))) {
		if(c == '\033')  {
			esc = 1;
		} else if(c == 3) {
			break;
		} else if(c == 15) {
			printf("\033[%ldBSuccessfully written data to file\n", lines_count-line);
			break;
		} else if(esc == 1 && c == '[') {
			esc = 2;
		} else if(esc == 2) {
			switch(c) {
				case 'A': // up
					if(line > 1) {
						column = 0;
						line--;
						size_t n = strlen(lines.list[line]);
						printf("\033[A\033[%ldD", n);
					}
					break;
				case 'B': // down
					if(line < lines_count) {
						printf("\033[%ldD", column);
						column = 0;
						line++;
						size_t n = strlen(lines.list[line-1]);
						printf("\033[%ldD\033[B", n);
					}
					break;
				case 'C': // right
				{
					size_t n = strlen(lines.list[line-1]);
					if(column < n) {
						printf("\033[C");
						column++;
					}
					break;
				}
				case 'D': // left
				{
					size_t n = strlen(lines.list[line-1]);
					if(column > 0) {
						printf("\033[D");
						column--;
					}
					break;
				}
			}
			esc = 0;
		} else if(c == 127) { // backspace
			size_t n = strlen(lines.list[line-1]);
			if(n == 0 && line > 1) {
				line--; lines_count--;
				n = strlen(lines.list[line-1]);
				column = n-1;
				printf("\033[A\033[%ldC", n-1);
			} else {
				lines.list[line-1][column] = 0; // set the last position of the character to null
				column = n-1;
				printf("\033[D\033[X%s", );
			}
		} else if(isgraph(c)) {
		}
	}
	string_free(&lines);
}

int main(int argc, char** argv) {
	if(argc < 2) {
		fprintf(stderr, "%s: please specify the file to edit as an argument\n", argv[0]);
		return 1;
	}
	const char* filename = argv[1];
	struct termios old_tio, new_tio;
	tcgetattr(0, &old_tio);
	new_tio = old_tio;
	new_tio.c_lflag = 0;
	tcsetattr(0, TCSANOW, &new_tio);
	cfmakeraw(&new_tio);
	edit_file(filename);
	tcsetattr(0, TCSANOW, &old_tio);
	return 0;
}
