#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define TRUE 1
#define FALSE 0
#define DEFAULT_LINES_CAPACITY 100
#define WORD_SIZE 8

#define min(X, Y) ((X) < (Y) ? (X) : (Y))
#define max(X, Y) ((X) > (Y) ? (X) : (Y))


typedef enum {
     IllegalCharacter,
     SyntaxError,
     TypeError,
     UnknownError
} error_code;

typedef struct BoolList_* BoolList;
/*
typedef struct StringNode_* StringNode;
typedef struct StringTable_* StringTable;
*/
struct BoolList_ {
	bool BOOL;
	BoolList next;
};

/*
struct StringNode_ {
	string text;
	StringNode next;
};

struct StringTable_ {
	size_t capacity;
	size_t size;
	StringNode* strings;
};
*/

typedef const char* string;


string String(char* text);
/*
StringNode make_string_node(string string);
int hash(char* text);
void set_table(StringTable table, string text);
string get_string(StringTable, string text);
*/

void* checked_malloc(size_t bytes);

void report_error(error_code error, string line, size_t line_pos, size_t char_pos, string error_msg, int panic_mode);

BoolList new_boollist(bool BOOL, BoolList next);



#endif
