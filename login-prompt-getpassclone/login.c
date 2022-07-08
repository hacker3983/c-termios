#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>

char* get_userinput(const char* prompt) {
	char* buff = calloc(1, sizeof(char)), c = '\0';
	size_t size = 0;
	printf("%s", prompt);
	while((c = fgetc(stdin))) {
		if(c == '\n') {
			break;
		}
		buff[size++] = c;
		char* temp = buff;
		buff = calloc(size+1, sizeof(char));
		strcpy(buff, temp);
		free(temp); temp = NULL;
	}
	return buff;
}

void echo_off(struct termios* old_tio) {
	struct termios new_tio;
	tcgetattr(0, &new_tio);
	*old_tio = new_tio;
	new_tio.c_lflag = ECHOE;
	tcsetattr(0, TCSANOW, &new_tio);
}

void echo_on(struct termios* old_tio) {
	tcsetattr(0, TCSANOW, old_tio);
}

int main() {
	struct termios old_tio;
	printf("Please login down below\n");
	char* username = get_userinput("Username: "), *password = NULL;
	echo_off(&old_tio);
	password = get_userinput("Password: ");
	printf("\n");
	echo_on(&old_tio);
	if(strcmp(username, "root") == 0 && strcmp(password, "toor") == 0) {
		printf("Successfully logged in as root\n");
	} else {
		printf("Username or Password incorrect\n");
	}
	free(username);
	free(password);
	return 0;
}
