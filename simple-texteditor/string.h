#ifndef _LIST_STRING_H
#define _LIST_STRING_H
#include "string.h"
typedef struct string {
	char** list;
	size_t size;
} string_t;

void string_init(string_t* string);
void string_append(string_t* string, char* buff);
void string_free(string_t* string);

void string_init(string_t* string) {
	string->list = malloc(sizeof(char*));
	string->size = 0;
}

void string_append(string_t* string, char* buff) {
	string->list[string->size] = calloc(strlen(buff)+1, sizeof(char*));
	strcpy(string->list[string->size], buff);
	string->size++;
	string->list = realloc(string->list, (string->size+1) * sizeof(char*));
}

void string_free(string_t* string) {
	for(size_t i=0;i<string->size;i++) {
		free(string->list[i]);
	}
	free(string->list);
	string->size = 0;
}
#endif
