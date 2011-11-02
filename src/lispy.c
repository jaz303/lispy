#include <stdio.h>

#include "lispy/lexer.h"
#include "lispy/object.h"
#include "lispy/parser.h"
#include "lispy/value.h"
#include "lispy/intern.h"

char *read_file(const char *filename) {
	FILE *file = fopen(filename, "r");
	return "(1 2 3 \"foo\" #t #f)";
	return "( 1 2 3 ( 4 (5 6) #t #f ) \"foobar\" )";
}

void indent(int indent) {
	int i;
	for (i = 0; i < indent; i++) putc(' ', stdout);
}

void pretty_print(VALUE v, int i);
void pretty_print(VALUE v, int i) {
	if (VALUE_IS_INT(v)) {
		indent(i); printf("%lld\n", INTVAL(v));
	} else if (VALUE_IS_BOOL(v)) {
		indent(i); printf("#%c\n", BOOLVAL(v) ? 't' : 'f');
	} else if (OBJ_IS_LIST(v)) {
		indent(i); printf("(\n");
		int j;
		for (j = 0; j < ((list_t*)v)->length; j++) {
			pretty_print(((list_t*)v)->values[j], i + 2);
		}
		indent(i); printf(")\n");
	} else if (OBJ_IS_STRING(v)) {
		indent(i); printf("\"%s\"\n", ((string_t*)v)->string);
	}
}

int main(int argc, char *argv[]) {
    
    intern_table_t intern;
    intern_table_init(&intern);

	lexer_t lexer;
	lexer_init(&lexer, read_file("test.lispy"));
	
    parser_t parser;
    parser.lexer = &lexer;
    parser.intern = &intern;
	
	parser_init(&parser);

	list_t *ast = parser_parse(&parser);

	if (ast) {
		pretty_print((VALUE)ast, 0);
	} else {
		printf("Parse error: %s\n", parser.error);
	}

	return 0;

}
