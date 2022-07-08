#include <stdio.h>
#include <termios.h>

int main() {
	struct termios old_tio, new_tio;
	tcgetattr(0, &old_tio);
	new_tio = old_tio;
	new_tio.c_lflag = 0;
	tcsetattr(0, TCSANOW, &new_tio);
	cfmakeraw(&new_tio);
	printf("Press any key to continue . . . ");
	fgetc(stdin);
	printf("\n");
	tcsetattr(0, TCSANOW, &old_tio);
	return 0;
}
