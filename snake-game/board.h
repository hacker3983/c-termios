#ifndef _BOARD_H
#define _BOARD_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <termios.h>

void set_tio_raw(struct termios* old_tio) {
	struct termios new_tio;
	tcgetattr(0, old_tio);
	new_tio = *old_tio;
	new_tio.c_lflag = 0;
	tcsetattr(0, TCSANOW, &new_tio);
	cfmakeraw(&new_tio);
}

void set_tio(struct termios* tio) {
	tcsetattr(0, TCSANOW, tio);
}

char** generate_board(int* lengthp, int* widthp) {
	// generate random width and height of the board
	int length = rand() % 20, width = rand() % 50;
	length = (length >= 7) ? length : 7;
	width = (width >= 11) ? width : 11;
	*lengthp = length, *widthp = width;
	char** board = calloc(length, sizeof(char*));
	for(int i=0;i<length;i++) {
		board[i] = calloc(width+1, sizeof(char));
	}
	// draw the wall of the board
	for(int i=0;i<width;i++) {
		board[0][i] = '.', board[length-1][i] = '.';
	}
	// draw the inside and outside of the board
	for(int i=1;i<length-1;i++) {
		board[i][0] = '.';
		
		for(int j=1;j<width-1;j++) {
			board[i][j] = ' ';
		}
		board[i][width-1] = '.';
	}
	return board;
}

void gen_snake_pos(int* row, int* column, int length, int width) {
	*row = rand() % length, *column = rand() % width;
	if(*row == 0) { *row = 1; }
	if(*row == length-1) { *row = length - 2; }
	if(*column == 0) { *column = 1; }
	if(*column == width-1) { *column = width - 2; }
}

void gen_food_pos(int* row, int* column,
		int snake_row,
		int length,
		int width) {
	*row = rand() % length, *column = rand() % width;
	if(*row == 0) { *row = 1; }
	if(*row == length-1) { *row = length - 2; }
	if(*column == 0) { *column = 1; }
	if(*column == width-1) { *column = width - 2; }
	if(*row == snake_row) {
		if(*row == length-2) {
			*row = 1;
		} else if(*row == 1) {
			*row = length - 2;
		} else {
			*row = 1;
		}
	}
}
void print_board(char** board, int length) {
	for(int i=0;i<length;i++) {
		printf("%s\n", board[i]);	
	}
}
#endif
