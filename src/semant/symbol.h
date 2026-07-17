#ifndef __ENV_H__
#define __ENV_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "semant/types.h"
#include "semant/translate.h"

typedef enum {
	Var_Env,
	Type_Env,
	Escape_Env
} Env_Kind;
struct EnvEntry_ {
	enum {
		Var_Entry,
		Escape_Entry,
		Function_Entry
	} kind;

	union {
		struct {TempLabel label; Tr_Level level; TypeList parameters; Type return_type;} function_entry;
		struct {int depth; bool escapes;} escape_entry;
		struct {Tr_Access access; Type variable_type;} var_entry;
	} u;
};

struct Symbol_ {
	string name;
	struct EnvEntry_* environment_entry;
	struct Symbol_* next; 

};

struct SymbolTable_ {
	size_t capacity;
	size_t size;
	struct Symbol_** symbols;
};

struct Environment_ {
	Env_Kind kind;
	struct SymbolTable_* table;
};

typedef struct EnvEntry_* EnvEntry;
typedef struct Symbol_* Symbol;
typedef struct Environment_* Environment;
typedef struct SymbolTable_* SymbolTable;

#define HASH_CONSTANT 35
#define DEFAULT_CAPACITY 100
#define LOAD_FACTOR_THRESHOLD 0.75



EnvEntry make_function_entry(Tr_Level level, TempLabel label, TypeList parameters, Type return_type);
EnvEntry make_var_entry(Tr_Access access, Type raw_type);
EnvEntry make_escape_entry(int depth);

Symbol make_symbol(string name, EnvEntry environment_entry);
SymbolTable make_symbol_table(size_t capacity);
Environment make_environment(size_t capacity, Env_Kind kind);


size_t hash(size_t capacity, string name);
Symbol get_symbol(Environment environment, string name);
Environment resize_environment(Environment environment, size_t new_capacity);
Environment insert_symbol(Environment environment, Symbol new_symbol);
Environment delete_symbol(Environment environment, string name);
Environment clone_environment(Environment environment);

#endif
