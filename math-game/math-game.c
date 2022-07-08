#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef struct question {
	int num1, op, num2, ans;
} question_t;

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

void gen_question(question_t* q) {
	char ops[] = "+-*/";
	int num1 = rand() % 101, num2 = rand() % 101,
	    op = rand() % 4, ans = 0;
	if(num1 < 1) {
		num1++;
	}
	if(num2 < 1) {
		num2++;
	}
	if(num1 < num2) {
		int temp = num1;
		num1 = num2, num2 = temp;
	} 
	switch(ops[op]) {
		case '+': ans = num1 + num2; break;
		case '-': ans = num1 - num2; break;
		case '/': ans = num1 / num2;  break;
		case '*': ans = num1 * num2; break;
	}
	q->num1 = num1;
	q->op = ops[op];
	q->num2 = num2;
	q->ans = ans;
}

int num_len(int num) {
	int len = 0;
	if(num == 0) { return 1; }
	while(num) {
		len++; num /= 10;
	}
	return len;
}
int main() {
	struct termios old_tio;
	set_raw(&old_tio);
	srand(time(NULL));
	question_t q;
	char c = 0, *ans = calloc(6, sizeof(char));
	int esc = 0, ans_len = 0, questions_count = 1, score = 0,
	    i = 0;
	gen_question(&q);
	printf("There are 10 questions. Answer the following question below with the correct answer.\n");
	printf("Question 1 of 10\n");
	printf("What is %d %c %d? ", q.num1, q.op, q.num2);
	sprintf(ans, "%d", q.ans);
	ans_len = num_len(q.ans);
	while((c = fgetc(stdin))) {
		if(isdigit(c)) {
			printf("%c", c);
			if(c != ans[i]) {
				//printf("%d\n", questions_count);
				if(questions_count < 10) {	
					printf("\n\033[31mIncorrect\033[0m\n");
					printf("\033[3AQuestion: %d of 10\n", questions_count+1);
					if(questions_count+1 != 10) {
						printf("\033[K");
						gen_question(&q);
						printf("What is %d %c %d? ", q.num1, q.op, q.num2);
						memset(ans, 0, ans_len);
						sprintf(ans, "%d", q.ans);
						ans_len = num_len(q.ans);
					}
					questions_count++;
					i = 0;
				}
			} else {
				i++;
			}
		}
		if(i == ans_len) {
			if(score == 0 && questions_count == 1) {
				score = 2;
			} else {
				score++;
			}
			printf("\n\033[K\033[32mCorrect!\033[0m\n");
			printf("\033[3AQuestion: %d of 10\n", questions_count+1);
			if(questions_count < 10) {
				if(questions_count+1 != 10) {
					printf("\033[K");
					gen_question(&q);
					printf("What is %d %c %d? ", q.num1, q.op, q.num2);
					memset(ans, 0, ans_len);
					sprintf(ans, "%d", q.ans);
					ans_len = num_len(q.ans);
				}
				questions_count++;
			}
			i = 0;
		}
		
		if(questions_count == 10) {
			if(score == 10) {
				printf("\n\033[32mCongratulations you win!\033[0m\n");
				printf("You got \033[34m10/10 = 100\033[0m%%\n");
				break;
			} else if(score <= 9 && score >= 7) {
				printf("\n\033[33mGood Job better luck next time!\033[0m\n");
				printf("You got \033[34m%d/10 = %d\033[0m%%\n", score, (int)(((float)score) / 10 * 100));
				break;
			} else {
				printf("\n\033[31mTry harder next time atleast you tried!\033[0m\n");
				printf("You got \033[34m%d/10 = %d\033[0m%%\n", score, (int)(((float)score) / 10 * 100));
				break;
			}
		}
		if(c == 'q') {
			printf("\n");
			break;
		}
	}
	set_tio(&old_tio);
	return 0;
}
