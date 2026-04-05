#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "util.h"
#include "lex/tokens.h"

extern FILE* yylin;
extern Lexer lexer;

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "\nMust specify .tig file\n");
	return EXIT_FAILURE;
    }
    
    char* file_name = argv[1];
    
    lexer = make_lexer();
    int token;
   
    while ((token = yylex()) != -1) {
         if (token == NEW_LINE) {
	     lexer->current_line++;
	     lexer->current_pos = 0;
	     continue;
	 }
	 
         Token new_token =  make_token(
	 	lexer->current_line, 
	 	lexer->current_pos,
	       	lexer->current_input_size,
		lexer->current_input,
                token
	 );
	 enqueue_token(lexer->queue, new_token);

	 if (new_token->token_type == END_OF_FILE)
	    break;
    }                           
}
