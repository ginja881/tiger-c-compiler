#include "lex/tokens.h"
#include "util.h"
#include <unistd.h>

extern FILE* yyin;
Lexer lexer;


int main(int argc, char* argv[]) {
       if (argc < 2) {
            fprintf(stderr, "\nMust specify .tig file\n");
	    _Exit(-1);
       }     
       
       lexer = make_lexer();

       for (size_t i = 1; i < argc;i++) {
             yyin = fopen(argv[i], "r");

	     int token;
	     while ((token = yylex()) != -1) {
	          if (token == NEW_LINE) {
		       lexer->current_line++;
		       lexer->current_pos = 0;
		  }
	          print_token_type(token);
                  printf("\n (TOKEN TYPE_INT): %d \n", token);                                                    
		  printf("\n (TOKEN TEXT): %s\n", lexer->current_input);
		  printf("\n (TOKEN LINE): %ld\n", lexer->current_line);
		  printf("\n (TOKEN POS): %ld\n", lexer->current_pos);
                  printf("\n (TOKEN TEXT SIZE): %ld\n", lexer->current_input_size);

                  sleep(10);		  
	     }

	     fclose(yyin);
       }

       return EXIT_SUCCESS;
}

