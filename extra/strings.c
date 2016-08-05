/*
 ============================================================================
 Name        : test.c
 Author      : Denis
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

int len(char const * string) {
	int i = 0;
	while(*string) {
		i++;
		string++;
	}
	return i;
}

void swap(char * a, char * b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}


char const *find_char( char const *source, char const *chars ) {
	while (*chars) {
		for (char const * i = source; *i; ++i) {
			if ( *chars == *i) {
				return i;
			}
		}
		++chars;
	}
	return NULL;
}

int del_substr( char *str, char const *substr ) {
	int is_in_string  = 0;
	int substr_len    = len(substr);
	int length        = 0;
	int index         = 0;
	for (int i = 0; str[i]; ++i) {
		length = 0;
		while ( str[i + length] == substr[length] ) {
			++length;
			if( !substr[length] ) {
				index = i;
				is_in_string = 1;
				goto exit_loop;
			}
		}
	}
	exit_loop:
	if ( is_in_string ) {
		int counter;
		for (counter = 0; str[index + length + counter]; ++counter) {
			str[index + counter] = str[index + length + counter];
		}
		str[index + counter] = '\0';
	}
	return is_in_string;
}


void reverse_string(char * string) {
	int size = len(string) - 1;
	for (int i = 0; i < size / 2; i++) {
		swap(string + i, string + size - i);
	}
}

int main(void) {
	char ana[] = "Ana areare mere.";
	//reverse_string(ana);
	printf("%s", ana);
	printf("\n");
	//printf("%s", find_char(ana, "era") );
	printf("\n");
	int x = del_substr(ana, "are");
	printf(" %d %s", x, ana);
	return EXIT_SUCCESS;
}
