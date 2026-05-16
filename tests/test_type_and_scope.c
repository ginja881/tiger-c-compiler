#include "parser/ast.h"
#include "lex/tokens.h"
#include "semant/semant.h"
#include "util.h"

Lexer lexer;
int panic_mode;
extern FILE* yyin;

int main(char** argv, int argc) {
	if (argc != 2) {
		fprintf(stderr, "\n Must specify a .tig file \n");
		return EXIT_FAILURE;
	}

	char* filename = argv[1];
	string extension = strrchr(filename, '.');
	if (!extension || strcmp(extension +1, "tig") != 0) {
		fprintf(stderr, "\n Must specify a .tig file\n");
		return EXIT_FAILURE;
	}


	lexer = make_lexer();
	int token;
	int panic_mode = FALSE;

	yyin = fopen(filename, "r");

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
		}
		else if (new_token->token_type == END_OF_FILE)
			break;
	}

	Parser parser = make_parser();
	parse_program(lexer, parser);

	SemanticAnalyzer sem = make_semantic_analyzer(parser);
        semantic_main(sem);

	printf("\n Success! Test Passed for Semantic Analyzer\n");
	return EXIT_SUCCESS;
}
