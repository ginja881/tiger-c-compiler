#include "util.h"


string String(char* text) {
    return strdup(text);
}
void* checked_malloc(size_t bytes) {
      void* new_data = malloc(bytes);
      assert(new_data);
      return new_data;
}
void report_error(error_code error, string line, size_t line_pos, size_t char_pos) {
     /*
      * @brief error output
      * @param error error code
      * @param line line that error occurred on
      * @param line_pos where the line is
      * @param char_pos what character in the line started the error
      */
     
     string error_status = "";

     switch (error) {
         case IllegalCharacter:
	 	error_status = "(Illegal Character)";
		break;
	 case SyntaxError:
	        error_status = "(Syntax Error)";
		break;
     }
     printf("\n---------------------------\n");
     fprintf(
        stderr,
     	"\n \e[0;31m%s error on line %ld at position %ld \e[0m\n ",
	error_status, 
	line_pos,
	char_pos+1
     );
     
     fprintf(stderr, "%s\n", line + char_pos);

     for (size_t i = 0; i < char_pos; i++)
        putc('-', stderr);

     fprintf(stderr, "^\n");
     _Exit(-1);
}
