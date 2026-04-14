#include "lex/tokens.h"
#include "util.h"
#include <unistd.h>

extern FILE* yyin;
Lexer lexer;

void print_token_type(int lex_token);

int main(int argc, char* argv[]) {
       if (argc < 2) {
            fprintf(stderr, "\nMust specify .tig file\n");
	    _Exit(-1);
       }     
       
       lexer = make_lexer();

       for (int i = 1; i < argc;i++) {
             yyin = fopen(argv[i], "r");
             
	     int token;
	     while ((token = yylex()) != -1) {
	          if (token == NEW_LINE) {
		       lexer->current_line++;
		       lexer->current_pos = 0;
		  }
	         

		  printf("\n \e[0;32m ---TOKEN INFORMATION--- \e[0m\n");
                  print_token_type(token);                                                   
		  printf("\n (TOKEN TEXT): %s\n", lexer->current_input);
		  printf("\n (TOKEN LINE): %ld\n", lexer->current_line);
		  printf("\n (TOKEN POS): %ld\n", lexer->current_pos);
                  printf("\n (TOKEN TEXT SIZE): %ld\n", lexer->current_input_size);

                  sleep(1);
		  
		  printf("\e[1;1H\e[2J");
	     }

	     fclose(yyin);
       }

       return EXIT_SUCCESS;
}

void print_token_type(int lex_token) {
      char* token = "\n";
      switch (lex_token) {
            case MUL: 
	    	token = "MULL";
	        break;
	    case ADD:
	        token = "ADD";
		break;
            case DIV:
	        token = "DIV";
	        break;
	    case MOD:
	        token = "MOD";
		break;
	    case SUB:
	        token = "SUB";
		break;
	    case GT:
	        token = "GT";
		break;
	    case GT_EQ:
	        token = "GT_EQ";
		break;
	    case LT:
	        token = "LT";
		break;
	    case LT_EQ:
	        token = "LT_EQ";
		break;
	    case NOT:
	        token = "NOT";
		break;
	    case COMPAR_EQ:
	        token = "COMPAR_EQ";
		break;
	    case COMPAR_AND:
	        token = "COMPAR_AND";
		break;
	    case COMPAR_OR:
	        token = "COMPAR_OR";
		break;
	    case BIT_AND:
	        token = "BIT_AND";
		break;
           case BIT_OR:
	        token = "BIT_OR";
		break;
	   case INCREMENT:
	        token = "INCREMENT";
		break;
	    case DECREMENT:
	        token = "DECREMENT";
		break;
	    case FOR:
	        token = "FOR";
		break;
	    case TO:
	    	token = "TO";
		break;
	    case IN: 
	        token = "IN";
		break;
	    case WHILE:
	        token = "WHILE";
		break;
	    case DO:
	        token = "DO";
		break;
	    case IF:
	        token = "IF";
		break;
	    case ELSE:
	        token = "ELSE";
		break;
	    case THEN:
	        token = "THEN";
		break;
	    case FUNCTION_DEF:
	        token = "FUNCTION_DEF";
		break;
	    case END:
	        token = "END";
		break;
	    case LET:
	        token = "LET";
		break;
	    case NULL_VAL:
	        token = "NULL_VAL";
		break;
	    case ID:
	        token = "ID";
		break;
	    case REAL:
	        token = "REAL";
		break;
	    case NUM:
	    	token = "NUM";
		break;
	    case SEMI_COLON:
	        token = "SEMI_COLON";
		break;
	    case COLON:
	    	token = "COLON";
		break;
	    case L_PAREN:
	    	token = "L_PAREN";
		break;
	    case R_PAREN:
	        token = "R_PAREN";
		break;
	    case TAB:
	        token = "TAB";
		break;
	    case NEW_LINE:
	        token = "NEW_LINE";
	    	break;
	    case L_CURLY_BRCKT:
	    	token = "L_CURLY_BRCKT";
		break;
	    case R_CURLY_BRCKT:
	    	token = "R_CURLY_BRCKT";
		break;
	    case L_SQUARE_BRCKT:
	    	token = "L_SQUARE_BRCKT";
		break;
	    case R_SQUARE_BRCKT:
	        token = "R_SQUARE_BRCKT";
		break;
	    case VAR_DEC:
	    	token = "VAR_DEC";
		break;
	    case TYPE_DEC:
	    	token = "TYPE_DEC";
		break;
	    case TYPE_ASSIGN:
	    	token = "TYPE_ASSIGN";
		break;
	    case VAR_ASSIGN:
	    	token = "VAR_ASSIGN";
		break;
	    case CHAR:
	    	token = "CHAR";
		break;
	    case COMMA:
	    	token = "COMMA";
		break;
	   case STRING:
	   	token = "STRING";
		break;
	   case MEMBER_REF:
	   	token = "MEMBER_REF";
		break;
	   case END_OF_FILE:
	   	token = "END_OF_FILE";
		break;
	    
      }
      printf("\n (TOKEN_TYPE) %s \n", token);
}
