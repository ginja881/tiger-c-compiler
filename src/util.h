#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
     IllegalCharacter,
     SyntaxError
} error_code;

typedef const char* string;


string String(char* text);

void report_error(error_code error, string line, size_t line_pos, size_t char_pos);



#endif
