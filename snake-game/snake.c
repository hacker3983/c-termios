#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "board.h"
#include <stdbool.h>

int main() {
	srand(time(NULL));
	int length = 0, width = 0, score = 0;
	char** board = generate_board(&length, &width);
	int snake_row = 0, snake_column = 0,
	    food_row = 0, food_column = 0;
	gen_snake_pos(&snake_row, &snake_column,
			length, width);
	gen_food_pos(&food_row, &food_column,
			snake_row, length, width);
	board[snake_row][snake_column] = '#';
	board[food_row][food_column] = 'o';
	printf("%d x %d\n", length, width);
	printf("Score: 0\n");
	print_board(board, length, width, snake_row, snake_column);
	printf("Use arrow keys or w, a, s, d keys to move, "
			"press q to quit\n");
	printf("\033[%dA", (length - (snake_row+1)) + 2);
	struct termios old_tio;
	set_tio_raw(&old_tio); // set tty to raw mode
	char c = '\0';
	int esc = 0;
	bool up = 0, down = 0, left = 0, right = 0;
	while((c = fgetc(stdin))) {
		/*up = (c == 'w' && snake_row > 1) || (esc == 2 && c == 'A' && snake_row > 1);
		down = (c == 's' && snake_row < length-2) || (esc == 2 && c == 'B' && snake_row < length-2);
		left = (c == 'a' && snake_column > 1) || (esc == 2 && c == 'D' && snake_column > 1);
		right = (c == 'd' && snake_column < width-2) || (esc == 2 && c == 'C' && snake_column < width-2);
		*/
		up = (c == 'w' || (esc == 2 && c == 'A'));
		down = (c == 's' || (esc == 2 && c == 'B'));
		left = (c == 'a' || (esc == 2 && c == 'D'));
		right = (c == 'd' || (esc == 2 && c == 'C'));
		if(c == '\033') {
			esc = 1;
		} else if(esc == 1 && c == '[') {
			esc = 2;	
		} else if(up && snake_row > 1) {
			// get rid of the snake at the current position
			board[snake_row][snake_column] = ' ';
			printf("\033[%dD%s", width,
				board[snake_row--]);
			// move the snake to the new position upwards
			board[snake_row][snake_column] = '#';
			/*printf("\033[A\033[%dD%s\033[0m",
				width,
				board[snake_row]);*/
			printf("\033[A");
			print_snake_row(board, width, snake_row, snake_column);
			// increase the score if the snake touches the food
			if(snake_row == food_row && snake_column == food_column) {
				score++;
				printf("\033[%dD\033[%dAScore: %d", width, length - ((length-1) - snake_row), score);
				printf("\033[%dB", length - ((length-1) - snake_row));
				gen_food_pos(&food_row, &food_column,
						snake_row, length, width);
				board[food_row][food_column] = 'o';
				if(food_row > snake_row) {
					printf("\033[%dD\033[%dB%s", width, food_row - snake_row, board[food_row]);
					printf("\033[%dA", food_row - snake_row);
				} else if(snake_row > food_row) {
					printf("\033[%dD\033[%dA%s", width, snake_row - food_row, board[food_row]);
					printf("\033[%dB", snake_row - food_row);
				}
			}
			esc = 0;
		} else if(down && snake_row < length-2) {
			board[snake_row][snake_column] = ' ';
			printf("\033[%dD%s", width,
				board[snake_row++]);
			board[snake_row][snake_column] = '#';
			printf("\033[B");
			print_snake_row(board, width, snake_row, snake_column);
			if(snake_row == food_row && snake_column == food_column) {
				score++;
				printf("\033[%dD\033[%dAScore: %d", width, (length+1) - (length - snake_row), score);
				printf("\033[%dB", (length+1) - (length - snake_row));
				gen_food_pos(&food_row, &food_column,
						snake_row, length, width);
				board[food_row][food_column] = 'o';
				if(food_row > snake_row) {
					printf("\033[%dD\033[%dB%s", width, food_row - snake_row, board[food_row]);
					printf("\033[%dA", food_row - snake_row);
				} else if(snake_row > food_row) {
					printf("\033[%dD\033[%dA%s", width, snake_row - food_row, board[food_row]);
					printf("\033[%dB", snake_row - food_row);
				}
			}
			esc = 0;
		} else if(left && snake_column > 1) {
			board[snake_row][snake_column--] = ' ';
			board[snake_row][snake_column] = '#';
			print_snake_row(board, width, snake_row, snake_column);
			if(snake_row == food_row && snake_column == food_column) {
			score++;
				printf("\033[%dD\033[%dAScore: %d", width, length - ((length-1) - snake_row), score);
				printf("\033[%dB", length - ((length-1) - snake_row));
				gen_food_pos(&food_row, &food_column,
						snake_row, length, width);
				board[food_row][food_column] = 'o';
				if(food_row > snake_row) {
					printf("\033[%dD\033[%dB%s", width, food_row - snake_row, board[food_row]);
					printf("\033[%dA", food_row - snake_row);
				} else if(snake_row > food_row) {
					printf("\033[%dD\033[%dA%s", width, snake_row - food_row, board[food_row]);
					printf("\033[%dB", snake_row - food_row);
				}
			}
			esc = 0;
		} else if(right && snake_column < width-2) {
			board[snake_row][snake_column++] = ' ';
			board[snake_row][snake_column] = '#';
			print_snake_row(board, width, snake_row, snake_column);
			if(snake_row == food_row && snake_column == food_column) {
				score++;
				printf("\033[%dD\033[%dAScore: %d", width, length - ((length-1) - snake_row), score);
				printf("\033[%dB", length - ((length-1) - snake_row));
				gen_food_pos(&food_row, &food_column,
						snake_row, length, width);
				board[food_row][food_column] = 'o';
				if(food_row > snake_row) {
					printf("\033[%dD\033[%dB%s", width, food_row - snake_row, board[food_row]);
					printf("\033[%dA", food_row - snake_row);
				} else if(snake_row > food_row) {
					printf("\033[%dD\033[%dA%s", width, snake_row - food_row, board[food_row]);
					printf("\033[%dB", snake_row - food_row);
				}
			}
			esc = 0;
		} else if(c == 'q') {
			printf("\033[%dB\033[%dD",
				(length+1) - snake_row,
				width
			);
			break;
		} /*else if(up || down || right || left) {
			// if the snake_column or row isn't true in one of the if statements this will be executed
			printf("\033[%dB\033[%dDYou lose!\n",
				(length+1) - snake_row,
				width);
			break;
		}*/
	}
	set_tio(&old_tio); // unset tty from raw mode
	for(int i=0;i<length;i++) { free(board[i]); }
	free(board);
	return 0;
}
