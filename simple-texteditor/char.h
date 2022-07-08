#ifndef _CHAR_H
#define _CHAR_H
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct char_s {
	char* buff;
	size_t size, length;
	int init;
} char_t;
void char_init(char_t* mychar);
void char_reinit(char_t* mychar);
bool char_push(char_t* mychar, char c);
bool char_pushptr(char_t* mychar, char* buff);
void char_pushf(char_t* mychar, const char* format, ...);
void char_reverse(char_t* mychar);
void char_swap(char_t* a, char_t* b);
void char_free(char_t* mychar);

// initialize char_t structure
void char_init(char_t* mychar) {
	mychar->buff = calloc(1, sizeof(char*));
	mychar->size = 1;
	mychar->length = 0;
	mychar->init = 1;
}
void char_reinit(char_t* mychar) {
	char_free(mychar);
	char_init(mychar);
}
// push a character to the structure
bool char_push(char_t* mychar, char c) {
	if(mychar->init == 1) {
		mychar->buff[mychar->length] = c;
		mychar->size++;
		mychar->length++;
		char* ptr = mychar->buff;
		mychar->buff = calloc(mychar->size, sizeof(char*));
		strcpy(mychar->buff, ptr);
		free(ptr);
		return true;
	}
	return false;
}
// push a char pointer to the structure
bool char_pushptr(char_t* mychar, char* buff) {
	if(buff != NULL && mychar->init) {
		mychar->size += strlen(buff);
		char* ptr = mychar->buff;
		mychar->buff = calloc(mychar->size, sizeof(char*));
		strcpy(mychar->buff, ptr);
		strcat(mychar->buff, buff);
		mychar->length += strlen(buff);
		free(ptr);
		return true;
	}
	return false;
}
// push a format string on to the structure
void char_pushf(char_t* mychar, const char* format, ...) {
        size_t i=0;
        va_list varg;
        va_start(varg, format);
        while(format[i]) {
                int num = 0;
                size_t bignum;
                if(format[i] == '%') {
                        i++;
                        switch(format[i]) {
                                case 'd': {
                                        char_t temp;
                                        char_init(&temp);
                                        num = va_arg(varg, int);
                                        while(num != 0) {
                                                char_push(&temp, (num % 10)+'0');
                                                num /= 10;
                                        }
                                        char_reverse(&temp);
                                        char_pushptr(mychar, temp.buff);
                                        char_free(&temp);
                                        break;
                                } case 's':
                                        char_pushptr(mychar, va_arg(varg, char*));
                                        break;
                                case 'c':
                                        char_push(mychar, (char)va_arg(varg, int));
                                        break;
                        }
                } else {
                        char_push(mychar, format[i]);
                }
                i++;
        }
        va_end(varg);
}
// reverse the string in char_t structure
void char_reverse(char_t* mychar) {
	char_t list;
	char_init(&list);
	for(int i=mychar->length-1;i>=0;i--) {
		char_push(&list, mychar->buff[i]);
	}
	free(mychar->buff);
	mychar->size = list.size;
	mychar->length = list.length;
	mychar->buff = list.buff;
}
// Exchanges the contents of the char_t with another
void char_swap(char_t* a, char_t* b) {
	char_t temp, temp2;
	char_init(&temp);
	char_init(&temp2);
	char_pushptr(&temp, a->buff);
	char_pushptr(&temp2, b->buff);
	free(b->buff);
	free(a->buff);
	b->buff = temp.buff;
	b->size = temp.size;
	b->length = temp.length;

	a->buff = temp2.buff;
	a->size = temp2.size;
	a->length = temp2.length;
}

// frees memory and resets everything
void char_free(char_t* mychar) {
	if(mychar->init == 1) {
		free(mychar->buff);
		mychar->size = 0;
		mychar->length = 0;
		mychar->init = 0;
	}
}
#endif
