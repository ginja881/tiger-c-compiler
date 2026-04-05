#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TRUE 1
#define FALSE 0

typedef enum {
     IllegalCharacter,
     SyntaxError
} error_code;

typedef const char* string;


string String(char* text);
void* checked_malloc(size_t bytes);
void report_error(error_code error, string line, size_t line_pos, size_t char_pos);



#endif
