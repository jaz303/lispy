#include <stdio.h>

#include "lispy/lexer.h"

char *read_file(const char *filename) {
	FILE *file = fopen(filename, "r");

	return " (+ -- -156 +2 #t  \"foo \\\"barn baz\") 1 0b1101101011 0x1010";
}

int main(int argc, char *argv[]) {

	lexer_t lexer;
	lexer_init(&lexer, read_file("test.lispy"));

	token_t token;
	while ((token = lexer_next(&lexer)) > 0) {
		printf("%d\t%d\t^%s$\n", token, (int) lexer.curr_int, lexer.curr_str);
	}

	printf("%d\t%d\t^%s$\n", token, (int) lexer.curr_int, lexer.curr_str);

	return 0;

}
