#include "parser/ast.h"
#include "util.h"
#include <unistd.h>

extern FILE* yyin;
Lexer lexer;



int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "\n Must specify only one .tig file\n");
		_Exit(-1);
	}
	lexer = make_lexer();

	yyin = fopen(argv[1], "r");
	int token;
	while ((token = yylex()) != -1) {
		Token new_token = make_token(
			lexer->current_line,
			lexer->current_pos,
			lexer->current_input_size,
			String(lexer->current_input),
			token
		);
		enqueue_token(lexer->queue, new_token);
		if (new_token->token_type == NEW_LINE) {
			lexer->current_line++;
			lexer->current_pos = 0;
			lexer->current_input_size = 0;
		}
		else if (new_token->token_type == END_OF_FILE)
		   break;
	}
	Parser parser = make_parser();

	parser->root = parse_program(lexer, parser);

	printf("\n %p is address of root of AST\n", parser->root);
	return EXIT_SUCCESS;
}
