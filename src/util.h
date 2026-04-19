#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TRUE 1
#define FALSE 0
#define DEFAULT_LINES_CAPACITY 100

#define min(X, Y) ((X) < (Y) ? (X) : (Y))
#define max(X, Y) ((X) > (Y) ? (X) : (Y))


typedef enum {
     IllegalCharacter,
     SyntaxError,
     TypeError,
     IndentError
} error_code;




typedef const char* string;



string String(char* text);
void* checked_malloc(size_t bytes);
void report_error(error_code error, string line, size_t line_pos, size_t char_pos, string error_msg);

/*
char** make_line_array(void);
void append(char* string);
char** resize(char** line_array, size_t new_capacity);
*/

#endif
