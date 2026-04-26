#include "util.h"


string String(char* text) {
    return strdup(text);
}
void* checked_malloc(size_t bytes) {
      void* new_data = malloc(bytes);
      assert(new_data);
      return new_data;
}

A_Line make_line(size_t characters, string line) {
	A_Line new_line = (A_Line)checked_malloc(sizeof(struct A_Line_));
	new_line->characters = characters;
	new_line->line = strdup(line);

	return new_line;
}

Line_Array make_line_array(size_t capacity) {
	Line_Array new_line_array = (Line_Array)checked_malloc(sizeof(struct Line_Array_));
	new_line_array->lines = (A_Line*)checked_malloc(sizeof(struct A_Line_) * capacity);
	new_line_array->line_count = 0;
	new_line_array->capacity = capacity;
	return new_line_array;
}
Line_Array resize_line_array(Line_Array line_array, size_t new_capacity) {
	Line_Array larger_line_array = make_line_array(new_capacity);
	larger_line_array->line_count = line_array->line_count;

	for (size_t i = 0; i < line_array->line_count; i++) {
		larger_line_array->lines[i] = line_array->lines[i];
	}
	return larger_line_array;
}
Line_Array append_line(Line_Array line_array, A_Line line) {
	if (line_array->line_count == line_array->capacity)
		line_array = resize_line_array(line_array, line_array->capacity*2);
	
	line_array->lines[line_array->line_count]   =  line;
	line_array->line_count++;

	return line_array;

}

void report_error(error_code error, string line, size_t line_pos, size_t char_pos, string error_msg) {
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
	 case IndentError:
	 	error_status = "(Indentation Error)";
		break;
	 case TypeError:
	 	error_status = "(Type Error)";
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
     printf(" %s \n", error_msg);

     printf("---------------------------\n");
 
     fprintf(stderr, "%s\n", line);

     for (size_t i = 0; i < char_pos; i++)
        putc('-', stderr);

     fprintf(stderr, "^\n");
     _Exit(-1);
}
