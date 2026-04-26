#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "util.h"
#include "lex/tokens.h"
#include "parser/ast.h"


extern FILE* yyin;
Lexer lexer;

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "\nMust specify .tig file\n");
	return EXIT_FAILURE;
    }
    
    char* file_name = argv[1];
    
    lexer = make_lexer();
    int token;
    yyin = fopen(file_name, "r");
    
    Line_Array line_array = make_line_array(DEFAULT_LINES_CAPACITY);

    while ((token = yylex()) != -1) {
        
	 
         Token new_token =  make_token(
	 	lexer->current_line, 
	 	lexer->current_pos,
	       	lexer->current_input_size,
		String(lexer->current_input),
                token
	 );
	 enqueue_token(lexer->queue, new_token);
         if (new_token->token_type == NEW_LINE) {
	 	A_Line new_line = make_line(lexer->current_input_size, String(lexer->current_line));
		line_array = append(line_array
	 	lexer->current_line++;
		lexer->current_pos = 0;
	 }
	 else if (new_token->token_type == END_OF_FILE)
	    break;
    }
    Parser parser = make_parser();
    parse_program(lexer, parser); 

    printf("\n DONE\n");
    return EXIT_SUCCESS;
}
