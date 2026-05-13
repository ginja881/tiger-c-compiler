#ifndef __ENV_H__
#define __ENV_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "semant/types.h"

struct EnvEntry_ {
	enum {
		Type_Entry,
		Function_Entry,
		Array_Entry,
		Record_Entry
	} kind;

	union {
		struct {TypeList parameters; Type return_type} function_entry;
		Type type_entry;
		struct {Type element_type; size_t size;} array_entry;
		struct {TypeList fields;} record_entry;
	} u;
};

struct Symbol_ {
	string name;
	struct EnvEntry_ environment_entry;
	struct Symbol_* next; 

};

struct Environment_ {
	size_t capacity;
	size_t size;
	struct Symbol_** symbols;
};

typedef struct EnvEntry_* EnvEntry;
typedef struct Symbol_* Symbol;
typedef struct Environment_* SymbolTable;


#define HASH_CONSTANT 35
#define DEFAULT_CAPACITY 11
#define LOAD_FACTOR_THRESHOLD 0.75

EnvEntry make_function_entry(TypeList parameters, Type return_type);
EnvEntry make_type_entry(Type raw_type);
EnvEntry make_array_entry(Type element_type, size_t size);
EnvEntry make_record_entry(TypeList fields);

Symbol make_symbol(string name, EnvEntry environment_entry);
Environment make_environment(size_t capacity);


size_t hash(size_t capacity, string name);
Symbol get(Environment environment, string name);
Environment resize(Environment environment, size_t new_capacity);
Environment insert(Environment environment, Symbol new_symbol);
Environment delete(Environment environment, string name);
#endif
