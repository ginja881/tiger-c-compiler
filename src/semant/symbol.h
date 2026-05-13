#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "tpyes.h"
typedef enum {
	Symbol_Field,
	Symbol_Simple,
	Symbol_Record,
	Symbol_Array_Type,
	Symbol_Function
} SymbolKind;

struct Symbol_ {
	string name;
	SymbolKind symbol_kind;
	Type type;
	struct Symbol_* next; 

};

struct SymbolTable_ {
	size_t capacity;
	size_t size;
	struct Symbol_** symbols;
};

typedef struct Symbol_* Symbol;
typedef struct SymbolTable_* SymbolTable;


#define HASH_CONSTANT 35
#define DEFAULT_CAPACITY 11
#define LOAD_FACTOR_THRESHOLD 0.75

Symbol make_symbol(string name, string value, Type type, SymbolKind symbol_kind);
SymbolTable make_symbol_table(size_t capacity);

void free_symbol(Symbol symbol);
void free_symbol_table(SymbolTable symbol_table);


size_t hash(size_t capacity, string name);
Symbol get(SymbolTable symbol_table, string name);
SymbolTable resize(SymbolTable, size_t new_capacity);
SymbolTable insert(SymbolTable symbol_table, Symbol new_symbol);
SymbolTable delete(SymbolTable, string name);
